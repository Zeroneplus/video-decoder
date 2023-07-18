
#include <algorithm>
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

    // cal poc for current slice
    slice->cal_poc(this);

    // construct ref pic list
    // only for P or B slice
    if (slice->is_B_slice()
        || slice->is_SP_slice()
        || slice->is_P_slice())
        reference_picture_lists_construction(slice);

    // parse slice data

    // dec ref pic marking
    if (slice->is_reference_slice())
        decoded_reference_picture_marking_process(slice);

    // update poc for mm5

    // update prev_frame_num_ or prev_ref_pic_poc_

    return ret;
}

std::vector<std::tuple<int, std::shared_ptr<Slice>>>
VideoDecoder::initialisation_process_for_the_reference_picture_list_for_P_and_SP_slices_in_frames(
    std::vector<std::tuple<int, std::shared_ptr<Slice>>>& ref_list_with_PicNum)
{
    std::vector<std::tuple<int, std::shared_ptr<Slice>>> ref_list_with_PicNum_short_term,
        ref_list_with_PicNum_long_term,
        ret;

    for (auto& picnum_with_slice : ref_list_with_PicNum) {
        auto& [pic_num, ref_slice] = picnum_with_slice;
        if (ref_slice->at_least_one_short_term_ref()) {
            ref_list_with_PicNum_short_term.push_back(picnum_with_slice);
        } else {
            ref_list_with_PicNum_long_term.push_back(picnum_with_slice);
        }
    }

    std::sort(ref_list_with_PicNum_short_term.begin(),
        ref_list_with_PicNum_short_term.end(),
        [](const std::tuple<int, std::shared_ptr<Slice>>& a, const std::tuple<int, std::shared_ptr<Slice>>& b) {
            return std::get<0>(a) > std::get<0>(b);
        });

    std::sort(ref_list_with_PicNum_long_term.begin(),
        ref_list_with_PicNum_long_term.end(),
        [](const std::tuple<int, std::shared_ptr<Slice>>& a, const std::tuple<int, std::shared_ptr<Slice>>& b) {
            return std::get<0>(a) < std::get<0>(b);
        });

    ret.insert(ret.end(), ref_list_with_PicNum_short_term.begin(), ref_list_with_PicNum_short_term.end());

    ret.insert(ret.end(), ref_list_with_PicNum_long_term.begin(), ref_list_with_PicNum_long_term.end());

    return ret;
}

std::pair<
    std::vector<std::tuple<int, std::shared_ptr<Slice>>>,
    std::vector<std::tuple<int, std::shared_ptr<Slice>>>>
VideoDecoder::initialisation_process_for_reference_picture_lists_for_B_slices_in_frames(
    int current_poc,
    std::vector<std::tuple<int, std::shared_ptr<Slice>>>& ref_list_with_POC_or_LongTermPicNum,
    std::shared_ptr<Slice> current_slice)
{
    std::vector<std::tuple<int, std::shared_ptr<Slice>>> ref_list_with_POC_short_term,
        ref_list_with_PicNum_long_term,
        list_0, list_1,
        poc_less_than_current_poc,
        poc_greater_than_current_poc;

    // split ref slice to long term and short term
    for (auto& poc_with_slice : ref_list_with_POC_or_LongTermPicNum) {
        auto& [poc_or_long_term_pic_num, ref_slice] = poc_with_slice;
        if (ref_slice->at_least_one_short_term_ref()) {
            ref_list_with_POC_short_term.push_back(poc_with_slice);
        } else {
            ref_list_with_PicNum_long_term.push_back(poc_with_slice);
        }
    }

    // split short term into two parts
    for (auto& poc_with_slice : ref_list_with_POC_short_term) {
        auto& [poc, ref_slice] = poc_with_slice;
        if (poc < current_poc)
            poc_less_than_current_poc.push_back(poc_with_slice);
        else {
            poc_greater_than_current_poc.push_back(poc_with_slice);
        }
    }

    // sort short term ref slice less than current poc in descending order
    std::sort(poc_less_than_current_poc.begin(),
        poc_less_than_current_poc.end(),
        [](const std::tuple<int, std::shared_ptr<Slice>>& a, const std::tuple<int, std::shared_ptr<Slice>>& b) {
            return std::get<0>(a) > std::get<0>(b);
        });

    // sort short term ref slice greater than current poc in ascending order
    std::sort(poc_greater_than_current_poc.begin(),
        poc_greater_than_current_poc.end(),
        [](const std::tuple<int, std::shared_ptr<Slice>>& a, const std::tuple<int, std::shared_ptr<Slice>>& b) {
            return std::get<0>(a) < std::get<0>(b);
        });

    // sort long term ref slice with ascending order
    std::sort(ref_list_with_PicNum_long_term.begin(),
        ref_list_with_PicNum_long_term.end(),
        [](const std::tuple<int, std::shared_ptr<Slice>>& a, const std::tuple<int, std::shared_ptr<Slice>>& b) {
            return std::get<0>(a) < std::get<0>(b);
        });

    // construct list 0
    list_0.insert(list_0.end(), poc_less_than_current_poc.begin(), poc_less_than_current_poc.end());

    list_0.insert(list_0.end(), poc_greater_than_current_poc.begin(), poc_greater_than_current_poc.end());

    list_0.insert(list_0.end(), ref_list_with_PicNum_long_term.begin(), ref_list_with_PicNum_long_term.end());

    // construct list 1
    list_1.insert(list_1.end(), poc_greater_than_current_poc.begin(), poc_greater_than_current_poc.end());

    list_1.insert(list_1.end(), poc_less_than_current_poc.begin(), poc_less_than_current_poc.end());

    list_1.insert(list_1.end(), ref_list_with_PicNum_long_term.begin(), ref_list_with_PicNum_long_term.end());

    /*
     * When the reference picture list RefPicList1 has more than
     * one entry and RefPicList1 is identical to the reference
       picture list RefPicList0, the first two entries RefPicList1[ 0 ]
       and RefPicList1[ 1 ] are switched.
     */
    // TODO

    // replace poc with pic num
    for (auto& [poc, ref_slice] : list_0) {
        poc = ref_slice->decoding_process_for_picture_numbers(current_slice);
    }

    for (auto& [poc, ref_slice] : list_1) {
        poc = ref_slice->decoding_process_for_picture_numbers(current_slice);
    }

    return { list_0, list_1 };
}

void VideoDecoder::reference_picture_lists_construction(std::shared_ptr<Slice> current_slice)
{
    if (current_slice->is_P_slice() || current_slice->is_SP_slice()) {

        // Initialisation process for the reference picture list for P and SP slices in frames
        std::vector<std::tuple<int, std::shared_ptr<Slice>>> ref_list_with_PicNum;
        for (auto& ref_slice : ref_slices_) {
            ref_list_with_PicNum.emplace_back(
                ref_slice->decoding_process_for_picture_numbers(current_slice),
                ref_slice);
        }
        auto ref_list_P_0 = initialisation_process_for_the_reference_picture_list_for_P_and_SP_slices_in_frames(ref_list_with_PicNum);

        current_slice->set_ref_list_P(std::move(ref_list_P_0));
    } else if (current_slice->is_B_slice()) {

        // initialisation process for reference picture lists for B slices in frames
        std::vector<std::tuple<int, std::shared_ptr<Slice>>> ref_list_with_POC_or_LongTermPicNum;
        int current_poc = current_slice->PicOrderCnt();
        for (auto& ref_slice : ref_slices_) {
            ref_list_with_POC_or_LongTermPicNum.emplace_back(ref_slice->PicOrderCntOrLongTermPicNum(), ref_slice);
        }
        auto ref_list_B_pair = initialisation_process_for_reference_picture_lists_for_B_slices_in_frames(
            current_poc,
            ref_list_with_POC_or_LongTermPicNum,
            current_slice);

        current_slice->set_ref_list_B(std::move(ref_list_B_pair));
    } else {
        spdlog::error("non SP/P/B slice call reference_picture_lists_construction");
        assert(false);
    }

    current_slice->modification_process_for_reference_picture_lists();
}

void VideoDecoder::decoded_reference_picture_marking_process(std::shared_ptr<Slice> current_slice)
{
}
