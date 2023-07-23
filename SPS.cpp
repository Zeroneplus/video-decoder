
#include <iostream>

#include "spdlog/spdlog.h"

#include "SPS.h"

Sps::Sps(std::shared_ptr<NalUnit::RbspData> rbsp)
    : rbsp_data_(std::move(rbsp))
{
}

int Sps::parse()
{
    profile_idc_ = rbsp_data_->read_u(8);
    spdlog::info("profile_idc {}", profile_idc_);

    constraint_set0_flag_ = rbsp_data_->read_u1();
    constraint_set1_flag_ = rbsp_data_->read_u1();
    constraint_set2_flag_ = rbsp_data_->read_u1();
    constraint_set3_flag_ = rbsp_data_->read_u1();
    constraint_set4_flag_ = rbsp_data_->read_u1();
    constraint_set5_flag_ = rbsp_data_->read_u1();
    reserved_zero_2bits_ = rbsp_data_->read_u(2);

    level_idc_ = rbsp_data_->read_u(8);
    spdlog::info("level_idc {}", level_idc_);

    seq_parameter_set_id_ = rbsp_data_->read_ue();
    spdlog::info("the current sps id {}", seq_parameter_set_id_);

    if (profile_idc_ == 100 || profile_idc_ == 110
        || profile_idc_ == 122 || profile_idc_ == 244
        || profile_idc_ == 44 || profile_idc_ == 83
        || profile_idc_ == 86 || profile_idc_ == 118
        || profile_idc_ == 128) {

        spdlog::trace("will read chroma_format_idc");
        chroma_format_idc_ = rbsp_data_->read_ue();
        if (chroma_format_idc_ == 3) {
            spdlog::trace("will read separate_colour_plane_flag");
            separate_colour_plane_flag_ = rbsp_data_->read_u1();
        }

        bit_depth_luma_minus8_ = rbsp_data_->read_ue();
        spdlog::trace("bit_depth_luma_minus8 {}", bit_depth_luma_minus8_);

        bit_depth_chroma_minus8_ = rbsp_data_->read_ue();
        spdlog::trace("bit_depth_chroma_minus8 {}", bit_depth_chroma_minus8_);

        qpprime_y_zero_transform_bypass_flag_ = rbsp_data_->read_u1();
        spdlog::trace("qpprime_y_zero_transform_bypass_flag_ {}", (bool)qpprime_y_zero_transform_bypass_flag_);

        seq_scaling_matrix_present_flag_ = rbsp_data_->read_u1();
        if (seq_scaling_matrix_present_flag_) {
            spdlog::error("seq_scaling_matrix_present is true, unsupported");
            return -1;
        } else
            spdlog::trace("seq_scaling_matrix_present_flag is false, OK");
    }

    spdlog::info("chroma_format_idc {}", chroma_format_idc_);

    // calculate Chroma Format、SubWidthC、SubHeightC
    switch (chroma_format_idc_) {
    case 0:
        assert(!separate_colour_plane_flag_);
        spdlog::info("chroma_format: monochrome");
        break;
    case 1:
        assert(!separate_colour_plane_flag_);
        spdlog::info("chroma_format: 4:2:0");
        SubWidthC_ = 2;
        SubHeightC_ = 2;
        break;
    case 2:
        assert(!separate_colour_plane_flag_);
        spdlog::info("chroma_format: 4:2:2");
        SubWidthC_ = 2;
        SubHeightC_ = 1;
        break;
    case 3:
        spdlog::info("chroma_format: 4:4:4");
        spdlog::info("  separate_colour_plane_flag {}",
            (bool)separate_colour_plane_flag_);
        if (!separate_colour_plane_flag_) {
            SubWidthC_ = 1;
            SubHeightC_ = 1;
        }
        break;
    default:
        spdlog::error("invalid chroma_format_idc {}", chroma_format_idc_);
        return -1;
    }

    if (separate_colour_plane_flag_ == 0)
        ChromaArrayType_ = chroma_format_idc_;
    else
        ChromaArrayType_ = 0;

    spdlog::warn("calculated ChromaArrayType {}", ChromaArrayType_);

    if (ChromaArrayType_ != 1) {
        spdlog::error("only yuv 4:2:0 is allowed");
        return -1;
    }

    log2_max_frame_num_minus4_ = rbsp_data_->read_ue();
    spdlog::warn("log2_max_frame_num_minus4 {}", log2_max_frame_num_minus4_);
    MaxFrameNum_ = 1 << (log2_max_frame_num_minus4_ + 4);
    spdlog::warn("allowed MaxFrameNum {}", MaxFrameNum_);

    pic_order_cnt_type_ = rbsp_data_->read_ue();
    if (pic_order_cnt_type_ == 0) {
        spdlog::warn("poc type is 0...");

        log2_max_pic_order_cnt_lsb_minus4_ = rbsp_data_->read_ue();
        spdlog::warn("log2_max_pic_order_cnt_lsb_minus4 {}", log2_max_pic_order_cnt_lsb_minus4_);

        MaxPicOrderCntLsb_ = 1 << (log2_max_pic_order_cnt_lsb_minus4_ + 4);
        spdlog::warn("allowed MaxPicOrderCntLsb {}", MaxPicOrderCntLsb_);
    } else if (pic_order_cnt_type_ == 1) {
        spdlog::warn("poc type is 1...");

        delta_pic_order_always_zero_flag_ = rbsp_data_->read_u1();
        if (delta_pic_order_always_zero_flag_)
            spdlog::warn("delta_pic_order_always_zero_flag is true, some fields in slice header will not present");
        else
            spdlog::info("delta_pic_order_always_zero_flag is false");

        offset_for_non_ref_pic_ = rbsp_data_->read_se();
        spdlog::info("offset_for_non_ref_pic {}", offset_for_non_ref_pic_);

        offset_for_top_to_bottom_field_ = rbsp_data_->read_se();
        spdlog::info("offset_for_top_to_bottom_field {}", offset_for_top_to_bottom_field_);

        num_ref_frames_in_pic_order_cnt_cycle_ = rbsp_data_->read_ue();
        spdlog::info("num_ref_frames_in_pic_order_cnt_cycle is {}", num_ref_frames_in_pic_order_cnt_cycle_);

        offset_for_ref_frame_.reserve(num_ref_frames_in_pic_order_cnt_cycle_);
        for (int i = 0; i < num_ref_frames_in_pic_order_cnt_cycle_; i++) {
            offset_for_ref_frame_[i] = rbsp_data_->read_se();
            spdlog::info("  offset_for_ref_frame[{}] {}", i, offset_for_ref_frame_[i]);
        }

        ExpectedDeltaPerPicOrderCntCycle_ = 0;
        for (int i = 0; i < num_ref_frames_in_pic_order_cnt_cycle_; i++)
            ExpectedDeltaPerPicOrderCntCycle_ += offset_for_ref_frame_[i];
        spdlog::info("calculated ExpectedDeltaPerPicOrderCntCycle {}", ExpectedDeltaPerPicOrderCntCycle_);

    } else {
        spdlog::warn("poc type is 2");
    }

    max_num_ref_frames_ = rbsp_data_->read_ue();
    spdlog::warn("max_num_ref_frames {}", max_num_ref_frames_);

    if (max_num_ref_frames_ == 0) {
        spdlog::error("max_num_ref_frames is 0");
        return -1;
    }

    gaps_in_frame_num_value_allowed_flag_ = rbsp_data_->read_u1();
    if (gaps_in_frame_num_value_allowed_flag_)
        spdlog::warn("gaps is allowed in frame num");
    else
        spdlog::warn("gaps is not allowed in frame num");

    pic_width_in_mbs_minus1_ = rbsp_data_->read_ue();
    PicWidthInMbs_ = pic_width_in_mbs_minus1_ + 1;
    PicWidthInSamplesL_ = 16 * PicWidthInMbs_;
    spdlog::info("pic_width_in_mbs_minus1 {}, PicWidthInMbs {}, PicWidthInSamplesL {}",
        pic_width_in_mbs_minus1_, PicWidthInMbs_, PicWidthInSamplesL_);

    pic_height_in_map_units_minus1_ = rbsp_data_->read_ue();
    PicHeightInMapUnits_ = pic_height_in_map_units_minus1_ + 1;

    frame_mbs_only_flag_ = rbsp_data_->read_u1();
    if (!frame_mbs_only_flag_) {
        mb_adaptive_frame_field_flag_ = rbsp_data_->read_u1();
        if (mb_adaptive_frame_field_flag_)
            spdlog::warn("this file allow field, frame, mbaff, "
                         "further info should be determined from slice header and mb header");
        else
            spdlog::warn("this file allow field, frame,"
                         "further info should be determined from slice header");

        spdlog::error("only frame mode is supported now");
        return -1;
    } else
        spdlog::warn("this file is frame_mbs_only");

    FrameHeightInMbs_ = (2 - frame_mbs_only_flag_) * PicHeightInMapUnits_;

    spdlog::info("pic_height_in_map_units_minus1 {}, PicHeightInMapUnits {}, FrameHeightInMbs {}, FrameHeightInSamplesL {}",
        pic_height_in_map_units_minus1_, PicHeightInMapUnits_, FrameHeightInMbs_, FrameHeightInMbs_ * 16);

    direct_8x8_inference_flag_ = rbsp_data_->read_u1();
    spdlog::info("direct_8x8_inference_flag {}", static_cast<bool>(direct_8x8_inference_flag_));

    frame_cropping_flag_ = rbsp_data_->read_u1();
    if (frame_cropping_flag_) {
        spdlog::warn("frame_cropping_flag is true");

        frame_crop_left_offset_ = rbsp_data_->read_ue();
        spdlog::trace("frame_crop_left_offset {}", frame_crop_left_offset_);

        frame_crop_right_offset_ = rbsp_data_->read_ue();
        spdlog::trace("frame_crop_right_offset {}", frame_crop_right_offset_);

        frame_crop_top_offset_ = rbsp_data_->read_ue();
        spdlog::trace("frame_crop_top_offset {}", frame_crop_top_offset_);

        frame_crop_bottom_offset_ = rbsp_data_->read_ue();
        spdlog::trace("frame_crop_bottom_offset {}", frame_crop_bottom_offset_);
    } else
        spdlog::warn("frame_cropping_flag is false");

    vui_parameters_present_flag_ = rbsp_data_->read_u1();

    if (vui_parameters_present_flag_)
        vui_.parse(rbsp_data_);

    if (!rbsp_data_->check_trailing_bits()) {
        spdlog::error("check_trailing_bits failed in read sps");
        return -1;
    }

    return 0;
}

void Sps::log()
{
    const char* space = "  ";
    std::cout << "SPS log:" << std::endl;
    std::cout << space << "profile_idc is " << profile_idc_ << std::endl;
    std::cout << space << "level_idc is " << level_idc_ << std::endl;
    std::cout << space << "seq_parameter_set_id is " << seq_parameter_set_id_ << std::endl;
    std::cout << space << "log2_max_frame_num_minus4 is " << log2_max_frame_num_minus4_ << std::endl;
    std::cout << space << space << "MaxFrameNum is " << MaxFrameNum_ << std::endl;
    std::cout << space << "pic_order_cnt_type is " << pic_order_cnt_type_ << std::endl;
    if (pic_order_cnt_type_ == 0) {
        std::cout << space << space << "log2_max_pic_order_cnt_lsb_minus4 is " << log2_max_pic_order_cnt_lsb_minus4_ << std::endl;
        std::cout << space << space << space << "MaxPicOrderCntLsb is " << MaxPicOrderCntLsb_ << std::endl;
    } else {
        std::cout << space << space << "delta_pic_order_always_zero_flag is " << delta_pic_order_always_zero_flag_ << std::endl;
        std::cout << space << space << "offset_for_non_ref_pic is " << offset_for_non_ref_pic_ << std::endl;
        std::cout << space << space << "offset_for_top_to_bottom_field is " << offset_for_top_to_bottom_field_ << std::endl;
        std::cout << space << space << "num_ref_frames_in_pic_order_cnt_cycle is " << num_ref_frames_in_pic_order_cnt_cycle_ << std::endl;
    }
    std::cout << space << "max_num_ref_frames is " << max_num_ref_frames_ << std::endl;
    std::cout << space << "gaps_in_frame_num_value_allowed_flag is " << gaps_in_frame_num_value_allowed_flag_ << std::endl;
    std::cout << space << space << "PicWidthInMbs is " << PicWidthInMbs_ << std::endl;
    std::cout << space << space << "PicWidthInSamplesL is " << PicWidthInSamplesL_ << std::endl;
    std::cout << space << "frame_mbs_only_flag is " << frame_mbs_only_flag_ << std::endl;
    if (!frame_mbs_only_flag_) {
        std::cout << space << space << "mb_adaptive_frame_field_flag is " << frame_mbs_only_flag_ << std::endl;
    }
    std::cout << space << "direct_8x8_inference_flag is " << direct_8x8_inference_flag_ << std::endl;
    std::cout << space << "frame_cropping_flag is " << frame_cropping_flag_ << std::endl;
    if (frame_cropping_flag_) {
        std::cout << space << space << "frame_crop_left_offset is " << frame_crop_left_offset_ << std::endl;
        std::cout << space << space << "frame_crop_right_offset is " << frame_crop_right_offset_ << std::endl;
        std::cout << space << space << "frame_crop_top_offset is " << frame_crop_top_offset_ << std::endl;
        std::cout << space << space << "frame_crop_bottom_offset is " << frame_crop_bottom_offset_ << std::endl;
    }
    std::cout << space << "vui_parameters_present_flag is " << vui_parameters_present_flag_ << std::endl;
}