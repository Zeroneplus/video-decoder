
#include "Vui.h"

int Hrd::parse(std::shared_ptr<NalUnit::RbspData> rbsp)
{
    cpb_cnt_minus1 = rbsp->read_ue();
    bit_rate_scale = rbsp->read_u(4);
    cpb_size_scale = rbsp->read_u(4);

    for (int SchedSelIdx = 0; SchedSelIdx <= cpb_cnt_minus1; SchedSelIdx++) {
        bit_rate_value_minus1.push_back(rbsp->read_ue());
        cpb_size_value_minus1.push_back(rbsp->read_ue());
        cbr_flag.push_back(rbsp->read_u1());
    }

    initial_cpb_removal_delay_length_minus1 = rbsp->read_u(5);
    cpb_removal_delay_length_minus1 = rbsp->read_u(5);
    dpb_output_delay_length_minus1 = rbsp->read_u(5);
    time_offset_length = rbsp->read_u(5);

    return 0;
}

int Vui::parse(std::shared_ptr<NalUnit::RbspData> rbsp)
{

    aspect_ratio_info_present_flag_ = rbsp->read_u1();
    if (aspect_ratio_info_present_flag_) {
        aspect_ratio_idc_ = rbsp->read_u(8);
        if (aspect_ratio_idc_ == 255 /* Extended_SAR */) {
            sar_width_ = rbsp->read_u(16);
            sar_height_ = rbsp->read_u(16);
        }
    }

    overscan_info_present_flag_ = rbsp->read_u1();
    if (overscan_info_present_flag_)
        overscan_appropriate_flag_ = rbsp->read_u1();
    video_signal_type_present_flag_ = rbsp->read_u1();
    if (video_signal_type_present_flag_) {
        video_format_ = rbsp->read_u(3);
        video_full_range_flag_ = rbsp->read_u1();
        colour_description_present_flag_ = rbsp->read_u1();
        if (colour_description_present_flag_) {
            colour_primaries_ = rbsp->read_u(8);
            transfer_characteristics_ = rbsp->read_u(8);
            matrix_coefficients_ = rbsp->read_u(8);
        }
    }

    chroma_loc_info_present_flag_ = rbsp->read_u1();
    if (chroma_loc_info_present_flag_) {
        chroma_sample_loc_type_top_field_ = rbsp->read_ue();
        chroma_sample_loc_type_bottom_field_ = rbsp->read_ue();
    }

    timing_info_present_flag_ = rbsp->read_u1();
    if (timing_info_present_flag_) {
        num_units_in_tick_ = rbsp->read_u(32);
        time_scale_ = rbsp->read_u(32);
        fixed_frame_rate_flag_ = rbsp->read_u1();
    }

    nal_hrd_parameters_present_flag_ = rbsp->read_u1();
    if (nal_hrd_parameters_present_flag_)
        nal_hrd_.parse(rbsp);
    vcl_hrd_parameters_present_flag_ = rbsp->read_u1();
    if (vcl_hrd_parameters_present_flag_)
        vcl_hrd_.parse(rbsp);

    if (nal_hrd_parameters_present_flag_ || vcl_hrd_parameters_present_flag_)
        low_delay_hrd_flag_ = rbsp->read_u1();

    pic_struct_present_flag_ = rbsp->read_u1();
    bitstream_restriction_flag_ = rbsp->read_u1();

    if (bitstream_restriction_flag_) {
        motion_vectors_over_pic_boundaries_flag_ = rbsp->read_u1();
        max_bytes_per_pic_denom_ = rbsp->read_ue();
        max_bits_per_mb_denom_ = rbsp->read_ue();
        log2_max_mv_length_horizontal_ = rbsp->read_ue();
        log2_max_mv_length_vertical_ = rbsp->read_ue();
        max_num_reorder_frames_ = rbsp->read_ue();
        max_dec_frame_buffering_ = rbsp->read_ue();
    }

    return 0;
}