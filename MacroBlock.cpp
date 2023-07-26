
#include "Slice.h"

namespace {
std::map<enum MbType, MbTypeDesc> mb_type_desc_map = {
    // I
    { MbType::I_NxN,
        {
            MbType::I_NxN, // enum MbType mb_type
            "I_NxN", // const char* name
            MbPartPredMode::Intra_4x4_Or_Intra_8x8, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_0_0_0,
        {
            MbType::I_16x16_0_0_0, // enum MbType mb_type
            "I_16x16_0_0_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_1_0_0,
        {
            MbType::I_16x16_1_0_0, // enum MbType mb_type
            "I_16x16_1_0_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_2_0_0,
        {
            MbType::I_16x16_2_0_0, // enum MbType mb_type
            "I_16x16_2_0_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_3_0_0,
        {
            MbType::I_16x16_3_0_0, // enum MbType mb_type
            "I_16x16_3_0_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_0_1_0,
        {
            MbType::I_16x16_0_1_0, // enum MbType mb_type
            "I_16x16_0_1_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_1_1_0,
        {
            MbType::I_16x16_1_1_0, // enum MbType mb_type
            "I_16x16_1_1_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_2_1_0,
        {
            MbType::I_16x16_2_1_0, // enum MbType mb_type
            "I_16x16_2_1_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_3_1_0,
        {
            MbType::I_16x16_3_1_0, // enum MbType mb_type
            "I_16x16_3_1_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_0_2_0,
        {
            MbType::I_16x16_0_2_0, // enum MbType mb_type
            "I_16x16_0_2_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_1_2_0,
        {
            MbType::I_16x16_1_2_0, // enum MbType mb_type
            "I_16x16_1_2_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_2_2_0,
        {
            MbType::I_16x16_2_2_0, // enum MbType mb_type
            "I_16x16_2_2_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_3_2_0,
        {
            MbType::I_16x16_3_2_0, // enum MbType mb_type
            "I_16x16_3_2_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_0_0_1,
        {
            MbType::I_16x16_0_0_1, // enum MbType mb_type
            "I_16x16_0_0_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_1_0_1,
        {
            MbType::I_16x16_1_0_1, // enum MbType mb_type
            "I_16x16_1_0_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_2_0_1,
        {
            MbType::I_16x16_2_0_1, // enum MbType mb_type
            "I_16x16_2_0_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_3_0_1,
        {
            MbType::I_16x16_3_0_1, // enum MbType mb_type
            "I_16x16_3_0_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_0_1_1,
        {
            MbType::I_16x16_0_1_1, // enum MbType mb_type
            "I_16x16_0_1_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_1_1_1,
        {
            MbType::I_16x16_1_1_1, // enum MbType mb_type
            "I_16x16_1_1_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_2_1_1,
        {
            MbType::I_16x16_2_1_1, // enum MbType mb_type
            "I_16x16_2_1_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_3_1_1,
        {
            MbType::I_16x16_3_1_1, // enum MbType mb_type
            "I_16x16_3_1_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_0_2_1,
        {
            MbType::I_16x16_0_2_1, // enum MbType mb_type
            "I_16x16_0_2_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_1_2_1,
        {
            MbType::I_16x16_1_2_1, // enum MbType mb_type
            "I_16x16_1_2_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_2_2_1,
        {
            MbType::I_16x16_2_2_1, // enum MbType mb_type
            "I_16x16_2_2_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_16x16_3_2_1,
        {
            MbType::I_16x16_3_2_1, // enum MbType mb_type
            "I_16x16_3_2_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::I_PCM,
        {
            MbType::I_PCM, // enum MbType mb_type
            "I_PCM", // const char* name
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },

    // SI

    { MbType::MbTypeSI,
        {
            MbType::MbTypeSI, // enum MbType mb_type
            "SI", // const char* name
            MbPartPredMode::Intra_4x4, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },

    // P or SP

    { MbType::P_L0_16x16,
        {
            MbType::P_L0_16x16, // enum MbType mb_type
            "P_L0_16x16", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::P_L0_L0_16x8,
        {
            MbType::P_L0_L0_16x8, // enum MbType mb_type
            "P_L0_L0_16x8", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            16, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::P_L0_L0_8x16,
        {
            MbType::P_L0_L0_8x16, // enum MbType mb_type
            "P_L0_L0_8x16", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            8, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::P_8x8,
        {
            MbType::P_8x8, // enum MbType mb_type
            "P_8x8", // const char* name
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            4, // int NumMbPart
            8, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::P_8x8ref0,
        {
            MbType::P_8x8ref0, // enum MbType mb_type
            "P_8x8ref0", // const char* name
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            4, // int NumMbPart
            8, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::P_Skip,
        {
            MbType::P_Skip, // enum MbType mb_type
            "P_Skip", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },

    // B
    { MbType::B_Direct_16x16,
        {
            MbType::B_Direct_16x16, // enum MbType mb_type
            "B_Direct_16x16", // const char* name
            MbPartPredMode::Direct, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            8, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::B_L0_16x16,
        {
            MbType::B_L0_16x16, // enum MbType mb_type
            "B_L0_16x16", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_L1_16x16,
        {
            MbType::B_L1_16x16, // enum MbType mb_type
            "B_L1_16x16", // const char* name
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_Bi_16x16,
        {
            MbType::B_Bi_16x16, // enum MbType mb_type
            "B_Bi_16x16", // const char* name
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_L0_L0_16x8,
        {
            MbType::B_L0_L0_16x8, // enum MbType mb_type
            "B_L0_L0_16x8", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            16, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::B_L0_L0_8x16,
        {
            MbType::B_L0_L0_8x16, // enum MbType mb_type
            "B_L0_L0_8x16", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            8, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_L1_L1_16x8,
        {
            MbType::B_L1_L1_16x8, // enum MbType mb_type
            "B_L1_L1_16x8", // const char* name
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            16, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::B_L1_L1_8x16,
        {
            MbType::B_L1_L1_8x16, // enum MbType mb_type
            "B_L1_L1_8x16", // const char* name
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            8, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_L0_L1_16x8,
        {
            MbType::B_L0_L1_16x8, // enum MbType mb_type
            "B_L0_L1_16x8", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            16, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::B_L0_L1_8x16,
        {
            MbType::B_L0_L1_8x16, // enum MbType mb_type
            "B_L0_L1_8x16", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            8, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_L1_L0_16x8,
        {
            MbType::B_L1_L0_16x8, // enum MbType mb_type
            "B_L1_L0_16x8", // const char* name
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            16, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::B_L1_L0_8x16,
        {
            MbType::B_L1_L0_8x16, // enum MbType mb_type
            "B_L1_L0_8x16", // const char* name
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            8, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_L0_Bi_16x8,
        {
            MbType::B_L0_Bi_16x8, // enum MbType mb_type
            "B_L0_Bi_16x8", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            16, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::B_L0_Bi_8x16,
        {
            MbType::B_L0_Bi_8x16, // enum MbType mb_type
            "B_L0_Bi_8x16", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            8, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_L1_Bi_16x8,
        {
            MbType::B_L1_Bi_16x8, // enum MbType mb_type
            "B_L1_Bi_16x8", // const char* name
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            16, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::B_L1_Bi_8x16,
        {
            MbType::B_L1_Bi_8x16, // enum MbType mb_type
            "B_L1_Bi_8x16", // const char* name
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            8, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_Bi_L0_16x8,
        {
            MbType::B_Bi_L0_16x8, // enum MbType mb_type
            "B_Bi_L0_16x8", // const char* name
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            16, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::B_Bi_L0_8x16,
        {
            MbType::B_Bi_L0_8x16, // enum MbType mb_type
            "B_Bi_L0_8x16", // const char* name
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            8, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_Bi_L1_16x8,
        {
            MbType::B_Bi_L1_16x8, // enum MbType mb_type
            "B_Bi_L1_16x8", // const char* name
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            16, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::B_Bi_L1_8x16,
        {
            MbType::B_Bi_L1_8x16, // enum MbType mb_type
            "B_Bi_L1_8x16", // const char* name
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::Pred_L1, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            8, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_Bi_Bi_16x8,
        {
            MbType::B_Bi_Bi_16x8, // enum MbType mb_type
            "B_Bi_Bi_16x8", // const char* name
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            16, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::B_Bi_Bi_8x16,
        {
            MbType::B_Bi_Bi_8x16, // enum MbType mb_type
            "B_Bi_Bi_8x16", // const char* name
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::BiPred, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            2, // int NumMbPart
            8, // int MbPartWidth
            16, // int MbPartHeight
        } },
    { MbType::B_8x8,
        {
            MbType::B_8x8, // enum MbType mb_type
            "B_8x8", // const char* name
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            4, // int NumMbPart
            8, // int MbPartWidth
            8, // int MbPartHeight
        } },
    { MbType::B_Skip,
        {
            MbType::B_Skip, // enum MbType mb_type
            "B_Skip", // const char* name
            MbPartPredMode::Direct, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            8, // int MbPartWidth
            8, // int MbPartHeight
        } },

    // NA
    { MbType::MbTypeNA,
        {
            MbType::MbTypeNA, // enum MbType mb_type
            "NA", // const char* name
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::MbPartPredModeNA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
};

// enum SubMbType sub_mb_type;
// const char *name;
// int NumSubMbPart;
// enum SubMbPredMode sub_mb_part_pred_mode;
// int SubMbPartWidth;
// int SubMbPartHeight;

std::map<enum SubMbType, SubMbTypeDesc> sub_mb_type_desc_map = {
    { SubMbType::P_L0_8x8,
        { SubMbType::P_L0_8x8,
            "P_L0_8x8",
            1,
            SubMbPredMode::Pred_L0,
            8,
            8 } },
};

}

void MacroBlock::determine_mb_type()
{
    if (slice_->is_I_slice()) {
        mb_type_proxy_ = MbTypeProxy { (enum MbType)mb_type_, slice_ };
    } else if (slice_->is_SI_slice()) {
        if (mb_type_ < 1) {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ + 100), slice_ };
        } else {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ - 1), slice_ };
        }
    } else if (slice_->is_P_slice() || slice_->is_SP_slice()) {
        if (mb_type_ < 5) {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ + 200), slice_ };
        } else {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ - 5), slice_ };
        }
    } else if (slice_->is_B_slice()) {
        if (mb_type_ < 23) {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ + 300), slice_ };
        } else {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ - 23), slice_ };
        }
    }
}

void MacroBlock::parse_MacroBlock()
{
    mb_type_ = rbsp_data_->read_ue();
    determine_mb_type();
}