
#include <functional>

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

    Inter_prediction_process();

    // update QP_Y_PREV
    slice_->update_QP_Y_PREV(QPY());
}

void MacroBlock::parse_B_Skip()
{
    mb_type_ = (int)MbType::B_Skip;
    mb_type_proxy_ = MbTypeProxy { (enum MbType)mb_type_, slice_, this };

    Inter_prediction_process();

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
    } else {
        Inter_prediction_process();
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
    if (mb_type_proxy_.no_subMbPartIdx())
        assert(subMbPartIdx == 0);

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
    // only inter pred has partitions
    assert(is_inter_pred());

    auto func = [mbPartIdx, subMbPartIdx, currSubMbType, this](int xD, int yD, bool is_C) -> std::tuple<int, int, int> {
        auto [x, y] = Inverse_macroblock_partition_scanning_process(mbPartIdx);
        int xS = 0, yS = 0;

        // B_Skip and B_Direct_16x16 is not sub partitioned?
        if (mb_type_proxy_.mb_type() == MbType::P_8x8
            || mb_type_proxy_.mb_type() == MbType::P_8x8ref0
            || mb_type_proxy_.mb_type() == MbType::B_8x8) {

            auto tmp = Inverse_sub_macroblock_partition_scanning_process(mbPartIdx, subMbPartIdx);
            xS = tmp.first;
            yS = tmp.second;
        }

        if (is_C) {
            int predPartWidth;

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

        auto [t_mbPartIdx, t_subMbPartIdx] = t;
        assert(0 <= t_mbPartIdx && t_mbPartIdx < 4
            && 0 <= t_subMbPartIdx && t_subMbPartIdx < 4);

        // TODO: Revisit this condition
        // When the partition given by mbPartIdxN and subMbPartIdxN is not yet decoded, the macroblock
        // partition mbPartIdxN and the sub-macroblock partition subMbPartIdxN are marked as not
        // available.
        if (mbAddrN == CurrMbAddr_) {
            if (t_mbPartIdx > mbPartIdx || (t_mbPartIdx == mbPartIdx && t_subMbPartIdx > subMbPartIdx))
                return { INT32_MIN, INT32_MIN, INT32_MIN };
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
// I_PCM has been excluded in Derivation_process_for_the_luma_bs()
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

        // TODO revisit this
        int luma4x4BlkIdx = slice_->Derivation_process_for_4x4_luma_block_indices(x, y);

        if (mb_type_proxy_.MbPartPredMode_0() == MbPartPredMode::Intra_16x16) {
            if (i16x16AClevel_non_zeros_[luma4x4BlkIdx] == INT32_MIN) {
                int non_zeros = 0;
                for (int i = 0; i < 15; i++)
                    non_zeros += (i16x16AClevel_[luma4x4BlkIdx][i] ? 1 : 0);
                i16x16AClevel_non_zeros_[luma4x4BlkIdx] = non_zeros;
            }
            return i16x16AClevel_non_zeros_[luma4x4BlkIdx] > 0;
        } else {
            if (level4x4_non_zeros_[luma4x4BlkIdx] == INT32_MIN) {
                int non_zeros = 0;
                for (int i = 0; i < 16; i++)
                    non_zeros += (level4x4_[luma4x4BlkIdx][i] ? 1 : 0);
                level4x4_non_zeros_[luma4x4BlkIdx] = non_zeros;
            }
            return level4x4_non_zeros_[luma4x4BlkIdx] > 0;
        }
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

// 8.4 Inter prediction process
//
void MacroBlock::Inter_prediction_process()
{
    if (slice_->PicOrderCnt() == 182) {
        // debug
        int lo = 0;
        lo += 2;
    }
    if (CurrMbAddr_ == 60) {
        // debug
        int lk = 0;
        lk -= 3;
    }

    int SubWidthC = sps_->SubWidthC();
    int SubHeightC = sps_->SubHeightC();
    int MbWidthC = sps_->MbWidthC();
    int MbHeightC = sps_->MbHeightC();

    std::vector<std::vector<int>> predL(16, std::vector<int>(16, 0));
    std::vector<std::vector<int>> predCb(MbWidthC, std::vector<int>(MbHeightC, 0));
    std::vector<std::vector<int>> predCr(MbWidthC, std::vector<int>(MbHeightC, 0));

    auto mb_type = mb_type_proxy_.mb_type();
    int num_mbPartIdx;

    // the NumMbPart() for B_Skip and B_Direct_16x16 are undefined in ITU Rec table xxx
    if (mb_type == MbType::B_Skip
        || mb_type == MbType::B_Direct_16x16)
        num_mbPartIdx = 4;
    else
        num_mbPartIdx = mb_type_proxy_.NumMbPart();

    assert(num_mbPartIdx > 0 && num_mbPartIdx <= 4);

    // usage of this?
    int MvCnt = 0;

    auto pred_func = [SubWidthC, SubHeightC, this](int mbPartIdx,
                         int subMbPartIdx,
                         int has_sub_idx,
                         int partWidth,
                         int partHeight,
                         int partWidthC,
                         int partHeightC,
                         /* output */
                         int& MvCnt,
                         std::vector<std::vector<int>>& predL,
                         std::vector<std::vector<int>>& predCb,
                         std::vector<std::vector<int>>& predCr) {
        auto&& [predPartL, predPartCb, predPartCr,
            mvL0, mvL1, mvCL0, mvCL1,
            refIdxL0, refIdxL1,
            predFlagL0, predFlagL1]
            = derive_predict(
                mbPartIdx,
                subMbPartIdx,
                has_sub_idx,
                partWidth,
                partHeight,
                partWidthC,
                partHeightC,
                /* output */
                MvCnt /* int& */);
        auto t1 = predPartL;
        auto t2 = predPartCb;
        auto t3 = predPartCr;

        MvL0_[mbPartIdx][subMbPartIdx][0] = mvL0[0];
        MvL0_[mbPartIdx][subMbPartIdx][1] = mvL0[1];
        MvL1_[mbPartIdx][subMbPartIdx][0] = mvL1[0];
        MvL1_[mbPartIdx][subMbPartIdx][1] = mvL1[1];
        RefIdxL0_[mbPartIdx] = refIdxL0;
        RefIdxL1_[mbPartIdx] = refIdxL1;
        PredFlagL0_[mbPartIdx] = predFlagL0;
        PredFlagL1_[mbPartIdx] = predFlagL1;

        auto [xP, yP] = Inverse_macroblock_partition_scanning_process(mbPartIdx);

        auto [xS, yS] = Inverse_sub_macroblock_partition_scanning_process(mbPartIdx, subMbPartIdx);

        for (int x = 0; x < partWidth; x++) {
            for (int y = 0; y < partHeight; y++) {
                predL[xP + xS + x][yP + yS + y] = predPartL[x][y];
            }
        }
        for (int x = 0; x < partWidthC; x++) {
            for (int y = 0; y < partHeightC; y++) {
                predCb[xP / SubWidthC + xS / SubWidthC + x][yP / SubHeightC + yS / SubHeightC + y]
                    = predPartCb[x][y];

                predCr[xP / SubWidthC + xS / SubWidthC + x][yP / SubHeightC + yS / SubHeightC + y]
                    = predPartCr[x][y];
            }
        }
    };

    if (mb_type != MbType::P_8x8
        && mb_type != MbType::P_8x8ref0
        && mb_type != MbType::B_Skip
        && mb_type != MbType::B_Direct_16x16
        && mb_type != MbType::B_8x8) {

        // no sub mb part

        bool has_sub_idx = false;
        int subMbPartIdx = 0;

        int partWidth = mb_type_proxy_.MbPartWidth();
        int partHeight = mb_type_proxy_.MbPartHeight();
        int partWidthC = partWidth / SubWidthC;
        int partHeightC = partHeight / SubHeightC;

        for (int mbPartIdx = 0; mbPartIdx < num_mbPartIdx; mbPartIdx++) {

            pred_func(mbPartIdx,
                subMbPartIdx,
                has_sub_idx,
                partWidth,
                partHeight,
                partWidthC,
                partHeightC,
                /* output */
                MvCnt /* int &*/,
                predL /* std::vector<std::vector<int>>& */,
                predCb /* std::vector<std::vector<int>>& */,
                predCr /* std::vector<std::vector<int>>& */);
        }
    } else if (mb_type == MbType::P_8x8
        || mb_type == MbType::P_8x8ref0
        || mb_type == MbType::B_8x8) {

        assert(num_mbPartIdx == 4);
        bool has_sub_idx = true;

        for (int mbPartIdx = 0; mbPartIdx < num_mbPartIdx; mbPartIdx++) {
            if (sub_mb_type_proxy_[mbPartIdx].sub_mb_type() != SubMbType::B_Direct_8x8) {
                int num_subMbPartIdx = sub_mb_type_proxy_[mbPartIdx].NumSubMbPart();
                int partWidth = sub_mb_type_proxy_[mbPartIdx].SubMbPartWidth();
                int partHeight = sub_mb_type_proxy_[mbPartIdx].SubMbPartHeight();
                int partWidthC = partWidth / SubWidthC;
                int partHeightC = partHeight / SubHeightC;

                for (int subMbPartIdx = 0; subMbPartIdx < num_subMbPartIdx; subMbPartIdx++) {

                    pred_func(mbPartIdx,
                        subMbPartIdx,
                        has_sub_idx,
                        partWidth,
                        partHeight,
                        partWidthC,
                        partHeightC,
                        /* output */
                        MvCnt /* int &*/,
                        predL /* std::vector<std::vector<int>>& */,
                        predCb /* std::vector<std::vector<int>>& */,
                        predCr /* std::vector<std::vector<int>>& */);
                }

            } else {
                assert(mb_type == MbType::B_8x8);

                int num_subMbPartIdx = 4;
                int partWidth = 4;
                int partHeight = 4;
                int partWidthC = partWidth / SubWidthC;
                int partHeightC = partHeight / SubHeightC;

                for (int subMbPartIdx = 0; subMbPartIdx < num_subMbPartIdx; subMbPartIdx++) {

                    pred_func(mbPartIdx,
                        subMbPartIdx,
                        has_sub_idx,
                        partWidth,
                        partHeight,
                        partWidthC,
                        partHeightC,
                        /* output */
                        MvCnt /* int &*/,
                        predL /* std::vector<std::vector<int>>& */,
                        predCb /* std::vector<std::vector<int>>& */,
                        predCr /* std::vector<std::vector<int>>& */);
                }
            }
        }
    } else if (mb_type == MbType::B_Skip
        || mb_type == MbType::B_Direct_16x16) {

        bool has_sub_idx = true;

        for (int mbPartIdx = 0; mbPartIdx < num_mbPartIdx; mbPartIdx++) {
            int num_subMbPartIdx = 4;
            int partWidth = 4;
            int partHeight = 4;
            int partWidthC = partWidth / SubWidthC;
            int partHeightC = partHeight / SubHeightC;

            for (int subMbPartIdx = 0; subMbPartIdx < num_subMbPartIdx; subMbPartIdx++) {

                pred_func(mbPartIdx,
                    subMbPartIdx,
                    has_sub_idx,
                    partWidth,
                    partHeight,
                    partWidthC,
                    partHeightC,
                    /* output */
                    MvCnt /* int &*/,
                    predL /* std::vector<std::vector<int>>& */,
                    predCb /* std::vector<std::vector<int>>& */,
                    predCr /* std::vector<std::vector<int>>& */);
            }
        }
    }

    for (int luma4x4BlkIdx = 0; luma4x4BlkIdx < 16; luma4x4BlkIdx++) {

        Specification_of_transform_decoding_process_for_4x4_luma_residual_blocks(
            predL,
            luma4x4BlkIdx);
    }

    // chroma
    Specification_of_transform_decoding_process_for_chroma_samples(
        predCb,
        true);

    Specification_of_transform_decoding_process_for_chroma_samples(
        predCr,
        false);

    // debug
    int lo = 9;
    lo -= 8;
}

// predPartL, predPartCb, predPartCr,
// mvL0, mvL1, mvCL0, mvCL1,
// refIdxL0, refIdxL1,
// predFlagL0, predFlagL1
//
std::tuple<std::vector<std::vector<int>>,
    std::vector<std::vector<int>>,
    std::vector<std::vector<int>>,
    std::array<int, 2>,
    std::array<int, 2>,
    std::array<int, 2>,
    std::array<int, 2>,
    int, int, int, int>
MacroBlock::derive_predict(
    int mbPartIdx,
    int subMbPartIdx,
    bool has_sub_idx,
    int partWidth,
    int partHeight,
    int partWidthC,
    int partHeightC,
    /* output */
    int& MvCnt)
{
    //debug
    if (mbPartIdx == 2) {
        int lp = 0;
        lp += 1;
    }

    // 8.4.1
    auto&& [mvL0, mvL1, mvCL0, mvCL1,
        refIdxL0, refIdxL1,
        predFlagL0, predFlagL1,
        subMvCnt]
        = Derivation_process_for_motion_vector_components_and_reference_indices(
            mbPartIdx,
            subMbPartIdx,
            has_sub_idx);

    MvCnt += subMvCnt;

    int logWDL = INT32_MIN, w0L = INT32_MIN, w1L = INT32_MIN, o0L = INT32_MIN, o1L = INT32_MIN;
    int logWDCb = INT32_MIN, w0Cb = INT32_MIN, w1Cb = INT32_MIN, o0Cb = INT32_MIN, o1Cb = INT32_MIN;
    int logWDCr = INT32_MIN, w0Cr = INT32_MIN, w1Cr = INT32_MIN, o0Cr = INT32_MIN, o1Cr = INT32_MIN;

    // 8.4.3
    if ((pps_->weighted_pred_flag() && slice_->is_P_or_SP())
        || (pps_->weighted_bipred_idc() > 0 && slice_->is_B_slice())) {

        // { { logWDL, w0L, w1L, o0L, o1L },
        // { logWDCb, w0Cb, w1Cb, o0Cb, o1Cb },
        // { logWDCr, w0Cr, w1Cr, o0Cr, o1Cr } }
        //

        auto&& [w1, w2, w3] = Derivation_process_for_prediction_weights(
            refIdxL0, refIdxL1,
            predFlagL0, predFlagL1);

        logWDL = std::get<0>(w1);
        w0L = std::get<1>(w1);
        w1L = std::get<2>(w1);
        o0L = std::get<3>(w1);
        o1L = std::get<4>(w1);

        logWDCb = std::get<0>(w2);
        w0Cb = std::get<1>(w2);
        w1Cb = std::get<2>(w2);
        o0Cb = std::get<3>(w2);
        o1Cb = std::get<4>(w2);

        logWDCr = std::get<0>(w3);
        w0Cr = std::get<1>(w3);
        w1Cr = std::get<2>(w3);
        o0Cr = std::get<3>(w3);
        o1Cr = std::get<4>(w3);
    }

    // 8.4.2
    auto&& [predPartL, predPartCb, predPartCr] = Decoding_process_for_Inter_prediction_samples(
        mbPartIdx,
        subMbPartIdx,
        partWidth,
        partHeight,
        partWidthC,
        partHeightC,
        mvL0,
        mvL1,
        mvCL0,
        mvCL1,
        refIdxL0,
        refIdxL1,
        predFlagL0,
        predFlagL1,
        { { logWDL, w0L, w1L, o0L, o1L },
            { logWDCb, w0Cb, w1Cb, o0Cb, o1Cb },
            { logWDCr, w0Cr, w1Cr, o0Cr, o1Cr } });

    return { predPartL, predPartCb, predPartCr,
        mvL0, mvL1, mvCL0, mvCL1,
        refIdxL0, refIdxL1,
        predFlagL0, predFlagL1 };
}

// 8.4.1 Derivation process for motion vector components and reference indices
//
// mvL0 mvL1
// mvCL0 mvCL1
// refIdxL0 refIdxL1
// predFlagL0 predFlagL1
// subMvCnt
//
std::tuple<
    std::array<int, 2>, std::array<int, 2>,
    std::array<int, 2>, std::array<int, 2>,
    int, int,
    int, int,
    int>
MacroBlock::Derivation_process_for_motion_vector_components_and_reference_indices(
    int mbPartIdx,
    int subMbPartIdx,
    bool has_sub_idx)
{
    assert(has_sub_idx == !mb_type_proxy_.no_subMbPartIdx());

    std::array<int, 2> mvL0, mvL1;
    std::array<int, 2> mvCL0, mvCL1;
    int refIdxL0, refIdxL1;
    int predFlagL0, predFlagL1;
    int subMvCnt;

    auto mb_type = mb_type_proxy_.mb_type();

    if (mb_type == MbType::P_Skip) {
        assert(mbPartIdx == 0 && subMbPartIdx == 0);

        // 8.4.1.1
        // mvL0, refIdxL0
        auto&& tmp = Derivation_process_for_luma_motion_vectors_for_skipped_macroblocks_in_P_and_SP_slices();
        mvL0 = std::get<0>(tmp);
        refIdxL0 = std::get<1>(tmp);

        predFlagL0 = 1;

        mvL1 = { INT32_MIN, INT32_MIN };
        refIdxL1 = INT32_MIN;
        predFlagL1 = 0;

        subMvCnt = 1;
    } else if (mb_type == MbType::B_Skip
        || mb_type == MbType::B_Direct_16x16
        || (mb_type == MbType::B_8x8
            && sub_mb_type_proxy_[mbPartIdx].sub_mb_type() == SubMbType::B_Direct_8x8)) {

        // 8.4.1.2
        //
        // refIdxL0, refIdxL1, mvL0, mvL1, subMvCnt, predFlagL0, predFlagL1
        //
        auto&& tmp = Derivation_process_for_luma_motion_vectors_for_B_Skip_B_Direct_16x16_and_B_Direct_8x8(
            mbPartIdx,
            subMbPartIdx);

        refIdxL0 = std::get<0>(tmp);
        refIdxL1 = std::get<1>(tmp);
        mvL0 = std::get<2>(tmp);
        mvL1 = std::get<3>(tmp);
        subMvCnt = std::get<4>(tmp);
        predFlagL0 = std::get<5>(tmp);
        predFlagL1 = std::get<6>(tmp);
    } else {
        bool is_Pred_L0 = false,
             is_Pred_L1 = false,
             is_BiPred = false;

        if (!has_sub_idx) {
            if (mb_type_proxy_.MbPartPredModeByIdx(mbPartIdx) == MbPartPredMode::Pred_L0) {
                is_Pred_L0 = true;
            } else if (mb_type_proxy_.MbPartPredModeByIdx(mbPartIdx) == MbPartPredMode::Pred_L1) {
                is_Pred_L1 = true;
            } else if (mb_type_proxy_.MbPartPredModeByIdx(mbPartIdx) == MbPartPredMode::BiPred) {
                is_BiPred = true;
            } else
                assert(false);
        } else {
            if (sub_mb_type_proxy_[mbPartIdx].SubMbPredMode() == SubMbPredMode::Pred_L0) {
                is_Pred_L0 = true;
            } else if (sub_mb_type_proxy_[mbPartIdx].SubMbPredMode() == SubMbPredMode::Pred_L1) {
                is_Pred_L1 = true;
            } else if (sub_mb_type_proxy_[mbPartIdx].SubMbPredMode() == SubMbPredMode::BiPred) {
                is_BiPred = true;
            } else
                assert(false);
        }

        if (is_Pred_L0) {
            refIdxL0 = ref_idx_l0_[mbPartIdx];
            predFlagL0 = 1;
            refIdxL1 = -1;
            predFlagL1 = 0;

            assert(refIdxL0 >= 0);
        } else if (is_Pred_L1) {
            refIdxL0 = -1;
            predFlagL0 = 0;
            refIdxL1 = ref_idx_l1_[mbPartIdx];
            predFlagL1 = 1;

            assert(refIdxL1 >= 0);
        } else if (is_BiPred) {
            refIdxL0 = ref_idx_l0_[mbPartIdx];
            predFlagL0 = 1;
            refIdxL1 = ref_idx_l1_[mbPartIdx];
            predFlagL1 = 1;

            assert(refIdxL0 >= 0 && refIdxL1 >= 0);
        }

        subMvCnt = predFlagL0 + predFlagL1;

        SubMbType currSubMbType;
        if (mb_type == MbType::B_8x8)
            currSubMbType = sub_mb_type_proxy_[mbPartIdx].sub_mb_type();
        else
            currSubMbType = SubMbType::NA;

        if (predFlagL0) {
            // 8.4.1.3
            auto mvpL0 = Derivation_process_for_luma_motion_vector_prediction_single(
                mbPartIdx,
                subMbPartIdx,
                refIdxL0,
                currSubMbType,
                0 /* listSuffixFlag */);

            mvL0[0] = mvpL0[0] + mvd_l0_[mbPartIdx][subMbPartIdx][0];
            mvL0[1] = mvpL0[1] + mvd_l0_[mbPartIdx][subMbPartIdx][1];
        }

        if (predFlagL1) {
            // 8.4.1.3
            auto mvpL1 = Derivation_process_for_luma_motion_vector_prediction_single(
                mbPartIdx,
                subMbPartIdx,
                refIdxL1,
                currSubMbType,
                1 /* listSuffixFlag */);

            mvL1[0] = mvpL1[0] + mvd_l1_[mbPartIdx][subMbPartIdx][0];
            mvL1[1] = mvpL1[1] + mvd_l1_[mbPartIdx][subMbPartIdx][1];
        }
    }

    auto ChromaArrayType = sps_->ChromaArrayType();
    if (ChromaArrayType != 0) {
        if (predFlagL0) {

            mvCL0 = Derivation_process_for_chroma_motion_vectors(
                mvL0,
                refIdxL0,
                0 /* listSuffixFlag */);
        }
        if (predFlagL1) {

            mvCL1 = Derivation_process_for_chroma_motion_vectors(
                mvL1,
                refIdxL1,
                1 /* listSuffixFlag */);
        }
    }

    return { mvL0, mvL1, mvCL0, mvCL1, refIdxL0, refIdxL1, predFlagL0, predFlagL1, subMvCnt };
}

// 8.4.1.1 Derivation process for luma motion vectors for skipped macroblocks in P and SP slices
//
// mvL0, refIdxL0
//
std::tuple<std::array<int, 2>, int>
MacroBlock::Derivation_process_for_luma_motion_vectors_for_skipped_macroblocks_in_P_and_SP_slices()
{
    auto mb_type = mb_type_proxy_.mb_type();
    assert(mb_type == MbType::P_Skip);

    int refIdxL0;
    std::array<int, 2> mvL0;

    refIdxL0 = 0;

    // 8.4.1.3.2

    // mbAddrA, mbPartIdxA, subMbPartIdxA, mv_refIdx_A,
    // mbAddrB, mbPartIdxB, subMbPartIdxB, mv_refIdx_B,
    // mbAddrC, mbPartIdxC, subMbPartIdxC, mv_refIdx_C,

    auto&& [A_0, B_0, C_0] = Derivation_process_for_motion_data_of_neighbouring_partitions(
        0 /* mbPartIdx */,
        0 /* subMbPartIdx */,
        SubMbType::NA /* currSubMbType */,
        0 /* listSuffixFlag */);

    int mbAddrA = std::get<0>(A_0);
    auto&& [mvL0A, refIdxL0A] = std::get<3>(A_0);

    int mbAddrB = std::get<0>(B_0);
    auto&& [mvL0B, refIdxL0B] = std::get<3>(B_0);

    if (mbAddrA == INT32_MIN
        || mbAddrB == INT32_MIN
        || (refIdxL0A == 0 && mvL0A[0] == 0 && mvL0A[1] == 0)
        || (refIdxL0B == 0 && mvL0B[0] == 0 && mvL0B[1] == 0)) {
        mvL0[0] = 0;
        mvL0[1] = 0;
    } else {
        // clause 8.4.1.3
        //
        mvL0 = Derivation_process_for_luma_motion_vector_prediction_single(
            0 /* mbPartIdx */,
            0 /* subMbPartIdx */,
            refIdxL0,
            SubMbType::NA /* currSubMbType */,
            0 /* listSuffixFlag */);
    }

    return { mvL0, refIdxL0 };
}

// 8.4.1.2 Derivation process for luma motion vectors for B_Skip, B_Direct_16x16, and B_Direct_8x8
//
// refIdxL0, refIdxL1, mvL0, mvL1, subMvCnt, predFlagL0, predFlagL1
//
std::tuple<int, int,
    std::array<int, 2>, std::array<int, 2>,
    int, int, int>
MacroBlock::Derivation_process_for_luma_motion_vectors_for_B_Skip_B_Direct_16x16_and_B_Direct_8x8(
    int mbPartIdx,
    int subMbPartIdx)
{
    auto mb_type = mb_type_proxy_.mb_type();
    assert(mb_type == MbType::B_Skip
        || mb_type == MbType::B_Direct_16x16
        || (mb_type == MbType::B_8x8 && sub_mb_type_proxy_[mbPartIdx].sub_mb_type() == SubMbType::B_Direct_8x8));

    int refIdxL0, refIdxL1;
    std::array<int, 2> mvL0, mvL1;
    int subMvCnt;
    int predFlagL0, predFlagL1;

    bool direct_spatial_mv_pred_flag = slice_->direct_spatial_mv_pred_flag();

    if (direct_spatial_mv_pred_flag) {

        // refIdxL0, refIdxL1, mvL0, mvL1, subMvCnt, predFlagL0, predFlagL1
        //
        // clause 8.4.1.2.2
        auto&& tmp = Derivation_process_for_spatial_direct_luma_motion_vector_and_reference_index_prediction_mode(
            mbPartIdx,
            subMbPartIdx);

        refIdxL0 = std::get<0>(tmp);
        refIdxL1 = std::get<1>(tmp);
        mvL0 = std::get<2>(tmp);
        mvL1 = std::get<3>(tmp);
        subMvCnt = std::get<4>(tmp);
        predFlagL0 = std::get<5>(tmp);
        predFlagL1 = std::get<6>(tmp);
    } else {

        // mvL0, mvL1, refIdxL0, refIdxL1, predFlagL0, predFlagL1
        //
        // clause 8.4.1.2.3
        auto&& tmp = Derivation_process_for_temporal_direct_luma_motion_vector_and_reference_index_prediction_mode(
            mbPartIdx,
            subMbPartIdx);

        mvL0 = std::get<0>(tmp);
        mvL1 = std::get<1>(tmp);
        refIdxL0 = std::get<2>(tmp);
        refIdxL1 = std::get<3>(tmp);
        predFlagL0 = std::get<4>(tmp);
        predFlagL1 = std::get<5>(tmp);

        // TODO: Why all 8x8 block share the same Mv?
        if (subMbPartIdx == 0)
            subMvCnt = 2;
        else
            subMvCnt = 0;
    }

    return { refIdxL0, refIdxL1, mvL0, mvL1, subMvCnt, predFlagL0, predFlagL1 };
}

// Done
//
// 8.4.1.2.1 Derivation process for the co-located 4x4 sub-macroblock partitions
//
// colPic, mbAddrCol, mvCol, refIdxCol, vertMvScale, refPicCol
std::tuple<Slice*, int, std::array<int, 2>, int, int, Slice*>
MacroBlock::Derivation_process_for_the_co_located_4x4_sub_macroblock_partitions(
    int mbPartIdx,
    int subMbPartIdx)
{
    Slice* colPic;
    int mbAddrCol;
    std::array<int, 2> mvCol;
    int refIdxCol;
    int vertMvScale;

    bool direct_8x8_inference_flag = sps_->direct_8x8_inference_flag();

    // now Only consider frame mode
    assert(!slice_->field_pic_flag());
    colPic = slice_->get_RefPicList1(0);
    assert(colPic);

    auto PicCodingStruct = [](Slice* pic) {
        if (pic->field_pic_flag()) {
            return FLD;
        } else {
            if (pic->mb_adaptive_frame_field_flag())
                return AFRM;
            else
                return FRM;
        }
    };

    int luma4x4BlkIdx;
    if (!direct_8x8_inference_flag)
        luma4x4BlkIdx = 4 * mbPartIdx + subMbPartIdx;
    else
        luma4x4BlkIdx = 5 * mbPartIdx;

    auto [xCol, yCol] = slice_->Inverse_4x4_luma_block_scanning_process(luma4x4BlkIdx);

    auto PicCodingStruct_CurrPic = PicCodingStruct(slice_);
    auto PicCodingStruct_colPic = PicCodingStruct(colPic);

    // only consider frame mode
    assert(PicCodingStruct_CurrPic == FRM && PicCodingStruct_colPic == FRM);

    mbAddrCol = CurrMbAddr_;
    int yM = yCol;
    vertMvScale = One_To_One;

    auto& mbCol = colPic->get_mb_by_addr(mbAddrCol);

    auto [mbPartIdxCol, subMbPartIdxCol] = mbCol.Derivation_process_for_macroblock_and_sub_macroblock_partition_indices(xCol, yM);

    int predFlagL0Col, predFlagL1Col;

    Slice* refPicCol = nullptr;

    if (mbCol.is_intra_pred()) {
        mvCol[0] = 0;
        mvCol[1] = 0;
        refIdxCol = -1;
    } else {
        predFlagL0Col = mbCol.get_PredFlagL0(mbPartIdxCol);
        predFlagL1Col = mbCol.get_PredFlagL1(mbPartIdxCol);
        if (predFlagL0Col) {
            mvCol[0] = mbCol.get_MvL0(mbPartIdxCol, subMbPartIdxCol)[0];
            mvCol[1] = mbCol.get_MvL0(mbPartIdxCol, subMbPartIdxCol)[1];
            refIdxCol = mbCol.get_RefIdxL0(mbPartIdxCol);

            // here the refIdxCol is the ref idx for colPic
            refPicCol = colPic->get_RefPicList0(refIdxCol);
        } else {
            assert(predFlagL1Col);

            mvCol[0] = mbCol.get_MvL1(mbPartIdxCol, subMbPartIdxCol)[0];
            mvCol[1] = mbCol.get_MvL1(mbPartIdxCol, subMbPartIdxCol)[1];
            refIdxCol = mbCol.get_RefIdxL1(mbPartIdxCol);

            // here the refIdxCol is the ref idx for colPic
            refPicCol = colPic->get_RefPicList1(refIdxCol);
        }
    }

    return { colPic, mbAddrCol, mvCol, refIdxCol, vertMvScale, refPicCol };
}

// 8.4.1.2.2 Derivation process for spatial direct luma motion vector and reference index prediction mode
//
// refIdxL0, refIdxL1, mvL0, mvL1, subMvCnt, predFlagL0, predFlagL1
std::tuple<
    int, int,
    std::array<int, 2>, std::array<int, 2>,
    int, int, int>
MacroBlock::Derivation_process_for_spatial_direct_luma_motion_vector_and_reference_index_prediction_mode(
    int mbPartIdx,
    int subMbPartIdx)
{
    auto direct_spatial_mv_pred_flag = slice_->direct_spatial_mv_pred_flag();
    assert(direct_spatial_mv_pred_flag);

    auto mb_type = mb_type_proxy_.mb_type();
    assert(mb_type == MbType::B_Skip
        || mb_type == MbType::B_Direct_16x16
        || (mb_type == MbType::B_8x8
            && sub_mb_type_proxy_[mbPartIdx].sub_mb_type() == SubMbType::B_Direct_8x8));

    int refIdxL0, refIdxL1;
    std::array<int, 2> mvL0, mvL1;
    int subMvCnt;
    int predFlagL0, predFlagL1;

    // TODO: shall we set the inferred SubMbType for B_Skip and B_Direct_16x16?
    SubMbType currSubMbType = SubMbType::NA;
    if (mb_type == MbType::B_8x8)
        currSubMbType = sub_mb_type_proxy_[mbPartIdx].sub_mb_type();

    // when direct_spatial_mv_pred_flag is true
    // the mbPartIdx and subMbPartIdx are set to 0
    // and the predPartWidthC is 16

    // mbAddrA, mbPartIdxA, subMbPartIdxA, mv_refIdx_A,
    // mbAddrB, mbPartIdxB, subMbPartIdxB, mv_refIdx_B,
    // mbAddrC, mbPartIdxC, subMbPartIdxC, mv_refIdx_C,

    // clause 8.4.1.3.2
    auto&& [A0, B0, C0] = Derivation_process_for_motion_data_of_neighbouring_partitions(
        0 /* mbPartIdx */,
        0 /* subMbPartIdx */,
        currSubMbType,
        0 /* listSuffixFlag */);

    auto&& [mvL0A, refIdxL0A] = std::get<3>(A0);
    auto&& [mvL0B, refIdxL0B] = std::get<3>(B0);
    auto&& [mvL0C, refIdxL0C] = std::get<3>(C0);

    // clause 8.4.1.3.2
    auto&& [A1, B1, C1] = Derivation_process_for_motion_data_of_neighbouring_partitions(
        0 /* mbPartIdx */,
        0 /* subMbPartIdx */,
        currSubMbType,
        1 /* listSuffixFlag */);

    auto&& [mvL1A, refIdxL1A] = std::get<3>(A1);
    auto&& [mvL1B, refIdxL1B] = std::get<3>(B1);
    auto&& [mvL1C, refIdxL1C] = std::get<3>(C1);

    // The motion vectors mvL0N, mvL1N and the reference indices refIdxL0N,
    // refIdxL1N are identical for all 4x4 sub-macroblock partitions of a macroblock.

    auto MinPositive = [](int x, int y) {
        if (x >= 0 && y >= 0)
            return std::min(x, y);
        else
            return std::max(x, y);
    };

    refIdxL0 = MinPositive(refIdxL0A, MinPositive(refIdxL0B, refIdxL0C));
    refIdxL1 = MinPositive(refIdxL1A, MinPositive(refIdxL1B, refIdxL1C));
    int directZeroPredictionFlag = 0;

    if (refIdxL0 < 0 && refIdxL1 < 0) {

        refIdxL0 = 0;
        refIdxL1 = 0;
        directZeroPredictionFlag = 1;
    }

    // clause 8.4.1.2.1
    // colPic, mbAddrCol, mvCol, refIdxCol, vertMvScale, refPicCol
    auto&& [colPic,
        mbAddrCol,
        mvCol,
        refIdxCol,
        vertMvScale,
        refPicCol]
        = Derivation_process_for_the_co_located_4x4_sub_macroblock_partitions(
            mbPartIdx,
            subMbPartIdx);

    int colZeroFlag;
    if (slice_->get_RefPicList1(0)->at_least_one_short_term_ref()
        && refIdxCol == 0
        && (mvCol[0] >= -1 && mvCol[0] <= 1
            && mvCol[1] >= -1 && mvCol[1] <= 1))

        colZeroFlag = 1;
    else
        colZeroFlag = 0;

    auto func = [directZeroPredictionFlag, colZeroFlag, currSubMbType, this](
                    int refIdxLX,
                    int listSuffixFlag) -> std::array<int, 2> {
        std::array<int, 2> mvLX;

        if (directZeroPredictionFlag == 1
            || refIdxLX < 0
            || (refIdxLX == 0 && colZeroFlag == 1)) {
            mvLX[0] = 0;
            mvLX[1] = 0;
        } else
            mvLX = Derivation_process_for_luma_motion_vector_prediction_single(
                0 /* mbPartIdx */,
                0 /* subMbPartIdx */,
                refIdxLX,
                currSubMbType,
                listSuffixFlag);

        return mvLX;
    };

    mvL0 = func(refIdxL0, 0);
    mvL1 = func(refIdxL1, 1);

    if (refIdxL0 >= 0 && refIdxL1 >= 0) {
        predFlagL0 = 1;
        predFlagL1 = 1;
    } else if (refIdxL0 >= 0 && refIdxL1 < 0) {
        predFlagL0 = 1;
        predFlagL1 = 0;
    } else if (refIdxL0 < 0 && refIdxL1 >= 0) {
        predFlagL0 = 0;
        predFlagL1 = 1;
    } else
        assert(false);

    if (subMbPartIdx != 0)
        subMvCnt = 0;
    else
        subMvCnt = predFlagL0 + predFlagL1;

    return { refIdxL0, refIdxL1, mvL0, mvL1, subMvCnt, predFlagL0, predFlagL1 };
}

// 8.4.1.2.3 Derivation process for temporal direct luma motion vector and reference index prediction mode
//
// mvL0, mvL1, refIdxL0, refIdxL1, predFlagL0, predFlagL1
std::tuple<
    std::array<int, 2>, std::array<int, 2>,
    int, int,
    int, int>
MacroBlock::Derivation_process_for_temporal_direct_luma_motion_vector_and_reference_index_prediction_mode(
    int mbPartIdx,
    int subMbPartIdx)
{
    auto direct_spatial_mv_pred_flag = slice_->direct_spatial_mv_pred_flag();
    assert(!direct_spatial_mv_pred_flag);

    auto mb_type = mb_type_proxy_.mb_type();
    assert(mb_type == MbType::B_Skip
        || mb_type == MbType::B_Direct_16x16
        || (mb_type == MbType::B_8x8
            && sub_mb_type_proxy_[mbPartIdx].sub_mb_type() == SubMbType::B_Direct_8x8));

    std::array<int, 2> mvL0, mvL1;
    int refIdxL0, refIdxL1;
    int predFlagL0, predFlagL1;

    // clause 8.4.1.2.1
    // colPic, mbAddrCol, mvCol, refIdxCol, vertMvScale, refPicCol
    auto&& [colPic,
        mbAddrCol,
        mvCol,
        refIdxCol,
        vertMvScale,
        refPicCol]
        = Derivation_process_for_the_co_located_4x4_sub_macroblock_partitions(
            mbPartIdx,
            subMbPartIdx);

    assert(vertMvScale == One_To_One);

    auto MapColToList0 = [vertMvScale, this](Slice* refPicCol) {
        if (vertMvScale == One_To_One) {
            // If field_pic_flag is equal to 0 and the current
            // macroblock is a field macroblock
            // TODO

            // Otherwise
            // field_pic_flag is equal to 1 or the current macroblock is a frame macroblock

            return slice_->find_lowest_Slice_idx_in_RefPicList0(refPicCol);

        } else {
            // TODO
        }
        return INT32_MIN;
    };

    refIdxL0 = ((refIdxCol < 0) ? 0 : MapColToList0(refPicCol));
    refIdxL1 = 0;

    // only consider frame mode, so mvCol[ 1 ] remains unchanged

    Slice* currPicOrField = slice_;
    Slice* pic1 = slice_->get_RefPicList1(0 /* refIdxL1 */);
    Slice* pic0 = slice_->get_RefPicList0(refIdxL0);

    if (pic0->at_least_one_long_term_ref()
        || DiffPicOrderCnt(pic1, pic0) == 0) {
        mvL0 = mvCol;
        mvL1 = { 0, 0 };
    } else {
        int tb = Clip3(-128, 127, DiffPicOrderCnt(currPicOrField, pic0));
        int td = Clip3(-128, 127, DiffPicOrderCnt(pic1, pic0));
        int tx = (16384 + std::abs(td / 2)) / td;
        int DistScaleFactor = Clip3(-1024, 1023, (tb * tx + 32) >> 6);

        mvL0[0] = (DistScaleFactor * mvCol[0] + 128) >> 8;
        mvL0[1] = (DistScaleFactor * mvCol[1] + 128) >> 8;

        mvL1[0] = mvL0[0] - mvCol[0];
        mvL1[1] = mvL0[1] - mvCol[1];
    }

    predFlagL0 = 1;
    predFlagL1 = 1;

    return { mvL0, mvL1, refIdxL0, refIdxL1, predFlagL0, predFlagL1 };
}

// Done
//
// 8.4.1.3 Derivation process for luma motion vector prediction
//
std::tuple<
    std::array<int, 2>,
    std::array<int, 2>>
MacroBlock::Derivation_process_for_luma_motion_vector_prediction(
    int mbPartIdx,
    int subMbPartIdx,
    int refIdxL0,
    int refIdxL1,
    SubMbType currSubMbType)
{
    std::array<int, 2> mvpL0 = Derivation_process_for_luma_motion_vector_prediction_single(
        mbPartIdx,
        subMbPartIdx,
        refIdxL0,
        currSubMbType,
        0);

    std::array<int, 2> mvpL1 = Derivation_process_for_luma_motion_vector_prediction_single(
        mbPartIdx,
        subMbPartIdx,
        refIdxL1,
        currSubMbType,
        1);

    return { mvpL0, mvpL1 };
}

// Done
//
std::array<int, 2>
MacroBlock::Derivation_process_for_luma_motion_vector_prediction_single(
    int mbPartIdx,
    int subMbPartIdx,
    int refIdxLX,
    SubMbType currSubMbType,
    int listSuffixFlag)
{
    assert(refIdxLX >= 0);

    std::array<int, 2> mvpLX;

    // mbAddrA, mbPartIdxA, subMbPartIdxA, mv_refIdx_A,
    // mbAddrB, mbPartIdxB, subMbPartIdxB, mv_refIdx_B,
    // mbAddrC, mbPartIdxC, subMbPartIdxC, mv_refIdx_C,
    auto t = Derivation_process_for_motion_data_of_neighbouring_partitions(
        mbPartIdx,
        subMbPartIdx,
        currSubMbType,
        listSuffixFlag);

    auto&& [mbAddrA, mbPartIdxA, subMbPartIdxA, mv_refIdx_A] = std::get<0>(t);
    auto&& [mvLXA, refIdxLXA] = mv_refIdx_A;

    auto&& [mbAddrB, mbPartIdxB, subMbPartIdxB, mv_refIdx_B] = std::get<1>(t);
    auto&& [mvLXB, refIdxLXB] = mv_refIdx_B;

    auto&& [mbAddrC, mbPartIdxC, subMbPartIdxC, mv_refIdx_C] = std::get<2>(t);
    auto&& [mvLXC, refIdxLXC] = mv_refIdx_C;

    if (mb_type_proxy_.MbPartWidth() == 16
        && mb_type_proxy_.MbPartHeight() == 8
        && mbPartIdx == 0
        && refIdxLXB == refIdxLX) {

        mvpLX = mvLXB;

    } else if (mb_type_proxy_.MbPartWidth() == 16
        && mb_type_proxy_.MbPartHeight() == 8
        && mbPartIdx == 1
        && refIdxLXA == refIdxLX) {

        mvpLX = mvLXA;

    } else if (mb_type_proxy_.MbPartWidth() == 8
        && mb_type_proxy_.MbPartHeight() == 16
        && mbPartIdx == 0
        && refIdxLXA == refIdxLX) {

        mvpLX = mvLXA;

    } else if (mb_type_proxy_.MbPartWidth() == 8
        && mb_type_proxy_.MbPartHeight() == 16
        && mbPartIdx == 1
        && refIdxLXC == refIdxLX) {

        mvpLX = mvLXC;

    } else {

        mvpLX = Derivation_process_for_median_luma_motion_vector_prediction(
            mbAddrA,
            mbPartIdxA,
            subMbPartIdxA,
            mvLXA,
            refIdxLXA,

            mbAddrB,
            mbPartIdxB,
            subMbPartIdxB,
            mvLXB,
            refIdxLXB,

            mbAddrC,
            mbPartIdxC,
            subMbPartIdxC,
            mvLXC,
            refIdxLXC,

            refIdxLX);
    }

    return mvpLX;
}

// Done
//
// 8.4.1.3.1 Derivation process for median luma motion vector prediction
std::array<int, 2>
MacroBlock::Derivation_process_for_median_luma_motion_vector_prediction(
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

    int refIdxLX)
{

    std::array<int, 2> mvpLX;

    bool A_available = mbAddrA != INT32_MIN
        && mbPartIdxA != INT32_MIN
        && subMbPartIdxA != INT32_MIN;

    bool B_available = mbAddrB != INT32_MIN
        && mbPartIdxB != INT32_MIN
        && subMbPartIdxB != INT32_MIN;

    bool C_available = mbAddrC != INT32_MIN
        && mbPartIdxC != INT32_MIN
        && subMbPartIdxC != INT32_MIN;

    if (!B_available && !C_available && A_available) {
        mvLXB = mvLXA;
        mvLXC = mvLXA;
        refIdxLXB = refIdxLXA;
        refIdxLXC = refIdxLXA;
    }

    bool A_idx_equal_to_cur = (refIdxLXA == refIdxLX);
    bool B_idx_equal_to_cur = (refIdxLXB == refIdxLX);
    bool C_idx_equal_to_cur = (refIdxLXC == refIdxLX);

    if (A_idx_equal_to_cur && !B_idx_equal_to_cur && !C_idx_equal_to_cur)
        mvpLX = mvLXA;

    else if (!A_idx_equal_to_cur && B_idx_equal_to_cur && !C_idx_equal_to_cur)
        mvpLX = mvLXB;

    else if (!A_idx_equal_to_cur && !B_idx_equal_to_cur && C_idx_equal_to_cur)
        mvpLX = mvLXC;

    else {
        mvpLX[0] = Median(mvLXA[0], mvLXB[0], mvLXC[0]);
        mvpLX[1] = Median(mvLXA[1], mvLXB[1], mvLXC[1]);
    }

    return mvpLX;
}

// Done
//
// 8.4.1.3.2 Derivation process for motion data of neighbouring partitions
//
// mbAddrA, mbPartIdxA, subMbPartIdxA, mv_refIdx_A,
// mbAddrB, mbPartIdxB, subMbPartIdxB, mv_refIdx_B,
// mbAddrC, mbPartIdxC, subMbPartIdxC, mv_refIdx_C,
std::tuple<
    std::tuple<int, int, int, std::tuple<std::array<int, 2>, int>>,
    std::tuple<int, int, int, std::tuple<std::array<int, 2>, int>>,
    std::tuple<int, int, int, std::tuple<std::array<int, 2>, int>>>
MacroBlock::Derivation_process_for_motion_data_of_neighbouring_partitions(
    int mbPartIdx,
    int subMbPartIdx,
    SubMbType currSubMbType,
    int listSuffixFlag)
{
    // debug
    int kl = 0;
    kl++;

    auto&& [A, B, C, D] = Derivation_process_for_neighbouring_partitions(
        mbPartIdx,
        subMbPartIdx,
        currSubMbType);

    auto mbAddrA = std::get<0>(A);
    auto mbPartIdxA = std::get<1>(A);
    auto subMbPartIdxA = std::get<2>(A);

    auto mbAddrB = std::get<0>(B);
    auto mbPartIdxB = std::get<1>(B);
    auto subMbPartIdxB = std::get<2>(B);

    auto mbAddrC = std::get<0>(C);
    auto mbPartIdxC = std::get<1>(C);
    auto subMbPartIdxC = std::get<2>(C);

    auto mbAddrD = std::get<0>(D);
    auto mbPartIdxD = std::get<1>(D);
    auto subMbPartIdxD = std::get<2>(D);

    if (mbAddrC == INT32_MIN || mbPartIdxC == INT32_MIN || subMbPartIdxC == INT32_MIN) {

        mbAddrC = mbAddrD;
        mbPartIdxC = mbPartIdxD;
        subMbPartIdxC = subMbPartIdxD;
    }

    std::array<int, 2> mvLXA, mvLXB, mvLXC;
    int refIdxLXA, refIdxLXB, refIdxLXC;

    auto func = [listSuffixFlag, this](int mbAddrN,
                    int mbPartIdxN,
                    int subMbPartIdxN)
        -> std::tuple<std::array<int, 2>, int> {
        if (mbAddrN == INT32_MIN || mbPartIdxN == INT32_MIN || subMbPartIdxN == INT32_MIN)
            return { std::array<int, 2>(), -1 };

        if (slice_->get_mb_by_addr(mbAddrN).is_intra_pred())
            return { std::array<int, 2>(), -1 };

        // each partition share the same predFlag
        if (slice_->get_mb_by_addr(mbAddrN).get_predFlag(listSuffixFlag, mbPartIdxN) == 0)
            return { std::array<int, 2>(), -1 };

        std::array<int, 2> mvLXN;
        int refIdxLXN;

        refIdxLXN = slice_->get_mb_by_addr(mbAddrN).get_RefIdx(listSuffixFlag, mbPartIdxN);
        mvLXN[0] = slice_->get_mb_by_addr(mbAddrN).get_Mv(listSuffixFlag, mbPartIdxN, subMbPartIdxN)[0];
        mvLXN[1] = slice_->get_mb_by_addr(mbAddrN).get_Mv(listSuffixFlag, mbPartIdxN, subMbPartIdxN)[1];

        // If the current macroblock is a field macroblock
        // and the macroblock mbAddrN is a frame macroblock
        //
        // TODO

        // if the current macroblock is a frame macroblock and
        // the macroblock mbAddrN is a field macroblock
        //
        // TODO

        return { mvLXN, refIdxLXN };
    };

    auto mv_refIdx_A = func(mbAddrA, mbPartIdxA, subMbPartIdxA);
    auto mv_refIdx_B = func(mbAddrB, mbPartIdxB, subMbPartIdxB);
    auto mv_refIdx_C = func(mbAddrC, mbPartIdxC, subMbPartIdxC);

    return { { mbAddrA, mbPartIdxA, subMbPartIdxA, mv_refIdx_A },
        { mbAddrB, mbPartIdxB, subMbPartIdxB, mv_refIdx_B },
        { mbAddrC, mbPartIdxC, subMbPartIdxC, mv_refIdx_C } };
}

// Done
//
// 8.4.1.4 Derivation process for chroma motion vectors
//
std::array<int, 2>
MacroBlock::Derivation_process_for_chroma_motion_vectors(
    std::array<int, 2> mvLX,
    int refIdxLX,
    int listSuffixFlag /* currently unused */)
{
    assert(sps_->ChromaArrayType() != 0);

    std::array<int, 2> mvCLX;

    if (sps_->ChromaArrayType() != 1 || is_frame_macroblock()) {
        mvCLX[0] = mvLX[0];
        mvCLX[1] = mvLX[1];
    } else {
        assert(false);
    }
    return mvCLX;
}

int MacroBlock::DiffPicOrderCnt(Slice* a, Slice* b)
{
    return a->PicOrderCnt() - b->PicOrderCnt();
}

// Done
//
// 8.4.3 Derivation process for prediction weights
//
// { { logWDL, w0L, w1L, o0L, o1L },
// { logWDCb, w0Cb, w1Cb, o0Cb, o1Cb },
// { logWDCr, w0Cr, w1Cr, o0Cr, o1Cr } }
//
std::tuple<
    std::tuple<int, int, int, int, int>,
    std::tuple<int, int, int, int, int>,
    std::tuple<int, int, int, int, int>>
MacroBlock::Derivation_process_for_prediction_weights(
    int refIdxL0, int refIdxL1,
    int predFlagL0, int predFlagL1)
{
    int implicitModeFlag, explicitModeFlag;

    if (pps_->weighted_bipred_idc() == 2
        && slice_->is_B_slice()
        && predFlagL0
        && predFlagL1) {

        implicitModeFlag = 1;
        explicitModeFlag = 0;
    } else if (pps_->weighted_bipred_idc() == 1
        && slice_->is_B_slice()
        && ((predFlagL0 + predFlagL1 == 1)
            || (predFlagL0 + predFlagL1 == 2))) {

        implicitModeFlag = 0;
        explicitModeFlag = 1;
    } else if (pps_->weighted_pred_flag()
        && slice_->is_P_or_SP()
        && predFlagL0) {

        implicitModeFlag = 0;
        explicitModeFlag = 1;
    } else {
        implicitModeFlag = 0;
        explicitModeFlag = 0;
    }

    auto func = [implicitModeFlag,
                    explicitModeFlag,
                    refIdxL0,
                    refIdxL1,
                    predFlagL0,
                    predFlagL1,
                    this](bool is_luma, int iCbCr) -> std::tuple<int, int, int, int, int> {
        int logWDC;
        int o0C;
        int o1C;
        int w0C;
        int w1C;

        if (implicitModeFlag) {
            logWDC = 5;
            o0C = 0;
            o1C = 0;

            // only consider frame mode
            Slice* currPicOrField = slice_;
            Slice* pic1 = slice_->get_RefPicList1(refIdxL1);
            Slice* pic0 = slice_->get_RefPicList0(refIdxL0);

            int tb = Clip3(-128, 127, DiffPicOrderCnt(currPicOrField, pic0));
            int td = Clip3(-128, 127, DiffPicOrderCnt(pic1, pic0));
            // be care when td = 0
            int tx = (td != 0) ? ((16384 + std::abs(td / 2)) / td) : INT32_MIN;
            int DistScaleFactor = Clip3(-1024, 1023, (tb * tx + 32) >> 6);

            if (DiffPicOrderCnt(pic1, pic0) == 0
                || pic1->at_least_one_long_term_ref()
                || pic0->at_least_one_long_term_ref()
                || (DistScaleFactor >> 2) < -64
                || (DistScaleFactor >> 2) > 128) {

                w0C = 32;
                w1C = 32;
            } else {
                w0C = 64 - (DistScaleFactor >> 2);
                w1C = DistScaleFactor >> 2;
            }

        } else if (explicitModeFlag) {
            // only frame mode
            int refIdxL0WP = refIdxL0;
            int refIdxL1WP = refIdxL1;
            if (is_luma) {
                // TODO access control
                logWDC = slice_->luma_log2_weight_denom_;

                if (predFlagL0)
                    w0C = slice_->luma_weight_l0_[refIdxL0WP];
                if (predFlagL1)
                    w1C = slice_->luma_weight_l1_[refIdxL1WP];
                if (predFlagL0)
                    o0C = slice_->luma_offset_l0_[refIdxL0WP] * (1 << (sps_->BitDepthY() - 8));
                if (predFlagL1)
                    o1C = slice_->luma_offset_l1_[refIdxL1WP] * (1 << (sps_->BitDepthY() - 8));
            } else {
                // TODO access control
                logWDC = slice_->chroma_log2_weight_denom_;

                auto& chroma_weight_l0 = iCbCr == 0 ? slice_->chroma_weight_l0_Cb_ : slice_->chroma_weight_l0_Cr_;
                auto& chroma_weight_l1 = iCbCr == 0 ? slice_->chroma_weight_l1_Cb_ : slice_->chroma_weight_l1_Cr_;
                auto& chroma_offset_l0 = iCbCr == 0 ? slice_->chroma_offset_l0_Cb_ : slice_->chroma_offset_l0_Cr_;
                auto& chroma_offset_l1 = iCbCr == 0 ? slice_->chroma_offset_l1_Cb_ : slice_->chroma_offset_l1_Cr_;

                if (predFlagL0)
                    w0C = chroma_weight_l0[refIdxL0WP];
                if (predFlagL1)
                    w1C = chroma_weight_l1[refIdxL1WP];
                if (predFlagL0)
                    o0C = chroma_offset_l0[refIdxL0WP] * (1 << (sps_->BitDepthC() - 8));
                if (predFlagL1)
                    o1C = chroma_offset_l1[refIdxL1WP] * (1 << (sps_->BitDepthC() - 8));
            }
        } else {
            // the following variables are unused
            logWDC = INT32_MIN;
            o0C = INT32_MIN;
            o1C = INT32_MIN;
            w0C = INT32_MIN;
            w1C = INT32_MIN;
        }
        return { logWDC, o0C, o1C, w0C, w1C };
    };

    auto [logWDL, o0L, o1L, w0L, w1L] = func(true /* is_luma */, -1 /* iCbCr */);
    auto [logWDCb, o0Cb, o1Cb, w0Cb, w1Cb] = func(false /* is_luma */, 0 /* iCbCr */);
    auto [logWDCr, o0Cr, o1Cr, w0Cr, w1Cr] = func(false /* is_luma */, 1 /* iCbCr */);

    if (explicitModeFlag && predFlagL0 && predFlagL1) {
        assert((w0L + w1L >= -128) && (w0L + w1L <= ((logWDL == 7) ? 127 : 128)));
        assert((w0Cb + w1Cb >= -128) && (w0Cb + w1Cb <= ((logWDCb == 7) ? 127 : 128)));
        assert((w0Cr + w1Cr >= -128) && (w0Cr + w1Cr <= ((logWDCr == 7) ? 127 : 128)));
    }

    return { { logWDL, w0L, w1L, o0L, o1L },
        { logWDCb, w0Cb, w1Cb, o0Cb, o1Cb },
        { logWDCr, w0Cr, w1Cr, o0Cr, o1Cr } };
}

// Done
//
// 8.4.2 Decoding process for Inter prediction samples
std::tuple<
    std::vector<std::vector<int>>,
    std::vector<std::vector<int>>,
    std::vector<std::vector<int>>>
MacroBlock::Decoding_process_for_Inter_prediction_samples(
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
        weight_info)
{
    auto&& [logWDL, w0L, w1L, o0L, o1L] = std::get<0>(weight_info);
    auto&& [logWDCb, w0Cb, w1Cb, o0Cb, o1Cb] = std::get<1>(weight_info);
    auto&& [logWDCr, w0Cr, w1Cr, o0Cr, o1Cr] = std::get<2>(weight_info);

    std::vector<std::vector<int>> predPartL0L(partWidth, std::vector<int>(partHeight));
    std::vector<std::vector<int>> predPartL1L(partWidth, std::vector<int>(partHeight));

    std::vector<std::vector<int>> predPartL0Cb(partWidthC, std::vector<int>(partHeightC));
    std::vector<std::vector<int>> predPartL1Cb(partWidthC, std::vector<int>(partHeightC));

    std::vector<std::vector<int>> predPartL0Cr(partWidthC, std::vector<int>(partHeightC));
    std::vector<std::vector<int>> predPartL1Cr(partWidthC, std::vector<int>(partHeightC));

    //debug
    if (mbPartIdx == 2) {
        int lp = 0;
        lp += 1;
    }

    if (predFlagL0) {
        // 8.4.2.1
        Slice* refPicL0 = Reference_picture_selection_process(refIdxL0, 0 /*listSuffixFlag*/);

        Fractional_sample_interpolation_process(
            mbPartIdx,
            subMbPartIdx,
            partWidth,
            partHeight,
            partWidthC,
            partHeightC,
            mvL0,
            mvCL0,
            refPicL0,
            0 /* listSuffixFlag */,
            /* output */
            predPartL0L,
            predPartL0Cb,
            predPartL0Cr);
    }

    if (predFlagL1) {
        // 8.4.2.1
        Slice* refPicL1 = Reference_picture_selection_process(refIdxL1, 1 /*listSuffixFlag*/);

        Fractional_sample_interpolation_process(
            mbPartIdx,
            subMbPartIdx,
            partWidth,
            partHeight,
            partWidthC,
            partHeightC,
            mvL1,
            mvCL1,
            refPicL1,
            1 /* listSuffixFlag */,
            /* output */
            predPartL1L,
            predPartL1Cb,
            predPartL1Cr);
    }

    // for debug
    auto&& [predPartL, predPartCb, predPartCr] = Weighted_sample_prediction_process_all(
        mbPartIdx,
        subMbPartIdx,
        predFlagL0,
        predFlagL1,
        predPartL0L,
        predPartL1L,
        predPartL0Cb,
        predPartL1Cb,
        predPartL0Cr,
        predPartL1Cr,
        logWDL, w0L, w1L, o0L, o1L,
        logWDCb, w0Cb, w1Cb, o0Cb, o1Cb,
        logWDCr, w0Cr, w1Cr, o0Cr, o1Cr);

    return { predPartL, predPartCb, predPartCr };
}

// Done
//
// 8.4.2.1 Reference picture selection process
Slice* MacroBlock::Reference_picture_selection_process(
    int refIdxLX,
    int listSuffixFlag)
{
    if (listSuffixFlag == 0)
        return slice_->get_RefPicList0(refIdxLX);
    else
        return slice_->get_RefPicList1(refIdxLX);
}

// Done
//
// 8.4.2.2 Fractional sample interpolation process
void MacroBlock::Fractional_sample_interpolation_process(
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
    std::vector<std::vector<int>>& predPartLXCr)
{

    auto [xO, yO] = slice_->Inverse_macroblock_scanning_process(CurrMbAddr_);
    auto [xM, yM] = Inverse_macroblock_partition_scanning_process(mbPartIdx);
    auto [xS, yS] = Inverse_sub_macroblock_partition_scanning_process(mbPartIdx, subMbPartIdx);

    int xAL = xO + xM + xS;
    int yAL = yO + yM + yS;

    for (int xL = 0; xL < partWidth; xL++) {
        for (int yL = 0; yL < partHeight; yL++) {
            int xIntL = xAL + (mvLX[0] >> 2) + xL;
            int yIntL = yAL + (mvLX[1] >> 2) + yL;
            int xFracL = mvLX[0] & 3; // 0b11
            int yFracL = mvLX[1] & 3;
            // 8.4.2.2.1
            predPartLXL[xL][yL] = Luma_sample_interpolation_process(
                xIntL, yIntL,
                xFracL, yFracL,
                refPicLX);
        }
    }

    int SubWidthC = sps_->SubWidthC();
    int SubHeightC = sps_->SubHeightC();

    for (int xC = 0; xC < partWidthC; xC++) {
        for (int yC = 0; yC < partHeightC; yC++) {
            int xIntC, yIntC, xFracC, yFracC;
            if (sps_->ChromaArrayType() == 1) {
                xIntC = (xAL / SubWidthC) + (mvCLX[0] >> 3) + xC;
                yIntC = (yAL / SubHeightC) + (mvCLX[1] >> 3) + yC;
                xFracC = mvCLX[0] & 7;
                yFracC = mvCLX[1] & 7;
            } else if (sps_->ChromaArrayType() == 2) {
                xIntC = (xAL / SubWidthC) + (mvCLX[0] >> 3) + xC;
                yIntC = (yAL / SubHeightC) + (mvCLX[1] >> 2) + yC;
                xFracC = mvCLX[0] & 7;
                yFracC = (mvCLX[1] & 3) << 1;
            } else {
                xIntC = xAL + (mvLX[0] >> 2) + xC;
                yIntC = yAL + (mvLX[1] >> 2) + yC;
                xFracC = (mvCLX[0] & 3);
                yFracC = (mvCLX[1] & 3);
            }

            if (sps_->ChromaArrayType() != 3) {
                // 8.4.2.2.2
                predPartLXCb[xC][yC] = Chroma_sample_interpolation_process(
                    xIntC, yIntC,
                    xFracC, yFracC,
                    refPicLX,
                    0 /* iCbCr */);

                // 8.4.2.2.2
                predPartLXCr[xC][yC] = Chroma_sample_interpolation_process(
                    xIntC, yIntC,
                    xFracC, yFracC,
                    refPicLX,
                    1 /* iCbCr */);
            } else {
                // TODO
                assert(false);
            }
        }
    }
}

// Done
//
// 8.4.2.2.1 Luma sample interpolation process
int MacroBlock::Luma_sample_interpolation_process(
    int xIntL, int yIntL,
    int xFracL, int yFracL,
    Slice* refPicLX)
{

    int refPicHeightEffectiveL = sps_->PicHeightInSamplesL();
    int PicWidthInSamplesL = sps_->PicWidthInSamplesL();

// borrow from h264_video_decoder_demo
#define getLumaSample(x, y) refPicLX->get_constructed_luma( \
    Clip3(0, PicWidthInSamplesL - 1, xIntL + x),            \
    Clip3(0, refPicHeightEffectiveL - 1, yIntL + y))

    int A = getLumaSample(0, -2);
    int B = getLumaSample(1, -2);
    int C = getLumaSample(0, -1);
    int D = getLumaSample(1, -1);
    int E = getLumaSample(-2, 0);
    int F = getLumaSample(-1, 0);
    int G = getLumaSample(0, 0);
    int H = getLumaSample(1, 0);
    int I = getLumaSample(2, 0);
    int J = getLumaSample(3, 0);
    int K = getLumaSample(-2, 1);
    int L = getLumaSample(-1, 1);
    int M = getLumaSample(0, 1);
    int N = getLumaSample(1, 1);
    int P = getLumaSample(2, 1);
    int Q = getLumaSample(3, 1);
    int R = getLumaSample(0, 2);
    int S = getLumaSample(1, 2);
    int T = getLumaSample(0, 3);
    int U = getLumaSample(1, 3);

    int X11 = getLumaSample(-2, -2);
    int X12 = getLumaSample(-1, -2);
    int X13 = getLumaSample(2, -2);
    int X14 = getLumaSample(3, -2);

    int X21 = getLumaSample(-2, -1);
    int X22 = getLumaSample(-1, -1);
    int X23 = getLumaSample(2, -1);
    int X24 = getLumaSample(3, -1);

    int X31 = getLumaSample(-2, 2);
    int X32 = getLumaSample(-1, 2);
    int X33 = getLumaSample(2, 2);
    int X34 = getLumaSample(3, 2);

    int X41 = getLumaSample(-2, 3);
    int X42 = getLumaSample(-1, 3);
    int X43 = getLumaSample(2, 3);
    int X44 = getLumaSample(3, 3);

#undef getLumaSample

#define a_6_tap_filter(v1, v2, v3, v4, v5, v6) \
    ((v1)-5 * (v2) + 20 * (v3) + 20 * (v4)-5 * (v5) + (v6))

    int b1 = (E - 5 * F + 20 * G + 20 * H - 5 * I + J);
    int h1 = (A - 5 * C + 20 * G + 20 * M - 5 * R + T);

    int s1 = a_6_tap_filter(K, L, M, N, P, Q);
    int m1 = a_6_tap_filter(B, D, H, N, S, U);

    int b = Clip1Y((b1 + 16) >> 5);
    int h = Clip1Y((h1 + 16) >> 5);
    int s = Clip1Y((s1 + 16) >> 5);
    int m = Clip1Y((m1 + 16) >> 5);

    int aa = a_6_tap_filter(X11, X12, A, B, X13, X14);
    int bb = a_6_tap_filter(X21, X22, C, D, X23, X24);
    int gg = a_6_tap_filter(X31, X32, R, S, X33, X34);
    int hh = a_6_tap_filter(X41, X42, T, U, X43, X44);

    int cc = a_6_tap_filter(X11, X21, E, K, X31, X41);
    int dd = a_6_tap_filter(X12, X22, F, L, X32, X42);
    int ee = a_6_tap_filter(X13, X23, I, P, X33, X43);
    int ff = a_6_tap_filter(X14, X24, J, Q, X34, X44);

    // TODO: what if use different calculation method of j1?
    // the following j1 and j2 should be same, since both are 2D tap filter for the whole block
    int j1 = a_6_tap_filter(cc, dd, h1, m1, ee, ff);
    // int j2 = a_6_tap_filter(aa, bb, b1, s1, gg, hh);
    // assert(j1 == j2);

    int j = Clip1Y((j1 + 512) >> 10);

#undef a_6_tap_filter

    int a = (G + b + 1) >> 1;
    int c = (H + b + 1) >> 1;
    int d = (G + h + 1) >> 1;
    int n = (M + h + 1) >> 1;
    int f = (b + j + 1) >> 1;
    int i = (h + j + 1) >> 1;
    int k = (j + m + 1) >> 1;
    int q = (j + s + 1) >> 1;

    int e = (b + h + 1) >> 1;
    int g = (b + m + 1) >> 1;
    int p = (h + s + 1) >> 1;
    int r = (m + s + 1) >> 1;

    int tmp[4][4] = {
        { G, d, h, n },
        { a, e, i, p },
        { b, f, j, q },
        { c, g, k, r },
    };

    return tmp[xFracL][yFracL];
}

// Done
//
// 8.4.2.2.2 Chroma sample interpolation process
int MacroBlock::Chroma_sample_interpolation_process(
    int xIntC, int yIntC,
    int xFracC, int yFracC,
    Slice* refPicLX,
    int iCbCr)
{
    int refPicHeightEffectiveC = sps_->PicHeightInSamplesC();
    int PicWidthInSamplesC = sps_->PicWidthInSamplesC();

    int xAC = Clip3(0, PicWidthInSamplesC - 1, xIntC);
    int xBC = Clip3(0, PicWidthInSamplesC - 1, xIntC + 1);
    int xCC = Clip3(0, PicWidthInSamplesC - 1, xIntC);
    int xDC = Clip3(0, PicWidthInSamplesC - 1, xIntC + 1);

    int yAC = Clip3(0, refPicHeightEffectiveC - 1, yIntC);
    int yBC = Clip3(0, refPicHeightEffectiveC - 1, yIntC);
    int yCC = Clip3(0, refPicHeightEffectiveC - 1, yIntC + 1);
    int yDC = Clip3(0, refPicHeightEffectiveC - 1, yIntC + 1);

    int A = refPicLX->get_constructed_chroma(iCbCr, xAC, yAC);
    int B = refPicLX->get_constructed_chroma(iCbCr, xBC, yBC);
    int C = refPicLX->get_constructed_chroma(iCbCr, xCC, yCC);
    int D = refPicLX->get_constructed_chroma(iCbCr, xDC, yDC);

    return ((8 - xFracC) * (8 - yFracC) * A
               + xFracC * (8 - yFracC) * B
               + (8 - xFracC) * yFracC * C
               + xFracC * yFracC * D + 32)
        >> 6;
}

// Done
//
// 8.4.2.3 Weighted sample prediction process
//
std::tuple<
    std::vector<std::vector<int>>,
    std::vector<std::vector<int>>,
    std::vector<std::vector<int>>>
MacroBlock::Weighted_sample_prediction_process_all(
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
    int logWDCr, int w0Cr, int w1Cr, int o0Cr, int o1Cr)
{
    int partWidth, partHeight;
    int partWidthC, partHeightC;

    // TODO is this correct?
    partWidth = predPartL0L.size();
    partHeight = predPartL0L[0].size();

    partWidthC = predPartL0Cb.size();
    partHeightC = predPartL0Cb[0].size();

    std::vector<std::vector<int>> predPartL;
    std::vector<std::vector<int>> predPartCb;
    std::vector<std::vector<int>> predPartCr;

    auto Default_weight_pred = [&predPartL, &predPartCb, &predPartCr,

                                   predFlagL0, predFlagL1,

                                   &predPartL0L, &predPartL1L,
                                   logWDL, w0L, w1L, o0L, o1L,

                                   &predPartL0Cb, &predPartL1Cb,
                                   logWDCb, w0Cb, w1Cb, o0Cb, o1Cb,

                                   &predPartL0Cr, &predPartL1Cr,
                                   logWDCr, w0Cr, w1Cr, o0Cr, o1Cr,

                                   this]() {
        predPartL = Default_weighted_sample_prediction_process(
            predFlagL0,
            predFlagL1,
            predPartL0L,
            predPartL1L,
            logWDL, w0L, w1L, o0L, o1L);

        predPartCb = Default_weighted_sample_prediction_process(
            predFlagL0,
            predFlagL1,
            predPartL0Cb,
            predPartL1Cb,
            logWDCb, w0Cb, w1Cb, o0Cb, o1Cb);

        predPartCr = Default_weighted_sample_prediction_process(
            predFlagL0,
            predFlagL1,
            predPartL0Cr,
            predPartL1Cr,
            logWDCr, w0Cr, w1Cr, o0Cr, o1Cr);
    };

    auto Weighted_pred = [&predPartL, &predPartCb, &predPartCr,

                             predFlagL0, predFlagL1,

                             &predPartL0L, &predPartL1L,
                             logWDL, w0L, w1L, o0L, o1L,

                             &predPartL0Cb, &predPartL1Cb,
                             logWDCb, w0Cb, w1Cb, o0Cb, o1Cb,

                             &predPartL0Cr, &predPartL1Cr,
                             logWDCr, w0Cr, w1Cr, o0Cr, o1Cr,

                             this]() {
        predPartL = Weighted_sample_prediction_process(
            predFlagL0,
            predFlagL1,
            predPartL0L,
            predPartL1L,
            logWDL, w0L, w1L, o0L, o1L,
            true /* is_luma */);

        predPartCb = Weighted_sample_prediction_process(
            predFlagL0,
            predFlagL1,
            predPartL0Cb,
            predPartL1Cb,
            logWDCb, w0Cb, w1Cb, o0Cb, o1Cb,
            false /* is_luma */);

        predPartCr = Weighted_sample_prediction_process(
            predFlagL0,
            predFlagL1,
            predPartL0Cr,
            predPartL1Cr,
            logWDCr, w0Cr, w1Cr, o0Cr, o1Cr,
            false /* is_luma */);
    };

    if (slice_->is_P_or_SP()) {
        assert(predFlagL0);

        if (predFlagL0) {
            if (!pps_->weighted_pred_flag()) {
                // clause 8.4.2.3.1
                Default_weight_pred();
            } else {
                // clause 8.4.2.3.2
                Weighted_pred();
            }
        }
    } else {
        assert(predFlagL0 || predFlagL1);
        assert(slice_->is_B_slice());

        if (pps_->weighted_bipred_idc() == 0) {
            // clause 8.4.2.3.1

            Default_weight_pred();

        } else if (pps_->weighted_bipred_idc() == 1) {
            // clause 8.4.2.3.2
            Weighted_pred();

        } else {
            assert(pps_->weighted_bipred_idc() == 2);

            if (predFlagL0 && predFlagL1) {
                // clause 8.4.2.3.2
                Weighted_pred();
            } else {
                // clause 8.4.2.3.1
                Default_weight_pred();
            }
        }
    }
    return { predPartL, predPartCb, predPartCr };
}

// Done
//
// 8.4.2.3.1 Default weighted sample prediction process
std::vector<std::vector<int>>
MacroBlock::Default_weighted_sample_prediction_process(
    int predFlagL0,
    int predFlagL1,
    const std::vector<std::vector<int>>& predPartL0C,
    const std::vector<std::vector<int>>& predPartL1C,
    int logWDC, int w0C, int w1C, int o0C, int o1C)
{
    int partWidth = predPartL0C.size();
    int partHeight = predPartL0C[0].size();

    std::vector<std::vector<int>> predPartC(partWidth, std::vector<int>(partHeight, 0));

    if (predFlagL0 && !predFlagL1) {
        for (int x = 0; x < partWidth; x++) {
            for (int y = 0; y < partHeight; y++) {
                predPartC[x][y] = predPartL0C[x][y];
            }
        }
    } else if (!predFlagL0 && predFlagL1) {
        for (int x = 0; x < partWidth; x++) {
            for (int y = 0; y < partHeight; y++) {
                predPartC[x][y] = predPartL1C[x][y];
            }
        }
    } else {
        for (int x = 0; x < partWidth; x++) {
            for (int y = 0; y < partHeight; y++) {
                predPartC[x][y] = (predPartL0C[x][y] + predPartL1C[x][y] + 1) >> 1;
            }
        }
    }
    return std::move(predPartC);
}

// Done
//
// 8.4.2.3.2 Weighted sample prediction process
std::vector<std::vector<int>>
MacroBlock::Weighted_sample_prediction_process(
    int predFlagL0,
    int predFlagL1,
    const std::vector<std::vector<int>>& predPartL0C,
    const std::vector<std::vector<int>>& predPartL1C,
    int logWDC, int w0C, int w1C, int o0C, int o1C,
    bool is_luma)
{
    int partWidth = predPartL0C.size();
    int partHeight = predPartL0C[0].size();

    std::vector<std::vector<int>> predPartC(partWidth, std::vector<int>(partHeight, 0));

    std::function<int(int)> Clip1;
    if (is_luma)
        Clip1 = std::bind(&MacroBlock::Clip1Y, this, std::placeholders::_1);
    else
        Clip1 = std::bind(&MacroBlock::Clip1C, this, std::placeholders::_1);

    if (predFlagL0 && !predFlagL1) {
        if (logWDC >= 1) {

            for (int x = 0; x < partWidth; x++) {
                for (int y = 0; y < partHeight; y++) {
                    predPartC[x][y] = Clip1(((predPartL0C[x][y] * w0C + (1 << (logWDC - 1))) >> logWDC) + o0C);
                }
            }

        } else {
            for (int x = 0; x < partWidth; x++) {
                for (int y = 0; y < partHeight; y++) {
                    predPartC[x][y] = Clip1(predPartL0C[x][y] * w0C + o0C);
                }
            }
        }
    } else if (!predFlagL0 && predFlagL1) {

        if (logWDC >= 1) {

            for (int x = 0; x < partWidth; x++) {
                for (int y = 0; y < partHeight; y++) {

                    predPartC[x][y] = Clip1(((predPartL1C[x][y] * w1C + (1 << (logWDC - 1))) >> logWDC) + o1C);
                }
            }

        } else {

            for (int x = 0; x < partWidth; x++) {
                for (int y = 0; y < partHeight; y++) {

                    predPartC[x][y] = Clip1(predPartL1C[x][y] * w1C + o1C);
                }
            }
        }

    } else {

        for (int x = 0; x < partWidth; x++) {
            for (int y = 0; y < partHeight; y++) {
                predPartC[x][y] = Clip1(((predPartL0C[x][y] * w0C
                                             + predPartL1C[x][y] * w1C
                                             + (1 << logWDC))
                                            >> (logWDC + 1))
                    + ((o0C + o1C + 1) >> 1));
            }
        }
    }

    return std::move(predPartC);
}

Slice* MacroBlock::get_RefPicList0_by_part(int mbPartIdx, int subMbPartIdx)
{
    if (PredFlagL0_[mbPartIdx]) {
        auto refidx0 = get_RefIdxL0(mbPartIdx);
        return slice_->get_RefPicList0(refidx0);
    } else
        return nullptr;
}

Slice* MacroBlock::get_RefPicList1_by_part(int mbPartIdx, int subMbPartIdx)
{
    if (PredFlagL1_[mbPartIdx]) {
        auto refidx1 = get_RefIdxL1(mbPartIdx);
        return slice_->get_RefPicList1(refidx1);
    } else
        return nullptr;
}

int MacroBlock::get_PredFlagL0_by_part(int mbPartIdx, int subMbPartIdx)
{
    return get_PredFlagL0(mbPartIdx);
}

int MacroBlock::get_PredFlagL1_by_part(int mbPartIdx, int subMbPartIdx)
{
    return get_PredFlagL1(mbPartIdx);
}

int MacroBlock::get_MvL0_x_by_part(int mbPartIdx, int subMbPartIdx)
{
    if (PredFlagL0_[mbPartIdx]) {
        return get_MvL0(mbPartIdx, subMbPartIdx)[0];
    } else
        return INT32_MIN; // ? TODO
}

int MacroBlock::get_MvL0_y_by_part(int mbPartIdx, int subMbPartIdx)
{
    if (PredFlagL0_[mbPartIdx]) {
        return get_MvL0(mbPartIdx, subMbPartIdx)[1];
    } else
        return INT32_MIN; // ? TODO
}

int MacroBlock::get_MvL1_x_by_part(int mbPartIdx, int subMbPartIdx)
{
    if (PredFlagL1_[mbPartIdx]) {
        return get_MvL1(mbPartIdx, subMbPartIdx)[0];
    } else
        return INT32_MIN; // ? TODO
}

int MacroBlock::get_MvL1_y_by_part(int mbPartIdx, int subMbPartIdx)
{
    if (PredFlagL1_[mbPartIdx]) {
        return get_MvL1(mbPartIdx, subMbPartIdx)[1];
    } else
        return INT32_MIN; // ? TODO
}
