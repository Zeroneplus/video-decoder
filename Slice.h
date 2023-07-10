#pragma once

#include "NalUnit.h"

class VideoDecoder;

class Slice {
public:
    Slice() = default;
    Slice(std::shared_ptr<NalUnit::RbspData> rbsp);
    int parse_slice_header(VideoDecoder* decoder);

    void log();

    int parse_ref_pic_list_modification();
    int parse_pred_weight_table();
    int parse_dec_ref_pic_marking(bool IdrPic);

    enum SliceType {
        P = 0,
        B,
        I,
        SP,
        SI,
        P_hight,
        B_hight,
        I_hight,
        SP_hight,
        SI_hight
    };

private:
    std::shared_ptr<NalUnit::RbspData> rbsp_data;
    /* Slice header fields */
    uint32_t first_mb_in_slice_;
    uint32_t slice_type_;
    uint32_t pic_parameter_set_id_;

    uint8_t colour_plane_id_ = 0;
    uint32_t frame_num_;

    uint8_t field_pic_flag_ = 0;
    uint8_t bottom_field_flag_ = 0;
    uint32_t idr_pic_id_ = 0;
    uint32_t pic_order_cnt_lsb_ = 0;
    int32_t delta_pic_order_cnt_bottom_ = 0;
    int32_t delta_pic_order_cnt_0_ = 0;
    int32_t delta_pic_order_cnt_1_ = 0;
    uint32_t redundant_pic_cnt_ = 0;
    uint8_t direct_spatial_mv_pred_flag_ = 0;
    uint8_t num_ref_idx_active_override_flag_ = 0;
    uint32_t num_ref_idx_l0_active_minus1_ = 0;
    uint32_t num_ref_idx_l1_active_minus1_ = 0;
    uint32_t cabac_init_idc_ = 0;
    int slice_qp_delta_;
    uint8_t sp_for_switch_flag_ = 0;
    int slice_qs_delta_ = 0;
    uint32_t disable_deblocking_filter_idc_;
    int slice_alpha_c0_offset_div2_;
    int slice_beta_offset_div2_;

    uint8_t ref_pic_list_modification_flag_l0_ = 0;
    uint8_t ref_pic_list_modification_flag_l1_ = 0;

    std::vector<std::tuple<uint32_t, uint32_t>> ref_pic_list_modification_l0_;
    std::vector<std::tuple<uint32_t, uint32_t>> ref_pic_list_modification_l1_;

    uint8_t no_output_of_prior_pics_flag_,
        long_term_reference_flag_,
        adaptive_ref_pic_marking_mode_flag_;
    std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> memory_management_control_operation_list_;

    int parse_single_ref_pic_list_modification(int idx);

public:
    enum SliceType slice_type;
};
