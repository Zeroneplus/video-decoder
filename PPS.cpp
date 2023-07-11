#include "PPS.h"

Pps::Pps(std::shared_ptr<NalUnit::RbspData> rbsp)
    : rbsp_data(std::move(rbsp))
{
}

int Pps::parse()
{
    pic_parameter_set_id_ = rbsp_data->read_ue();
    seq_parameter_set_id_ = rbsp_data->read_ue();
    entropy_coding_mode_flag_ = rbsp_data->read_u1();
    bottom_field_pic_order_in_frame_present_flag_ = rbsp_data->read_u1();

    if (!bottom_field_pic_order_in_frame_present_flag_)
        std::cout << "warn: bottom_field_pic_order_in_frame_present_flag is false" << std::endl;

    num_slice_groups_minus1_ = rbsp_data->read_ue();

    if (num_slice_groups_minus1_ > 0) {
        std::cout << "num_slice_groups_minus1 > 0" << std::endl;
        return -1;
    }

    num_ref_idx_l0_default_active_minus1_ = rbsp_data->read_ue();
    num_ref_idx_l1_default_active_minus1_ = rbsp_data->read_ue();

    weighted_pred_flag_ = rbsp_data->read_u1();
    weighted_bipred_idc_ = rbsp_data->read_u(2);

    pic_init_qp_minus26_ = rbsp_data->read_se();
    pic_init_qs_minus26_ = rbsp_data->read_se();

    chroma_qp_index_offset_ = rbsp_data->read_se();

    deblocking_filter_control_present_flag_ = rbsp_data->read_u1();

    constrained_intra_pred_flag_ = rbsp_data->read_u1();

    redundant_pic_cnt_present_flag_ = rbsp_data->read_u1();

    if (!rbsp_data->eof()) {
        // std::cout << "  has transform_8x8_mode_flag" << std::endl;
        transform_8x8_mode_flag_ = rbsp_data->read_u1();
        pic_scaling_matrix_present_flag_ = rbsp_data->read_u1();
        if (pic_scaling_matrix_present_flag_) {
            std::cout << "pic_scaling_matrix_present_flag_ is 1" << std::endl;
            return -1;
        }
    }

    return 0;
}

void Pps::log()
{
    const char* space = "  ";
    std::cout << "PPS log:" << std::endl;

    std::cout << space << "pic_parameter_set_id is " << pic_parameter_set_id_ << std::endl;
    std::cout << space << "seq_parameter_set_id is " << seq_parameter_set_id_ << std::endl;
    std::cout << space << "entropy_coding_mode_flag is " << (int32_t)entropy_coding_mode_flag_ << std::endl;
    std::cout << space << "bottom_field_pic_order_in_frame_present_flag is " << (int32_t)bottom_field_pic_order_in_frame_present_flag_ << std::endl;
    std::cout << space << "num_slice_groups_minus1 is " << num_slice_groups_minus1_ << std::endl;
    std::cout << space << "num_ref_idx_l0_default_active_minus1 is " << num_ref_idx_l0_default_active_minus1_ << std::endl;
    std::cout << space << "num_ref_idx_l1_default_active_minus1 is " << num_ref_idx_l1_default_active_minus1_ << std::endl;
    std::cout << space << "weighted_pred_flag is " << (int32_t)weighted_pred_flag_ << std::endl;
    std::cout << space << "weighted_bipred_idc is " << weighted_bipred_idc_ << std::endl;
    std::cout << space << "pic_init_qp_minus26 is " << pic_init_qp_minus26_ << std::endl;
    std::cout << space << "pic_init_qs_minus26 is " << pic_init_qs_minus26_ << std::endl;
    std::cout << space << "chroma_qp_index_offset is " << chroma_qp_index_offset_ << std::endl;
    std::cout << space << "deblocking_filter_control_present_flag is " << (int32_t)deblocking_filter_control_present_flag_ << std::endl;
    std::cout << space << "constrained_intra_pred_flag is " << (int32_t)constrained_intra_pred_flag_ << std::endl;
    std::cout << space << "redundant_pic_cnt_present_flag is " << (int32_t)redundant_pic_cnt_present_flag_ << std::endl;
    std::cout << space << "transform_8x8_mode_flag is " << (int32_t)transform_8x8_mode_flag_ << std::endl;
    std::cout << space << "pic_scaling_matrix_present_flag is " << (int32_t)pic_scaling_matrix_present_flag_ << std::endl;
}