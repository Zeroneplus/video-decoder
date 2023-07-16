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

    int add_slice(std::shared_ptr<NalUnit::RbspData> rbsp);

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

    void reference_picture_lists_construction(std::shared_ptr<Slice> current_slice);

private:
    std::map<uint32_t, std::shared_ptr<Sps>> sps_map;
    std::map<uint32_t, std::shared_ptr<Pps>> pps_map;

    /* prevPicOrderCntMsb, prevPicOrderCntLsb */
    std::pair<int, int> prev_ref_pic_poc_ = { 0, 0 };
    /* prevFrameNum, prevFrameNumOffset */
    std::pair<int, int> prev_frame_num_ = { 0, 0 };

    std::vector<std::shared_ptr<Slice>> ref_slices_;
    std::vector<std::shared_ptr<Slice>> output_slices_;

    std::vector<std::tuple<int, std::shared_ptr<Slice>>>
    initialisation_process_for_the_reference_picture_list_for_P_and_SP_slices_in_frames(
        std::vector<std::tuple<int, std::shared_ptr<Slice>>>& ref_list_with_PicNum);

    std::pair<std::vector<std::tuple<int, std::shared_ptr<Slice>>>,
        std::vector<std::tuple<int, std::shared_ptr<Slice>>>>
    initialisation_process_for_reference_picture_lists_for_B_slices_in_frames(
        int current_poc,
        std::vector<std::tuple<int, std::shared_ptr<Slice>>>& ref_list_with_POC_or_LongTermPicNum);
};
