#pragma once

#include "NalUnit.h"

class Slice {
public:
    Slice() = default;
    Slice(std::shared_ptr<NalUnit::RbspData> rbsp);
    int parse_slice_header();

    void log();

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
    int32_t idr_pic_id_ = 0;
    uint32_t pic_order_cnt_lsb_ = 0;
    int32_t delta_pic_order_cnt_bottom_ = 0;
    int32_t delta_pic_order_cnt_0_ = 0;
    int32_t delta_pic_order_cnt_1_ = 0;
    uint32_t redundant_pic_cnt_ = 0;
};