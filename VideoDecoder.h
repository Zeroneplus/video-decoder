#pragma once

#include <map>
#include <memory>

#include "FileStream.h"
#include "PPS.h"
#include "SPS.h"

class VideoDecoder {
public:
    VideoDecoder() = default;

    int add_sps(std::shared_ptr<NalUnit::RbspData> rbsp);

private:
    std::map<int, std::shared_ptr<Sps>> sps_map;
    std::map<int, std::shared_ptr<Pps>> pps_map;
};