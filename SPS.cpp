
#include <iostream>

#include "SPS.h"

Sps::Sps(std::shared_ptr<NalUnit::RbspData> rbsp)
    : rbsp_data(std::move(rbsp))
{
}

int Sps::parse()
{
    profile_idc_ = rbsp_data->read_u(8);
    constraint_set0_flag_ = rbsp_data->read_u1();
    constraint_set1_flag_ = rbsp_data->read_u1();
    constraint_set2_flag_ = rbsp_data->read_u1();
    constraint_set3_flag_ = rbsp_data->read_u1();
    constraint_set4_flag_ = rbsp_data->read_u1();
    constraint_set5_flag_ = rbsp_data->read_u1();
    reserved_zero_2bits_ = rbsp_data->read_u(2);
    level_idc_ = rbsp_data->read_u(8);
    seq_parameter_set_id_ = rbsp_data->read_ue();

    if (profile_idc_ == 100 || profile_idc_ == 110
        || profile_idc_ == 122 || profile_idc_ == 244
        || profile_idc_ == 44 || profile_idc_ == 83
        || profile_idc_ == 86 || profile_idc_ == 118
        || profile_idc_ == 128) {

        chroma_format_idc_ = rbsp_data->read_ue();
        if (chroma_format_idc_ == 3)
            separate_colour_plane_flag_ = rbsp_data->read_u1();
        bit_depth_luma_minus8_ = rbsp_data->read_ue();
        bit_depth_chroma_minus8_ = rbsp_data->read_ue();
        qpprime_y_zero_transform_bypass_flag_ = rbsp_data->read_u1();
        seq_scaling_matrix_present_flag_ = rbsp_data->read_u1();
        if (seq_scaling_matrix_present_flag_) {
            std::cout << "unsupported profile_idc" << std::endl;
            return -1;
        }
    }

    if (separate_colour_plane_flag_ == 0)
        ChromaArrayType = chroma_format_idc_;
    else
        ChromaArrayType = 0;

    log2_max_frame_num_minus4_ = rbsp_data->read_ue();
    MaxFrameNum = 1 << (log2_max_frame_num_minus4_ + 4);

    pic_order_cnt_type_ = rbsp_data->read_ue();
    if (pic_order_cnt_type_ == 0) {
        log2_max_pic_order_cnt_lsb_minus4_ = rbsp_data->read_ue();
        MaxPicOrderCntLsb = 1 << (log2_max_pic_order_cnt_lsb_minus4_ + 4);
    } else if (pic_order_cnt_type_ == 1) {
        delta_pic_order_always_zero_flag_ = rbsp_data->read_u1();
        offset_for_non_ref_pic_ = rbsp_data->read_se();
        offset_for_top_to_bottom_field_ = rbsp_data->read_se();
        num_ref_frames_in_pic_order_cnt_cycle_ = rbsp_data->read_ue();
        offset_for_ref_frame_.reserve(num_ref_frames_in_pic_order_cnt_cycle_);
        for (int i = 0; i < num_ref_frames_in_pic_order_cnt_cycle_; i++)
            offset_for_ref_frame_[i] = rbsp_data->read_se();

        ExpectedDeltaPerPicOrderCntCycle = 0;
        for (int i = 0; i < num_ref_frames_in_pic_order_cnt_cycle_; i++)
            ExpectedDeltaPerPicOrderCntCycle += offset_for_ref_frame_[i];
    }

    max_num_ref_frames_ = rbsp_data->read_ue();

    gaps_in_frame_num_value_allowed_flag_ = rbsp_data->read_u1();

    pic_width_in_mbs_minus1_ = rbsp_data->read_ue();
    PicWidthInMbs = pic_width_in_mbs_minus1_ + 1;
    PicWidthInSamplesL = 16 * PicWidthInMbs;

    pic_height_in_map_units_minus1_ = rbsp_data->read_ue();
    PicHeightInMapUnits = pic_height_in_map_units_minus1_ + 1;

    frame_mbs_only_flag_ = rbsp_data->read_u1();
    if (!frame_mbs_only_flag_)
        mb_adaptive_frame_field_flag_ = rbsp_data->read_u1();

    FrameHeightInMbs = (2 - frame_mbs_only_flag_) * PicHeightInMapUnits;

    direct_8x8_inference_flag_ = rbsp_data->read_u1();

    frame_cropping_flag_ = rbsp_data->read_u1();
    if (frame_cropping_flag_) {
        frame_crop_left_offset_ = rbsp_data->read_ue();
        frame_crop_right_offset_ = rbsp_data->read_ue();
        frame_crop_top_offset_ = rbsp_data->read_ue();
        frame_crop_bottom_offset_ = rbsp_data->read_ue();
    }

    vui_parameters_present_flag_ = rbsp_data->read_u1();

    if (vui_parameters_present_flag_) {
        std::cout << "vui_parameters_present_flag is 1" << std::endl;
        // return -1;
    }

    if (!rbsp_data->eof()) {
        std::cout << "after parse, sps rbsp is not ended" << std::endl;
        // return -1;
    }
    return 0;
}

void Sps::log()
{
    const char* space = "  ";
    std::cout << "SPS log:" << std::endl;
    std::cout << space << "profile_idc is " << (uint32_t)profile_idc_ << std::endl;
    std::cout << space << "level_idc is " << (uint32_t)level_idc_ << std::endl;
    std::cout << space << "seq_parameter_set_id is " << seq_parameter_set_id_ << std::endl;
    std::cout << space << "log2_max_frame_num_minus4 is " << log2_max_frame_num_minus4_ << std::endl;
    std::cout << space << space << "MaxFrameNum is " << MaxFrameNum << std::endl;
    std::cout << space << "pic_order_cnt_type is " << pic_order_cnt_type_ << std::endl;
    if (pic_order_cnt_type_ == 0) {
        std::cout << space << space << "log2_max_pic_order_cnt_lsb_minus4 is " << log2_max_pic_order_cnt_lsb_minus4_ << std::endl;
        std::cout << space << space << space << "MaxPicOrderCntLsb is " << MaxPicOrderCntLsb << std::endl;
    } else {
        std::cout << space << space << "delta_pic_order_always_zero_flag is " << (uint32_t)delta_pic_order_always_zero_flag_ << std::endl;
        std::cout << space << space << "offset_for_non_ref_pic is " << offset_for_non_ref_pic_ << std::endl;
        std::cout << space << space << "offset_for_top_to_bottom_field is " << offset_for_top_to_bottom_field_ << std::endl;
        std::cout << space << space << "num_ref_frames_in_pic_order_cnt_cycle is " << num_ref_frames_in_pic_order_cnt_cycle_ << std::endl;
    }
    std::cout << space << "max_num_ref_frames is " << max_num_ref_frames_ << std::endl;
    std::cout << space << "gaps_in_frame_num_value_allowed_flag is " << (uint32_t)gaps_in_frame_num_value_allowed_flag_ << std::endl;
    std::cout << space << space << "PicWidthInMbs is " << PicWidthInMbs << std::endl;
    std::cout << space << space << "PicWidthInSamplesL is " << PicWidthInSamplesL << std::endl;
    std::cout << space << "frame_mbs_only_flag is " << (uint32_t)frame_mbs_only_flag_ << std::endl;
    if (!frame_mbs_only_flag_) {
        std::cout << space << space << "mb_adaptive_frame_field_flag is " << (uint32_t)frame_mbs_only_flag_ << std::endl;
    }
    std::cout << space << "direct_8x8_inference_flag is " << (uint32_t)direct_8x8_inference_flag_ << std::endl;
    std::cout << space << "frame_cropping_flag is " << (uint32_t)frame_cropping_flag_ << std::endl;
    if (frame_cropping_flag_) {
        std::cout << space << space << "frame_crop_left_offset is " << frame_crop_left_offset_ << std::endl;
        std::cout << space << space << "frame_crop_right_offset is " << frame_crop_right_offset_ << std::endl;
        std::cout << space << space << "frame_crop_top_offset is " << frame_crop_top_offset_ << std::endl;
        std::cout << space << space << "frame_crop_bottom_offset is " << frame_crop_bottom_offset_ << std::endl;
    }
    std::cout << space << "vui_parameters_present_flag is " << (uint32_t)vui_parameters_present_flag_ << std::endl;
}