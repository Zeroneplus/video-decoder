
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
    slice->parse_slice_data(this);

    // dec ref pic marking
    if (slice->is_reference_slice())
        decoded_reference_picture_marking_process(slice);

    // update poc for mm5
    slice->update_poc_when_has_mm_op_5();

    // update prev_frame_num_ or prev_ref_pic_poc_
    slice->update_prev_frame_num_or_prev_ref_pic_poc(this);

    // output yuv data for test

    return ret;
}

std::vector<std::tuple<int, std::shared_ptr<Slice>>>
VideoDecoder::initialisation_process_for_the_reference_picture_list_for_P_and_SP_slices_in_frames(
    std::vector<std::tuple<int, std::shared_ptr<Slice>>>& ref_list_with_PicNum)
{
    std::vector<std::tuple<int, std::shared_ptr<Slice>>> ref_list_with_ShortTermPicNum,
        ref_list_with_LongTermPicNum,
        ret;

    for (auto& picnum_with_slice : ref_list_with_PicNum) {
        auto& [pic_num, ref_slice] = picnum_with_slice;
        if (ref_slice->at_least_one_short_term_ref()) {
            ref_list_with_ShortTermPicNum.push_back(picnum_with_slice);
        } else {
            ref_list_with_LongTermPicNum.push_back(picnum_with_slice);
        }
    }

    // will we have negative ShortTermPicNum in P slice?
    std::sort(ref_list_with_ShortTermPicNum.begin(),
        ref_list_with_ShortTermPicNum.end(),
        [](const std::tuple<int, std::shared_ptr<Slice>>& a, const std::tuple<int, std::shared_ptr<Slice>>& b) {
            return std::get<0>(a) > std::get<0>(b);
        });

    std::sort(ref_list_with_LongTermPicNum.begin(),
        ref_list_with_LongTermPicNum.end(),
        [](const std::tuple<int, std::shared_ptr<Slice>>& a, const std::tuple<int, std::shared_ptr<Slice>>& b) {
            return std::get<0>(a) < std::get<0>(b);
        });

    ret.insert(ret.end(), ref_list_with_ShortTermPicNum.begin(), ref_list_with_ShortTermPicNum.end());

    ret.insert(ret.end(), ref_list_with_LongTermPicNum.begin(), ref_list_with_LongTermPicNum.end());

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
    std::vector<std::tuple<int, std::shared_ptr<Slice>>> ref_list_with_ShortTermPOC,
        ref_list_with_LongTermPicNum,
        list_0, list_1,
        poc_less_than_current_poc,
        poc_greater_than_current_poc;

    // split ref slice to long term and short term
    for (auto& poc_with_slice : ref_list_with_POC_or_LongTermPicNum) {
        auto& [poc_or_long_term_pic_num, ref_slice] = poc_with_slice;
        if (ref_slice->at_least_one_short_term_ref()) {
            ref_list_with_ShortTermPOC.push_back(poc_with_slice);
        } else {
            ref_list_with_LongTermPicNum.push_back(poc_with_slice);
        }
    }

    // split short term into two parts
    for (auto& poc_with_slice : ref_list_with_ShortTermPOC) {
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
    std::sort(ref_list_with_LongTermPicNum.begin(),
        ref_list_with_LongTermPicNum.end(),
        [](const std::tuple<int, std::shared_ptr<Slice>>& a, const std::tuple<int, std::shared_ptr<Slice>>& b) {
            return std::get<0>(a) < std::get<0>(b);
        });

    // construct list 0
    list_0.insert(list_0.end(), poc_less_than_current_poc.begin(), poc_less_than_current_poc.end());

    list_0.insert(list_0.end(), poc_greater_than_current_poc.begin(), poc_greater_than_current_poc.end());

    list_0.insert(list_0.end(), ref_list_with_LongTermPicNum.begin(), ref_list_with_LongTermPicNum.end());

    // construct list 1
    list_1.insert(list_1.end(), poc_greater_than_current_poc.begin(), poc_greater_than_current_poc.end());

    list_1.insert(list_1.end(), poc_less_than_current_poc.begin(), poc_less_than_current_poc.end());

    list_1.insert(list_1.end(), ref_list_with_LongTermPicNum.begin(), ref_list_with_LongTermPicNum.end());

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
    if (current_slice->is_idr_pic()) {
        // mark all reference picture as non-ref
        for (auto& slice : ref_slices_) {
            slice->mark_as_non_ref(MarkScope::All);
            // push all to output
            output_slices_.push_back(std::move(slice));
        }
        ref_slices_.clear();

        if (current_slice->long_term_reference_flag()) {
            current_slice->mark_as_long_or_short_term_ref(MarkScope::All, true /* long_term */);
            current_slice->set_long_term_frame_idx(0);

            set_max_long_term_frame_idx(0);
        } else {
            current_slice->mark_as_long_or_short_term_ref(MarkScope::All, false /* long_term */);

            set_max_long_term_frame_idx(-1 /* "no long-term frame indices" */);
        }

    } else {
        if (current_slice->adaptive_ref_pic_marking_mode_flag()) {
            current_slice->adaptive_memory_control_decoded_reference_picture_marking_process(this);

        } else {
            // Sliding window decoded reference picture marking process
            sliding_window_decoded_reference_picture_marking_process(current_slice);
        }

        if (!current_slice->at_least_one_long_term_ref()) {
            // mark as short term
            current_slice->mark_as_long_or_short_term_ref(MarkScope::All, false /* long_term */);
        }
    }

    // add to ref_slices_
    ref_slices_.push_back(current_slice);

    // check ref slice size
}

std::vector<std::tuple<int, std::shared_ptr<Slice>>>
VideoDecoder::get_short_term_ref_list_with_PicNum(const std::shared_ptr<Slice>& current_slice)
{

    std::vector<std::tuple<int, std::shared_ptr<Slice>>> short_term_ref_list_with_PicNum;
    for (auto& ref_slice : ref_slices_) {
        if (ref_slice->at_least_one_short_term_ref()) {
            short_term_ref_list_with_PicNum.emplace_back(
                ref_slice->decoding_process_for_picture_numbers(current_slice), ref_slice);
        }
    }

    return short_term_ref_list_with_PicNum;
}

std::vector<std::tuple<int, std::shared_ptr<Slice>>>
VideoDecoder::get_long_term_ref_list_with_PicNum(const std::shared_ptr<Slice>& current_slice)
{

    std::vector<std::tuple<int, std::shared_ptr<Slice>>> long_term_ref_list_with_PicNum;
    for (auto& ref_slice : ref_slices_) {
        if (ref_slice->at_least_one_long_term_ref()) {
            long_term_ref_list_with_PicNum.emplace_back(
                ref_slice->decoding_process_for_picture_numbers(current_slice), ref_slice);
        }
    }

    return long_term_ref_list_with_PicNum;
}

// Marking process of a short-term reference picture as "unused for reference"
void VideoDecoder::mark_short_term_as_unref_by_picNumX(int picNumX, const std::shared_ptr<Slice>& current_slice)
{
    for (auto it = ref_slices_.begin(); it != ref_slices_.end(); it++) {
        if ((*it)->at_least_one_short_term_ref() && (*it)->decoding_process_for_picture_numbers(current_slice) == picNumX) {
            (*it)->mark_as_non_ref(MarkScope::All);

            output_slices_.push_back(std::move(*it));
            ref_slices_.erase(it);
            return;
        }
    }
    assert(false);
}

void VideoDecoder::possiblely_mark_long_term_as_unref_by_long_term_pic_num(int LongTermPicNum, const std::shared_ptr<Slice>& current_slice)
{
    for (auto it = ref_slices_.begin(); it != ref_slices_.end(); it++) {
        if ((*it)->at_least_one_long_term_ref() && (*it)->decoding_process_for_picture_numbers(current_slice) == LongTermPicNum) {
            (*it)->mark_as_non_ref(MarkScope::All);

            output_slices_.push_back(std::move(*it));
            ref_slices_.erase(it);
            return;
        }
    }
}

// Marking process of a long-term reference picture as "unused for reference"
void VideoDecoder::mark_long_term_as_unref_by_long_term_pic_num(int LongTermPicNum, const std::shared_ptr<Slice>& current_slice)
{
    for (auto it = ref_slices_.begin(); it != ref_slices_.end(); it++) {
        if ((*it)->at_least_one_long_term_ref() && (*it)->decoding_process_for_picture_numbers(current_slice) == LongTermPicNum) {
            (*it)->mark_as_non_ref(MarkScope::All);

            output_slices_.push_back(std::move(*it));
            ref_slices_.erase(it);
            return;
        }
    }
    assert(false);
}

void VideoDecoder::mark_short_term_as_long_term(int picNumX, int long_term_frame_idx, const std::shared_ptr<Slice>& current_slice)
{
    // if long_term_frame_idx already exist, mark as unref
    possiblely_mark_long_term_as_unref_by_long_term_pic_num(long_term_frame_idx, current_slice);

    // mark short term as long term
    for (auto it = ref_slices_.begin(); it != ref_slices_.end(); it++) {
        if ((*it)->at_least_one_short_term_ref() && (*it)->decoding_process_for_picture_numbers(current_slice) == picNumX) {

            (*it)->mark_as_long_or_short_term_ref(MarkScope::All, true /* bool long_term */);

            (*it)->set_long_term_frame_idx(long_term_frame_idx);
            // TODO check max long_term_frame_idx

            return;
        }
    }
    assert(false);
}

void VideoDecoder::set_max_long_term_frame_idx(int max_long_term_frame_idx)
{
    max_long_term_frame_idx_ = max_long_term_frame_idx;
}

void VideoDecoder::set_max_long_term_frame_idx_and_mark_long_term_as_unref(
    int max_long_term_frame_idx_plus1,
    const std::shared_ptr<Slice>& current_slice)
{
    int max_long_term_frame_idx = max_long_term_frame_idx_plus1 - 1;

    // set max_long_term_frame_idx
    // no need to distinguish "no long-term frame indices"
    set_max_long_term_frame_idx(max_long_term_frame_idx);

    auto it = ref_slices_.begin();
    while (it != ref_slices_.end()) {
        if ((*it)->at_least_one_long_term_ref()
            && (*it)->long_term_frame_idx() > max_long_term_frame_idx) {
            (*it)->mark_as_non_ref(MarkScope::All);

            output_slices_.push_back(std::move(*it));
            it = ref_slices_.erase(it);
        } else
            it++;
    }
}

void VideoDecoder::mark_all_unref_and_set_max_long_term_frame_idx_to_none()
{

    auto it = ref_slices_.begin();
    while (it != ref_slices_.end()) {
        (*it)->mark_as_non_ref(MarkScope::All);

        output_slices_.push_back(std::move(*it));
        it = ref_slices_.erase(it);
    }

    set_max_long_term_frame_idx(-1);
}

void VideoDecoder::sliding_window_decoded_reference_picture_marking_process(std::shared_ptr<Slice> current_slice)
{
    // only consider frame, field is not allowed
    int size = ref_slices_.size();
    if (size == std::max(1, current_slice->max_num_ref_frames())) {

        std::vector<std::tuple<int, std::shared_ptr<Slice>>> short_term_ref_list_with_FrameNumWrap;
        for (auto& ref_slice : ref_slices_) {
            if (ref_slice->at_least_one_short_term_ref()) {
                short_term_ref_list_with_FrameNumWrap.emplace_back(
                    ref_slice->decoding_process_for_picture_numbers(current_slice), ref_slice);
            }
        }

        // the condition that numShortTerm is greater than 0 shall be fulfilled
        if (short_term_ref_list_with_FrameNumWrap.empty()) {
            spdlog::error("short_term_ref_list_with_FrameNumWrap is empty");
            assert(false);
        }

        auto it = std::min_element(short_term_ref_list_with_FrameNumWrap.begin(),
            short_term_ref_list_with_FrameNumWrap.end(),
            [](const std::tuple<int, std::shared_ptr<Slice>>& a,
                const std::tuple<int, std::shared_ptr<Slice>>& b) {
                if (std::get<0>(a) == std::get<0>(b)) {
                    spdlog::error("min_element() for short_term_ref_list_with_FrameNumWrap have equal pic num");
                    assert(false);
                }
                return std::get<0>(a) < std::get<0>(b);
            });
        if (it == short_term_ref_list_with_FrameNumWrap.end()) {
            spdlog::error("min_element for short_term_ref_list_with_FrameNumWrap is null");
            assert(false);
        }

        auto target_ref_slice = std::get<1>(*it);

        auto it_slice = std::find_if(ref_slices_.begin(), ref_slices_.end(),
            [target_ref_slice](const std::shared_ptr<Slice>& value) {
                return target_ref_slice == value; //
            });
        if (it_slice == ref_slices_.end()) {
            spdlog::error("cannot find slice to be removed in ref_slices_");
            assert(false);
        }

        (*it_slice)->mark_as_non_ref(MarkScope::All);

        output_slices_.push_back(std::move(*it_slice));
        ref_slices_.erase(it_slice);
    }
}
