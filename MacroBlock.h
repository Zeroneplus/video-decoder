
#include "NalUnit.h"

class Slice;

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
    MbTypeSI = 100, // TODO

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
    MbTypeNA = 400, // TODO
};

enum class MbPartPredMode {
    MbPartPredModeNA, // TODO
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
    Direct,
    Pred_L0,
    Pred_L1,
    BiPred,
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
    MbTypeProxy(enum MbType mb_type, Slice* slice)
        : mb_type_(mb_type)
        , slice_(slice)
    {
    }

private:
    enum MbType mb_type_ { MbType::MbTypeNA };
    Slice* slice_ { nullptr };
};

class MacroBlock {

public:
    MacroBlock() = default;

    MacroBlock(Slice* slice, std::shared_ptr<NalUnit::RbspData> rbsp_data)
        : slice_(slice)
        , rbsp_data_(rbsp_data)
    {
    }

    void parse_MacroBlock();

    void determine_mb_type();

private:
    Slice* slice_;
    std::shared_ptr<NalUnit::RbspData> rbsp_data_;

    int mb_type_;

    MbTypeProxy mb_type_proxy_;
};