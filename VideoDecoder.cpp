
#include <iostream>
#include <utility>

#include "spdlog/spdlog.h"

#include "VideoDecoder.h"

int VideoDecoder::add_sps(std::shared_ptr<NalUnit::RbspData> rbsp)
{
    std::shared_ptr<Sps> sps = std::make_shared<Sps>(std::move(rbsp));
    int ret = sps->parse();
    int sps_id = sps->sps_id();
    if (!ret) {
        if (sps_map.find(sps_id) != sps_map.end()) {
            spdlog::warn("will replace an exsiting sps with new sps");
            sps_map.erase(sps_id);
        }

        sps_map.insert(std::make_pair(sps_id, sps));
    }
    return ret;
}

int VideoDecoder::add_pps(std::shared_ptr<NalUnit::RbspData> rbsp)
{
    std::shared_ptr<Pps> pps = std::make_shared<Pps>(std::move(rbsp));
    int ret = pps->parse();
    int pps_id = pps->pps_id();
    if (!ret) {
        if (pps_map.find(pps_id) != pps_map.end()) {
            spdlog::warn("will replace an exsiting pps with new pps");
            pps_map.erase(pps_id);
        }
        pps_map.insert(std::make_pair(pps_id, pps));
    }
    return ret;
}

int VideoDecoder::add_slice(std::shared_ptr<NalUnit::RbspData> rbsp)
{
    std::shared_ptr<Slice> slice = std::make_shared<Slice>(std::move(rbsp));
    int ret = slice->parse_slice_header(this);

    return ret;
}