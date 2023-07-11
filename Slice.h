#pragma once

#include "NalUnit.h"

class VideoDecoder;

class Slice {
public:
    Slice() = default;
    Slice(std::shared_ptr<NalUnit::RbspData> rbsp);
    int parse_slice_header(VideoDecoder* decoder);

    void log();
    void log_header();

    int parse_ref_pic_list_modification();
    int parse_pred_weight_table();
    int parse_dec_ref_pic_marking(bool IdrPic);

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

private:
    std::shared_ptr<NalUnit::RbspData> rbsp_data;
    /* Slice header fields */
    int first_mb_in_slice_;
    int slice_type_;
    int pic_parameter_set_id_;

    int colour_plane_id_ = -1;
    int frame_num_;

    int field_pic_flag_ = 0;
    int bottom_field_flag_ = 0;

    int idr_pic_id_ = -1;

    int pic_order_cnt_lsb_ = -1;
    int delta_pic_order_cnt_bottom_ = INT32_MIN;
    int delta_pic_order_cnt_0_ = INT32_MIN;
    int delta_pic_order_cnt_1_ = INT32_MIN;

    int redundant_pic_cnt_ = -1;

    int direct_spatial_mv_pred_flag_ = 0;

    int num_ref_idx_active_override_flag_ = 0;
    int num_ref_idx_l0_active_minus1_ = INT32_MIN;
    int num_ref_idx_l1_active_minus1_ = INT32_MIN;

    int cabac_init_idc_ = INT32_MIN;

    int slice_qp_delta_;

    int sp_for_switch_flag_ = 0;

    int slice_qs_delta_ = INT32_MIN;

    int disable_deblocking_filter_idc_ = INT32_MIN;
    int slice_alpha_c0_offset_div2_ = INT32_MIN;
    int slice_beta_offset_div2_ = INT32_MIN;

    int ref_pic_list_modification_flag_l0_ = 0;
    int ref_pic_list_modification_flag_l1_ = 0;

    std::vector<std::tuple<int, int>> ref_pic_list_modification_l0_;
    std::vector<std::tuple<int, int>> ref_pic_list_modification_l1_;

    int no_output_of_prior_pics_flag_ = 0,
        long_term_reference_flag_ = 0,
        adaptive_ref_pic_marking_mode_flag_ = 0;
    std::vector<std::tuple<int, int, int>> memory_management_control_operation_list_;

    int parse_single_ref_pic_list_modification(int idx);

public:
    enum SliceType slice_type;
};
