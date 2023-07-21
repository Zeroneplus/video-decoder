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

    std::pair<int, int>& prev_ref_pic_poc()
    {
        return prev_ref_pic_poc_;
    }

    std::pair<int, int>& prev_frame_num()
    {
        return prev_frame_num_;
    }

    void decoded_reference_picture_marking_process(std::shared_ptr<Slice> current_slice);

    std::vector<std::shared_ptr<Slice>>& ref_slices()
    {
        return ref_slices_;
    }

    void mark_short_term_as_unref_by_picNumX(int picNumX,
        const std::shared_ptr<Slice>& current_slice);

    void mark_long_term_as_unref_by_long_term_pic_num(int LongTermPicNum,
        const std::shared_ptr<Slice>& current_slice);

    void set_max_long_term_frame_idx(int max_long_term_frame_idx);

    void possiblely_mark_long_term_as_unref_by_long_term_pic_num(
        int LongTermPicNum,
        const std::shared_ptr<Slice>& current_slice);

    void mark_short_term_as_long_term(
        int picNumX,
        int long_term_frame_idx,
        const std::shared_ptr<Slice>& current_slice);

    void set_max_long_term_frame_idx_and_mark_long_term_as_unref(
        int max_long_term_frame_idx_plus1,
        const std::shared_ptr<Slice>& current_slice);

    void mark_all_unref_and_set_max_long_term_frame_idx_to_none();

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
        std::vector<std::tuple<int, std::shared_ptr<Slice>>>& ref_list_with_POC_or_LongTermPicNum,
        std::shared_ptr<Slice> current_slice);

    void sliding_window_decoded_reference_picture_marking_process(std::shared_ptr<Slice> current_slice);

    void purge_non_ref_slice();

    std::vector<std::tuple<int, std::shared_ptr<Slice>>>
    get_short_term_ref_list_with_PicNum(const std::shared_ptr<Slice>& current_slice);

    std::vector<std::tuple<int, std::shared_ptr<Slice>>>
    get_long_term_ref_list_with_PicNum(const std::shared_ptr<Slice>& current_slice);

    int max_long_term_frame_idx_ = -1;
};
