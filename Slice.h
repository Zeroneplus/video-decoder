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
};