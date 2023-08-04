
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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
            1, // int NumMbPart
            16, // int MbPartWidth
            16, // int MbPartHeight
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

void MacroBlock::parse_MacroBlock()
{
    mb_type_ = rbsp_data_->read_ue();
    determine_mb_type();

    spdlog::trace("CurrMbAddr is {}, the MacroBlock type is {}", CurrMbAddr_, mb_type_proxy_.name());

    if (mb_type_proxy_.mb_type() == MbType::I_PCM) {
        while (!rbsp_data_->byte_aligned())
            rbsp_data_->read_u1();

        for (int i = 0; i < 256; i++) {
            pcm_sample_luma_[i] = rbsp_data_->read_u(sps_->bit_depth_luma());
        }

        for (int i = 0; i < 2 * sps_->MbHeightC() * sps_->MbWidthC(); i++) {
            pcm_sample_chroma_.push_back(rbsp_data_->read_u(sps_->bit_depth_chroma()));
        }
    } else {
        // here call MbPartPredMode_0() is safe, as the MbType::I_NxN has been excluded
        // but NumMbPart() for SI MB is not defined in spec.
        if (mb_type_proxy_.mb_type() != MbType::I_NxN
            && mb_type_proxy_.MbPartPredMode_0() != MbPartPredMode::Intra_16x16
            && mb_type_proxy_.NumMbPart() == 4) {
            parse_sub_mb_pred();
            for (int mbPartIdx = 0; mbPartIdx < 4; mbPartIdx++) { // B_Skip is not processed here
                if (sub_mb_type_proxy_[mbPartIdx].sub_mb_type() != SubMbType::B_Direct_8x8) {
                    if (sub_mb_type_proxy_[mbPartIdx].NumSubMbPart() > 1)
                        noSubMbPartSizeLessThan8x8Flag_ = 0;

                } else if (!sps_->direct_8x8_inference_flag()) // if sps_->direct_8x8_inference_flag() is true
                    noSubMbPartSizeLessThan8x8Flag_ = 0; // B_Direct_8x8 will not be sub partitioned
            }
        } else {
            if (pps_->transform_8x8_mode_flag()
                && mb_type_proxy_.mb_type() == MbType::I_NxN)
                transform_size_8x8_flag_ = rbsp_data_->read_u1();
            parse_mb_pred();
        }
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

    } else if (mb_type_proxy_.MbPartPredMode_0() != MbPartPredMode::Direct) { // if Direct mode, no ref idx or mvd

        for (int mbPartIdx = 0; mbPartIdx < mb_type_proxy_.NumMbPart(); mbPartIdx++) {

            if ((slice_->num_ref_idx_l0_active_minus1() > 0 || slice_->mb_field_decoding_flag() != slice_->field_pic_flag())
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

    for (int mbPartIdx = 0; mbPartIdx < 4; mbPartIdx++)
        if ((slice_->num_ref_idx_l0_active_minus1() > 0 || slice_->mb_field_decoding_flag() != slice_->field_pic_flag())
            && mb_type_proxy_.mb_type() != MbType::P_8x8ref0
            && sub_mb_type_proxy_[mbPartIdx].sub_mb_type() != SubMbType::B_Direct_8x8
            && sub_mb_type_proxy_[mbPartIdx].SubMbPredMode() != SubMbPredMode::Pred_L1)
            ref_idx_l0_[mbPartIdx] = rbsp_data_->read_te(slice_->num_ref_idx_l0_active_minus1());

    for (int mbPartIdx = 0; mbPartIdx < 4; mbPartIdx++)
        if ((slice_->num_ref_idx_l1_active_minus1() > 0 || slice_->mb_field_decoding_flag() != slice_->field_pic_flag())
            && sub_mb_type_proxy_[mbPartIdx].sub_mb_type() != SubMbType::B_Direct_8x8
            && sub_mb_type_proxy_[mbPartIdx].SubMbPredMode() != SubMbPredMode::Pred_L0)
            ref_idx_l1_[mbPartIdx] = rbsp_data_->read_te(slice_->num_ref_idx_l1_active_minus1());

    for (int mbPartIdx = 0; mbPartIdx < 4; mbPartIdx++)
        if (sub_mb_type_proxy_[mbPartIdx].sub_mb_type() != SubMbType::B_Direct_8x8
            && sub_mb_type_proxy_[mbPartIdx].SubMbPredMode() != SubMbPredMode::Pred_L1)
            for (int subMbPartIdx = 0;
                 subMbPartIdx < sub_mb_type_proxy_[mbPartIdx].NumSubMbPart();
                 subMbPartIdx++)
                for (int compIdx = 0; compIdx < 2; compIdx++)
                    mvd_l0_[mbPartIdx][subMbPartIdx][compIdx] = rbsp_data_->read_se();

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
    int x, y;
    if (mb_type_proxy_.mb_type() == MbType::P_8x8
        || mb_type_proxy_.mb_type() == MbType::P_8x8ref0
        || mb_type_proxy_.mb_type() == MbType::B_8x8) {
        x = InverseRasterScan(subMbPartIdx, sub_mb_type_proxy_[mbPartIdx].SubMbPartWidth(),
            sub_mb_type_proxy_[mbPartIdx].SubMbPartHeight(), 8, 0);

        y = InverseRasterScan(subMbPartIdx, sub_mb_type_proxy_[mbPartIdx].SubMbPartWidth(),
            sub_mb_type_proxy_[mbPartIdx].SubMbPartHeight(), 8, 1);
    } else {
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
MacroBlock::Derivation_process_for_neighbouring_partitions(int mbPartIdx, int subMbPartIdx)
{
    auto func = [mbPartIdx, subMbPartIdx, this](int xD, int yD, bool is_C) -> std::tuple<int, int, int> {
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

            // P_Skip B_Skip B_Direct_16x16 are not considered as partitioned？
            if (mb_type_proxy_.mb_type() == MbType::P_Skip
                || mb_type_proxy_.mb_type() == MbType::B_Skip
                || mb_type_proxy_.mb_type() == MbType::B_Direct_16x16)
                predPartWidth = 16;
            else if (mb_type_proxy_.mb_type() == MbType::B_8x8) {
                // TODO: direct_spatial_mv_pred_flag
                // ?
                if (sub_mb_type_proxy_[mbPartIdx].sub_mb_type() == SubMbType::B_Direct_8x8)
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

        auto t = slice_->get_mb_by_addr(mbAddrN).Derivation_process_for_macroblock_and_sub_macroblock_partition_indices(xW, yW);
        std::tuple<int, int, int> res = { mbAddrN, std::get<0>(t), std::get<1>(t) };

        // TODO:
        // When the partition given by mbPartIdxN and subMbPartIdxN is not yet decoded, the macroblock
        // partition mbPartIdxN and the sub-macroblock partition subMbPartIdxN are marked as not
        // available.
        if (mbAddrN == CurrMbAddr_) {
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