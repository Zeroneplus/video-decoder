#include "PPS.h"
#include "VideoDecoder.h"
#include "spdlog/spdlog.h"

Pps::Pps(std::shared_ptr<NalUnit::RbspData> rbsp, VideoDecoder* ptr)
    : rbsp_data_(std::move(rbsp))
    , video_dec_(ptr)
{
}

int Pps::parse()
{
    pic_parameter_set_id_ = rbsp_data_->read_ue();
    spdlog::info("pps id for current pps {}", pic_parameter_set_id_);

    seq_parameter_set_id_ = rbsp_data_->read_ue();
    spdlog::info("sps id for current pps {}", seq_parameter_set_id_);

    entropy_coding_mode_flag_ = rbsp_data_->read_u1();
    if (entropy_coding_mode_flag_) {
        spdlog::error("entropy_coding_mode_flag is true, unsupported");
        return -1;
    } else
        spdlog::warn("entropy_coding_mode_flag is false, OK");

    bottom_field_pic_order_in_frame_present_flag_ = rbsp_data_->read_u1();

    spdlog::warn("bottom_field_pic_order_in_frame_present_flag {}, this may affect slice header",
        static_cast<bool>(bottom_field_pic_order_in_frame_present_flag_));

    num_slice_groups_minus1_ = rbsp_data_->read_ue();

    if (num_slice_groups_minus1_ > 0) {
        spdlog::error("num_slice_groups_minus1 > 0, unsupported");
        return -1;
    } else
        spdlog::warn("num_slice_groups_minus1 = 0, OK");

    num_ref_idx_l0_default_active_minus1_ = rbsp_data_->read_ue();
    num_ref_idx_l1_default_active_minus1_ = rbsp_data_->read_ue();
    spdlog::warn("num_ref_idx_l0_default_active_minus1 {}, num_ref_idx_l1_default_active_minus1 {}",
        num_ref_idx_l0_default_active_minus1_, num_ref_idx_l1_default_active_minus1_);

    weighted_pred_flag_ = rbsp_data_->read_u1();
    weighted_bipred_idc_ = rbsp_data_->read_u(2);
    spdlog::info("weighted_pred_flag {}, weighted_bipred_idc {}",
        static_cast<bool>(weighted_pred_flag_), weighted_bipred_idc_);

    pic_init_qp_minus26_ = rbsp_data_->read_se();
    pic_init_qs_minus26_ = rbsp_data_->read_se();
    spdlog::info("pic_init_qp_minus26 {}, pic_init_qs_minus26 {}", pic_init_qp_minus26_, pic_init_qs_minus26_);

    chroma_qp_index_offset_ = rbsp_data_->read_se();
    spdlog::info("chroma_qp_index_offset {}", chroma_qp_index_offset_);

    deblocking_filter_control_present_flag_ = rbsp_data_->read_u1();
    spdlog::warn("deblocking_filter_control_present_flag {}, this may affect slice header",
        static_cast<bool>(deblocking_filter_control_present_flag_));

    constrained_intra_pred_flag_ = rbsp_data_->read_u1();
    spdlog::warn("constrained_intra_pred_flag {}", static_cast<bool>(constrained_intra_pred_flag_));

    redundant_pic_cnt_present_flag_ = rbsp_data_->read_u1();
    if (redundant_pic_cnt_present_flag_) {
        spdlog::error("redundant_pic_cnt_present_flag is true, unsupported");
        return -1;
    } else
        spdlog::warn("redundant_pic_cnt_present_flag is false, OK");

    // When second_chroma_qp_index_offset is not present,
    // it shall be inferred to be equal to chroma_qp_index_offset.
    second_chroma_qp_index_offset_ = chroma_qp_index_offset_;
    if (rbsp_data_->more_rbsp_data()) {
        spdlog::trace("will read transform_8x8_mode_flag");
        transform_8x8_mode_flag_ = rbsp_data_->read_u1();
        pic_scaling_matrix_present_flag_ = rbsp_data_->read_u1();
        if (pic_scaling_matrix_present_flag_) {
            spdlog::error("pic_scaling_matrix_present_flag is true, unsupported");
            return -1;
        }
        second_chroma_qp_index_offset_ = rbsp_data_->read_se();
        spdlog::trace("second_chroma_qp_index_offset {}", second_chroma_qp_index_offset_);
    }

    spdlog::info("transform_8x8_mode_flag {}", static_cast<bool>(transform_8x8_mode_flag_));

    if (!rbsp_data_->check_trailing_bits()) {
        spdlog::error("check_trailing_bits failed in read pps");
        return -1;
    }

    return 0;
}

void Pps::log()
{
    const char* space = "  ";
    std::cout << "PPS log:" << std::endl;

    std::cout << space << "pic_parameter_set_id is " << pic_parameter_set_id_ << std::endl;
    std::cout << space << "seq_parameter_set_id is " << seq_parameter_set_id_ << std::endl;
    std::cout << space << "entropy_coding_mode_flag is " << entropy_coding_mode_flag_ << std::endl;
    std::cout << space << "bottom_field_pic_order_in_frame_present_flag is " << bottom_field_pic_order_in_frame_present_flag_ << std::endl;
    std::cout << space << "num_slice_groups_minus1 is " << num_slice_groups_minus1_ << std::endl;
    std::cout << space << "num_ref_idx_l0_default_active_minus1 is " << num_ref_idx_l0_default_active_minus1_ << std::endl;
    std::cout << space << "num_ref_idx_l1_default_active_minus1 is " << num_ref_idx_l1_default_active_minus1_ << std::endl;
    std::cout << space << "weighted_pred_flag is " << weighted_pred_flag_ << std::endl;
    std::cout << space << "weighted_bipred_idc is " << weighted_bipred_idc_ << std::endl;
    std::cout << space << "pic_init_qp_minus26 is " << pic_init_qp_minus26_ << std::endl;
    std::cout << space << "pic_init_qs_minus26 is " << pic_init_qs_minus26_ << std::endl;
    std::cout << space << "chroma_qp_index_offset is " << chroma_qp_index_offset_ << std::endl;
    std::cout << space << "deblocking_filter_control_present_flag is " << deblocking_filter_control_present_flag_ << std::endl;
    std::cout << space << "constrained_intra_pred_flag is " << constrained_intra_pred_flag_ << std::endl;
    std::cout << space << "redundant_pic_cnt_present_flag is " << redundant_pic_cnt_present_flag_ << std::endl;
    std::cout << space << "transform_8x8_mode_flag is " << transform_8x8_mode_flag_ << std::endl;
    std::cout << space << "pic_scaling_matrix_present_flag is " << pic_scaling_matrix_present_flag_ << std::endl;
}

std::vector<int>& Pps::pic_scaling_list(int idx)
{
    // now only consider pic_scaling_matrix_present_flag=false
    return video_dec_->get_sps_by_id(seq_parameter_set_id_)->seq_scaling_list(idx);
}
