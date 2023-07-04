
#pragma once

#include "NalUnit.h"

class Pps {
public:
    Pps() = default;
    Pps(std::shared_ptr<NalUnit::RbspData> rbsp);
    int parse();

    void log();
    int pps_id() { return pic_parameter_set_id_; }

private:
    std::shared_ptr<NalUnit::RbspData> rbsp_data;
    /* pps field */
    uint32_t pic_parameter_set_id_;
    uint32_t seq_parameter_set_id_;
    uint8_t entropy_coding_mode_flag_;
    uint8_t bottom_field_pic_order_in_frame_present_flag_;
    uint32_t num_slice_groups_minus1_;

    uint32_t num_ref_idx_l0_default_active_minus1_;
    uint32_t num_ref_idx_l1_default_active_minus1_;

    uint8_t weighted_pred_flag_;
    uint8_t weighted_bipred_idc_;
    int32_t pic_init_qp_minus26_;
    int32_t pic_init_qs_minus26_;
    int32_t chroma_qp_index_offset_;
    uint8_t deblocking_filter_control_present_flag_;
    uint8_t constrained_intra_pred_flag_;
    uint8_t redundant_pic_cnt_present_flag_;
    uint8_t transform_8x8_mode_flag_ = 0;
    uint8_t pic_scaling_matrix_present_flag_ = 0;
};