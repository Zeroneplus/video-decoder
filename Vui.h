
#pragma once

#include "NalUnit.h"

struct Hrd {
    int parse(std::shared_ptr<NalUnit::RbspData> rbsp);
    int cpb_cnt_minus1;
    int bit_rate_scale;
    int cpb_size_scale;
    std::vector<int> bit_rate_value_minus1;
    std::vector<int> cpb_size_value_minus1;
    std::vector<int> cbr_flag;
    int initial_cpb_removal_delay_length_minus1;
    int cpb_removal_delay_length_minus1;
    int dpb_output_delay_length_minus1;
    int time_offset_length;
};

class Vui {
public:
    int parse(std::shared_ptr<NalUnit::RbspData> rbsp);

private:
    Hrd nal_hrd_, vcl_hrd_;

    int aspect_ratio_info_present_flag_;
    int aspect_ratio_idc_;
    int sar_width_;
    int sar_height_;
    int overscan_info_present_flag_;
    int overscan_appropriate_flag_;
    int video_signal_type_present_flag_;
    int video_format_;
    int video_full_range_flag_;
    int colour_description_present_flag_;
    int colour_primaries_;
    int transfer_characteristics_;
    int matrix_coefficients_;
    int chroma_loc_info_present_flag_;
    int chroma_sample_loc_type_top_field_;
    int chroma_sample_loc_type_bottom_field_;
    int timing_info_present_flag_;
    int num_units_in_tick_;
    int time_scale_;
    int fixed_frame_rate_flag_;
    int nal_hrd_parameters_present_flag_;
    int vcl_hrd_parameters_present_flag_;
    int low_delay_hrd_flag_;
    int pic_struct_present_flag_;
    int bitstream_restriction_flag_;
    int motion_vectors_over_pic_boundaries_flag_;
    int max_bytes_per_pic_denom_;
    int max_bits_per_mb_denom_;
    int log2_max_mv_length_horizontal_;
    int log2_max_mv_length_vertical_;
    int max_num_reorder_frames_;
    int max_dec_frame_buffering_;
};