
#pragma once

#include "NalUnit.h"

class Pps {
public:
    Pps() = default;
    Pps(std::shared_ptr<NalUnit::RbspData> rbsp);
    int parse();

    void log();

    int pps_id() { return pic_parameter_set_id_; }

    int sps_id() { return seq_parameter_set_id_; }

    bool bottom_field_pic_order_in_frame_present()
    {
        return bottom_field_pic_order_in_frame_present_flag_;
    }

    bool redundant_pic_cnt_present()
    {
        return redundant_pic_cnt_present_flag_;
    }

    bool weighted_pred()
    {
        return weighted_pred_flag_;
    }

    bool weighted_bipred()
    {
        return weighted_bipred_idc_;
    }

    bool entropy_coding_mode()
    {
        return entropy_coding_mode_flag_;
    }

    bool deblocking_filter_control_present()
    {
        return deblocking_filter_control_present_flag_;
    }

    int num_slice_groups_minus1()
    {
        return num_slice_groups_minus1_;
    }

    int num_ref_idx_l0_default_active_minus1()
    {
        return num_ref_idx_l0_default_active_minus1_;
    }
    int num_ref_idx_l1_default_active_minus1()
    {
        return num_ref_idx_l1_default_active_minus1_;
    }

    int pic_init_qp_minus26()
    {
        return pic_init_qp_minus26_;
    }

    int pic_init_qs_minus26()
    {
        return pic_init_qs_minus26_;
    }

private:
    std::shared_ptr<NalUnit::RbspData> rbsp_data_;
    /* pps field */
    int pic_parameter_set_id_;
    int seq_parameter_set_id_;
    int entropy_coding_mode_flag_;
    int bottom_field_pic_order_in_frame_present_flag_;
    int num_slice_groups_minus1_;

    int num_ref_idx_l0_default_active_minus1_;
    int num_ref_idx_l1_default_active_minus1_;

    int weighted_pred_flag_;
    int weighted_bipred_idc_;
    int pic_init_qp_minus26_;
    int pic_init_qs_minus26_;
    int chroma_qp_index_offset_;
    int deblocking_filter_control_present_flag_;
    int constrained_intra_pred_flag_;
    int redundant_pic_cnt_present_flag_;
    int transform_8x8_mode_flag_ = 0;
    int pic_scaling_matrix_present_flag_ = 0;
    int second_chroma_qp_index_offset_ = 0;
};