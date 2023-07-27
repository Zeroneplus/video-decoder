
#pragma once

#include "NalUnit.h"
#include "Vui.h"

class Sps {
public:
    Sps() = default;
    Sps(std::shared_ptr<NalUnit::RbspData> rbsp);
    int parse();

    void log();
    void dump();
    int sps_id() { return seq_parameter_set_id_; }
    bool separate_colour_plane()
    {
        return separate_colour_plane_flag_;
    }
    int frame_num_bits()
    {
        return log2_max_frame_num_minus4_ + 4;
    }
    bool frame_mbs_only()
    {
        return frame_mbs_only_flag_;
    }
    int pic_order_cnt_type()
    {
        return pic_order_cnt_type_;
    }
    int poc_lsb_bits()
    {
        return log2_max_pic_order_cnt_lsb_minus4_ + 4;
    }
    bool delta_pic_order_always_zero()
    {
        return delta_pic_order_always_zero_flag_;
    }

    int num_ref_frames_in_pic_order_cnt_cycle()
    {
        return num_ref_frames_in_pic_order_cnt_cycle_;
    }

    int max_num_ref_frames()
    {
        return max_num_ref_frames_;
    }

    std::vector<int>& offset_for_ref_frame() { return offset_for_ref_frame_; }
    int offset_for_non_ref_pic() { return offset_for_non_ref_pic_; }
    int offset_for_top_to_bottom_field() { return offset_for_top_to_bottom_field_; }

    int bit_depth_luma()
    {
        return bit_depth_luma_minus8_ + 8;
    }

    int bit_depth_chroma()
    {
        return bit_depth_chroma_minus8_ + 8;
    }

private:
    std::shared_ptr<NalUnit::RbspData> rbsp_data_;
    /* sps field */
    int profile_idc_;
    int constraint_set0_flag_;
    int constraint_set1_flag_;
    int constraint_set2_flag_;
    int constraint_set3_flag_;
    int constraint_set4_flag_;
    int constraint_set5_flag_;
    int reserved_zero_2bits_;
    int level_idc_;
    int seq_parameter_set_id_;

    int chroma_format_idc_ = 1; // default is 1
    int separate_colour_plane_flag_ = 0; // default is 0
    int bit_depth_luma_minus8_ = 0;
    int bit_depth_chroma_minus8_ = 0;
    int qpprime_y_zero_transform_bypass_flag_ = 0;
    int seq_scaling_matrix_present_flag_ = 0;

    int log2_max_frame_num_minus4_;

    int pic_order_cnt_type_;

    int log2_max_pic_order_cnt_lsb_minus4_;

    int delta_pic_order_always_zero_flag_;
    int offset_for_non_ref_pic_;
    int offset_for_top_to_bottom_field_;
    int num_ref_frames_in_pic_order_cnt_cycle_;
    std::vector<int> offset_for_ref_frame_;

    int max_num_ref_frames_;

    int gaps_in_frame_num_value_allowed_flag_;

    int pic_width_in_mbs_minus1_;
    int pic_height_in_map_units_minus1_;

    int frame_mbs_only_flag_;
    int mb_adaptive_frame_field_flag_ = 0;

    int direct_8x8_inference_flag_;

    int frame_cropping_flag_;
    int frame_crop_left_offset_ = 0;
    int frame_crop_right_offset_ = 0;
    int frame_crop_top_offset_ = 0;
    int frame_crop_bottom_offset_ = 0;

    int vui_parameters_present_flag_;
    Vui vui_;

public:
    int ChromaArrayType() { return ChromaArrayType_; }
    bool MbaffFlag() { return mb_adaptive_frame_field_flag_; }
    int FrameHeightInMbs() { return FrameHeightInMbs_; }
    int MbHeightC() { return 16 / SubHeightC_; }
    int MbWidthC() { return 16 / SubWidthC_; }
    int PicWidthInMbs() { return PicWidthInMbs_; }
    int PicWidthInSamplesL() { return PicWidthInSamplesL_; }
    int MaxFrameNum() { return MaxFrameNum_; }
    int MaxPicOrderCntLsb() { return MaxPicOrderCntLsb_; }
    int ExpectedDeltaPerPicOrderCntCycle() { return ExpectedDeltaPerPicOrderCntCycle_; }

private:
    int ChromaArrayType_;

    int MaxFrameNum_;

    int MaxPicOrderCntLsb_;
    int ExpectedDeltaPerPicOrderCntCycle_;

    int PicWidthInMbs_;
    int PicWidthInSamplesL_;
    int PicWidthInSamplesC_;
    int PicHeightInMapUnits_;
    int FrameHeightInMbs_;

    int SubWidthC_ = -1;
    int SubHeightC_ = -1;
};