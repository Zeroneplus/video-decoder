#pragma once

#include <functional>

#include "NalUnit.h"

class Slice;
class Sps;
class Pps;
class MacroBlock;

enum class ResidualData {
    luma,
    cb,
    cr
};

enum class PicConsType {
    _16x16_luma,
    _16x16_chroma,
    _4x4_luma,
    _4x4_chroma
};

enum class ResidualType {
    i16x16DClevel,
    i16x16AClevel,
    ChromaDCLevel_Cb,
    ChromaDCLevel_Cr,
    ChromaACLevel_Cb,
    ChromaACLevel_Cr,
    level4x4,
    level8x8, // unused in cavlc
};

enum class MbType {
    // I
    I_NxN = 0,
    I_16x16_0_0_0,
    I_16x16_1_0_0,
    I_16x16_2_0_0,
    I_16x16_3_0_0,
    I_16x16_0_1_0,
    I_16x16_1_1_0,
    I_16x16_2_1_0,
    I_16x16_3_1_0,
    I_16x16_0_2_0,
    I_16x16_1_2_0,
    I_16x16_2_2_0,
    I_16x16_3_2_0,
    I_16x16_0_0_1,
    I_16x16_1_0_1,
    I_16x16_2_0_1,
    I_16x16_3_0_1,
    I_16x16_0_1_1,
    I_16x16_1_1_1,
    I_16x16_2_1_1,
    I_16x16_3_1_1,
    I_16x16_0_2_1,
    I_16x16_1_2_1,
    I_16x16_2_2_1,
    I_16x16_3_2_1,
    I_PCM,

    // SI
    SI = 100,

    // P or SP
    P_L0_16x16 = 200,
    P_L0_L0_16x8,
    P_L0_L0_8x16,
    P_8x8,
    P_8x8ref0,
    P_Skip,

    // B
    B_Direct_16x16 = 300,
    B_L0_16x16,
    B_L1_16x16,
    B_Bi_16x16,
    B_L0_L0_16x8,
    B_L0_L0_8x16,
    B_L1_L1_16x8,
    B_L1_L1_8x16,
    B_L0_L1_16x8,
    B_L0_L1_8x16,
    B_L1_L0_16x8,
    B_L1_L0_8x16,
    B_L0_Bi_16x8,
    B_L0_Bi_8x16,
    B_L1_Bi_16x8,
    B_L1_Bi_8x16,
    B_Bi_L0_16x8,
    B_Bi_L0_8x16,
    B_Bi_L1_16x8,
    B_Bi_L1_8x16,
    B_Bi_Bi_16x8,
    B_Bi_Bi_8x16,
    B_8x8,
    B_Skip,

    // NA
    NA = 400,
};

enum class MbPartPredMode {
    NA,
    Intra_4x4,
    Intra_8x8,
    Intra_4x4_Or_Intra_8x8, // not determined
    Intra_16x16,
    Pred_L0,
    Pred_L1,
    BiPred,
    Direct,
};

struct MbTypeDesc {
    enum MbType mb_type;
    const char* name;
    enum MbPartPredMode mb_part_pred_mode_0;
    enum MbPartPredMode mb_part_pred_mode_1;
    int Intra16x16PredMode;
    int CodedBlockPatternChroma;
    int CodedBlockPatternLuma;
    int NumMbPart;
    int MbPartWidth;
    int MbPartHeight;
};

enum class SubMbType {
    // P
    P_L0_8x8 = 0,
    P_L0_8x4,
    P_L0_4x8,
    P_L0_4x4,
    P_inferred,
    // B
    B_Direct_8x8 = 100,
    B_L0_8x8,
    B_L1_8x8,
    B_Bi_8x8,
    B_L0_8x4,
    B_L0_4x8,
    B_L1_8x4,
    B_L1_4x8,
    B_Bi_8x4,
    B_Bi_4x8,
    B_L0_4x4,
    B_L1_4x4,
    B_Bi_4x4,
    B_inferred,

    // NA
    NA = 200,
};

enum class SubMbPredMode {
    NA,
    Pred_L0,
    Pred_L1,
    BiPred,
    Direct,
};

struct SubMbTypeDesc {
    enum SubMbType sub_mb_type;
    const char* name;
    int NumSubMbPart;
    enum SubMbPredMode sub_mb_part_pred_mode;
    int SubMbPartWidth;
    int SubMbPartHeight;
};

// use macro so we can convert between 4x4 and 8x8
//
#define Intra_4x4_Vertical 0
#define Intra_4x4_Horizontal 1
#define Intra_4x4_DC 2
#define Intra_4x4_Diagonal_Down_Left 3
#define Intra_4x4_Diagonal_Down_Right 4
#define Intra_4x4_Vertical_Right 5
#define Intra_4x4_Horizontal_Down 6
#define Intra_4x4_Vertical_Left 7
#define Intra_4x4_Horizontal_Up 8

#define Intra_8x8_Vertical 0
#define Intra_8x8_Horizontal 1
#define Intra_8x8_DC 2
#define Intra_8x8_Diagonal_Down_Left 3
#define Intra_8x8_Diagonal_Down_Right 4
#define Intra_8x8_Vertical_Right 5
#define Intra_8x8_Horizontal_Down 6
#define Intra_8x8_Vertical_Left 7
#define Intra_8x8_Horizontal_Up 8

#define Intra_16x16_Vertical 0
#define Intra_16x16_Horizontal 1
#define Intra_16x16_DC 2
#define Intra_16x16_Plane 3

#define Intra_Chroma_DC 0
#define Intra_Chroma_Horizontal 1
#define Intra_Chroma_Vertical 2
#define Intra_Chroma_Plane 3

#define One_To_One 0
#define Frm_To_Fld 1
#define Fld_To_Frm 2

#define FLD 0
#define AFRM 1
#define FRM 2

class MbTypeProxy {
public:
    MbTypeProxy() = default;
    MbTypeProxy(enum MbType mb_type, Slice* slice, MacroBlock* mb);

    enum MbType mb_type()
    {
        return mb_type_;
    }

    enum MbPartPredMode MbPartPredMode_0();
    enum MbPartPredMode MbPartPredMode_1();

    // this function should only be called in MacroBlock::parse_mb_pred()
    enum MbPartPredMode MbPartPredModeByIdx(int idx);

    int NumMbPart();

    int CodedBlockPatternLuma();
    int CodedBlockPatternChroma();

    int Intra16x16PredMode();

    const char* name();

    int MbPartWidth();
    int MbPartHeight();

    bool is_intra_pred();

    // P_Skip is not partitioned
    //
    // B_Skip and B_Direct_16x16 are partitioned but do not have NumMbPart()
    bool no_subMbPartIdx()
    {
        return mb_type_ != MbType::P_8x8
            && mb_type_ != MbType::P_8x8ref0
            && mb_type_ != MbType::B_8x8
            && mb_type_ != MbType::B_Skip
            && mb_type_ != MbType::B_Direct_16x16;
    }

private:
    enum MbType mb_type_ { MbType::NA };
    Slice* slice_ { nullptr };
    MacroBlock* mb_ { nullptr };
    Sps* sps_ { nullptr };
    Pps* pps_ { nullptr };
};

class SubMbTypeProxy {

public:
    SubMbTypeProxy() = default;

    SubMbTypeProxy(enum SubMbType sub_mb_type)
        : sub_mb_type_(sub_mb_type)
    {
    }

    SubMbTypeProxy& operator=(const SubMbTypeProxy&) = default;

    int NumSubMbPart();

    int SubMbPartWidth();
    int SubMbPartHeight();

    enum SubMbType sub_mb_type()
    {
        return sub_mb_type_;
    }

    enum SubMbPredMode SubMbPredMode();

private:
    enum SubMbType sub_mb_type_ { SubMbType::NA };
};

class MacroBlock {

public:
    friend class Slice;

    MacroBlock() = default;

    MacroBlock(Slice* slice, std::shared_ptr<NalUnit::RbspData> rbsp_data, int CurrMbAddr);

    void parse_MacroBlock();

    void determine_mb_type();

    void parse_mb_pred();

    void parse_sub_mb_pred();

    void parse_residual(int startIdx, int endIdx);

    bool transform_size_8x8_flag()
    {
        return transform_size_8x8_flag_;
    }

    void set_coded_block_pattern()
    {
        CodedBlockPatternLuma_ = coded_block_pattern_ % 16;
        CodedBlockPatternChroma_ = coded_block_pattern_ / 16;
    }

    int CodedBlockPatternLuma()
    {
        return CodedBlockPatternLuma_;
    }

    int CodedBlockPatternChroma()
    {
        return CodedBlockPatternChroma_;
    }

    void residual_luma(int startIdx, int endIdx);

    void residual_block(int* coeffLevel,
        int startIdx,
        int endIdx,
        int maxNumCoeff,
        enum ResidualType,
        int BlkIdx);

    int InverseRasterScan(int a, int b, int c, int d, int e)
    {
        if (e == 0)
            return (a % (d / b)) * b;
        else if (e == 1)
            return (a / (d / b)) * c;
        return INT32_MIN;
    }

    std::pair<int, int> Inverse_macroblock_partition_scanning_process(int mbPartIdx);
    std::pair<int, int> Inverse_sub_macroblock_partition_scanning_process(int mbPartIdx, int subMbPartIdx);

    bool is_intra_pred();
    bool is_inter_pred();

    std::tuple<int, int>
    Derivation_process_for_macroblock_and_sub_macroblock_partition_indices(int xP, int yP);

    std::tuple<std::tuple<int, int, int>,
        std::tuple<int, int, int>,
        std::tuple<int, int, int>,
        std::tuple<int, int, int>>
    Derivation_process_for_neighbouring_partitions(int mbPartIdx, int subMbPartIdx, SubMbType currSubMbType);

    int get_nN(int idx, bool is_luma, int chroma_idx);

    bool not_available_due_to_constrained_intra(int mbAddrN);

    int QPY();

    int QPPrimeY();

    bool TransformBypassModeFlag();

    void Derivation_process_for_Intra4x4PredMode(int luma4x4BlkIdx);

    int getIntra4x4PredMode(int luma4x4BlkIdxN);

    int getIntra8x8PredModefor4x4Blk(int luma4x4BlkIdxN);

    std::vector<std::vector<int>> Intra_4x4_sample_prediction(int luma4x4BlkIdx);

    bool is_field_macroblock();

    bool is_frame_macroblock();

    bool has_luma_transform_coefficient_levels_in_x_y(int x, int y);

    enum MbType mb_type();

    enum MbPartPredMode MbPartPredMode_0();

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

    std::vector<std::vector<int>>
    Intra_16x16_prediction_process_for_luma_samples();

    void Intra_prediction_process_for_chroma_samples();

    std::vector<std::vector<int>>
    Intra_prediction_process_for_chroma_sample(int idx);

    void Sample_construction_process_for_I_PCM_macroblocks();

    void Intra_4x4_prediction_process_for_luma_samples();

    void Specification_of_transform_decoding_process_for_4x4_luma_residual_blocks(
        const std::vector<std::vector<int>>& predL,
        int luma4x4BlkIdx);

    void Specification_of_transform_decoding_process_for_luma_samples_of_Intra_16x16_macroblock_prediction_mode(
        const std::vector<std::vector<int>>& predL);

    void Specification_of_transform_decoding_process_for_chroma_samples(
        const std::vector<std::vector<int>>& predC,
        bool is_cb);

    std::vector<std::vector<int>>
    Inverse_scanning_process_for_4x4_transform_coefficients_and_scaling_lists(
        const int* input, bool is_zig_zag);

    int Derivation_process_for_chroma_quantisation_parameters(
        bool is_cb,
        bool is_SP_or_SI);

    int deblock_QPC(bool is_cb);

    std::function<int(int, int, int)>
    Derivation_process_for_scaling_functions(
        ResidualData residual_type,
        bool is_4x4);

    std::vector<std::vector<int>>
    Scaling_and_transformation_process_for_DC_transform_coefficients_for_Intra_16x16_macroblock_type(
        int bitDepth,
        int qP,
        const std::vector<std::vector<int>>& c);

    std::vector<std::vector<int>>
    Scaling_and_transformation_process_for_chroma_DC_transform_coefficients(
        const std::vector<std::vector<int>>& c,
        bool is_cb);

    std::vector<std::vector<int>>
    Transformation_process_for_chroma_DC_transform_coefficients(
        const std::vector<std::vector<int>>& c,
        bool is_cb);

    std::vector<std::vector<int>>
    Scaling_process_for_chroma_DC_transform_coefficients(
        const std::vector<std::vector<int>>& f,
        int bitDepth,
        int qP,
        bool is_cb);

    std::vector<std::vector<int>>
    Scaling_and_transformation_process_for_residual_4x4_blocks(
        const std::vector<std::vector<int>>& c,
        ResidualData residual_type);

    std::vector<std::vector<int>>
    Scaling_process_for_residual_4x4_blocks(
        int bitDepth,
        int qP,
        const std::vector<std::vector<int>>& c,
        ResidualData residual_type);

    std::vector<std::vector<int>>
    Transformation_process_for_residual_4x4_blocks(
        int bitDepth,
        const std::vector<std::vector<int>>& d);

    void Picture_construction_process_prior_to_deblocking_filter_process(
        PicConsType BlkType,
        int BlkIdx,
        const std::vector<std::vector<int>>& u,
        bool is_cb);

    bool is_in_SP_or_SI();

    int FilterOffsetA();
    int FilterOffsetB();

    void parse_P_Skip();

    void parse_B_Skip();

private:
    Slice* slice_;
    std::shared_ptr<NalUnit::RbspData> rbsp_data_;

    Sps* sps_;
    Pps* pps_;

    int CurrMbAddr_;

    int mb_type_ { INT32_MIN };

    MbTypeProxy mb_type_proxy_;

    int pcm_sample_luma_[256] = { 0 };
    std::vector<int> pcm_sample_chroma_;

    int transform_size_8x8_flag_ = 0;
    int coded_block_pattern_ = 0;
    int noSubMbPartSizeLessThan8x8Flag_ = 1;

    int CodedBlockPatternLuma_ = 0;
    int CodedBlockPatternChroma_ = 0;

    int mb_qp_delta_ = 0;

    int prev_intra4x4_pred_mode_flag_[16] = { 0 };
    int rem_intra4x4_pred_mode_[16] = { 0 };

    std::vector<int> Intra4x4PredMode_ { std::vector<int>(16, INT32_MIN) };

    int prev_intra8x8_pred_mode_flag_[4] = { 0 };
    int rem_intra8x8_pred_mode_[4] = { 0 };

    std::vector<int> Intra8x8PredMode_ { std::vector<int>(4, INT32_MIN) };

    int intra_chroma_pred_mode_ = INT32_MIN;

    int ref_idx_l0_[4] = { 0 };
    int ref_idx_l1_[4] = { 0 };

    int mvd_l0_[4][4][2] = { 0 };
    int mvd_l1_[4][4][2] = { 0 };

    SubMbTypeProxy sub_mb_type_proxy_[4];

    int i16x16DClevel_[16] = { 0 };
    int i16x16AClevel_[16][16] = { 0 }; // [16][15]
    int level4x4_[16][16] = { 0 };
    int level8x8_[4][64] = { 0 };

    std::vector<int> i16x16AClevel_non_zeros_ { std::vector<int>(16, INT32_MIN) };
    std::vector<int> level4x4_non_zeros_ { std::vector<int>(16, INT32_MIN) };

    int (&Intra16x16DCLevel_)[16] = i16x16DClevel_;
    int (&Intra16x16ACLevel_)[16][16] = i16x16AClevel_; // [16][15]
    int (&LumaLevel4x4_)[16][16] = level4x4_;
    int (&LumaLevel8x8_)[4][64] = level8x8_;

    int ChromaDCLevel_[2][16] = { 0 };
    int ChromaACLevel_[2][16][16] = { 0 }; // [2][16][15]

    std::vector<std::vector<int>> ChromaACLevel_non_zeros_ {
        std::vector<std::vector<int>>(2, std::vector<int>(16, INT32_MIN))
    };

    int QPY_internal_ = INT32_MIN;

    int MvL0_[4][4][2] = { 0 };
    int MvL1_[4][4][2] = { 0 };
    int RefIdxL0_[4] = { 0 };
    int RefIdxL1_[4] = { 0 };
    int PredFlagL0_[4] = { 0 };
    int PredFlagL1_[4] = { 0 };

public:
    int get_predFlag(int listSuffixFlag, int mbPartIdx)
    {
        if (listSuffixFlag == 0)
            return get_PredFlagL0(mbPartIdx);
        else if (listSuffixFlag == 1)
            return get_PredFlagL1(mbPartIdx);

        assert(false);
        return INT32_MIN;
    }
    int get_PredFlagL0(int mbPartIdx)
    {
        return PredFlagL0_[mbPartIdx];
    }
    int get_PredFlagL1(int mbPartIdx)
    {
        return PredFlagL1_[mbPartIdx];
    }

    int get_RefIdx(int listSuffixFlag, int mbPartIdx)
    {
        if (listSuffixFlag == 0)
            return get_RefIdxL0(mbPartIdx);
        else if (listSuffixFlag == 1)
            return get_RefIdxL1(mbPartIdx);

        assert(false);
        return INT32_MIN;
    }

    int get_RefIdxL0(int mbPartIdx)
    {
        return RefIdxL0_[mbPartIdx];
    }
    int get_RefIdxL1(int mbPartIdx)
    {
        return RefIdxL1_[mbPartIdx];
    }

    using array2 = int (&)[2];

    array2 get_Mv(int listSuffixFlag, int mbPartIdx, int subMbPartIdx)
    {
        static int tmp[2] = { INT32_MIN, INT32_MIN };
        if (listSuffixFlag == 0)
            return get_MvL0(mbPartIdx, subMbPartIdx);
        else if (listSuffixFlag == 1)
            return get_MvL1(mbPartIdx, subMbPartIdx);

        assert(false);
        return tmp;
    }

    array2 get_MvL0(int mbPartIdx, int subMbPartIdx)
    {
        return MvL0_[mbPartIdx][subMbPartIdx];
    }
    array2 get_MvL1(int mbPartIdx, int subMbPartIdx)
    {
        return MvL1_[mbPartIdx][subMbPartIdx];
    }

    int Median(int x, int y, int z)
    {
        return x + y + z
            - std::min(x, std::min(y, z))
            - std::max(x, std::max(y, z));
    }

    int DiffPicOrderCnt(Slice* a, Slice* b);

    Slice* get_RefPicList0_by_part(int mbPartIdx, int subMbPartIdx);
    Slice* get_RefPicList1_by_part(int mbPartIdx, int subMbPartIdx);
    int get_PredFlagL0_by_part(int mbPartIdx, int subMbPartIdx);
    int get_PredFlagL1_by_part(int mbPartIdx, int subMbPartIdx);
    int get_MvL0_x_by_part(int mbPartIdx, int subMbPartIdx);
    int get_MvL0_y_by_part(int mbPartIdx, int subMbPartIdx);
    int get_MvL1_x_by_part(int mbPartIdx, int subMbPartIdx);
    int get_MvL1_y_by_part(int mbPartIdx, int subMbPartIdx);

public:
    void Inter_prediction_process();

    std::tuple<std::vector<std::vector<int>>,
        std::vector<std::vector<int>>,
        std::vector<std::vector<int>>,
        std::array<int, 2>,
        std::array<int, 2>,
        std::array<int, 2>,
        std::array<int, 2>,
        int, int, int, int>
    derive_predict(
        int mbPartIdx,
        int subMbPartIdx,
        bool has_sub_idx,
        int partWidth,
        int partHeight,
        int partWidthC,
        int partHeightC,
        /* output */
        int& MvCnt);

    std::tuple<
        std::array<int, 2>, std::array<int, 2>,
        std::array<int, 2>, std::array<int, 2>,
        int, int,
        int, int,
        int>
    Derivation_process_for_motion_vector_components_and_reference_indices(
        int mbPartIdx,
        int subMbPartIdx,
        bool has_sub_idx);

    std::tuple<std::array<int, 2>, int>
    Derivation_process_for_luma_motion_vectors_for_skipped_macroblocks_in_P_and_SP_slices();

    std::tuple<int, int,
        std::array<int, 2>, std::array<int, 2>,
        int, int, int>
    Derivation_process_for_luma_motion_vectors_for_B_Skip_B_Direct_16x16_and_B_Direct_8x8(
        int mbPartIdx,
        int subMbPartIdx);

    std::tuple<Slice*, int, std::array<int, 2>, int, int, Slice*>
    Derivation_process_for_the_co_located_4x4_sub_macroblock_partitions(
        int mbPartIdx,
        int subMbPartIdx);

    std::tuple<
        int, int,
        std::array<int, 2>, std::array<int, 2>,
        int, int, int>
    Derivation_process_for_spatial_direct_luma_motion_vector_and_reference_index_prediction_mode(
        int mbPartIdx,
        int subMbPartIdx);

    std::tuple<
        std::array<int, 2>, std::array<int, 2>,
        int, int,
        int, int>
    Derivation_process_for_temporal_direct_luma_motion_vector_and_reference_index_prediction_mode(
        int mbPartIdx,
        int subMbPartIdx);

    std::tuple<
        std::array<int, 2>,
        std::array<int, 2>>
    Derivation_process_for_luma_motion_vector_prediction(
        int mbPartIdx,
        int subMbPartIdx,
        int refIdxL0,
        int refIdxL1,
        SubMbType currSubMbType);

    std::array<int, 2>
    Derivation_process_for_luma_motion_vector_prediction_single(
        int mbPartIdx,
        int subMbPartIdx,
        int refIdxLX,
        SubMbType currSubMbType,
        int listSuffixFlag);

    std::array<int, 2>
    Derivation_process_for_median_luma_motion_vector_prediction(
        int mbAddrA,
        int mbPartIdxA,
        int subMbPartIdxA,
        std::array<int, 2> mvLXA,
        int refIdxLXA,

        int mbAddrB,
        int mbPartIdxB,
        int subMbPartIdxB,
        std::array<int, 2> mvLXB,
        int refIdxLXB,

        int mbAddrC,
        int mbPartIdxC,
        int subMbPartIdxC,
        std::array<int, 2> mvLXC,
        int refIdxLXC,

        int refIdxLX);

    std::tuple<
        std::tuple<int, int, int, std::tuple<std::array<int, 2>, int>>,
        std::tuple<int, int, int, std::tuple<std::array<int, 2>, int>>,
        std::tuple<int, int, int, std::tuple<std::array<int, 2>, int>>>
    Derivation_process_for_motion_data_of_neighbouring_partitions(
        int mbPartIdx,
        int subMbPartIdx,
        SubMbType currSubMbType,
        int listSuffixFlag);

    std::array<int, 2>
    Derivation_process_for_chroma_motion_vectors(
        std::array<int, 2> mvLX,
        int refIdxLX,
        int listSuffixFlag /* currently unused */);

    std::tuple<
        std::tuple<int, int, int, int, int>,
        std::tuple<int, int, int, int, int>,
        std::tuple<int, int, int, int, int>>
    Derivation_process_for_prediction_weights(
        int refIdxL0, int refIdxL1,
        int predFlagL0, int predFlagL1);

    std::tuple<
        std::vector<std::vector<int>>,
        std::vector<std::vector<int>>,
        std::vector<std::vector<int>>>
    Decoding_process_for_Inter_prediction_samples(
        int mbPartIdx,
        int subMbPartIdx,
        int partWidth,
        int partHeight,
        int partWidthC,
        int partHeightC,
        std::array<int, 2> mvL0,
        std::array<int, 2> mvL1,
        std::array<int, 2> mvCL0,
        std::array<int, 2> mvCL1,
        int refIdxL0,
        int refIdxL1,
        int predFlagL0,
        int predFlagL1,
        std::tuple<
            std::tuple<int, int, int, int, int>,
            std::tuple<int, int, int, int, int>,
            std::tuple<int, int, int, int, int>>
            weight_info);

    Slice* Reference_picture_selection_process(
        int refIdxLX,
        int listSuffixFlag);

    void Fractional_sample_interpolation_process(
        int mbPartIdx,
        int subMbPartIdx,
        int partWidth,
        int partHeight,
        int partWidthC,
        int partHeightC,
        std::array<int, 2> mvLX,
        std::array<int, 2> mvCLX,
        Slice* refPicLX,
        int listSuffixFlag,

        /* output */
        std::vector<std::vector<int>>& predPartLXL,
        std::vector<std::vector<int>>& predPartLXCb,
        std::vector<std::vector<int>>& predPartLXCr);

    int Luma_sample_interpolation_process(
        int xIntL, int yIntL,
        int xFracL, int yFracL,
        Slice* refPicLX);

    int Chroma_sample_interpolation_process(
        int xIntC, int yIntC,
        int xFracC, int yFracC,
        Slice* refPicLX,
        int iCbCr);

    std::tuple<
        std::vector<std::vector<int>>,
        std::vector<std::vector<int>>,
        std::vector<std::vector<int>>>
    Weighted_sample_prediction_process_all(
        int mbPartIdx,
        int subMbPartIdx,
        int predFlagL0,
        int predFlagL1,
        const std::vector<std::vector<int>>& predPartL0L,
        const std::vector<std::vector<int>>& predPartL1L,
        const std::vector<std::vector<int>>& predPartL0Cb,
        const std::vector<std::vector<int>>& predPartL1Cb,
        const std::vector<std::vector<int>>& predPartL0Cr,
        const std::vector<std::vector<int>>& predPartL1Cr,
        int logWDL, int w0L, int w1L, int o0L, int o1L,
        int logWDCb, int w0Cb, int w1Cb, int o0Cb, int o1Cb,
        int logWDCr, int w0Cr, int w1Cr, int o0Cr, int o1Cr);

    std::vector<std::vector<int>>
    Default_weighted_sample_prediction_process(
        int predFlagL0,
        int predFlagL1,
        const std::vector<std::vector<int>>& predPartL0C,
        const std::vector<std::vector<int>>& predPartL1C,
        int logWDC, int w0C, int w1C, int o0C, int o1C);

    std::vector<std::vector<int>>
    Weighted_sample_prediction_process(
        int predFlagL0,
        int predFlagL1,
        const std::vector<std::vector<int>>& predPartL0C,
        const std::vector<std::vector<int>>& predPartL1C,
        int logWDC, int w0C, int w1C, int o0C, int o1C,
        bool is_luma);
};