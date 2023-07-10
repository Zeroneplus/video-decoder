#pragma once

#include <map>
#include <memory>

#include "FileStream.h"
#include "PPS.h"
#include "SPS.h"
#include "Slice.h"

class VideoDecoder {
public:
    VideoDecoder() = default;

    int add_sps(std::shared_ptr<NalUnit::RbspData> rbsp);
    int add_pps(std::shared_ptr<NalUnit::RbspData> rbsp);

    std::shared_ptr<Pps> get_pps_by_id(uint32_t id)
    {
        auto it = pps_map.find(id);
        if (it != pps_map.end()) {
            return it->second;
        }
        return nullptr;
    }
    std::shared_ptr<Sps> get_sps_by_id(uint32_t id)
    {
        auto it = sps_map.find(id);
        if (it != sps_map.end()) {
            return it->second;
        }
        return nullptr;
    }

    template <typename T>
    int add_xps(std::shared_ptr<NalUnit::RbspData> rbsp);

private:
    std::map<uint32_t, std::shared_ptr<Sps>> sps_map;
    std::map<uint32_t, std::shared_ptr<Pps>> pps_map;
};
