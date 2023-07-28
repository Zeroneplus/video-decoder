
#include "NalUnit.h"

class Slice;
class Sps;
class Pps;
class MacroBlock;

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

    enum MbPartPredMode MbPartPredModeByIdx(int idx);
    int NumMbPart();

    int CodedBlockPatternLuma();
    int CodedBlockPatternChroma();

    int Intra16x16PredMode();

    const char* name();

private:
    enum MbType mb_type_ { MbType::NA };
    Slice* slice_ { nullptr };
    MacroBlock* mb_;
    Sps* sps_;
    Pps* pps_;
};

class MacroBlock {

public:
    MacroBlock() = default;

    MacroBlock(Slice* slice, std::shared_ptr<NalUnit::RbspData> rbsp_data);

    void parse_MacroBlock();

    void determine_mb_type();

    void parse_mb_pred();

    void parse_sub_mb_pred();

    void parse_residual();

    bool transform_size_8x8_flag()
    {
        return transform_size_8x8_flag_;
    }

    void set_coded_block_pattern();

    int CodedBlockPatternLuma()
    {
        return CodedBlockPatternLuma_;
    }

    int CodedBlockPatternChroma()
    {
        return CodedBlockPatternChroma_;
    }

private:
    Slice* slice_;
    std::shared_ptr<NalUnit::RbspData> rbsp_data_;

    Sps* sps_;
    Pps* pps_;

    int mb_type_ { INT32_MIN };

    MbTypeProxy mb_type_proxy_;

    std::vector<int> pcm_sample_luma_;
    std::vector<int> pcm_sample_chroma_;

    int transform_size_8x8_flag_ = 0;
    int coded_block_pattern_ = 0;

    int CodedBlockPatternLuma_ = 0;
    int CodedBlockPatternChroma_ = 0;

    int mb_qp_delta_ = 0;

    int prev_intra4x4_pred_mode_flag_[16] = { 0 };
    int rem_intra4x4_pred_mode_[16] = { 0 };

    int prev_intra8x8_pred_mode_flag_[4] = { 0 };
    int rem_intra8x8_pred_mode_[4] = { 0 };

    int intra_chroma_pred_mode_ = 0;

    int ref_idx_l0_[4] = { 0 };
    int ref_idx_l1_[4] = { 0 };
};