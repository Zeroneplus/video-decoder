#pragma once

#include "NalUnit.h"

class Slice;
class Sps;
class Pps;
class MacroBlock;

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

    std::tuple<int, int>
    Derivation_process_for_macroblock_and_sub_macroblock_partition_indices(int xP, int yP);

    std::tuple<std::tuple<int, int, int>,
        std::tuple<int, int, int>,
        std::tuple<int, int, int>,
        std::tuple<int, int, int>>
    Derivation_process_for_neighbouring_partitions(int mbPartIdx, int subMbPartIdx);

    int get_nN(int idx, bool is_luma, int chroma_idx);

    bool not_available_due_to_constrained_intra(int mbAddrN);

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

    int prev_intra8x8_pred_mode_flag_[4] = { 0 };
    int rem_intra8x8_pred_mode_[4] = { 0 };

    int intra_chroma_pred_mode_ = INT32_MIN;

    int ref_idx_l0_[4] = { 0 };
    int ref_idx_l1_[4] = { 0 };

    int mvd_l0_[4][4][2] = { 0 };
    int mvd_l1_[4][4][2] = { 0 };

    SubMbTypeProxy sub_mb_type_proxy_[4];

    int i16x16DClevel_[16] = { 0 };
    int i16x16AClevel_[16][15] = { 0 };
    int level4x4_[16][16] = { 0 };
    int level8x8_[4][64] = { 0 };

    std::vector<int> i16x16AClevel_non_zeros_ { std::vector<int>(16, INT32_MIN) };
    std::vector<int> level4x4_non_zeros_ { std::vector<int>(16, INT32_MIN) };

    int (&Intra16x16DCLevel_)[16] = i16x16DClevel_;
    int (&Intra16x16ACLevel_)[16][15] = i16x16AClevel_;
    int (&LumaLevel4x4_)[16][16] = level4x4_;
    int (&LumaLevel8x8_)[4][64] = level8x8_;

    int ChromaDCLevel_[2][16] = { 0 };
    int ChromaACLevel_[2][16][15] = { 0 };

    std::vector<std::vector<int>> ChromaACLevel_non_zeros_ {
        std::vector<std::vector<int>>(2, std::vector<int>(16, INT32_MIN))
    };
};