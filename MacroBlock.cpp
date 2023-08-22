
#include "Cavlc.h"
#include "Slice.h"
#include "spdlog/spdlog.h"

namespace {

// for Intra pred, NumMbPart MbPartWidth MbPartHeight is not defined
std::map<enum MbType, MbTypeDesc> mb_type_desc_map = {
    // I
    { MbType::I_NxN,
        {
            MbType::I_NxN, // enum MbType mb_type
            "I_NxN", // const char* name
            MbPartPredMode::Intra_4x4_Or_Intra_8x8, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_0_0_0,
        {
            MbType::I_16x16_0_0_0, // enum MbType mb_type
            "I_16x16_0_0_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_1_0_0,
        {
            MbType::I_16x16_1_0_0, // enum MbType mb_type
            "I_16x16_1_0_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_2_0_0,
        {
            MbType::I_16x16_2_0_0, // enum MbType mb_type
            "I_16x16_2_0_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_3_0_0,
        {
            MbType::I_16x16_3_0_0, // enum MbType mb_type
            "I_16x16_3_0_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_0_1_0,
        {
            MbType::I_16x16_0_1_0, // enum MbType mb_type
            "I_16x16_0_1_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_1_1_0,
        {
            MbType::I_16x16_1_1_0, // enum MbType mb_type
            "I_16x16_1_1_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_2_1_0,
        {
            MbType::I_16x16_2_1_0, // enum MbType mb_type
            "I_16x16_2_1_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_3_1_0,
        {
            MbType::I_16x16_3_1_0, // enum MbType mb_type
            "I_16x16_3_1_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_0_2_0,
        {
            MbType::I_16x16_0_2_0, // enum MbType mb_type
            "I_16x16_0_2_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_1_2_0,
        {
            MbType::I_16x16_1_2_0, // enum MbType mb_type
            "I_16x16_1_2_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_2_2_0,
        {
            MbType::I_16x16_2_2_0, // enum MbType mb_type
            "I_16x16_2_2_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_3_2_0,
        {
            MbType::I_16x16_3_2_0, // enum MbType mb_type
            "I_16x16_3_2_0", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            0, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_0_0_1,
        {
            MbType::I_16x16_0_0_1, // enum MbType mb_type
            "I_16x16_0_0_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_1_0_1,
        {
            MbType::I_16x16_1_0_1, // enum MbType mb_type
            "I_16x16_1_0_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_2_0_1,
        {
            MbType::I_16x16_2_0_1, // enum MbType mb_type
            "I_16x16_2_0_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_3_0_1,
        {
            MbType::I_16x16_3_0_1, // enum MbType mb_type
            "I_16x16_3_0_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            0, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_0_1_1,
        {
            MbType::I_16x16_0_1_1, // enum MbType mb_type
            "I_16x16_0_1_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_1_1_1,
        {
            MbType::I_16x16_1_1_1, // enum MbType mb_type
            "I_16x16_1_1_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_2_1_1,
        {
            MbType::I_16x16_2_1_1, // enum MbType mb_type
            "I_16x16_2_1_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_3_1_1,
        {
            MbType::I_16x16_3_1_1, // enum MbType mb_type
            "I_16x16_3_1_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            1, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_0_2_1,
        {
            MbType::I_16x16_0_2_1, // enum MbType mb_type
            "I_16x16_0_2_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            0, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_1_2_1,
        {
            MbType::I_16x16_1_2_1, // enum MbType mb_type
            "I_16x16_1_2_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            1, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_2_2_1,
        {
            MbType::I_16x16_2_2_1, // enum MbType mb_type
            "I_16x16_2_2_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            2, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_16x16_3_2_1,
        {
            MbType::I_16x16_3_2_1, // enum MbType mb_type
            "I_16x16_3_2_1", // const char* name
            MbPartPredMode::Intra_16x16, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            3, // int Intra16x16PredMode
            2, // int CodedBlockPatternChroma
            15, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
    { MbType::I_PCM,
        {
            MbType::I_PCM, // enum MbType mb_type
            "I_PCM", // const char* name
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },

    // SI

    { MbType::SI,
        {
            MbType::SI, // enum MbType mb_type
            "SI", // const char* name
            MbPartPredMode::Intra_4x4, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },

    // P or SP

    { MbType::P_L0_16x16,
        {
            MbType::P_L0_16x16, // enum MbType mb_type
            "P_L0_16x16", // const char* name
            MbPartPredMode::Pred_L0, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
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
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
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
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
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
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
        } },

    // B

    // B_Direct_16x16 does not have NumMbPart, but has MbPartWidth and MbPartHeight
    { MbType::B_Direct_16x16,
        {
            MbType::B_Direct_16x16, // enum MbType mb_type
            "B_Direct_16x16", // const char* name
            MbPartPredMode::Direct, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
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
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
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
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
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
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
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
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
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
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            8, // int MbPartWidth
            8, // int MbPartHeight
        } },

    // NA
    { MbType::NA,
        {
            MbType::NA, // enum MbType mb_type
            "NA", // const char* name
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_0
            MbPartPredMode::NA, // enum MbPartPredMode mb_part_pred_mode_1
            INT32_MIN, // int Intra16x16PredMode
            INT32_MIN, // int CodedBlockPatternChroma
            INT32_MIN, // int CodedBlockPatternLuma
            INT32_MIN, // int NumMbPart
            INT32_MIN, // int MbPartWidth
            INT32_MIN, // int MbPartHeight
        } },
};

std::map<enum SubMbType, SubMbTypeDesc> sub_mb_type_desc_map = {
    // P
    { SubMbType::P_L0_8x8,
        { SubMbType::P_L0_8x8,
            "P_L0_8x8",
            1,
            SubMbPredMode::Pred_L0,
            8,
            8 } },
    { SubMbType::P_L0_8x4,
        { SubMbType::P_L0_8x4,
            "P_L0_8x4",
            2,
            SubMbPredMode::Pred_L0,
            8,
            4 } },
    { SubMbType::P_L0_4x8,
        { SubMbType::P_L0_4x8,
            "P_L0_4x8",
            2,
            SubMbPredMode::Pred_L0,
            4,
            8 } },
    { SubMbType::P_L0_4x4,
        { SubMbType::P_L0_4x4,
            "P_L0_4x4",
            4,
            SubMbPredMode::Pred_L0,
            4,
            4 } },
    { SubMbType::P_inferred,
        { SubMbType::P_inferred,
            "P_inferred",
            INT32_MIN,
            SubMbPredMode::NA,
            INT32_MIN,
            INT32_MIN } },

    // B
    { SubMbType::B_Direct_8x8,
        { SubMbType::B_Direct_8x8,
            "B_Direct_8x8",
            4,
            SubMbPredMode::Direct,
            4,
            4 } },
    { SubMbType::B_L0_8x8,
        { SubMbType::B_L0_8x8,
            "B_L0_8x8",
            1,
            SubMbPredMode::Pred_L0,
            8,
            8 } },
    { SubMbType::B_L1_8x8,
        { SubMbType::B_L1_8x8,
            "B_L1_8x8",
            1,
            SubMbPredMode::Pred_L1,
            8,
            8 } },
    { SubMbType::B_Bi_8x8,
        { SubMbType::B_Bi_8x8,
            "B_Bi_8x8",
            1,
            SubMbPredMode::BiPred,
            8,
            8 } },
    { SubMbType::B_L0_8x4,
        { SubMbType::B_L0_8x4,
            "B_L0_8x4",
            2,
            SubMbPredMode::Pred_L0,
            8,
            4 } },
    { SubMbType::B_L0_4x8,
        { SubMbType::B_L0_4x8,
            "B_L0_4x8",
            2,
            SubMbPredMode::Pred_L0,
            4,
            8 } },
    { SubMbType::B_L1_8x4,
        { SubMbType::B_L1_8x4,
            "B_L1_8x4",
            2,
            SubMbPredMode::Pred_L1,
            8,
            4 } },
    { SubMbType::B_L1_4x8,
        { SubMbType::B_L1_4x8,
            "B_L1_4x8",
            2,
            SubMbPredMode::Pred_L1,
            4,
            8 } },
    { SubMbType::B_Bi_8x4,
        { SubMbType::B_Bi_8x4,
            "B_Bi_8x4",
            2,
            SubMbPredMode::BiPred,
            8,
            4 } },
    { SubMbType::B_Bi_4x8,
        { SubMbType::B_Bi_4x8,
            "B_Bi_4x8",
            2,
            SubMbPredMode::BiPred,
            4,
            8 } },
    { SubMbType::B_L0_4x4,
        { SubMbType::B_L0_4x4,
            "B_L0_4x4",
            4,
            SubMbPredMode::Pred_L0,
            4,
            4 } },
    { SubMbType::B_L1_4x4,
        { SubMbType::B_L1_4x4,
            "B_L1_4x4",
            4,
            SubMbPredMode::Pred_L1,
            4,
            4 } },
    { SubMbType::B_Bi_4x4,
        { SubMbType::B_Bi_4x4,
            "B_Bi_4x4",
            4,
            SubMbPredMode::BiPred,
            4,
            4 } },
    { SubMbType::B_inferred,
        { SubMbType::B_inferred,
            "B_inferred",
            4,
            SubMbPredMode::Direct,
            4,
            4 } },
};

}

enum MbPartPredMode MbTypeProxy::MbPartPredMode_0()
{
    if (mb_type_ == MbType::I_NxN) {
        if (mb_->transform_size_8x8_flag())
            return MbPartPredMode::Intra_8x8;
        else
            return MbPartPredMode::Intra_4x4;
    }

    return mb_type_desc_map[mb_type_].mb_part_pred_mode_0;
}

enum MbPartPredMode MbTypeProxy::MbPartPredMode_1()
{
    return mb_type_desc_map[mb_type_].mb_part_pred_mode_1;
}

int MbTypeProxy::NumMbPart()
{
    return mb_type_desc_map[mb_type_].NumMbPart;
}

const char* MbTypeProxy::name()
{
    return mb_type_desc_map[mb_type_].name;
}

MbTypeProxy::MbTypeProxy(enum MbType mb_type, Slice* slice, MacroBlock* mb)
    : mb_type_(mb_type)
    , slice_(slice)
    , mb_(mb)
    , sps_(slice->get_sps())
    , pps_(slice->get_pps())
{
}

MacroBlock::MacroBlock(Slice* slice, std::shared_ptr<NalUnit::RbspData> rbsp_data, int CurrMbAddr)
    : slice_(slice)
    , rbsp_data_(rbsp_data)
    , sps_(slice->get_sps())
    , pps_(slice->get_pps())
    , CurrMbAddr_(CurrMbAddr)
{
}

void MacroBlock::determine_mb_type()
{
    if (slice_->is_I_slice()) {
        mb_type_proxy_ = MbTypeProxy { (enum MbType)mb_type_, slice_, this };
    } else if (slice_->is_SI_slice()) {
        if (mb_type_ < 1) {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ + 100), slice_, this };
        } else {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ - 1), slice_, this };
        }
    } else if (slice_->is_P_slice() || slice_->is_SP_slice()) {
        if (mb_type_ < 5) {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ + 200), slice_, this };
        } else {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ - 5), slice_, this };
        }
    } else if (slice_->is_B_slice()) {
        if (mb_type_ < 23) {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ + 300), slice_, this };
        } else {
            mb_type_proxy_ = MbTypeProxy { (enum MbType)(mb_type_ - 23), slice_, this };
        }
    }
}

void MacroBlock::parse_P_Skip()
{
    mb_type_ = (int)MbType::P_Skip;
    mb_type_proxy_ = MbTypeProxy { (enum MbType)mb_type_, slice_, this };

    // update QP_Y_PREV
    slice_->update_QP_Y_PREV(QPY());
}

void MacroBlock::parse_B_Skip()
{
    mb_type_ = (int)MbType::B_Skip;
    mb_type_proxy_ = MbTypeProxy { (enum MbType)mb_type_, slice_, this };

    // update QP_Y_PREV
    slice_->update_QP_Y_PREV(QPY());
}

// here we parse non-skip MB
void MacroBlock::parse_MacroBlock()
{
    mb_type_ = rbsp_data_->read_ue();
    determine_mb_type();

    spdlog::trace("CurrMbAddr is {}, the MacroBlock type is {}", CurrMbAddr_, mb_type_proxy_.name());

    if (mb_type_proxy_.mb_type() == MbType::I_PCM) {
        // TODO: fully test I_PCM
        assert(false);

        while (!rbsp_data_->byte_aligned())
            rbsp_data_->read_u1();

        for (int i = 0; i < 256; i++) {
            pcm_sample_luma_[i] = rbsp_data_->read_u(sps_->bit_depth_luma());
        }

        for (int i = 0; i < 2 * sps_->MbHeightC() * sps_->MbWidthC(); i++) {
            pcm_sample_chroma_.push_back(rbsp_data_->read_u(sps_->bit_depth_chroma()));
        }
    } else {
        // here call MbPartPredMode_0() is safe, as the MbType::I_NxN has been excluded,
        // but NumMbPart() for SI MB is not defined in spec.
        //
        // NOTE: B_Direct_16x16 has NumMbPart N/A
        //
        if (mb_type_proxy_.mb_type() != MbType::I_NxN
            && mb_type_proxy_.MbPartPredMode_0() != MbPartPredMode::Intra_16x16
            && mb_type_proxy_.NumMbPart() == 4) {

            // B_Skip P_Skip is not processed here
            assert(mb_type_proxy_.mb_type() == MbType::P_8x8
                || mb_type_proxy_.mb_type() == MbType::P_8x8ref0
                || mb_type_proxy_.mb_type() == MbType::B_8x8);

            parse_sub_mb_pred();
            for (int mbPartIdx = 0; mbPartIdx < 4; mbPartIdx++) {
                if (sub_mb_type_proxy_[mbPartIdx].sub_mb_type() != SubMbType::B_Direct_8x8) {
                    if (sub_mb_type_proxy_[mbPartIdx].NumSubMbPart() > 1)
                        noSubMbPartSizeLessThan8x8Flag_ = 0;

                } else if (!sps_->direct_8x8_inference_flag())
                    // if sps_->direct_8x8_inference_flag() is true,
                    // B_Direct_8x8 will not be sub partitioned
                    noSubMbPartSizeLessThan8x8Flag_ = 0;
            }
        } else {
            if (pps_->transform_8x8_mode_flag()
                && mb_type_proxy_.mb_type() == MbType::I_NxN)
                transform_size_8x8_flag_ = rbsp_data_->read_u1();
            parse_mb_pred();
        }

        // the residual data for B_Skip P_Skip is 0
        // but maybe not 0 for B_Direct_16x16 and B_Direct_8x8
        //
        if (mb_type_proxy_.MbPartPredMode_0() != MbPartPredMode::Intra_16x16) {

            coded_block_pattern_ = rbsp_data_->read_me(
                mb_type_proxy_.MbPartPredMode_0() != MbPartPredMode::Intra_4x4
                && mb_type_proxy_.MbPartPredMode_0() != MbPartPredMode::Intra_8x8);

            set_coded_block_pattern();

            if (mb_type_proxy_.CodedBlockPatternLuma() > 0
                && pps_->transform_8x8_mode_flag()
                && mb_type_proxy_.mb_type() != MbType::I_NxN
                && noSubMbPartSizeLessThan8x8Flag_
                && (mb_type_proxy_.mb_type() != MbType::B_Direct_16x16
                    || sps_->direct_8x8_inference_flag())) {
                transform_size_8x8_flag_ = rbsp_data_->read_u1();
            }
        }

        spdlog::trace("  CodedBlockPatternLuma {}, CodedBlockPatternChroma {}",
            mb_type_proxy_.CodedBlockPatternLuma(),
            mb_type_proxy_.CodedBlockPatternChroma());

        if (mb_type_proxy_.CodedBlockPatternLuma() > 0
            || mb_type_proxy_.CodedBlockPatternChroma() > 0
            || mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_16x16) {
            mb_qp_delta_ = rbsp_data_->read_se();
            parse_residual(0, 15);
        }
    }

    // decode MB
    if (is_intra_pred()) {
        // luma
        if (mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_4x4) {
            Intra_4x4_prediction_process_for_luma_samples();
        } else if (mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_16x16) {
            auto predL16x16 = Intra_16x16_prediction_process_for_luma_samples();
            Specification_of_transform_decoding_process_for_luma_samples_of_Intra_16x16_macroblock_prediction_mode(predL16x16);
        } else
            assert(false);

        // chroma
        auto predCb = Intra_prediction_process_for_chroma_sample(0);
        Specification_of_transform_decoding_process_for_chroma_samples(
            predCb,
            true);

        auto predCr = Intra_prediction_process_for_chroma_sample(1);
        Specification_of_transform_decoding_process_for_chroma_samples(
            predCr,
            false);
    }

    // update QP_Y_PREV
    slice_->update_QP_Y_PREV(QPY());
}

int MacroBlock::QPY()
{
    if (QPY_internal_ != INT32_MIN)
        return QPY_internal_;

    QPY_internal_ = ((slice_->QP_Y_PREV() + mb_qp_delta_ + 52 + 2 * sps_->QpBdOffsetY())
                        % (52 + sps_->QpBdOffsetY()))
        - sps_->QpBdOffsetY();

    return QPY_internal_;
}

int MacroBlock::QPPrimeY()
{
    return QPY() + sps_->QpBdOffsetY();
}

bool MacroBlock::TransformBypassModeFlag()
{
    if (sps_->qpprime_y_zero_transform_bypass_flag() && QPPrimeY() == 0)
        return true;
    else
        return false;
}

enum MbPartPredMode MbTypeProxy::MbPartPredModeByIdx(int idx)
{
    assert(idx >= 0 && idx < mb_type_desc_map[mb_type_].NumMbPart);

    if (idx == 0) {
        return mb_type_desc_map[mb_type_].mb_part_pred_mode_0;
    } else if (idx == 1)
        return mb_type_desc_map[mb_type_].mb_part_pred_mode_1;
    else {
        assert(false); // TODO
    }
}

void MacroBlock::parse_mb_pred()
{
    if (mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_4x4
        || mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_8x8
        || mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_16x16) {

        if (mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_4x4) {

            for (int luma4x4BlkIdx = 0; luma4x4BlkIdx < 16; luma4x4BlkIdx++) {
                prev_intra4x4_pred_mode_flag_[luma4x4BlkIdx] = rbsp_data_->read_u1();
                if (!prev_intra4x4_pred_mode_flag_[luma4x4BlkIdx])
                    rem_intra4x4_pred_mode_[luma4x4BlkIdx] = rbsp_data_->read_u(3);
            }
        }

        if (mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_8x8) {

            for (int luma8x8BlkIdx = 0; luma8x8BlkIdx < 4; luma8x8BlkIdx++) {
                prev_intra8x8_pred_mode_flag_[luma8x8BlkIdx] = rbsp_data_->read_u1();
                if (!prev_intra8x8_pred_mode_flag_[luma8x8BlkIdx])
                    rem_intra8x8_pred_mode_[luma8x8BlkIdx] = rbsp_data_->read_u(3);
            }
        }

        if (sps_->ChromaArrayType() == 1 || sps_->ChromaArrayType() == 2)
            intra_chroma_pred_mode_ = rbsp_data_->read_ue();

    } else if (mb_type_proxy_.MbPartPredMode_0() != MbPartPredMode::Direct) {
        // if Direct mode, no ref idx or mvd here

        for (int mbPartIdx = 0; mbPartIdx < mb_type_proxy_.NumMbPart(); mbPartIdx++) {

            if ((slice_->num_ref_idx_l0_active_minus1() > 0
                    || slice_->mb_field_decoding_flag() != slice_->field_pic_flag())
                && mb_type_proxy_.MbPartPredModeByIdx(mbPartIdx) != MbPartPredMode::Pred_L1)
                // only consider frame mode
                ref_idx_l0_[mbPartIdx] = rbsp_data_->read_te(slice_->num_ref_idx_l0_active_minus1());
        }

        for (int mbPartIdx = 0; mbPartIdx < mb_type_proxy_.NumMbPart(); mbPartIdx++) {

            if ((slice_->num_ref_idx_l1_active_minus1() > 0 || slice_->mb_field_decoding_flag() != slice_->field_pic_flag())
                && mb_type_proxy_.MbPartPredModeByIdx(mbPartIdx) != MbPartPredMode::Pred_L0)
                ref_idx_l1_[mbPartIdx] = rbsp_data_->read_te(slice_->num_ref_idx_l1_active_minus1());
        }

        for (int mbPartIdx = 0; mbPartIdx < mb_type_proxy_.NumMbPart(); mbPartIdx++) {
            if (mb_type_proxy_.MbPartPredModeByIdx(mbPartIdx) != MbPartPredMode::Pred_L1)
                for (int compIdx = 0; compIdx < 2; compIdx++)
                    mvd_l0_[mbPartIdx][0][compIdx] = rbsp_data_->read_se();
        }

        for (int mbPartIdx = 0; mbPartIdx < mb_type_proxy_.NumMbPart(); mbPartIdx++) {
            if (mb_type_proxy_.MbPartPredModeByIdx(mbPartIdx) != MbPartPredMode::Pred_L0)
                for (int compIdx = 0; compIdx < 2; compIdx++)
                    mvd_l1_[mbPartIdx][0][compIdx] = rbsp_data_->read_se();
        }
    }
}

enum SubMbPredMode SubMbTypeProxy::SubMbPredMode()
{
    return sub_mb_type_desc_map[sub_mb_type_].sub_mb_part_pred_mode;
}

int SubMbTypeProxy::NumSubMbPart()
{
    return sub_mb_type_desc_map[sub_mb_type_].NumSubMbPart;
}

void MacroBlock::parse_sub_mb_pred()
{
    for (int mbPartIdx = 0; mbPartIdx < 4; mbPartIdx++) {
        int sub_mb_type = rbsp_data_->read_ue();
        if (slice_->is_P_slice() || slice_->is_SP_slice())
            sub_mb_type_proxy_[mbPartIdx] = SubMbTypeProxy { static_cast<enum SubMbType>(sub_mb_type) };
        else if (slice_->is_B_slice())
            sub_mb_type_proxy_[mbPartIdx] = SubMbTypeProxy { static_cast<enum SubMbType>(sub_mb_type + 100) };
        else
            assert(false);
    }

    // for P_8x8ref0, ref idx 0 is 0,
    // for B_Direct_8x8, ref idx 0 are inferred
    for (int mbPartIdx = 0; mbPartIdx < 4; mbPartIdx++)
        if ((slice_->num_ref_idx_l0_active_minus1() > 0 || slice_->mb_field_decoding_flag() != slice_->field_pic_flag())
            && mb_type_proxy_.mb_type() != MbType::P_8x8ref0
            && sub_mb_type_proxy_[mbPartIdx].sub_mb_type() != SubMbType::B_Direct_8x8
            && sub_mb_type_proxy_[mbPartIdx].SubMbPredMode() != SubMbPredMode::Pred_L1)
            ref_idx_l0_[mbPartIdx] = rbsp_data_->read_te(slice_->num_ref_idx_l0_active_minus1());

    // for B_Direct_8x8, ref idx 1 are inferred
    for (int mbPartIdx = 0; mbPartIdx < 4; mbPartIdx++)
        if ((slice_->num_ref_idx_l1_active_minus1() > 0 || slice_->mb_field_decoding_flag() != slice_->field_pic_flag())
            && sub_mb_type_proxy_[mbPartIdx].sub_mb_type() != SubMbType::B_Direct_8x8
            && sub_mb_type_proxy_[mbPartIdx].SubMbPredMode() != SubMbPredMode::Pred_L0)
            ref_idx_l1_[mbPartIdx] = rbsp_data_->read_te(slice_->num_ref_idx_l1_active_minus1());

    // for B_Direct_8x8, mvd 0 are inferred
    for (int mbPartIdx = 0; mbPartIdx < 4; mbPartIdx++)
        if (sub_mb_type_proxy_[mbPartIdx].sub_mb_type() != SubMbType::B_Direct_8x8
            && sub_mb_type_proxy_[mbPartIdx].SubMbPredMode() != SubMbPredMode::Pred_L1)
            for (int subMbPartIdx = 0;
                 subMbPartIdx < sub_mb_type_proxy_[mbPartIdx].NumSubMbPart();
                 subMbPartIdx++)
                for (int compIdx = 0; compIdx < 2; compIdx++)
                    mvd_l0_[mbPartIdx][subMbPartIdx][compIdx] = rbsp_data_->read_se();

    // for B_Direct_8x8, mvd 1 are inferred
    for (int mbPartIdx = 0; mbPartIdx < 4; mbPartIdx++)
        if (sub_mb_type_proxy_[mbPartIdx].sub_mb_type() != SubMbType::B_Direct_8x8
            && sub_mb_type_proxy_[mbPartIdx].SubMbPredMode() != SubMbPredMode::Pred_L0)
            for (int subMbPartIdx = 0;
                 subMbPartIdx < sub_mb_type_proxy_[mbPartIdx].NumSubMbPart();
                 subMbPartIdx++)
                for (int compIdx = 0; compIdx < 2; compIdx++)
                    mvd_l1_[mbPartIdx][subMbPartIdx][compIdx] = rbsp_data_->read_se();
}

void MacroBlock::parse_residual(int startIdx, int endIdx)
{
    // only consider cavlc coding

    residual_luma(startIdx, endIdx);

    /*
        0 (0b00) -- All chroma transform coefficient levels are equal to 0.
        1 (0b01) -- One or more chroma DC transform coefficient levels shall be non-zero valued.
                        All chroma AC transform coefficient levels are equal to 0.
        2 (0b10) -- Zero or more chroma DC transform coefficient levels are non-zero valued.
                        One or more chroma AC transform coefficient levels shall be non-zero valued.
    */

    if (sps_->ChromaArrayType() == 1 || sps_->ChromaArrayType() == 2) {
        int NumC8x8 = 4 / (sps_->SubWidthC() * sps_->SubHeightC());
        for (int iCbCr = 0; iCbCr < 2; iCbCr++)
            if ((mb_type_proxy_.CodedBlockPatternChroma() & 3) && startIdx == 0) // 3 -> 0b11
                residual_block(ChromaDCLevel_[iCbCr], 0, 4 * NumC8x8 - 1, 4 * NumC8x8,
                    iCbCr == 0 ? ResidualType::ChromaDCLevel_Cb : ResidualType::ChromaDCLevel_Cr,
                    INT32_MIN /* unused */);
            else {
                // for (int i = 0; i < 4 * NumC8x8; i++)
                //     ChromaDCLevel_[iCbCr][i] = 0;
            }

        for (int iCbCr = 0; iCbCr < 2; iCbCr++)
            for (int i8x8 = 0; i8x8 < NumC8x8; i8x8++) // 8x8 Chroma block in raster scan order
                for (int i4x4 = 0; i4x4 < 4; i4x4++)
                    if (mb_type_proxy_.CodedBlockPatternChroma() & 2) // 2 -> 0b10
                        residual_block(ChromaACLevel_[iCbCr][i8x8 * 4 + i4x4],
                            std::max(0, startIdx - 1), endIdx - 1, 15,
                            iCbCr == 0 ? ResidualType::ChromaACLevel_Cb : ResidualType::ChromaACLevel_Cr,
                            i8x8 * 4 + i4x4);
                    else {
                        // for (int i = 0; i < 15; i++)
                        //     ChromaACLevel_[iCbCr][i8x8 * 4 + i4x4][i] = 0;
                    }

    } else if (sps_->ChromaArrayType() == 3) {
        // TODO
    }
}

int MacroBlock::get_nN(int idx, bool is_luma, int chroma_idx)
{
    bool has_ac_data;

    if (is_luma) {
        int i8x8 = idx / 4;
        has_ac_data = mb_type_proxy_.CodedBlockPatternLuma() & (1 << i8x8);
    } else {
        has_ac_data = mb_type_proxy_.CodedBlockPatternChroma() & 2;
    }

    // TODO: maybe no need to distinguish between has_ac_data and case 3
    //
    if ((mb_type_proxy_.mb_type() == MbType::P_Skip || mb_type_proxy_.mb_type() == MbType::B_Skip)
        || (mb_type_proxy_.mb_type() != MbType::I_PCM && !has_ac_data))
        return 0;
    else if (mb_type_proxy_.mb_type() == MbType::I_PCM)
        return 16;
    else {
        if (is_luma) {
            if (mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_16x16) {
                if (i16x16AClevel_non_zeros_[idx] == INT32_MIN) {
                    int non_zeros = 0;
                    for (int i = 0; i < 15; i++)
                        non_zeros += (i16x16AClevel_[idx][i] ? 1 : 0);
                    i16x16AClevel_non_zeros_[idx] = non_zeros;
                }
                return i16x16AClevel_non_zeros_[idx];
            } else {
                if (level4x4_non_zeros_[idx] == INT32_MIN) {
                    int non_zeros = 0;
                    for (int i = 0; i < 16; i++)
                        non_zeros += (level4x4_[idx][i] ? 1 : 0);
                    level4x4_non_zeros_[idx] = non_zeros;
                }
                return level4x4_non_zeros_[idx];
            }
        } else {
            if (ChromaACLevel_non_zeros_[chroma_idx][idx] == INT32_MIN) {
                int non_zeros = 0;
                for (int i = 0; i < 15; i++)
                    non_zeros += (ChromaACLevel_[chroma_idx][idx][i] ? 1 : 0);
                ChromaACLevel_non_zeros_[chroma_idx][idx] = non_zeros;
            }
            return ChromaACLevel_non_zeros_[chroma_idx][idx];
        }
    }
}

bool MacroBlock::not_available_due_to_constrained_intra(int mbAddrN)
{
    return is_intra_pred()
        && pps_->constrained_intra_pred_flag()
        && !slice_->get_mb_by_addr(mbAddrN).is_intra_pred()
        && rbsp_data_->slice_data_partitioning_is_in_use();
}

// TODO: shall we add iCbCr parameter?
//       No Need to add this, as we already have 'enum ResidualType residual_type'
//
// cavlc coding
void MacroBlock::residual_block(int* coeffLevel,
    int startIdx,
    int endIdx,
    int maxNumCoeff,
    enum ResidualType residual_type,
    int BlkIdx)
{
    int nC = INT32_MIN;

    auto get_nC = [this](int mbAddrA,
                      int BlkIdxA,
                      int mbAddrB,
                      int BlkIdxB,
                      bool is_luma,
                      int chroma_idx) -> int {
        int nC;
        bool availableFlagA = true;
        bool availableFlagB = true;

        if (mbAddrA == INT32_MIN // is this safe? yes
            || not_available_due_to_constrained_intra(mbAddrA))
            availableFlagA = false;

        if (mbAddrB == INT32_MIN
            || not_available_due_to_constrained_intra(mbAddrB))
            availableFlagB = false;

        int nA, nB;
        if (availableFlagA) {
            nA = slice_->get_mb_by_addr(mbAddrA).get_nN(BlkIdxA, is_luma, chroma_idx);
        }
        if (availableFlagB) {
            nB = slice_->get_mb_by_addr(mbAddrB).get_nN(BlkIdxB, is_luma, chroma_idx);
        }

        if (availableFlagA && availableFlagB)
            nC = (nA + nB + 1) >> 1;
        else if (availableFlagA)
            nC = nA;
        else if (availableFlagB)
            nC = nB;
        else
            nC = 0;

        return nC;
    };

    // init all coeffLevel to zero
    for (int i = 0; i < maxNumCoeff; i++)
        coeffLevel[i] = 0;

    switch (residual_type) {
    case ResidualType::ChromaDCLevel_Cb:
    case ResidualType::ChromaDCLevel_Cr: {
        if (sps_->ChromaArrayType() == 1)
            nC = -1;
        else if (sps_->ChromaArrayType() == 2)
            nC = -2;
        else
            assert(false);

        break;
    }
    case ResidualType::i16x16DClevel: // luma
    case ResidualType::i16x16AClevel:
    case ResidualType::level4x4: {

        auto [mbAddrA, luma4x4BlkIdxA, mbAddrB, luma4x4BlkIdxB] = slice_->Derivation_process_for_neighbouring_4x4_luma_blocks(CurrMbAddr_, BlkIdx);

        nC = get_nC(mbAddrA,
            luma4x4BlkIdxA,
            mbAddrB,
            luma4x4BlkIdxB,
            true,
            INT32_MIN);

        break;
    }
    case ResidualType::ChromaACLevel_Cb: // chroma
    case ResidualType::ChromaACLevel_Cr: {
        auto [mbAddrA, chroma4x4BlkIdxA, mbAddrB, chroma4x4BlkIdxB] = slice_->Derivation_process_for_neighbouring_4x4_chroma_blocks(CurrMbAddr_, BlkIdx);

        nC = get_nC(mbAddrA,
            chroma4x4BlkIdxA,
            mbAddrB,
            chroma4x4BlkIdxB,
            false,
            residual_type == ResidualType::ChromaACLevel_Cb ? 0 : 1);

        break;
    }
    default: {
        assert(false);
        break;
    }
    }

    int coeff_token;
    int coeff_token_bit_length;
    int TrailingOnes;
    int TotalCoeff;

    coeff_token = rbsp_data_->peek_u(16);

    coeff_token_table(nC,
        coeff_token,
        coeff_token_bit_length /* int& */,
        TrailingOnes /* int& */,
        TotalCoeff /* int& */);

    rbsp_data_->read_u(coeff_token_bit_length);

    int levelVal[64]; // this should be enough
    int runVal[64];

    if (TotalCoeff > 0) {
        int suffixLength = 0;
        if (TotalCoeff > 10 && TrailingOnes < 3)
            suffixLength = 1;

        for (int i = 0; i < TotalCoeff; i++)
            if (i < TrailingOnes) {
                int trailing_ones_sign_flag = rbsp_data_->read_u1();
                levelVal[i] = 1 - 2 * trailing_ones_sign_flag;
            } else {
                int level_prefix = rbsp_data_->read_level_prefix();
                int levelCode = std::min(15, level_prefix) << suffixLength;

                int levelSuffixSize;
                if (level_prefix == 14 && suffixLength == 0)
                    levelSuffixSize = 4;
                else if (level_prefix >= 15)
                    levelSuffixSize = level_prefix - 3;
                else
                    levelSuffixSize = suffixLength;

                if (suffixLength > 0 || level_prefix >= 14) {
                    assert(levelSuffixSize > 0);
                    levelCode += rbsp_data_->read_u(levelSuffixSize);
                }

                if (level_prefix >= 15 && suffixLength == 0)
                    levelCode += 15;

                if (level_prefix >= 16)
                    levelCode += (1 << (level_prefix - 3)) - 4096;

                if (i == TrailingOnes && TrailingOnes < 3)
                    levelCode += 2;

                if (levelCode % 2 == 0)
                    levelVal[i] = (levelCode + 2) >> 1;
                else
                    levelVal[i] = (-levelCode - 1) >> 1;

                if (suffixLength == 0)
                    suffixLength = 1;

                if (std::abs(levelVal[i]) > (3 << (suffixLength - 1))
                    && suffixLength < 6)
                    suffixLength++;
            }

        int zerosLeft = 0;
        if (TotalCoeff < endIdx - startIdx + 1) {
            int total_zeros;
            get_total_zeros(
                rbsp_data_,
                maxNumCoeff,
                TotalCoeff /* int tzVlcIndex */,
                total_zeros /* int & */);
            zerosLeft = total_zeros;
        }
        for (int i = 0; i < TotalCoeff - 1; i++) {
            if (zerosLeft > 0) {
                int run_before;

                get_run_before(
                    rbsp_data_,
                    zerosLeft,
                    run_before /* int & */);

                runVal[i] = run_before;
            } else
                runVal[i] = 0;
            zerosLeft = zerosLeft - runVal[i];
        }
        runVal[TotalCoeff - 1] = zerosLeft;
        int coeffNum = -1;
        for (int i = TotalCoeff - 1; i >= 0; i--) {
            coeffNum += runVal[i] + 1;
            coeffLevel[startIdx + coeffNum] = levelVal[i];
        }
    }
}

void MacroBlock::residual_luma(int startIdx, int endIdx)
{
    if (startIdx == 0
        && mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_16x16)
        residual_block(i16x16DClevel_, 0, 15, 16, ResidualType::i16x16DClevel, 0);

    for (int i8x8 = 0; i8x8 < 4; i8x8++) {
        // in cavlc coding, even if transform_size_8x8_flag_ is true,
        // the 8x8 block is processed in 4x4 block
        if (!transform_size_8x8_flag_ || !pps_->entropy_coding_mode())
            for (int i4x4 = 0; i4x4 < 4; i4x4++) {
                if (mb_type_proxy_.CodedBlockPatternLuma() & (1 << i8x8))
                    if (mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_16x16)
                        residual_block(i16x16AClevel_[i8x8 * 4 + i4x4],
                            std::max(0, startIdx - 1), endIdx - 1, 15,
                            ResidualType::i16x16AClevel, i8x8 * 4 + i4x4);
                    else
                        residual_block(level4x4_[i8x8 * 4 + i4x4], startIdx, endIdx, 16,
                            ResidualType::level4x4, i8x8 * 4 + i4x4);
                else if (mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_16x16) {
                    // for (int i = 0; i < 15; i++)
                    //     i16x16AClevel_[i8x8 * 4 + i4x4][i] = 0;
                } else {
                    // for (int i = 0; i < 16; i++)
                    //     level4x4_[i8x8 * 4 + i4x4][i] = 0;
                }

                // if transform_size_8x8_flag_, copy from 4x4 to 8x8, so we can use
                // 8x8 in later transform process
                if (!pps_->entropy_coding_mode() && transform_size_8x8_flag_)
                    for (int i = 0; i < 16; i++)
                        level8x8_[i8x8][4 * i + i4x4] = level4x4_[i8x8 * 4 + i4x4][i];
            }
        else if (mb_type_proxy_.CodedBlockPatternLuma() & (1 << i8x8)) {
            // TODO cabac coding
            // residual_block(level8x8_[i8x8], 4 * startIdx, 4 * endIdx + 3, 64, ?, ?);
        } else {
            // for (int i = 0; i < 64; i++)
            //     level8x8_[i8x8][i] = 0;
        }
    }
}

int MbTypeProxy::CodedBlockPatternLuma()
{
    if (MbPartPredMode_0() == MbPartPredMode::Intra_16x16) {
        return mb_type_desc_map[mb_type_].CodedBlockPatternLuma;
    } else
        return mb_->CodedBlockPatternLuma();
}

int MbTypeProxy::CodedBlockPatternChroma()
{
    if (MbPartPredMode_0() == MbPartPredMode::Intra_16x16) {
        return mb_type_desc_map[mb_type_].CodedBlockPatternChroma;
    } else
        return mb_->CodedBlockPatternChroma();
}

int MbTypeProxy::Intra16x16PredMode()
{
    if (MbPartPredMode_0() == MbPartPredMode::Intra_16x16) {
        return mb_type_desc_map[mb_type_].Intra16x16PredMode;
    } else
        return INT32_MIN;
}

// TODO: what if B_Skip or B_Direct_16x16
// it seems that B_Skip and B_Direct_16x16 are 8x8 partitioned and 4x4 sub-partitioned?
// but why the NumMbPart() for B_Skip and B_Direct_16x16 are NA?
// what is the mbPartIdx for B_Skip and B_Direct_16x16?
//
// 6.4.2.1 Inverse macroblock partition scanning process
std::pair<int, int> MacroBlock::Inverse_macroblock_partition_scanning_process(int mbPartIdx)
{
    assert(!mb_type_proxy_.is_intra_pred());

    int x = InverseRasterScan(mbPartIdx,
        mb_type_proxy_.MbPartWidth(),
        mb_type_proxy_.MbPartHeight(),
        16, 0);
    int y = InverseRasterScan(mbPartIdx,
        mb_type_proxy_.MbPartWidth(),
        mb_type_proxy_.MbPartHeight(),
        16, 1);
    return { x, y };
}

int MbTypeProxy::MbPartWidth()
{
    return mb_type_desc_map[mb_type_].MbPartWidth;
}

int MbTypeProxy::MbPartHeight()
{
    return mb_type_desc_map[mb_type_].MbPartHeight;
}

// B_Skip and B_Direct_16x16 should have entries in 'sub_mb_type_desc_map'?
//
// 6.4.2.2 Inverse sub-macroblock partition scanning process
std::pair<int, int> MacroBlock::Inverse_sub_macroblock_partition_scanning_process(int mbPartIdx, int subMbPartIdx)
{
    assert(!mb_type_proxy_.no_subMbPartIdx());

    int x, y;
    if (mb_type_proxy_.mb_type() == MbType::P_8x8
        || mb_type_proxy_.mb_type() == MbType::P_8x8ref0
        || mb_type_proxy_.mb_type() == MbType::B_8x8) {
        x = InverseRasterScan(subMbPartIdx, sub_mb_type_proxy_[mbPartIdx].SubMbPartWidth(),
            sub_mb_type_proxy_[mbPartIdx].SubMbPartHeight(), 8, 0);

        y = InverseRasterScan(subMbPartIdx, sub_mb_type_proxy_[mbPartIdx].SubMbPartWidth(),
            sub_mb_type_proxy_[mbPartIdx].SubMbPartHeight(), 8, 1);
    } else {
        /* MbType::B_Skip or MbType::B_Direct_16x16 */
        x = InverseRasterScan(subMbPartIdx, 4, 4, 8, 0);
        y = InverseRasterScan(subMbPartIdx, 4, 4, 8, 1);
    }
    return { x, y };
}

int SubMbTypeProxy::SubMbPartWidth()
{
    return sub_mb_type_desc_map[sub_mb_type_].SubMbPartWidth;
}

int SubMbTypeProxy::SubMbPartHeight()
{
    return sub_mb_type_desc_map[sub_mb_type_].SubMbPartHeight;
}

bool MacroBlock::is_intra_pred()
{
    return mb_type_proxy_.is_intra_pred();
}

bool MacroBlock::is_inter_pred()
{
    return !is_intra_pred();
}

bool MbTypeProxy::is_intra_pred()
{
    assert((int)mb_type_ != 400);
    return (int)mb_type_ >= 0 && (int)mb_type_ <= 100;
}

// TODO: how to distinguish between real mbPartIdx 0 or Intra MB?
//
//
// 6.4.13.4 Derivation process for macroblock and sub-macroblock partition indices
std::tuple<int, int>
MacroBlock::Derivation_process_for_macroblock_and_sub_macroblock_partition_indices(int xP, int yP)
{
    int mbPartIdx, subMbPartIdx;
    if (mb_type_proxy_.is_intra_pred()) {
        mbPartIdx = 0; // TODO
    } else {
        mbPartIdx = (16 / mb_type_proxy_.MbPartWidth()) * (yP / mb_type_proxy_.MbPartHeight()) + (xP / mb_type_proxy_.MbPartWidth());
    }

    if (mb_type_proxy_.no_subMbPartIdx())
        subMbPartIdx = 0; // TODO
    else if (mb_type_proxy_.mb_type() == MbType::B_Skip
        || mb_type_proxy_.mb_type() == MbType::B_Direct_16x16)
        subMbPartIdx = (8 / 4) * ((yP % 8) / 4) + ((xP % 8) / 4);
    else if (mb_type_proxy_.mb_type() == MbType::P_8x8
        || mb_type_proxy_.mb_type() == MbType::P_8x8ref0
        || mb_type_proxy_.mb_type() == MbType::B_8x8)
        subMbPartIdx = (8 / sub_mb_type_proxy_[mbPartIdx].SubMbPartWidth()) * ((yP % 8) / sub_mb_type_proxy_[mbPartIdx].SubMbPartHeight())
            + ((xP % 8) / sub_mb_type_proxy_[mbPartIdx].SubMbPartWidth());

    return { mbPartIdx, subMbPartIdx };
}

// when B_Skip and B_Direct_16x16 are decoded, consider as a whole MB
// when B_Skip and B_Direct_16x16 are used as a neighbor reference MB, consider as partitioned
//
// 6.4.11.7 Derivation process for neighbouring partitions
std::tuple<std::tuple<int, int, int>,
    std::tuple<int, int, int>,
    std::tuple<int, int, int>,
    std::tuple<int, int, int>>
MacroBlock::Derivation_process_for_neighbouring_partitions(
    int mbPartIdx,
    int subMbPartIdx,
    SubMbType currSubMbType)
{
    auto func = [mbPartIdx, subMbPartIdx, currSubMbType, this](int xD, int yD, bool is_C) -> std::tuple<int, int, int> {
        auto [x, y] = Inverse_macroblock_partition_scanning_process(mbPartIdx);
        int xS = 0, yS = 0;
        int predPartWidth;

        // B_Skip and B_Direct_16x16 is not sub partitioned?
        if (mb_type_proxy_.mb_type() == MbType::P_8x8
            || mb_type_proxy_.mb_type() == MbType::P_8x8ref0
            || mb_type_proxy_.mb_type() == MbType::B_8x8) {
            auto tmp = Inverse_sub_macroblock_partition_scanning_process(mbPartIdx, subMbPartIdx);
            xS = tmp.first;
            yS = tmp.second;
        }

        if (is_C) {

            // P_Skip B_Skip B_Direct_16x16 are not considered as partitioned ?
            if (mb_type_proxy_.mb_type() == MbType::P_Skip
                || mb_type_proxy_.mb_type() == MbType::B_Skip
                || mb_type_proxy_.mb_type() == MbType::B_Direct_16x16)
                predPartWidth = 16;
            else if (mb_type_proxy_.mb_type() == MbType::B_8x8) {
                // TODO: direct_spatial_mv_pred_flag
                // ?
                if (currSubMbType == SubMbType::B_Direct_8x8)
                    predPartWidth = 16;
                else
                    predPartWidth = sub_mb_type_proxy_[mbPartIdx].SubMbPartWidth();
            } else if (mb_type_proxy_.mb_type() == MbType::P_8x8
                || mb_type_proxy_.mb_type() == MbType::P_8x8ref0)
                predPartWidth = sub_mb_type_proxy_[mbPartIdx].SubMbPartWidth();
            else
                predPartWidth = mb_type_proxy_.MbPartWidth();

            xD = predPartWidth;
            yD = -1;
        }

        int xN = x + xS + xD;
        int yN = y + yS + yD;

        auto [mbAddrN, xW, yW] = slice_->Derivation_process_for_neighbouring_locations(CurrMbAddr_,
            xN,
            yN,
            true);
        if (mbAddrN == INT32_MIN) // not available, this should be safe
            return { INT32_MIN, INT32_MIN, INT32_MIN };

        auto& mb_N = slice_->get_mb_by_addr(mbAddrN);

        auto t = mb_N.Derivation_process_for_macroblock_and_sub_macroblock_partition_indices(xW, yW);
        std::tuple<int, int, int> res = { mbAddrN, std::get<0>(t), std::get<1>(t) };

        int t_mbPartIdx = std::get<1>(res);
        int t_subMbPartIdx = std::get<2>(res);
        assert(0 <= t_mbPartIdx && t_mbPartIdx < 4
            && 0 <= t_subMbPartIdx && t_subMbPartIdx < 4);

        // TODO: Revisit this condition
        // When the partition given by mbPartIdxN and subMbPartIdxN is not yet decoded, the macroblock
        // partition mbPartIdxN and the sub-macroblock partition subMbPartIdxN are marked as not
        // available.
        if (mbAddrN == CurrMbAddr_) {
            if (t_mbPartIdx > mbPartIdx || (t_mbPartIdx == mbPartIdx && t_subMbPartIdx > subMbPartIdx))
                res = { INT32_MIN, INT32_MIN, INT32_MIN };
        }

        return res;
    };

    auto [mbAddrA, mbPartIdxA, subMbPartIdxA] = func(-1, 0, false);
    auto [mbAddrB, mbPartIdxB, subMbPartIdxB] = func(0, -1, false);
    auto [mbAddrC, mbPartIdxC, subMbPartIdxC] = func(INT32_MIN, INT32_MIN, true);
    auto [mbAddrD, mbPartIdxD, subMbPartIdxD] = func(-1, -1, false);

    return { { mbAddrA, mbPartIdxA, subMbPartIdxA },
        { mbAddrB, mbPartIdxB, subMbPartIdxB },
        { mbAddrC, mbPartIdxC, subMbPartIdxC },
        { mbAddrD, mbPartIdxD, subMbPartIdxD } };
}

// 8.3.1 Intra_4x4 prediction process for luma samples
//
void MacroBlock::Intra_4x4_prediction_process_for_luma_samples()
{
    assert(mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_4x4);

    // first dim is width ?
    //
    std::vector<std::vector<int>> predL(16, std::vector<int>(16));

    for (int luma4x4BlkIdx = 0; luma4x4BlkIdx < 16; luma4x4BlkIdx++) {

        Derivation_process_for_Intra4x4PredMode(luma4x4BlkIdx);

        auto pred4x4L = Intra_4x4_sample_prediction(luma4x4BlkIdx);

        auto [xO, yO] = slice_->Inverse_4x4_luma_block_scanning_process(luma4x4BlkIdx);

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                predL[xO + x][yO + y] = pred4x4L[x][y];
            }
        }

        Specification_of_transform_decoding_process_for_4x4_luma_residual_blocks(
            predL,
            luma4x4BlkIdx);
    }
}

// 8.3.1.1 Derivation process for Intra4x4PredMode
void MacroBlock::Derivation_process_for_Intra4x4PredMode(int luma4x4BlkIdx)
{
    auto [mbAddrA,
        luma4x4BlkIdxA,
        mbAddrB,
        luma4x4BlkIdxB]
        = slice_->Derivation_process_for_neighbouring_4x4_luma_blocks(CurrMbAddr_, luma4x4BlkIdx);

    bool dcPredModePredictedFlag = false;
    if (mbAddrA == INT32_MIN
        || mbAddrB == INT32_MIN
        || (slice_->get_mb_by_addr(mbAddrA).is_inter_pred() && pps_->constrained_intra_pred_flag())
        || (slice_->get_mb_by_addr(mbAddrB).is_inter_pred() && pps_->constrained_intra_pred_flag())) {
        dcPredModePredictedFlag = true;
    }

    auto func = [dcPredModePredictedFlag, this](int mbAddrN, int luma4x4BlkIdxN) {
        int intraMxMPredModeN;
        if (dcPredModePredictedFlag
            || (slice_->get_mb_by_addr(mbAddrN).MbPartPredMode_0() != MbPartPredMode::Intra_4x4
                && slice_->get_mb_by_addr(mbAddrN).MbPartPredMode_0() != MbPartPredMode::Intra_8x8))
            intraMxMPredModeN = 2;
        else {
            if (slice_->get_mb_by_addr(mbAddrN).MbPartPredMode_0() == MbPartPredMode::Intra_4x4)
                intraMxMPredModeN = slice_->get_mb_by_addr(mbAddrN).getIntra4x4PredMode(luma4x4BlkIdxN);
            else if (slice_->get_mb_by_addr(mbAddrN).MbPartPredMode_0() == MbPartPredMode::Intra_8x8)
                intraMxMPredModeN = slice_->get_mb_by_addr(mbAddrN).getIntra8x8PredModefor4x4Blk(luma4x4BlkIdxN);
            else
                assert(false);
        }
        return intraMxMPredModeN;
    };

    int intraMxMPredModeA = func(mbAddrA, luma4x4BlkIdxA);
    int intraMxMPredModeB = func(mbAddrB, luma4x4BlkIdxB);
    int predIntra4x4PredMode = std::min(intraMxMPredModeA, intraMxMPredModeB);
    if (prev_intra4x4_pred_mode_flag_[luma4x4BlkIdx])
        Intra4x4PredMode_[luma4x4BlkIdx] = predIntra4x4PredMode;
    else {
        if (rem_intra4x4_pred_mode_[luma4x4BlkIdx] < predIntra4x4PredMode)
            Intra4x4PredMode_[luma4x4BlkIdx] = rem_intra4x4_pred_mode_[luma4x4BlkIdx];
        else
            Intra4x4PredMode_[luma4x4BlkIdx] = rem_intra4x4_pred_mode_[luma4x4BlkIdx] + 1;
    }
}

int MacroBlock::getIntra4x4PredMode(int luma4x4BlkIdxN)
{
    return Intra4x4PredMode_[luma4x4BlkIdxN];
}

int MacroBlock::getIntra8x8PredModefor4x4Blk(int luma4x4BlkIdxN)
{
    return Intra8x8PredMode_[luma4x4BlkIdxN >> 2];
}

// 8.3.1.2 Intra_4x4 sample prediction
//
std::vector<std::vector<int>> MacroBlock::Intra_4x4_sample_prediction(int luma4x4BlkIdx)
{
    auto [xO, yO] = slice_->Inverse_4x4_luma_block_scanning_process(luma4x4BlkIdx);

    auto func = [xO, yO, luma4x4BlkIdx, this](int x, int y) {
        int res;
        int xN = xO + x;
        int yN = yO + y;

        auto [mbAddrN,
            xW,
            yW]
            = slice_->Derivation_process_for_neighbouring_locations(CurrMbAddr_, xN, yN, true /* is_luma */);

        if (mbAddrN == INT32_MIN
            || (slice_->get_mb_by_addr(mbAddrN).is_inter_pred()
                && pps_->constrained_intra_pred_flag())
            || (slice_->get_mb_by_addr(mbAddrN).mb_type() == MbType::SI
                && pps_->constrained_intra_pred_flag()
                && mb_type() != MbType::SI)
            || (x > 3 && (luma4x4BlkIdx == 3 || luma4x4BlkIdx == 11)))
            res = INT32_MIN;
        else {
            auto [xM, yM] = slice_->Inverse_macroblock_scanning_process(mbAddrN);
            if (slice_->MbaffFrameFlag() && slice_->get_mb_by_addr(mbAddrN).is_field_macroblock()) {
                assert(false);
            } else {
                res = slice_->get_constructed_luma(xM + xW, yM + yW);
            }
        }
        return res;
    };

    int p_minus1_minus1 = func(-1, -1);
    int p_minus1_0 = func(-1, 0);
    int p_minus1_1 = func(-1, 1);
    int p_minus1_2 = func(-1, 2);
    int p_minus1_3 = func(-1, 3);

    int p_0_minus1 = func(0, -1);
    int p_1_minus1 = func(1, -1);
    int p_2_minus1 = func(2, -1);
    int p_3_minus1 = func(3, -1);
    int p_4_minus1 = func(4, -1);
    int p_5_minus1 = func(5, -1);
    int p_6_minus1 = func(6, -1);
    int p_7_minus1 = func(7, -1);

    if (p_3_minus1 != INT32_MIN) {
        if (p_4_minus1 == INT32_MIN)
            p_4_minus1 = p_3_minus1;
        if (p_5_minus1 == INT32_MIN)
            p_5_minus1 = p_3_minus1;
        if (p_6_minus1 == INT32_MIN)
            p_6_minus1 = p_3_minus1;
        if (p_7_minus1 == INT32_MIN)
            p_7_minus1 = p_3_minus1;
    }

    // NOTE: x are first idx, and column are second idx
    // so we use row to represent x, and column to represent y
    int tp[17][17];
    int* tp_p = tp[1] + 1;
    int(&p)[16][17] = *((int(*)[16][17])tp_p); // be care of column stride

    p[-1][-1] = p_minus1_minus1;
    p[-1][0] = p_minus1_0;
    p[-1][1] = p_minus1_1;
    p[-1][2] = p_minus1_2;
    p[-1][3] = p_minus1_3;

    p[0][-1] = p_0_minus1;
    p[1][-1] = p_1_minus1;
    p[2][-1] = p_2_minus1;
    p[3][-1] = p_3_minus1;
    p[4][-1] = p_4_minus1;
    p[5][-1] = p_5_minus1;
    p[6][-1] = p_6_minus1;
    p[7][-1] = p_7_minus1;

    // 1st dim is x, 2nd dim is y
    std::vector<std::vector<int>> pred4x4L(4, std::vector<int>(4, INT32_MIN));

    switch (Intra4x4PredMode_[luma4x4BlkIdx]) {
    case Intra_4x4_Vertical: {
        assert(p[0][-1] != INT32_MIN
            && p[1][-1] != INT32_MIN
            && p[2][-1] != INT32_MIN
            && p[3][-1] != INT32_MIN);

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                pred4x4L[x][y] = p[x][-1];
            }
        }
        break;
    }
    case Intra_4x4_Horizontal: {
        assert(p[-1][0] != INT32_MIN
            && p[-1][1] != INT32_MIN
            && p[-1][2] != INT32_MIN
            && p[-1][3] != INT32_MIN);

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                pred4x4L[x][y] = p[-1][y];
            }
        }

        break;
    }
    case Intra_4x4_DC: {
        int m = INT32_MIN;

        if (p_0_minus1 != INT32_MIN && p_1_minus1 != INT32_MIN
            && p_2_minus1 != INT32_MIN && p_3_minus1 != INT32_MIN
            && p_minus1_0 != INT32_MIN && p_minus1_1 != INT32_MIN
            && p_minus1_2 != INT32_MIN && p_minus1_3 != INT32_MIN)
            m = (p_0_minus1 + p_1_minus1 + p_2_minus1 + p_3_minus1
                    + p_minus1_0 + p_minus1_1 + p_minus1_2 + p_minus1_3 + 4)
                >> 3;
        else if ((p_0_minus1 == INT32_MIN || p_1_minus1 == INT32_MIN
                     || p_2_minus1 == INT32_MIN || p_3_minus1 == INT32_MIN)
            && (p_minus1_0 != INT32_MIN && p_minus1_1 != INT32_MIN
                && p_minus1_2 != INT32_MIN && p_minus1_3 != INT32_MIN))
            m = (p_minus1_0 + p_minus1_1 + p_minus1_2 + p_minus1_3 + 2) >> 2;
        else if ((p_0_minus1 != INT32_MIN && p_1_minus1 != INT32_MIN
                     && p_2_minus1 != INT32_MIN && p_3_minus1 != INT32_MIN)
            && (p_minus1_0 == INT32_MIN || p_minus1_1 == INT32_MIN
                || p_minus1_2 == INT32_MIN || p_minus1_3 == INT32_MIN))
            m = (p_0_minus1 + p_1_minus1 + p_2_minus1 + p_3_minus1 + 2) >> 2;
        else
            m = 1 << (sps_->bit_depth_luma() - 1);

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                pred4x4L[x][y] = m;
            }
        }

        break;
    }
    case Intra_4x4_Diagonal_Down_Left: {
        assert(p_0_minus1 != INT32_MIN
            && p_1_minus1 != INT32_MIN
            && p_2_minus1 != INT32_MIN
            && p_3_minus1 != INT32_MIN
            && p_4_minus1 != INT32_MIN
            && p_5_minus1 != INT32_MIN
            && p_6_minus1 != INT32_MIN
            && p_7_minus1 != INT32_MIN);

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                if (x == 3 && y == 3)
                    pred4x4L[x][y] = (p_6_minus1 + 3 * p_7_minus1 + 2) >> 2;
                else
                    pred4x4L[x][y] = (p[x + y][-1] + 2 * p[x + y + 1][-1] + p[x + y + 2][-1] + 2) >> 2;
            }
        }

        break;
    }
    case Intra_4x4_Diagonal_Down_Right: {
        assert(p[0][-1] != INT32_MIN
            && p[1][-1] != INT32_MIN
            && p[2][-1] != INT32_MIN
            && p[3][-1] != INT32_MIN
            && p[-1][-1] != INT32_MIN
            && p[-1][0] != INT32_MIN
            && p[-1][1] != INT32_MIN
            && p[-1][2] != INT32_MIN
            && p[-1][3] != INT32_MIN);

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                if (x > y)
                    pred4x4L[x][y] = (p[x - y - 2][-1] + 2 * p[x - y - 1][-1] + p[x - y][-1] + 2) >> 2;
                else if (x < y)
                    pred4x4L[x][y] = (p[-1][y - x - 2] + 2 * p[-1][y - x - 1] + p[-1][y - x] + 2) >> 2;
                else
                    pred4x4L[x][y] = (p[0][-1] + 2 * p[-1][-1] + p[-1][0] + 2) >> 2;
            }
        }

        break;
    }
    case Intra_4x4_Vertical_Right: {
        assert(p[0][-1] != INT32_MIN
            && p[1][-1] != INT32_MIN
            && p[2][-1] != INT32_MIN
            && p[3][-1] != INT32_MIN
            && p[-1][-1] != INT32_MIN
            && p[-1][0] != INT32_MIN
            && p[-1][1] != INT32_MIN
            && p[-1][2] != INT32_MIN
            && p[-1][3] != INT32_MIN);

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                int zVR = 2 * x - y;
                if (zVR == 0 || zVR == 2
                    || zVR == 4 || zVR == 6)
                    pred4x4L[x][y] = (p[x - (y >> 1) - 1][-1] + p[x - (y >> 1)][-1] + 1) >> 1;
                else if (zVR == 1 || zVR == 3 || zVR == 5)
                    pred4x4L[x][y] = (p[x - (y >> 1) - 2][-1] + 2 * p[x - (y >> 1) - 1][-1]
                                         + p[x - (y >> 1)][-1] + 2)
                        >> 2;
                else if (zVR == -1)
                    pred4x4L[x][y] = (p[-1][0] + 2 * p[-1][-1] + p[0][-1] + 2) >> 2;
                else if (zVR == -2 || zVR == -3)
                    pred4x4L[x][y] = (p[-1][y - 1] + 2 * p[-1][y - 2] + p[-1][y - 3] + 2) >> 2;
                else
                    assert(false);
            }
        }

        break;
    }
    case Intra_4x4_Horizontal_Down: {

        assert(p[0][-1] != INT32_MIN
            && p[1][-1] != INT32_MIN
            && p[2][-1] != INT32_MIN
            && p[3][-1] != INT32_MIN
            && p[-1][-1] != INT32_MIN
            && p[-1][0] != INT32_MIN
            && p[-1][1] != INT32_MIN
            && p[-1][2] != INT32_MIN
            && p[-1][3] != INT32_MIN);

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                int zHD = 2 * y - x;
                if (zHD == 0 || zHD == 2 || zHD == 4 || zHD == 6)
                    pred4x4L[x][y] = (p[-1][y - (x >> 1) - 1] + p[-1][y - (x >> 1)] + 1) >> 1;
                else if (zHD == 1 || zHD == 3 || zHD == 5)
                    pred4x4L[x][y] = (p[-1][y - (x >> 1) - 2] + 2 * p[-1][y - (x >> 1) - 1]
                                         + p[-1][y - (x >> 1)] + 2)
                        >> 2;
                else if (zHD == -1)
                    pred4x4L[x][y] = (p[-1][0] + 2 * p[-1][-1] + p[0][-1] + 2) >> 2;
                else if (zHD == -2 || zHD == -3)
                    pred4x4L[x][y] = (p[x - 1][-1] + 2 * p[x - 2][-1] + p[x - 3][-1] + 2) >> 2;
                else
                    assert(false);
            }
        }

        break;
    }
    case Intra_4x4_Vertical_Left: {
        assert(p[0][-1] != INT32_MIN
            && p[1][-1] != INT32_MIN
            && p[2][-1] != INT32_MIN
            && p[3][-1] != INT32_MIN
            && p[4][-1] != INT32_MIN
            && p[5][-1] != INT32_MIN
            && p[6][-1] != INT32_MIN
            && p[7][-1] != INT32_MIN);

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                if (y == 2 || y == 0)
                    pred4x4L[x][y] = (p[x + (y >> 1)][-1] + p[x + (y >> 1) + 1][-1] + 1) >> 1;
                else
                    pred4x4L[x][y] = (p[x + (y >> 1)][-1] + 2 * p[x + (y >> 1) + 1][-1]
                                         + p[x + (y >> 1) + 2][-1] + 2)
                        >> 2;
            }
        }

        break;
    }
    case Intra_4x4_Horizontal_Up: {
        assert(p[-1][0] != INT32_MIN
            && p[-1][1] != INT32_MIN
            && p[-1][2] != INT32_MIN
            && p[-1][3] != INT32_MIN);

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                int zHU = x + 2 * y;
                if (zHU == 0 || zHU == 2 || zHU == 4)
                    pred4x4L[x][y] = (p[-1][y + (x >> 1)] + p[-1][y + (x >> 1) + 1] + 1) >> 1;
                else if (zHU == 1 || zHU == 3)
                    pred4x4L[x][y] = (p[-1][y + (x >> 1)] + 2 * p[-1][y + (x >> 1) + 1]
                                         + p[-1][y + (x >> 1) + 2] + 2)
                        >> 2;
                else if (zHU == 5)
                    pred4x4L[x][y] = (p[-1][2] + 3 * p[-1][3] + 2) >> 2;
                else
                    pred4x4L[x][y] = p[-1][3];
            }
        }

        break;
    }
    default:
        assert(false);
        break;
    }

    return std::move(pred4x4L);
}

bool MacroBlock::is_field_macroblock()
{
    // now only frame is allowed
    return false;
}

bool MacroBlock::is_frame_macroblock()
{
    return !is_field_macroblock();
}

// what if I_PCM P_Skip B_Skip ?
//
// I_PCM has been prevented in Derivation_process_for_the_luma_bs()
// P_Skip/B_Skip don't care about transform_size_8x8_flag, and this
// function should always return false for P_Skip/B_Skip
//
bool MacroBlock::has_luma_transform_coefficient_levels_in_x_y(
    int x, int y)
{
    if (transform_size_8x8_flag()) {
        // 8x8 are not considered
        assert(false);
    } else {

        int luma4x4BlkIdx = slice_->Derivation_process_for_4x4_luma_block_indices(x, y);
        int i8x8 = luma4x4BlkIdx / 4;

        return mb_type_proxy_.CodedBlockPatternLuma() & (1 << i8x8);
    }
    return false;
}

bool MacroBlock::is_in_SP_or_SI()
{
    return slice_->is_SP_or_SI();
}

int MacroBlock::FilterOffsetA()
{
    return slice_->FilterOffsetA();
}

int MacroBlock::FilterOffsetB()
{
    return slice_->FilterOffsetB();
}

enum MbType MacroBlock::mb_type()
{
    return mb_type_proxy_.mb_type();
}

enum MbPartPredMode MacroBlock::MbPartPredMode_0()
{
    return mb_type_proxy_.MbPartPredMode_0();
}

// 8.3.3 Intra_16x16 prediction process for luma samples
std::vector<std::vector<int>>
MacroBlock::Intra_16x16_prediction_process_for_luma_samples()
{
    auto func = [this](int xN, int yN) {
        int res;

        auto [mbAddrN,
            xW,
            yW]
            = slice_->Derivation_process_for_neighbouring_locations(CurrMbAddr_, xN, yN, true /* is_luma */);

        if (mbAddrN == INT32_MIN
            || (slice_->get_mb_by_addr(mbAddrN).is_inter_pred()
                && pps_->constrained_intra_pred_flag())
            || (slice_->get_mb_by_addr(mbAddrN).mb_type() == MbType::SI
                && pps_->constrained_intra_pred_flag()))
            res = INT32_MIN;
        else {
            auto [xM, yM] = slice_->Inverse_macroblock_scanning_process(mbAddrN);
            if (slice_->MbaffFrameFlag() && slice_->get_mb_by_addr(mbAddrN).is_field_macroblock()) {
                assert(false);
            } else {
                res = slice_->get_constructed_luma(xM + xW, yM + yW);
            }
        }

        return res;
    };

    int p_minus1_minus1 = func(-1, -1);
    int p_minus1_0 = func(-1, 0);
    int p_minus1_1 = func(-1, 1);
    int p_minus1_2 = func(-1, 2);
    int p_minus1_3 = func(-1, 3);
    int p_minus1_4 = func(-1, 4);
    int p_minus1_5 = func(-1, 5);
    int p_minus1_6 = func(-1, 6);
    int p_minus1_7 = func(-1, 7);
    int p_minus1_8 = func(-1, 8);
    int p_minus1_9 = func(-1, 9);
    int p_minus1_10 = func(-1, 10);
    int p_minus1_11 = func(-1, 11);
    int p_minus1_12 = func(-1, 12);
    int p_minus1_13 = func(-1, 13);
    int p_minus1_14 = func(-1, 14);
    int p_minus1_15 = func(-1, 15);

    int p_0_minus1 = func(0, -1);
    int p_1_minus1 = func(1, -1);
    int p_2_minus1 = func(2, -1);
    int p_3_minus1 = func(3, -1);
    int p_4_minus1 = func(4, -1);
    int p_5_minus1 = func(5, -1);
    int p_6_minus1 = func(6, -1);
    int p_7_minus1 = func(7, -1);
    int p_8_minus1 = func(8, -1);
    int p_9_minus1 = func(9, -1);
    int p_10_minus1 = func(10, -1);
    int p_11_minus1 = func(11, -1);
    int p_12_minus1 = func(12, -1);
    int p_13_minus1 = func(13, -1);
    int p_14_minus1 = func(14, -1);
    int p_15_minus1 = func(15, -1);

    // NOTE: x y are opposite with row/column
    // so here we use row to represent x, and column to represent y
    int tp[17][17];
    int* tp_p = tp[1] + 1;
    int(&p)[16][17] = *((int(*)[16][17])tp_p); // be care of column stride

    p[-1][-1] = p_minus1_minus1;
    p[-1][0] = p_minus1_0;
    p[-1][1] = p_minus1_1;
    p[-1][2] = p_minus1_2;
    p[-1][3] = p_minus1_3;
    p[-1][4] = p_minus1_4;
    p[-1][5] = p_minus1_5;
    p[-1][6] = p_minus1_6;
    p[-1][7] = p_minus1_7;
    p[-1][8] = p_minus1_8;
    p[-1][9] = p_minus1_9;
    p[-1][10] = p_minus1_10;
    p[-1][11] = p_minus1_11;
    p[-1][12] = p_minus1_12;
    p[-1][13] = p_minus1_13;
    p[-1][14] = p_minus1_14;
    p[-1][15] = p_minus1_15;

    p[0][-1] = p_0_minus1;
    p[1][-1] = p_1_minus1;
    p[2][-1] = p_2_minus1;
    p[3][-1] = p_3_minus1;
    p[4][-1] = p_4_minus1;
    p[5][-1] = p_5_minus1;
    p[6][-1] = p_6_minus1;
    p[7][-1] = p_7_minus1;
    p[8][-1] = p_8_minus1;
    p[9][-1] = p_9_minus1;
    p[10][-1] = p_10_minus1;
    p[11][-1] = p_11_minus1;
    p[12][-1] = p_12_minus1;
    p[13][-1] = p_13_minus1;
    p[14][-1] = p_14_minus1;
    p[15][-1] = p_15_minus1;

    auto all_vertical_are_available = [&p]() {
        return p[0][-1] != INT32_MIN
            && p[1][-1] != INT32_MIN
            && p[2][-1] != INT32_MIN
            && p[3][-1] != INT32_MIN
            && p[4][-1] != INT32_MIN
            && p[5][-1] != INT32_MIN
            && p[6][-1] != INT32_MIN
            && p[7][-1] != INT32_MIN
            && p[8][-1] != INT32_MIN
            && p[9][-1] != INT32_MIN
            && p[10][-1] != INT32_MIN
            && p[11][-1] != INT32_MIN
            && p[12][-1] != INT32_MIN
            && p[13][-1] != INT32_MIN
            && p[14][-1] != INT32_MIN
            && p[15][-1] != INT32_MIN;
    };
    auto all_horizental_are_available = [&p]() {
        return p[-1][0] != INT32_MIN
            && p[-1][1] != INT32_MIN
            && p[-1][2] != INT32_MIN
            && p[-1][3] != INT32_MIN
            && p[-1][4] != INT32_MIN
            && p[-1][5] != INT32_MIN
            && p[-1][6] != INT32_MIN
            && p[-1][7] != INT32_MIN
            && p[-1][8] != INT32_MIN
            && p[-1][9] != INT32_MIN
            && p[-1][10] != INT32_MIN
            && p[-1][11] != INT32_MIN
            && p[-1][12] != INT32_MIN
            && p[-1][13] != INT32_MIN
            && p[-1][14] != INT32_MIN
            && p[-1][15] != INT32_MIN;
    };

    // 1st dim is x, 2nd dim is y
    std::vector<std::vector<int>> predL(16, std::vector<int>(16, INT32_MIN));

    switch (mb_type_proxy_.Intra16x16PredMode()) {
    case Intra_16x16_Vertical: {
        assert(all_vertical_are_available());

        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                predL[x][y] = p[x][-1];
            }
        }
        break;
    }
    case Intra_16x16_Horizontal: {
        assert(all_horizental_are_available());

        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                predL[x][y] = p[-1][y];
            }
        }

        break;
    }
    case Intra_16x16_DC: {
        int m = 0;

        if (all_vertical_are_available() && all_horizental_are_available()) {
            for (int x = 0; x < 16; x++)
                m += p[x][-1];
            for (int y = 0; y < 16; y++)
                m += p[-1][y];

            m = (m + 16) >> 5;

        } else if (!all_vertical_are_available()
            && all_horizental_are_available()) {
            for (int y = 0; y < 16; y++)
                m += p[-1][y];

            m = (m + 8) >> 4;
        } else if (all_vertical_are_available()
            && !all_horizental_are_available()) {
            for (int x = 0; x < 16; x++)
                m += p[x][-1];

            m = (m + 8) >> 4;

        } else
            m = 1 << (sps_->bit_depth_luma() - 1);

        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                predL[x][y] = m;
            }
        }

        break;
    }
    case Intra_16x16_Plane: {

        assert(p[-1][-1] != INT32_MIN
            && all_horizental_are_available()
            && all_vertical_are_available());

        int H = 0;
        for (int x = 0; x < 8; x++)
            H += (x + 1) * (p[8 + x][-1] - p[6 - x][-1]);

        int V = 0;
        for (int y = 0; y < 8; y++)
            V += (y + 1) * (p[-1][8 + y] - p[-1][6 - y]);

        int a = 16 * (p[-1][15] + p[15][-1]);
        int b = (5 * H + 32) >> 6;
        int c = (5 * V + 32) >> 6;

        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                predL[x][y] = Clip1Y((a + b * (x - 7) + c * (y - 7) + 16) >> 5);
            }
        }

        break;
    }
    default:
        assert(false);
        break;
    }

    return std::move(predL);
}

int MacroBlock::Clip1Y(int x)
{
    return Clip3(0, (1 << sps_->bit_depth_luma()) - 1, x);
}
int MacroBlock::Clip1C(int x)
{
    return Clip3(0, (1 << sps_->bit_depth_chroma()) - 1, x);
}

// TODO : revisit this func
//
// 8.3.4 Intra prediction process for chroma samples
std::vector<std::vector<int>>
MacroBlock::Intra_prediction_process_for_chroma_sample(int idx /* cb or cr */)
{
    auto func = [idx, this](int xN, int yN) {
        int res;

        auto [mbAddrN,
            xW,
            yW]
            = slice_->Derivation_process_for_neighbouring_locations(CurrMbAddr_, xN, yN, false /* is_luma */);

        if (mbAddrN == INT32_MIN
            || (slice_->get_mb_by_addr(mbAddrN).is_inter_pred()
                && pps_->constrained_intra_pred_flag())
            || (slice_->get_mb_by_addr(mbAddrN).mb_type() == MbType::SI
                && pps_->constrained_intra_pred_flag()
                && mb_type() != MbType::SI))
            res = INT32_MIN;
        else {
            auto [xL, yL] = slice_->Inverse_macroblock_scanning_process(mbAddrN);
            int xM = (xL >> 4) * sps_->MbWidthC();
            int yM = ((yL >> 4) * sps_->MbHeightC()) + (yL % 2);
            // only consider frame mode
            res = slice_->get_constructed_chroma(idx, xM + xW, yM + yW);
        }

        return res;
    };

    int MbWidthC = sps_->MbWidthC();
    int MbHeightC = sps_->MbHeightC();
    int ChromaArrayType = sps_->ChromaArrayType();

    // 17x17 should enough
    //
    // NOTE: x y are opposite with row/column
    // so here we use row to represent x, and column to represent y
    int tp[17][17];
    int* tp_p = tp[1] + 1;
    int(&p)[16][17] = *((int(*)[16][17])tp_p); // be care of column stride

    for (int y = -1; y < MbHeightC; y++)
        p[-1][y] = func(-1, y);

    for (int x = 0; x < MbWidthC; x++)
        p[x][-1] = func(x, -1);

    auto all_horizental_are_available = [MbHeightC, &p]() {
        for (int y = 0; y < MbHeightC; y++)
            if (p[-1][y] == INT32_MIN)
                return false;
        return true;
    };

    auto all_vertical_are_available = [MbWidthC, &p]() {
        for (int x = 0; x < MbWidthC; x++)
            if (p[x][-1] == INT32_MIN)
                return false;
        return true;
    };

    auto vertical_are_available_by4 = [&p](int xO) {
        for (int x = 0; x < 4; x++)
            if (p[xO + x][-1] == INT32_MIN)
                return false;
        return true;
    };
    auto horizental_are_available_by4 = [&p](int yO) {
        for (int y = 0; y < 4; y++)
            if (p[-1][y + yO] == INT32_MIN)
                return false;
        return true;
    };

    auto x_sum_by_4 = [&p](int xO) {
        int res = 0;
        for (int x = 0; x < 4; x++)
            res += p[xO + x][-1];
        return res;
    };
    auto y_sum_by_4 = [&p](int yO) {
        int res = 0;
        for (int y = 0; y < 4; y++)
            res += p[-1][yO + y];
        return res;
    };

    std::vector<std::vector<int>> predC(MbWidthC, std::vector<int>(MbHeightC, INT32_MIN));

    switch (intra_chroma_pred_mode_) {
    case Intra_Chroma_DC: {
        int NumC4x4 = 1 << (ChromaArrayType + 1);
        for (int chroma4x4BlkIdx = 0; chroma4x4BlkIdx < NumC4x4; chroma4x4BlkIdx++) {
            int m = 0;
            auto [xO, yO] = slice_->Inverse_4x4_chroma_block_scanning_process(chroma4x4BlkIdx);
            if ((xO == 0 && yO == 0) || (xO > 0 && yO > 0)) {
                if (vertical_are_available_by4(xO)
                    && horizental_are_available_by4(yO))
                    m = (x_sum_by_4(xO) + y_sum_by_4(yO) + 4) >> 3;
                else if (!vertical_are_available_by4(xO)
                    && horizental_are_available_by4(yO))
                    m = (y_sum_by_4(yO) + 2) >> 2;
                else if (vertical_are_available_by4(xO)
                    && !horizental_are_available_by4(yO))
                    m = (x_sum_by_4(xO) + 2) >> 2;
                else
                    m = 1 << (sps_->bit_depth_chroma() - 1);
            } else if (xO > 0 && yO == 0) {
                if (vertical_are_available_by4(xO))
                    m = (x_sum_by_4(xO) + 2) >> 2;
                else if (horizental_are_available_by4(yO))
                    m = (y_sum_by_4(yO) + 2) >> 2;
                else
                    m = 1 << (sps_->bit_depth_chroma() - 1);
            } else if (xO == 0 && yO > 0) {
                if (horizental_are_available_by4(yO))
                    m = (y_sum_by_4(yO) + 2) >> 2;
                else if (vertical_are_available_by4(xO))
                    m = (x_sum_by_4(xO) + 2) >> 2;
                else
                    m = 1 << (sps_->bit_depth_chroma() - 1);
            }

            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 4; y++) {
                    predC[x + xO][y + yO] = m;
                }
            }
        }
        break;
    }
    case Intra_Chroma_Horizontal: {
        assert(all_horizental_are_available());
        for (int x = 0; x < MbWidthC; x++) {
            for (int y = 0; y < MbHeightC; y++) {
                predC[x][y] = p[-1][y];
            }
        }
        break;
    }
    case Intra_Chroma_Vertical: {
        assert(all_vertical_are_available());
        for (int x = 0; x < MbWidthC; x++) {
            for (int y = 0; y < MbHeightC; y++) {
                predC[x][y] = p[x][-1];
            }
        }
        break;
    }
    case Intra_Chroma_Plane: {
        assert(p[-1][-1] != INT32_MIN
            && all_horizental_are_available()
            && all_vertical_are_available());

        int xCF = ((ChromaArrayType == 3) ? 4 : 0);
        int yCF = ((ChromaArrayType != 1) ? 4 : 0);

        int H = 0;
        for (int x = 0; x <= (3 + xCF); x++)
            H += (x + 1) * (p[4 + xCF + x][-1] - p[2 + xCF - x][-1]);

        int V = 0;
        for (int y = 0; y <= (3 + yCF); y++)
            V += (y + 1) * (p[-1][4 + yCF + y] - p[-1][2 + yCF - y]);

        int a = 16 * (p[-1][MbHeightC - 1] + p[MbWidthC - 1][-1]);
        int b = ((34 - 29 * (ChromaArrayType == 3 ? 1 : 0)) * H + 32) >> 6;
        int c = ((34 - 29 * (ChromaArrayType != 1 ? 1 : 0)) * V + 32) >> 6;

        for (int x = 0; x < MbWidthC; x++) {
            for (int y = 0; y < MbHeightC; y++) {
                predC[x][y] = Clip1C((a + b * (x - 3 - xCF) + c * (y - 3 - yCF) + 16) >> 5);
            }
        }
        break;
    }
    default:
        assert(false);
        break;
    }

    return std::move(predC);
}

// 8.3.5 Sample construction process for I_PCM macroblocks
void MacroBlock::Sample_construction_process_for_I_PCM_macroblocks()
{
    // only consider frame mode
    int dy = 1;

    auto [xP, yP] = slice_->Inverse_macroblock_scanning_process(CurrMbAddr_);

    for (int i = 0; i < 256; i++)
        slice_->set_constructed_luma(xP + (i % 16), yP + dy * (i / 16), pcm_sample_luma_[i]);

    int MbWidthC = sps_->MbWidthC();
    int MbHeightC = sps_->MbHeightC();
    int SubWidthC = sps_->SubWidthC();
    int SubHeightC = sps_->SubHeightC();

    assert(sps_->ChromaArrayType() != 0);

    for (int i = 0; i < MbWidthC * MbHeightC; i++) {
        slice_->set_constructed_chroma(0,
            (xP / SubWidthC) + (i % MbWidthC),
            ((yP + SubHeightC - 1) / SubHeightC) + dy * (i / MbWidthC),
            pcm_sample_chroma_[i]);

        slice_->set_constructed_chroma(1,
            (xP / SubWidthC) + (i % MbWidthC),
            ((yP + SubHeightC - 1) / SubHeightC) + dy * (i / MbWidthC),
            pcm_sample_chroma_[i + MbWidthC * MbHeightC]);
    }
}

// 8.5.1 Specification of transform decoding process for 4x4 luma residual blocks
void MacroBlock::Specification_of_transform_decoding_process_for_4x4_luma_residual_blocks(
    const std::vector<std::vector<int>>& predL,
    int luma4x4BlkIdx)
{
    std::vector<std::vector<int>> u(4, std::vector<int>(4, 0));

    auto c = Inverse_scanning_process_for_4x4_transform_coefficients_and_scaling_lists(
        LumaLevel4x4_[luma4x4BlkIdx], true /* is_zigzag */);

    auto r = Scaling_and_transformation_process_for_residual_4x4_blocks(
        c, ResidualData::luma);

    if (TransformBypassModeFlag()
        && mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_4x4
        && (Intra4x4PredMode_[luma4x4BlkIdx] == 0
            || Intra4x4PredMode_[luma4x4BlkIdx] == 1)) {
        // TODO
        assert(false);
    }

    auto [xO, yO] = slice_->Inverse_4x4_luma_block_scanning_process(luma4x4BlkIdx);

    // predL and r are transpose
    // so 1st dim of u is height, 2nd dim is width
    //
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            u[i][j] = Clip1Y(predL[xO + j][yO + i] + r[i][j]);
        }
    }

    Picture_construction_process_prior_to_deblocking_filter_process(
        PicConsType::_4x4_luma,
        luma4x4BlkIdx,
        u,
        false /* is_cb */);
}

std::pair<int, int>
inverse_16x16_luma_DC(int idx)
{
    switch (idx) {
    case 0:
        return { 0, 0 };
    case 1:
        return { 0, 1 };
    case 2:
        return { 1, 0 };
    case 3:
        return { 1, 1 };

    case 4:
        return { 0, 2 };
    case 5:
        return { 0, 3 };
    case 6:
        return { 1, 2 };
    case 7:
        return { 1, 3 };

    case 8:
        return { 2, 0 };
    case 9:
        return { 2, 1 };
    case 10:
        return { 3, 0 };
    case 11:
        return { 3, 1 };

    case 12:
        return { 2, 2 };
    case 13:
        return { 2, 3 };
    case 14:
        return { 3, 2 };
    case 15:
        return { 3, 3 };
    }
    return { INT32_MIN, INT32_MIN };
}

// 8.5.2 Specification of transform decoding process for luma samples of Intra_16x16 macroblock prediction mode
void MacroBlock::Specification_of_transform_decoding_process_for_luma_samples_of_Intra_16x16_macroblock_prediction_mode(
    const std::vector<std::vector<int>>& predL)
{
    // decode DC
    auto c = Inverse_scanning_process_for_4x4_transform_coefficients_and_scaling_lists(
        Intra16x16DCLevel_, true /* is_zigzag */);
    auto dcY = Scaling_and_transformation_process_for_DC_transform_coefficients_for_Intra_16x16_macroblock_type(
        sps_->BitDepthY(), QPPrimeY(), c);

    // first dim is width, second is height
    // consistent with predL
    std::vector<std::vector<int>> rMb(16, std::vector<int>(16));

    int lumaList[16];
    for (int luma4x4BlkIdx = 0; luma4x4BlkIdx < 16; luma4x4BlkIdx++) {
        auto [i, j] = inverse_16x16_luma_DC(luma4x4BlkIdx);
        lumaList[0] = dcY[i][j];

        for (int k = 1; k < 16; k++) {
            lumaList[k] = Intra16x16ACLevel_[luma4x4BlkIdx][k - 1];
        }

        auto c = Inverse_scanning_process_for_4x4_transform_coefficients_and_scaling_lists(
            lumaList, true /* is_zigzag */);
        auto r = Scaling_and_transformation_process_for_residual_4x4_blocks(
            c, ResidualData::luma);

        auto [xO, yO] = slice_->Inverse_4x4_luma_block_scanning_process(luma4x4BlkIdx);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                rMb[xO + j][yO + i] = r[i][j];
            }
        }
    }

    // TODO: TransformBypassModeFlag is true

    // 1st dim is height, 2nd dim is width
    //
    std::vector<std::vector<int>> u(16, std::vector<int>(16));
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            u[i][j] = Clip1Y(predL[j][i] + rMb[j][i]);
        }
    }

    Picture_construction_process_prior_to_deblocking_filter_process(
        PicConsType::_16x16_luma,
        -1 /* unused */,
        u,
        false /* is_cb */);
}

std::pair<int, int>
inverse_NxN_chroma_DC(int chroma4x4BlkIdx)
{
    switch (chroma4x4BlkIdx) {
    case 0:
        return { 0, 0 };
    case 1:
        return { 0, 1 };
    case 2:
        return { 1, 0 };
    case 3:
        return { 1, 1 };

    case 4:
        return { 2, 0 };
    case 5:
        return { 2, 1 };
    case 6:
        return { 3, 0 };
    case 7:
        return { 3, 1 };
    }
    return { INT32_MIN, INT32_MIN };
}

// 8.5.4 Specification of transform decoding process for chroma samples
//
void MacroBlock::Specification_of_transform_decoding_process_for_chroma_samples(
    const std::vector<std::vector<int>>& predC,
    bool is_cb)
{
    // only consider (ChromaArrayType==1 || ChromaArrayType==2)

    int iCbCr = 1;
    if (is_cb)
        iCbCr = 0;

    int numChroma4x4Blks = (sps_->MbWidthC() / 4) * (sps_->MbHeightC() / 4);

    std::vector<std::vector<int>> c;
    if (sps_->ChromaArrayType() == 1) {
        c = { { ChromaDCLevel_[iCbCr][0], ChromaDCLevel_[iCbCr][1] },
            { ChromaDCLevel_[iCbCr][2], ChromaDCLevel_[iCbCr][3] } };
    } else if (sps_->ChromaArrayType() == 2) {
        // TODO ?
        c = { { ChromaDCLevel_[iCbCr][0], ChromaDCLevel_[iCbCr][2] },
            { ChromaDCLevel_[iCbCr][1], ChromaDCLevel_[iCbCr][5] },
            { ChromaDCLevel_[iCbCr][3], ChromaDCLevel_[iCbCr][6] },
            { ChromaDCLevel_[iCbCr][4], ChromaDCLevel_[iCbCr][7] } };
    }

    auto dcC = Scaling_and_transformation_process_for_chroma_DC_transform_coefficients(
        c,
        is_cb);

    // rMb has the same dims as predC
    //
    std::vector<std::vector<int>> rMb(sps_->MbWidthC(), std::vector<int>(sps_->MbHeightC()));
    assert(predC.size() == rMb.size() && predC[0].size() == rMb[0].size());

    int chromaList[16];
    for (int chroma4x4BlkIdx = 0; chroma4x4BlkIdx < numChroma4x4Blks; chroma4x4BlkIdx++) {
        auto [i, j] = inverse_NxN_chroma_DC(chroma4x4BlkIdx);
        chromaList[0] = dcC[i][j];
        for (int k = 1; k < 16; k++) {
            chromaList[k] = ChromaACLevel_[iCbCr][chroma4x4BlkIdx][k - 1];
        }

        auto c = Inverse_scanning_process_for_4x4_transform_coefficients_and_scaling_lists(
            chromaList, true /* is_zigzag */);

        auto r = Scaling_and_transformation_process_for_residual_4x4_blocks(
            c, is_cb ? ResidualData::cb : ResidualData::cr);

        auto [xO, yO] = slice_->Inverse_4x4_chroma_block_scanning_process(chroma4x4BlkIdx);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                // the dim of rMb and r is tanspose
                //
                rMb[xO + j][yO + i] = r[i][j];
            }
        }
    }

    // TODO: TransformBypassModeFlag
    // 1st dim is height,2nd dim is width
    //
    std::vector<std::vector<int>> u(sps_->MbHeightC(), std::vector<int>(sps_->MbWidthC()));

    for (int i = 0; i < sps_->MbHeightC(); i++) {
        for (int j = 0; j < sps_->MbWidthC(); j++) {
            u[i][j] = Clip1C(predC[j][i] + rMb[j][i]);
        }
    }

    Picture_construction_process_prior_to_deblocking_filter_process(
        PicConsType::_16x16_chroma,
        -1,
        u,
        is_cb);
}

// Done
//
// 8.5.6 Inverse scanning process for 4x4 transform coefficients and scaling lists
//
std::vector<std::vector<int>>
MacroBlock::Inverse_scanning_process_for_4x4_transform_coefficients_and_scaling_lists(
    const int* input, bool is_zig_zag)
{
    // assert(input.size() == 16);

    // the row are x and column are y ?
    // inconsistent with predL or predC
    //
    std::vector<std::vector<int>> c(4, std::vector<int>(4, INT32_MIN));

    if (is_zig_zag) {
        c[0][0] = input[0];
        c[0][1] = input[1];
        c[1][0] = input[2];
        c[2][0] = input[3];
        c[1][1] = input[4];
        c[0][2] = input[5];
        c[0][3] = input[6];
        c[1][2] = input[7];
        c[2][1] = input[8];
        c[3][0] = input[9];
        c[3][1] = input[10];
        c[2][2] = input[11];
        c[1][3] = input[12];
        c[2][3] = input[13];
        c[3][2] = input[14];
        c[3][3] = input[15];
    } else {
        assert(false);
        c[0][0] = input[0];
        c[1][0] = input[1];
        c[0][1] = input[2];
        c[2][0] = input[3];
        c[3][0] = input[4];
        c[1][1] = input[5];
        c[2][1] = input[6];
        c[3][1] = input[7];
        c[0][2] = input[8];
        c[1][2] = input[9];
        c[2][2] = input[10];
        c[3][2] = input[11];
        c[0][3] = input[12];
        c[1][3] = input[13];
        c[2][3] = input[14];
        c[3][3] = input[15];
    }

    return std::move(c);
}

int MacroBlock::deblock_QPC(bool is_cb)
{
    auto func = [is_cb, this](int QPY_) {
        if (QPY_ == INT32_MIN)
            QPY_ = QPY();

        int qPOffset;
        if (is_cb)
            qPOffset = pps_->chroma_qp_index_offset();
        else
            qPOffset = pps_->second_chroma_qp_index_offset();

        int qPI = Clip3(-sps_->QpBdOffsetC(), 51, QPY_ + qPOffset);

        int QPC;
        if (qPI < 30)
            QPC = qPI;
        else if (qPI >= 30 && qPI <= 33)
            QPC = qPI - 1;
        else if (qPI >= 34 && qPI <= 36)
            QPC = qPI - 2;
        else if (qPI >= 37 && qPI <= 38)
            QPC = qPI - 3;
        else if (qPI >= 39 && qPI <= 40)
            QPC = qPI - 4;
        else if (qPI == 41)
            QPC = qPI - 5;
        else if (qPI >= 42 && qPI <= 44)
            QPC = 37;
        else if (qPI >= 45 && qPI <= 47)
            QPC = 38;
        else if (qPI >= 48 && qPI <= 51)
            QPC = 39;
        else
            assert(false);

        return QPC;
    };

    // TODO: what if is_SP_or_SI ?
    // in deblock filter, SP/SI is the same as other slice, QSY is not used
    if (mb_type() == MbType::I_PCM)
        return func(0);
    else
        return func(INT32_MIN);
}

// Done
//
// 8.5.8 Derivation process for chroma quantisation parameters
// if is_SP_or_SI, return QSC
// if not is_SP_or_SI, return QP'C
//
int MacroBlock::Derivation_process_for_chroma_quantisation_parameters(
    bool is_cb,
    bool is_SP_or_SI)
{
    int QPY_;

    if (!is_SP_or_SI)
        QPY_ = QPY();
    else
        QPY_ = slice_->QSY();

    int qPOffset;
    if (is_cb)
        qPOffset = pps_->chroma_qp_index_offset();
    else
        qPOffset = pps_->second_chroma_qp_index_offset();

    int qPI = Clip3(-sps_->QpBdOffsetC(), 51, QPY_ + qPOffset);

    int QPC;
    if (qPI < 30)
        QPC = qPI;
    else if (qPI >= 30 && qPI <= 33)
        QPC = qPI - 1;
    else if (qPI >= 34 && qPI <= 36)
        QPC = qPI - 2;
    else if (qPI >= 37 && qPI <= 38)
        QPC = qPI - 3;
    else if (qPI >= 39 && qPI <= 40)
        QPC = qPI - 4;
    else if (qPI == 41)
        QPC = qPI - 5;
    else if (qPI >= 42 && qPI <= 44)
        QPC = 37;
    else if (qPI >= 45 && qPI <= 47)
        QPC = 38;
    else if (qPI >= 48 && qPI <= 51)
        QPC = 39;
    else
        assert(false);

    int QPPrimeC = QPC + sps_->QpBdOffsetC();

    if (is_SP_or_SI)
        return QPC; // QSC
    else
        return QPPrimeC; // QP'C
}

// Done
//
// 8.5.9 Derivation process for scaling functions
//
// now only consider 4x4
std::function<int(int, int, int)>
MacroBlock::Derivation_process_for_scaling_functions(
    ResidualData residual_type,
    bool is_4x4)
{
    assert(is_4x4);

    bool mbIsInterFlag = false;
    if (is_inter_pred())
        mbIsInterFlag = true;

    int iYCbCr = INT32_MIN;
    if (sps_->separate_colour_plane_flag())
        assert(false);
    else if (residual_type == ResidualData::luma)
        iYCbCr = 0;
    else if (residual_type == ResidualData::cb)
        iYCbCr = 1;
    else if (residual_type == ResidualData::cr)
        iYCbCr = 2;

    int scaling_list_4x4_idx = iYCbCr + ((mbIsInterFlag) ? 3 : 0);

    std::vector<std::vector<int>> weightScale4x4 = Inverse_scanning_process_for_4x4_transform_coefficients_and_scaling_lists(
        pps_->ScalingList4x4(scaling_list_4x4_idx).data() /*TODO*/, true /* is_zig_zag */);

    return [weightScale4x4](int m, int i, int j) -> int {
        int k;
        if (i % 2 == 0 && j % 2 == 0)
            k = 0;
        else if (i % 2 == 1 && j % 2 == 1)
            k = 1;
        else
            k = 2;
        int v[6][3] = {
            { 10, 16, 13 },
            { 11, 18, 14 },
            { 13, 20, 16 },
            { 14, 23, 18 },
            { 16, 25, 20 },
            { 18, 29, 23 }
        };
        return weightScale4x4[i][j] * v[m][k];
    };
}

// Done
//
std::vector<std::vector<int>>
matrix_mul(const std::vector<std::vector<int>>& left, const std::vector<std::vector<int>>& right)
{
    int m = left.size();
    assert(left[0].size() == right.size());
    int n = left[0].size();
    int p = right[0].size();

    std::vector<std::vector<int>> C(m, std::vector<int>(p, 0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            for (int a = 0; a < n; a++) {
                C[i][j] += left[i][a] * right[a][j];
            }
        }
    }

    return std::move(C);
}

// Done
//
// 8.5.10 Scaling and transformation process for DC transform coefficients for Intra_16x16 macroblock type
std::vector<std::vector<int>>
MacroBlock::Scaling_and_transformation_process_for_DC_transform_coefficients_for_Intra_16x16_macroblock_type(
    int bitDepth,
    int qP,
    const std::vector<std::vector<int>>& c)
{

    if (TransformBypassModeFlag()) {
        return c;
    } else {
        std::vector<std::vector<int>> right = {
            { 1, 1, 1, 1 },
            { 1, 1, -1, -1 },
            { 1, -1, -1, 1 },
            { 1, -1, 1, -1 },
        };
        auto tmp = matrix_mul(c, right);

        auto f = matrix_mul(right, tmp);

        std::vector<std::vector<int>> dcY(4, std::vector<int>(4, INT32_MIN));

        auto LevelScale4x4 = Derivation_process_for_scaling_functions(ResidualData::luma, true /* is_4x4 */);

        for (int i = 0; i < f.size(); i++) {
            for (int j = 0; j < f[0].size(); j++) {
                if (qP >= 36) {
                    dcY[i][j] = (f[i][j] * LevelScale4x4(qP % 6, 0, 0)) << (qP / 6 - 6);
                } else {
                    dcY[i][j] = (f[i][j] * LevelScale4x4(qP % 6, 0, 0) + (1 << (5 - qP / 6)))
                        >> (6 - qP / 6);
                }
            }
        }

        return std::move(dcY);
    }
}

// Done
//
// 8.5.11 Scaling and transformation process for chroma DC transform coefficients
std::vector<std::vector<int>>
MacroBlock::Scaling_and_transformation_process_for_chroma_DC_transform_coefficients(
    const std::vector<std::vector<int>>& c,
    bool is_cb)
{
    assert(sps_->ChromaArrayType() == 1 || sps_->ChromaArrayType() == 2);

    int bitDepth = sps_->BitDepthC();
    int qP = Derivation_process_for_chroma_quantisation_parameters(
        is_cb, false /* is_SP_or_SI */);

    if (TransformBypassModeFlag()) {
        auto dcC = c;
        return std::move(dcC);
    } else {

        // where is the bitDepth para?
        auto f = Transformation_process_for_chroma_DC_transform_coefficients(c, is_cb);

        auto dcC = Scaling_process_for_chroma_DC_transform_coefficients(
            f, bitDepth, qP, is_cb);

        return std::move(dcC);
    }
}

// Done
//
// 8.5.11.1 Transformation process for chroma DC transform coefficients
std::vector<std::vector<int>>
MacroBlock::Transformation_process_for_chroma_DC_transform_coefficients(
    const std::vector<std::vector<int>>& c,
    bool is_cb)
{
    if (sps_->ChromaArrayType() == 1) {
        std::vector<std::vector<int>> right = {
            { 1, 1 },
            { 1, -1 }
        };

        auto tmp = matrix_mul(c, right);
        auto f = matrix_mul(right, tmp);

        return std::move(f);

    } else if (sps_->ChromaArrayType() == 2) {
        std::vector<std::vector<int>> left = {
            { 1, 1, 1, 1 },
            { 1, 1, -1, -1 },
            { 1, -1, -1, 1 },
            { 1, -1, 1, -1 }
        };

        std::vector<std::vector<int>> right = {
            { 1, 1 },
            { 1, -1 }
        };

        auto tmp = matrix_mul(c, right);
        auto f = matrix_mul(left, tmp);

        return std::move(f);
    }
    return c;
}

// Done
//
// 8.5.11.2 Scaling process for chroma DC transform coefficients
std::vector<std::vector<int>>
MacroBlock::Scaling_process_for_chroma_DC_transform_coefficients(
    const std::vector<std::vector<int>>& f,
    int bitDepth,
    int qP,
    bool is_cb)
{
    std::vector<std::vector<int>> dcC = f;
    auto LevelScale4x4 = Derivation_process_for_scaling_functions(
        is_cb ? ResidualData::cb : ResidualData::cr,
        true /* is_4x4 */);

    if (sps_->ChromaArrayType() == 1) {
        for (int i = 0; i < f.size(); i++) {
            for (int j = 0; j < f[0].size(); j++) {
                dcC[i][j] = ((f[i][j] * LevelScale4x4(qP % 6, 0, 0)) << (qP / 6)) >> 5;
            }
        }
    } else if (sps_->ChromaArrayType() == 2) {
        int qPDC = qP + 3;
        for (int i = 0; i < f.size(); i++) {
            for (int j = 0; j < f[0].size(); j++) {
                if (qPDC >= 36) {
                    dcC[i][j] = (f[i][j] * LevelScale4x4(qPDC % 6, 0, 0)) << (qPDC / 6 - 6);
                } else
                    dcC[i][j] = (f[i][j] * LevelScale4x4(qPDC % 6, 0, 0) + (1 << (5 - qPDC / 6))) >> (6 - qPDC / 6);
            }
        }
    }
    return std::move(dcC);
}

// Done
//
// 8.5.12 Scaling and transformation process for residual 4x4 blocks
std::vector<std::vector<int>>
MacroBlock::Scaling_and_transformation_process_for_residual_4x4_blocks(
    const std::vector<std::vector<int>>& c,
    ResidualData residual_type)
{
    assert(c.size() == 4 && c[0].size() == 4);

    bool is_luma = (residual_type == ResidualData::luma);
    bool is_cb = (residual_type == ResidualData::cb);

    int bitDepth;
    if (is_luma)
        bitDepth = sps_->BitDepthY();
    else
        bitDepth = sps_->BitDepthC();

    int sMbFlag = 0;
    if (mb_type() == MbType::SI
        || (is_inter_pred() && slice_->is_SP_slice()))
        sMbFlag = 1;

    int qP;
    if (is_luma && sMbFlag == 0)
        qP = QPPrimeY();
    else if (is_luma && sMbFlag == 1)
        qP = slice_->QSY();
    else if (!is_luma && sMbFlag == 0) {
        qP = Derivation_process_for_chroma_quantisation_parameters(is_cb, false /* is_SP_or_SI */);
    } else if (!is_luma && sMbFlag == 1) {
        qP = Derivation_process_for_chroma_quantisation_parameters(is_cb, true /* is_SP_or_SI */);
    }

    if (TransformBypassModeFlag()) {
        auto r = c;
        return std::move(r);
    } else {
        auto d = Scaling_process_for_residual_4x4_blocks(
            bitDepth, qP, c, residual_type);
        auto r = Transformation_process_for_residual_4x4_blocks(
            bitDepth, d);
        return std::move(r);
    }
}

// Done
//
// 8.5.12.1 Scaling process for residual 4x4 blocks
std::vector<std::vector<int>>
MacroBlock::Scaling_process_for_residual_4x4_blocks(
    int bitDepth,
    int qP,
    const std::vector<std::vector<int>>& c,
    ResidualData residual_type)
{
    std::vector<std::vector<int>> d = c;

    auto LevelScale4x4 = Derivation_process_for_scaling_functions(residual_type, true /* is_4x4 */);

    for (int i = 0; i < c.size(); i++) {
        for (int j = 0; j < c[0].size(); j++) {
            if (qP >= 24)
                d[i][j]
                    = (c[i][j] * LevelScale4x4(qP % 6, i, j)) << (qP / 6 - 4);
            else
                d[i][j]
                    = (c[i][j] * LevelScale4x4(qP % 6, i, j) + (1 << (3 - qP / 6))) >> (4 - qP / 6);
        }
    }

    if (residual_type != ResidualData::luma
        || (residual_type == ResidualData::luma
            && mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_16x16))
        d[0][0] = c[0][0];

    return std::move(d);
}

// Done
//
// 8.5.12.2 Transformation process for residual 4x4 blocks
std::vector<std::vector<int>>
MacroBlock::Transformation_process_for_residual_4x4_blocks(
    int bitDepth,
    const std::vector<std::vector<int>>& d)
{
    std::vector<std::vector<int>> r = d;

    int e[4][4];
    for (int i = 0; i < 4; i++)
        e[i][0] = d[i][0] + d[i][2];
    for (int i = 0; i < 4; i++)
        e[i][1] = d[i][0] - d[i][2];
    for (int i = 0; i < 4; i++)
        e[i][2] = (d[i][1] >> 1) - d[i][3];
    for (int i = 0; i < 4; i++)
        e[i][3] = d[i][1] + (d[i][3] >> 1);

    int f[4][4];
    for (int i = 0; i < 4; i++)
        f[i][0] = e[i][0] + e[i][3];
    for (int i = 0; i < 4; i++)
        f[i][1] = e[i][1] + e[i][2];
    for (int i = 0; i < 4; i++)
        f[i][2] = e[i][1] - e[i][2];
    for (int i = 0; i < 4; i++)
        f[i][3] = e[i][0] - e[i][3];

    int g[4][4];
    for (int j = 0; j < 4; j++)
        g[0][j] = f[0][j] + f[2][j];
    for (int j = 0; j < 4; j++)
        g[1][j] = f[0][j] - f[2][j];
    for (int j = 0; j < 4; j++)
        g[2][j] = (f[1][j] >> 1) - f[3][j];
    for (int j = 0; j < 4; j++)
        g[3][j] = f[1][j] + (f[3][j] >> 1);

    int h[4][4];
    for (int j = 0; j < 4; j++)
        h[0][j] = g[0][j] + g[3][j];
    for (int j = 0; j < 4; j++)
        h[1][j] = g[1][j] + g[2][j];
    for (int j = 0; j < 4; j++)
        h[2][j] = g[1][j] - g[2][j];
    for (int j = 0; j < 4; j++)
        h[3][j] = g[0][j] - g[3][j];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            r[i][j] = (h[i][j] + 32) >> 6;
        }
    }

    return std::move(r);
}

// 8.5.14 Picture construction process prior to deblocking filter process
void MacroBlock::Picture_construction_process_prior_to_deblocking_filter_process(
    PicConsType BlkType,
    int BlkIdx,
    const std::vector<std::vector<int>>& u,
    bool is_cb)
{
    auto [xP, yP] = slice_->Inverse_macroblock_scanning_process(CurrMbAddr_);

    if (BlkType == PicConsType::_16x16_luma || BlkType == PicConsType::_4x4_luma) {
        int xO, yO;
        int nE;
        if (BlkType == PicConsType::_16x16_luma) {
            xO = 0;
            yO = 0;
            nE = 16;
        } else if (BlkType == PicConsType::_4x4_luma) {
            auto t = slice_->Inverse_4x4_luma_block_scanning_process(BlkIdx);
            nE = 4;
            xO = t.first;
            yO = t.second;

        } else {
            assert(false);
        }

        for (int i = 0; i < nE; i++) {
            for (int j = 0; j < nE; j++) {
                // only consider frame mode
                //
                // 1st dim of u is height, 2nd dim is width
                slice_->set_constructed_luma(xP + xO + j, yP + yO + i, u[i][j]);
            }
        }

    } else {
        int nW, nH;
        int xO, yO;
        if (BlkType == PicConsType::_16x16_chroma) {
            nW = sps_->MbWidthC();
            nH = sps_->MbHeightC();
            xO = 0;
            yO = 0;
        } else if (BlkType == PicConsType::_4x4_chroma) {
            nW = 4;
            nH = 4;
            if (sps_->ChromaArrayType() == 1 || sps_->ChromaArrayType() == 2) {
                auto t = slice_->Inverse_4x4_chroma_block_scanning_process(BlkIdx);
                xO = t.first;
                yO = t.second;
            } else {
                assert(false);
            }
        }

        int SubWidthC = sps_->SubWidthC();
        int SubHeightC = sps_->SubHeightC();

        for (int i = 0; i < nH; i++) {
            for (int j = 0; j < nW; j++) {
                // only consider frame mode
                //
                // int idx, int x, int y, int value
                //
                // 1st dim of u is height, 2nd dim is width
                slice_->set_constructed_chroma(is_cb ? 0 : 1,
                    xP / SubWidthC + xO + j,
                    yP / SubHeightC + yO + i,
                    u[i][j]);
            }
        }
    }
}