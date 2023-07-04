
#pragma once

#include "NalUnit.h"

class Sps {
public:
    Sps() = default;
    Sps(std::shared_ptr<NalUnit::RbspData> rbsp);
    int parse();

    void log();
    int sps_id() { return seq_parameter_set_id_; }

private:
    std::shared_ptr<NalUnit::RbspData> rbsp_data;
    /* sps field */
    uint8_t profile_idc_;
    uint8_t constraint_set0_flag_;
    uint8_t constraint_set1_flag_;
    uint8_t constraint_set2_flag_;
    uint8_t constraint_set3_flag_;
    uint8_t constraint_set4_flag_;
    uint8_t constraint_set5_flag_;
    uint8_t reserved_zero_2bits_;
    uint8_t level_idc_;
    uint32_t seq_parameter_set_id_;

    uint32_t chroma_format_idc_ = 1;
    uint8_t separate_colour_plane_flag_ = 0;
    uint32_t bit_depth_luma_minus8_ = 0;
    uint32_t bit_depth_chroma_minus8_ = 0;
    uint8_t qpprime_y_zero_transform_bypass_flag_ = 0;
    uint8_t seq_scaling_matrix_present_flag_ = 0;

    uint32_t log2_max_frame_num_minus4_;

    uint32_t pic_order_cnt_type_;

    uint32_t log2_max_pic_order_cnt_lsb_minus4_;

    uint8_t delta_pic_order_always_zero_flag_;
    int32_t offset_for_non_ref_pic_;
    int32_t offset_for_top_to_bottom_field_;
    uint32_t num_ref_frames_in_pic_order_cnt_cycle_;
    std::vector<int32_t> offset_for_ref_frame_;

    uint32_t max_num_ref_frames_;

    uint8_t gaps_in_frame_num_value_allowed_flag_;

    uint32_t pic_width_in_mbs_minus1_;
    uint32_t pic_height_in_map_units_minus1_;

    uint8_t frame_mbs_only_flag_;
    uint8_t mb_adaptive_frame_field_flag_ = 0;

    uint8_t direct_8x8_inference_flag_;

    uint8_t frame_cropping_flag_;
    uint32_t frame_crop_left_offset_ = 0;
    uint32_t frame_crop_right_offset_ = 0;
    uint32_t frame_crop_top_offset_ = 0;
    uint32_t frame_crop_bottom_offset_ = 0;

    uint8_t vui_parameters_present_flag_;

public:
    uint32_t ChromaArrayType;

    uint32_t MaxFrameNum;

    uint32_t MaxPicOrderCntLsb;
    int32_t ExpectedDeltaPerPicOrderCntCycle;

    uint32_t PicWidthInMbs;
    uint32_t PicWidthInSamplesL;
    uint32_t PicWidthInSamplesC;
    uint32_t PicHeightInMapUnits;
    uint32_t FrameHeightInMbs;
};