#pragma once

#include <cassert>

#include "MacroBlock.h"
#include "NalUnit.h"
#include "PPS.h"
#include "SPS.h"

class VideoDecoder;

enum SliceType {
    P = 0,
    B,
    I,
    SP,
    SI,
    P_high,
    B_high,
    I_high,
    SP_high,
    SI_high
};

enum class DataType {
    luma,
    cb,
    cr
};

enum RefStatus {
    NonRef = 0,
    ShortTerm,
    LongTerm
};

enum MemManCtrlOp {
    EndLoop = 0,
    ShortTermToNonRef, // 1
    LongTermToNonRef, // 2
    ShortTermToLongTerm, // 3
    SetMaxLongTermAndPurge, // 4
    MarkAllNonRef, // 5
    CurToLongTerm // 6
};

enum SliceFieldFrameType {
    Unknown,
    Frame,
    TopField,
    BottomField
};

enum MarkScope {
    All,
    Top,
    Bottom
};

class Slice : public std::enable_shared_from_this<Slice> {
public:
    friend class MacroBlock;

    Slice() = default;
    Slice(std::shared_ptr<NalUnit::RbspData> rbsp);
    int parse_slice_header(VideoDecoder* decoder);

    int parse_slice_data(VideoDecoder* decoder);

    void log();
    void log_header();

    int parse_ref_pic_list_modification();
    int parse_pred_weight_table();
    int parse_dec_ref_pic_marking();

    bool bottom_field_pic_order_in_frame_present();

    enum SliceFieldFrameType slice_field_frame_type()
    {
        if (is_frame_)
            return SliceFieldFrameType::Frame;
        else if (is_top_field_)
            return SliceFieldFrameType::TopField;
        else if (is_bottom_field_)
            return SliceFieldFrameType::BottomField;
        else
            return SliceFieldFrameType::Unknown;
    }

    const char* slice_type_str(enum SliceType type)
    {
        switch (type) {
        case SliceType::P:
        case SliceType::P_high:
            return "P Slice";
        case SliceType::B:
        case SliceType::B_high:
            return "B Slice";
        case SliceType::I:
        case SliceType::I_high:
            return "I Slice";
        case SliceType::SP:
        case SliceType::SP_high:
            return "SP Slice";
        case SliceType::SI:
        case SliceType::SI_high:
            return "SI Slice";
        }
        return "Unknown Slice";
    }

    const char* pic_type_str()
    {
        switch (slice_type_enum_) {
        case SliceType::P:
        case SliceType::P_high:
            return "_P";
        case SliceType::B:
        case SliceType::B_high:
            return "_B";
        case SliceType::I:
        case SliceType::I_high:
            return "_I";
        case SliceType::SP:
        case SliceType::SP_high:
            return "_SP";
        case SliceType::SI:
        case SliceType::SI_high:
            return "_SI";
        }
        return "Unknown Slice";
    }

    bool is_P_slice()
    {
        return (slice_type_enum_ == SliceType::P) || (slice_type_enum_ == SliceType::P_high);
    }

    bool is_B_slice()
    {
        return (slice_type_enum_ == SliceType::B) || (slice_type_enum_ == SliceType::B_high);
    }

    bool is_I_slice()
    {
        return (slice_type_enum_ == SliceType::I) || (slice_type_enum_ == SliceType::I_high);
    }

    bool is_SP_slice()
    {
        return (slice_type_enum_ == SliceType::SP) || (slice_type_enum_ == SliceType::SP_high);
    }

    bool is_P_or_SP()
    {
        return is_P_slice() || is_SP_slice();
    }

    bool is_SI_slice()
    {
        return (slice_type_enum_ == SliceType::SI) || (slice_type_enum_ == SliceType::SI_high);
    }

    bool is_SP_or_SI()
    {
        return is_SP_slice() || is_SI_slice();
    }

    bool is_reference_slice()
    {
        return rbsp_data_->nal_ref_idc() != 0;
    }

    bool is_idr_pic()
    {
        return rbsp_data_->idr_pic_flag();
    }

    bool long_term_reference_flag()
    {
        return long_term_reference_flag_;
    }

    bool adaptive_ref_pic_marking_mode_flag()
    {
        return adaptive_ref_pic_marking_mode_flag_;
    }

    int max_num_ref_frames()
    {
        return sps_->max_num_ref_frames();
    }

    void mark_as_long_or_short_term_ref(enum MarkScope scope, bool long_term)
    {
        enum RefStatus status = long_term ? RefStatus::LongTerm : RefStatus::ShortTerm;

        switch (scope) {
        case MarkScope::All: {
            if (is_frame_) {
                frame_ref_status_ = status;
                top_field_ref_status_ = status;
                bottom_field_ref_status_ = status;
            } else if (is_top_field_) { // note: currently not consider complementary pair
                top_field_ref_status_ = status;
            } else if (is_bottom_field_) {
                bottom_field_ref_status_ = status;
            }

            break;
        }
        case MarkScope::Top: {

            if (is_frame_) {
                top_field_ref_status_ = status;
                if (top_field_ref_status_ == bottom_field_ref_status_)
                    frame_ref_status_ = top_field_ref_status_;
            } else if (is_top_field_) {
                top_field_ref_status_ = status;
            } else if (is_bottom_field_) {
            }

            break;
        }
        case MarkScope::Bottom: {
            if (is_frame_) {
                bottom_field_ref_status_ = status;
                if (top_field_ref_status_ == bottom_field_ref_status_)
                    frame_ref_status_ = top_field_ref_status_;
            } else if (is_top_field_) {
            } else if (is_bottom_field_) {
                bottom_field_ref_status_ = status;
            }
            break;
        }
        default:
            assert(false);
            break;
        }
    }

    void mark_as_non_ref(enum MarkScope scope)
    {
        switch (scope) {
        case MarkScope::All: {
            if (is_frame_) {
                frame_ref_status_ = RefStatus::NonRef;
                top_field_ref_status_ = RefStatus::NonRef;
                bottom_field_ref_status_ = RefStatus::NonRef;
            } else {
                assert(false);
            }
            break;
        }
        default:
            assert(false);
            break;
        }
    }

    bool has_mm_op_5()
    {
        return has_mm_op_5_;
    }

    void update_poc_when_has_mm_op_5()
    {
        if (has_mm_op_5_) {
            int tempPicOrderCnt = PicOrderCnt();

            if (is_frame_) {
                TopFieldOrderCnt_ -= tempPicOrderCnt;
                BottomFieldOrderCnt_ -= tempPicOrderCnt;
            } else if (is_top_field_) {
                TopFieldOrderCnt_ -= tempPicOrderCnt;
            } else if (is_bottom_field_) {
                BottomFieldOrderCnt_ -= tempPicOrderCnt;
            }
        }
    }

    void update_prev_frame_num_or_prev_ref_pic_poc(VideoDecoder* vdec);

    void set_long_term_frame_idx(int idx)
    {
        LongTermFrameIdx_ = idx;
    }

    int long_term_frame_idx()
    {
        return LongTermFrameIdx_;
    }

    bool MbaffFrameFlag()
    {
        return !field_pic_flag_ && sps_->MbaffFlag();
    }

    int PicHeightInMbs()
    {
        return sps_->FrameHeightInMbs() / (1 + field_pic_flag_);
    }

    int PicHeightInSamplesL()
    {
        return PicHeightInMbs() * 16;
    }

    int PicHeightInSamplesC()
    {
        return PicHeightInMbs() * sps_->MbHeightC();
    }

    int PicSizeInMbs()
    {
        return PicHeightInMbs() * sps_->PicWidthInMbs();
    }

    int MaxPicNum()
    {
        return field_pic_flag_ ? 2 * sps_->MaxFrameNum() : sps_->MaxFrameNum();
    }

    int CurrPicNum()
    {
        return field_pic_flag_ ? 2 * FrameNum() + 1 : FrameNum();
    }

    int FrameNum()
    {
        return frame_num_;
    }

    int num_ref_idx_l0_active_minus1()
    {
        return final_num_ref_idx_l0_active_minus1_;
    }

    int num_ref_idx_l1_active_minus1()
    {
        return final_num_ref_idx_l1_active_minus1_;
    }

    int SliceQPY()
    {
        return 26 + pps_->pic_init_qp_minus26() + slice_qp_delta_;
    }

    int QSY()
    {
        return 26 + pps_->pic_init_qs_minus26() + slice_qs_delta_;
    }

    int FilterOffsetA()
    {
        return slice_alpha_c0_offset_div2_ << 1;
    }

    int FilterOffsetB()
    {
        return slice_beta_offset_div2_ << 1;
    }

    bool field_pic_flag()
    {
        return field_pic_flag_;
    }

    bool mb_field_decoding_flag()
    {
        return mb_field_decoding_flag_;
    }

    bool bottom_field_flag()
    {
        return bottom_field_flag_;
    }

    /* Decoding process for picture order count
     * POC is decoded for every slice
     */
    void cal_poc(VideoDecoder* dec);

    void cal_poc_0(const std::pair<int, int>& prev_ref_pic_poc);

    void update_prev_poc_0(std::pair<int, int>& prev_ref_pic_poc);

    void cal_poc_1(const std::pair<int, int>& prev_frame_num);

    void update_prev_frame_num_1_or_2(std::pair<int, int>& prev_frame_num);

    void cal_poc_2(const std::pair<int, int>& prev_frame_num);

    void re_cal_poc_when_has_mm5();

    bool at_least_one_short_term_ref();

    bool at_least_one_long_term_ref();

    bool at_least_one_ref();

    bool check_ref_status();

    // ref pic list construct
    /* pic num is calculated for every ref pic
     */
    int decoding_process_for_picture_numbers(std::shared_ptr<Slice> current_slice);
    int PicOrderCnt();

    int PicOrderCntOrLongTermPicNum();
    void set_ref_list_P(std::vector<std::tuple<int, std::shared_ptr<Slice>>> ref_list_P_0);
    void set_ref_list_B(
        std::pair<
            std::vector<std::tuple<int, std::shared_ptr<Slice>>>,
            std::vector<std::tuple<int, std::shared_ptr<Slice>>>>
            ref_list_B_pair);

    void modification_process_for_reference_picture_lists();

    int modification_process_of_reference_picture_lists_for_short_term_reference_pictures(
        int refIdxLX,
        std::vector<std::tuple<int, std::shared_ptr<Slice>>>& RefPicListX,
        std::vector<std::tuple<int, std::shared_ptr<Slice>>>& RefPicListX_init,
        int& picNumLXPred,
        int num_ref_idx_lX_active_minus1,
        int modification_of_pic_nums_idc,
        int abs_diff_pic_num_minus1);

    int modification_process_of_reference_picture_lists_for_long_term_reference_pictures(
        int refIdxLX,
        std::vector<std::tuple<int, std::shared_ptr<Slice>>>& RefPicListX,
        std::vector<std::tuple<int, std::shared_ptr<Slice>>>& RefPicListX_init,
        int num_ref_idx_lX_active_minus1,
        int long_term_pic_num);

    void adaptive_memory_control_decoded_reference_picture_marking_process(VideoDecoder* vdec);

    int NextMbAddress(int CurrMbAddr);

    Sps* get_sps()
    {
        return sps_.get();
    }

    Pps* get_pps()
    {
        return pps_.get();
    }

    void add_MacroBlock(int CurrMbAddr, std::shared_ptr<MacroBlock> mb);

    // geometric method
    int InverseRasterScan(int a, int b, int c, int d, int e)
    {
        if (e == 0)
            return (a % (d / b)) * b;
        else if (e == 1)
            return (a / (d / b)) * c;
        return INT32_MIN;
    }

    std::pair<int, int> Inverse_macroblock_scanning_process(int mbAddr);

    int get_cavlc_nC_for_luma(int CurrMbAddr, int BlkIdx);

    std::pair<int, int> Inverse_4x4_luma_block_scanning_process(int luma4x4BlkIdx);

    std::pair<int, int> Inverse_4x4_chroma_block_scanning_process(int chroma4x4BlkIdx);

    std::tuple<int, int, int, int>
    Derivation_process_for_neighbouring_macroblock_addresses_and_their_availability(int CurrMbAddr);

    // mbAddrN xW yW
    std::tuple<int, int, int>
    Derivation_process_for_neighbouring_locations(int CurrMbAddr, int xN, int yN, bool is_luma);

    std::pair<int, int>
    Derivation_process_for_neighbouring_macroblocks(int CurrMbAddr, bool is_luma);

    std::tuple<int, int, int, int>
    Derivation_process_for_neighbouring_4x4_luma_blocks(int CurrMbAddr, int luma4x4BlkIdx);

    std::tuple<int, int, int, int>
    Derivation_process_for_neighbouring_4x4_chroma_blocks(int CurrMbAddr, int chroma4x4BlkIdx);

    int Derivation_process_for_4x4_luma_block_indices(int xP, int yP);

    int Derivation_process_for_4x4_chroma_block_indices(int xP, int yP);

    MacroBlock& get_mb_by_addr(int addr);

    bool mb_availability(int mbAddr, int CurrMbAddr);

    int QP_Y_PREV()
    {
        return QP_Y_PREV_;
    }

    void update_QP_Y_PREV(int QP_Y_PREV)
    {
        QP_Y_PREV_ = QP_Y_PREV;
    }

    int get_constructed_luma(int x, int y);

    void set_constructed_luma(int x, int y, int value);

    int get_constructed_chroma(int iCbCr, int x, int y);

    void set_constructed_chroma(int iCbCr, int x, int y, int value);

    void write_yuv(std::string file_name);

    int get_constructed_luma_or_chroma(
        DataType data_type,
        int x,
        int y);

    void set_constructed_luma_or_chroma(
        DataType data_type,
        int x,
        int y,
        int value);

    int Clip3(int x, int y, int z)
    {
        if (z < x)
            return x;
        else if (z > y)
            return y;
        else
            return z;
    }

    int Clip1Y(int x);

    int Clip1C(int x);

    std::tuple<
        std::array<int, 3>,
        std::array<int, 3>>
    Filtering_process_for_edges_for_bS_equal_to_4(
        int (&p)[4],
        int (&q)[4],
        bool chromaEdgeFlag,
        bool chromaStyleFilteringFlag,
        int a,
        int b);

    std::tuple<
        std::array<int, 3>,
        std::array<int, 3>>
    Filtering_process_for_edges_with_bS_less_than_4(
        int (&p)[4],
        int (&q)[4],
        bool chromaEdgeFlag,
        bool chromaStyleFilteringFlag,
        int bS,
        int b,
        int indexA);

    std::tuple<bool, int, int, int>
    Derivation_process_for_the_thresholds_for_each_block_edge(
        int p0, int q0, int p1, int q1,
        bool chromaEdgeFlag, int bS,
        int filterOffsetA, int filterOffsetB, int qPp, int qPq);

    int Derivation_process_for_the_luma_bs(
        bool verticalEdgeFlag,
        int x_of_p0, /* the x coordinate in the total picture */
        int y_of_p0,
        int x_of_q0,
        int y_of_q0);

    void Deblocking_filter_process();

    void Filtering_process_for_block_edges(
        bool chromaEdgeFlag,
        int iCbCr,
        bool verticalEdgeFlag,
        bool fieldModeInFrameFilteringFlag,
        const std::vector<int>& xE,
        const std::vector<int>& yE,
        int CurrMbAddr);

    std::tuple<
        std::array<int, 3>,
        std::array<int, 3>>
    Filtering_process_for_a_set_of_samples_across_a_horizontal_or_vertical_block_edge(
        bool chromaEdgeFlag,
        bool verticalEdgeFlag,
        int (&p)[4],
        int (&q)[4],
        int CurrMbAddr,
        int x_of_p0, /* the x coordinate in the total picture */
        int y_of_p0,
        int x_of_q0,
        int y_of_q0,
        bool is_cb);

    int get_mb_addr_from_x_y(int x, int y);

    MacroBlock& get_mb_from_x_y(int x, int y);

    bool direct_spatial_mv_pred_flag()
    {
        return direct_spatial_mv_pred_flag_;
    }

    Slice* get_RefPicList0(int idx);
    Slice* get_RefPicList1(int idx);

    bool mb_adaptive_frame_field_flag();

    int find_lowest_Slice_idx_in_RefPicList0(Slice* refPicCol);

    int DiffPicOrderCnt(Slice* a, Slice* b);

private:
    std::shared_ptr<NalUnit::RbspData> rbsp_data_;
    std::shared_ptr<Pps> pps_;
    std::shared_ptr<Sps> sps_;
    /* Slice header fields */
    int first_mb_in_slice_;
    int slice_type_;
    int pic_parameter_set_id_;

    int colour_plane_id_ = INT32_MIN;
    int frame_num_;

    int field_pic_flag_ = 0;
    int bottom_field_flag_ = 0;

    int idr_pic_id_ = INT32_MIN;

    int pic_order_cnt_lsb_ = INT32_MIN;
    int delta_pic_order_cnt_bottom_ = 0;

    int delta_pic_order_cnt_0_ = 0;
    int delta_pic_order_cnt_1_ = 0;

    int redundant_pic_cnt_ = INT32_MIN;

    int direct_spatial_mv_pred_flag_ = 0;

    int num_ref_idx_active_override_flag_ = 0;
    int num_ref_idx_l0_active_minus1_ = INT32_MIN;
    int num_ref_idx_l1_active_minus1_ = INT32_MIN;

    int cabac_init_idc_ = INT32_MIN;

    int slice_qp_delta_;

    int sp_for_switch_flag_ = 0;

    int slice_qs_delta_ = INT32_MIN;

    // need more accurate initial value
    int disable_deblocking_filter_idc_ = 0;
    int slice_alpha_c0_offset_div2_ = 0;
    int slice_beta_offset_div2_ = 0;

    int ref_pic_list_modification_flag_l0_ = 0;
    int ref_pic_list_modification_flag_l1_ = 0;

    std::vector<std::tuple<int, int>> ref_pic_list_modification_l0_;
    std::vector<std::tuple<int, int>> ref_pic_list_modification_l1_;

    int no_output_of_prior_pics_flag_ = 0,
        long_term_reference_flag_ = 0,
        adaptive_ref_pic_marking_mode_flag_ = 0;
    std::vector<std::tuple<int, int, int>> memory_management_control_operation_list_;

    int parse_single_ref_pic_list_modification(int idx);
    int parse_single_pred_weight_table(int idx);

    enum SliceType slice_type_enum_;

    int final_num_ref_idx_l0_active_minus1_ = INT32_MIN;
    int final_num_ref_idx_l1_active_minus1_ = INT32_MIN;

    int luma_log2_weight_denom_ = INT32_MIN;
    int chroma_log2_weight_denom_ = INT32_MIN;

    std::vector<int> luma_weight_l0_, luma_offset_l0_;
    std::vector<int> chroma_weight_l0_Cb_, chroma_offset_l0_Cb_;
    std::vector<int> chroma_weight_l0_Cr_, chroma_offset_l0_Cr_;

    std::vector<int> luma_weight_l1_, luma_offset_l1_;
    std::vector<int> chroma_weight_l1_Cb_, chroma_offset_l1_Cb_;
    std::vector<int> chroma_weight_l1_Cr_, chroma_offset_l1_Cr_;

    bool is_frame_ = false,
         is_top_field_ = false,
         is_bottom_field_ = false;

    RefStatus frame_ref_status_ = RefStatus::NonRef,
              top_field_ref_status_ = RefStatus::NonRef,
              bottom_field_ref_status_ = RefStatus::NonRef;

    bool has_mm_op_5_ = false;
    int TopFieldOrderCnt_ = 0, BottomFieldOrderCnt_ = 0;

    int PicOrderCntMsb_ = 0;
    int FrameNumOffset_ = 0;

    // ref pic list construct
    int LongTermFrameIdx_ = 0;
    std::vector<std::tuple<int, std::shared_ptr<Slice>>>
        ref_list_P_0_,
        ref_list_B_0_,
        ref_list_B_1_,
        ref_list_P_0_init_,
        ref_list_B_0_init_,
        ref_list_B_1_init_;
    int picNumL0Pred_ = INT32_MIN, picNumL1Pred_ = INT32_MIN;
    std::vector<std::tuple<int, std::shared_ptr<Slice>>>*RefPicList0_, *RefPicList1_;

    // yuv data
    std::vector<int> yuv_data_;
    int *y_data_, *u_data_, *v_data_;

    int mb_field_decoding_flag_;

    std::map<int, std::shared_ptr<MacroBlock>> mb_map_;

    int QP_Y_PREV_ = INT32_MIN;

    std::map<std::tuple<bool, int, int>, int> bs_map_;
};
