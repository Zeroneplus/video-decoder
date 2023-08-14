
#include "Slice.h"
#include "VideoDecoder.h"

#include "spdlog/spdlog.h"

Slice::Slice(std::shared_ptr<NalUnit::RbspData> rbsp)
    : rbsp_data_(std::move(rbsp))
{
}

bool Slice::bottom_field_pic_order_in_frame_present()
{
    return pps_->bottom_field_pic_order_in_frame_present() && !field_pic_flag_;
}

int Slice::parse_single_pred_weight_table(int idx)
{

    int luma_weight_flag, chroma_weight_flag;

    int total_num;
    std::vector<int>*luma_weight, *luma_offset,
        *chroma_weight_Cb, *chroma_offset_Cb,
        *chroma_weight_Cr, *chroma_offset_Cr;

    if (idx == 0) {
        total_num = num_ref_idx_l0_active_minus1();

        luma_weight = &luma_weight_l0_;
        luma_offset = &luma_offset_l0_;
        chroma_weight_Cb = &chroma_weight_l0_Cb_;
        chroma_offset_Cb = &chroma_offset_l0_Cb_;
        chroma_weight_Cr = &chroma_weight_l0_Cr_;
        chroma_offset_Cr = &chroma_offset_l0_Cr_;
    } else {
        total_num = num_ref_idx_l1_active_minus1();

        luma_weight = &luma_weight_l1_;
        luma_offset = &luma_offset_l1_;
        chroma_weight_Cb = &chroma_weight_l1_Cb_;
        chroma_offset_Cb = &chroma_offset_l1_Cb_;
        chroma_weight_Cr = &chroma_weight_l1_Cr_;
        chroma_offset_Cr = &chroma_offset_l1_Cr_;
    }

    for (int i = 0; i <= total_num; i++) {
        luma_weight_flag = rbsp_data_->read_u1();
        if (luma_weight_flag) {
            luma_weight->push_back(rbsp_data_->read_se());
            luma_offset->push_back(rbsp_data_->read_se());
        } else {
            luma_weight->push_back(1 << luma_log2_weight_denom_);
            luma_offset->push_back(0);
        }

        if (sps_->ChromaArrayType() != 0) {
            chroma_weight_flag = rbsp_data_->read_u1();
            if (chroma_weight_flag) {
                chroma_weight_Cb->push_back(rbsp_data_->read_se());
                chroma_offset_Cb->push_back(rbsp_data_->read_se());
                chroma_weight_Cr->push_back(rbsp_data_->read_se());
                chroma_offset_Cr->push_back(rbsp_data_->read_se());
            } else {
                chroma_weight_Cb->push_back(1 << chroma_log2_weight_denom_);
                chroma_offset_Cb->push_back(0);
                chroma_weight_Cr->push_back(1 << chroma_log2_weight_denom_);
                chroma_offset_Cr->push_back(0);
            }
        }
    }

    spdlog::trace("pred_weight_table_{}:", idx);

    spdlog::trace("  luma_weight_l{}", idx);
    for (int i = 0; i <= total_num; i++)
        spdlog::trace("    {}", (*luma_weight)[i]);

    spdlog::trace("  luma_offset_l{}", idx);
    for (int i = 0; i <= total_num; i++)
        spdlog::trace("    {}", (*luma_offset)[i]);

    spdlog::trace("  chroma_weight_Cb_l{}", idx);
    for (int i = 0; i <= total_num; i++)
        spdlog::trace("    {}", (*chroma_weight_Cb)[i]);

    spdlog::trace("  chroma_offset_Cb_l{}", idx);
    for (int i = 0; i <= total_num; i++)
        spdlog::trace("    {}", (*chroma_offset_Cb)[i]);

    spdlog::trace("  chroma_weight_Cr_l{}", idx);
    for (int i = 0; i <= total_num; i++)
        spdlog::trace("    {}", (*chroma_weight_Cr)[i]);

    spdlog::trace("  chroma_offset_Cr_l{}", idx);
    for (int i = 0; i <= total_num; i++)
        spdlog::trace("    {}", (*chroma_offset_Cr)[i]);

    return 0;
}

int Slice::parse_pred_weight_table()
{
    luma_log2_weight_denom_ = rbsp_data_->read_ue();
    spdlog::trace("luma_log2_weight_denom is {}", luma_log2_weight_denom_);

    if (sps_->ChromaArrayType() != 0) {
        spdlog::trace("will read chroma_log2_weight_denom");

        chroma_log2_weight_denom_ = rbsp_data_->read_ue();
        spdlog::trace("chroma_log2_weight_denom is {}", chroma_log2_weight_denom_);
    }

    parse_single_pred_weight_table(0);

    if (is_B_slice())
        parse_single_pred_weight_table(1);

    return 0;
}

int Slice::parse_single_ref_pic_list_modification(int idx)
{
    int modification_of_pic_nums_idc = 0, abs_diff_pic_num_minus1 = 0,
        long_term_pic_num = 0;
    int ret = 0;
    std::vector<std::tuple<int, int>>* list;

    if (idx == 0)
        list = &ref_pic_list_modification_l0_;
    else if (idx == 1)
        list = &ref_pic_list_modification_l1_;
    else {
        spdlog::error("error ref_pic_list idx");
        return -1;
    }

    do {
        modification_of_pic_nums_idc = rbsp_data_->read_ue();
        if (modification_of_pic_nums_idc == 0
            || modification_of_pic_nums_idc == 1) {
            abs_diff_pic_num_minus1 = rbsp_data_->read_ue();
            list->emplace_back(modification_of_pic_nums_idc, abs_diff_pic_num_minus1);
        } else if (modification_of_pic_nums_idc == 2) {
            long_term_pic_num = rbsp_data_->read_ue();
            list->emplace_back(modification_of_pic_nums_idc, long_term_pic_num);
        } else if (modification_of_pic_nums_idc > 3) {
            spdlog::error("invalid modification_of_pic_nums_idc");
            return -1;
        }
    } while (modification_of_pic_nums_idc != 3);

    spdlog::warn("ref_pic_list_modification_{}:", idx);
    if (!list->empty()) {
        for (auto& entry : *list) {
            spdlog::warn("  modification_op {}, {}",
                std::get<0>(entry), std::get<1>(entry));
        }
    } else
        spdlog::warn("  null", idx);

    return ret;
}

int Slice::parse_ref_pic_list_modification()
{
    int ret = 0;

    if (!is_I_slice() && !is_SI_slice()) {
        ref_pic_list_modification_flag_l0_ = rbsp_data_->read_u1();
        spdlog::warn("ref_pic_list_modification_flag_l0 {}", (bool)ref_pic_list_modification_flag_l0_);
        if (ref_pic_list_modification_flag_l0_) {
            spdlog::trace("will parse ref_pic_list_modification 0");
            ret = parse_single_ref_pic_list_modification(0);
        } else
            spdlog::warn("ref_pic_list_modification_flag_l0 is false, no need to modify ref_pic_list_0");
    }
    if (is_B_slice()) {
        ref_pic_list_modification_flag_l1_ = rbsp_data_->read_u1();
        spdlog::warn("ref_pic_list_modification_flag_l1 {}", (bool)ref_pic_list_modification_flag_l1_);
        if (ref_pic_list_modification_flag_l1_) {
            spdlog::trace("will parse ref_pic_list_modification 1");
            ret = parse_single_ref_pic_list_modification(1);
        } else
            spdlog::warn("ref_pic_list_modification_flag_l1 is false, no need to modify ref_pic_list_1");
    }
    return ret;
}

int Slice::parse_dec_ref_pic_marking()
{
    int memory_management_control_operation,
        difference_of_pic_nums_minus1,
        long_term_pic_num,
        long_term_frame_idx,
        max_long_term_frame_idx_plus1;
    if (rbsp_data_->idr_pic_flag()) {
        spdlog::warn("dec_ref_pic_marking: this slice is a idr pic");

        no_output_of_prior_pics_flag_ = rbsp_data_->read_u1();
        spdlog::trace("no_output_of_prior_pics_flag {}", (bool)no_output_of_prior_pics_flag_);

        if (no_output_of_prior_pics_flag_)
            spdlog::error("no_output_of_prior_pics_flag is true, don't know how to handle this cases");

        long_term_reference_flag_ = rbsp_data_->read_u1();
        spdlog::trace("long_term_reference_flag {}", (bool)long_term_reference_flag_);

        if (long_term_reference_flag_) {
            spdlog::warn("this idr will be set as long-term ref pic with long-term idx 0");
        } else
            spdlog::warn("this idr will be set as short-term ref pic");

    } else {
        spdlog::warn("dec_ref_pic_marking:: this slice is a non-idr ref pic");

        adaptive_ref_pic_marking_mode_flag_ = rbsp_data_->read_u1();
        spdlog::warn("adaptive_ref_pic_marking_mode_flag {}", (bool)adaptive_ref_pic_marking_mode_flag_);

        if (adaptive_ref_pic_marking_mode_flag_) {
            do {
                memory_management_control_operation = rbsp_data_->read_ue();
                if (memory_management_control_operation == 1) {
                    difference_of_pic_nums_minus1 = rbsp_data_->read_ue();
                    memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
                        difference_of_pic_nums_minus1, 0);
                } else if (memory_management_control_operation == 2) {
                    long_term_pic_num = rbsp_data_->read_ue();
                    memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
                        long_term_pic_num, 0);
                } else if (memory_management_control_operation == 3) {
                    difference_of_pic_nums_minus1 = rbsp_data_->read_ue();
                    long_term_frame_idx = rbsp_data_->read_ue();
                    memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
                        difference_of_pic_nums_minus1,
                        long_term_frame_idx);
                } else if (memory_management_control_operation == 4) {
                    max_long_term_frame_idx_plus1 = rbsp_data_->read_ue();
                    memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
                        max_long_term_frame_idx_plus1,
                        0);
                } else if (memory_management_control_operation == 5) {
                    spdlog::warn("has memory_management_control_operation 5, which may be not properly handled");
                    has_mm_op_5_ = true;
                    memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
                        0,
                        0);
                } else if (memory_management_control_operation == 6) {
                    long_term_frame_idx = rbsp_data_->read_ue();
                    memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
                        long_term_frame_idx,
                        0);
                } else if (memory_management_control_operation > 6) {
                    spdlog::error("invalid memory_management_control_operation");
                    return -1;
                }
            } while (memory_management_control_operation != 0);

            spdlog::warn("dec_ref_pic_marking:");
            for (int i = 0; i < memory_management_control_operation_list_.size(); i++) {
                spdlog::warn("  memory_management_control_operation {}, {}, {}",
                    std::get<0>(memory_management_control_operation_list_[i]),
                    std::get<1>(memory_management_control_operation_list_[i]),
                    std::get<2>(memory_management_control_operation_list_[i]));
            }
        } else
            spdlog::warn("adaptive_ref_pic_marking_mode_flag is false");
    }
    return 0;
}

int Slice::parse_slice_header(VideoDecoder* decoder)
{
    spdlog::warn("**********parse slice header**********");

    first_mb_in_slice_ = rbsp_data_->read_ue();
    spdlog::info("first_mb_in_slice: {}", first_mb_in_slice_);

    slice_type_ = rbsp_data_->read_ue();
    slice_type_enum_ = (enum SliceType)slice_type_;
    spdlog::warn("slice type: {}, {}", slice_type_, slice_type_str(slice_type_enum_));

    pic_parameter_set_id_ = rbsp_data_->read_ue();
    spdlog::info("pps id for current slice: {}", pic_parameter_set_id_);

    pps_ = decoder->get_pps_by_id(pic_parameter_set_id_);
    if (!pps_) {
        spdlog::error("cannot find pps for current slice");
        return -1;
    }

    sps_ = decoder->get_sps_by_id(pps_->sps_id());
    if (!sps_) {
        spdlog::error("cannot find sps for current slice");
        return -1;
    }

    if (sps_->separate_colour_plane_flag()) {
        colour_plane_id_ = rbsp_data_->read_u(2);
        spdlog::warn("this slice has colour_plane_id {}, check if it is a 4:4:4 chroma format", colour_plane_id_);
    } else
        spdlog::warn("this slice does not have colour_plane_id");

    frame_num_ = rbsp_data_->read_u(sps_->frame_num_bits());
    spdlog::warn("frame_num for this slice: {}", frame_num_);

    if (!sps_->frame_mbs_only()) {
        spdlog::trace("will read field_pic_flag");
        field_pic_flag_ = rbsp_data_->read_u1();
        if (field_pic_flag_) {
            spdlog::trace("will read bottom_field_flag");
            bottom_field_flag_ = rbsp_data_->read_u1();
        }
    }

    if (field_pic_flag_) {
        if (bottom_field_flag_) {
            spdlog::warn("this slice is bottom field");
            is_bottom_field_ = true;
        } else {
            spdlog::warn("this slice is top field");
            is_top_field_ = true;
        }
    } else {
        spdlog::warn("this slice is frame");
        is_frame_ = true;
    }

    if (rbsp_data_->idr_pic_flag()) {
        idr_pic_id_ = rbsp_data_->read_ue();
        spdlog::warn("this slice is an idr pic, idr_pic_id: {}", idr_pic_id_);
    } else
        spdlog::warn("this slice isn't an idr pic, does not have idr_pic_id");

    if (sps_->pic_order_cnt_type() == 0) {
        spdlog::warn("this slice has a poc type of 0");

        pic_order_cnt_lsb_ = rbsp_data_->read_u(sps_->poc_lsb_bits());
        spdlog::warn("pic_order_cnt_lsb: {}", pic_order_cnt_lsb_);

        if (bottom_field_pic_order_in_frame_present()) {
            delta_pic_order_cnt_bottom_ = rbsp_data_->read_se();
            spdlog::warn("delta_pic_order_cnt_bottom present: {}", delta_pic_order_cnt_bottom_);
        } else
            spdlog::warn("delta_pic_order_cnt_bottom isn't present, top_poc may equal bottom_poc");
    }

    if (sps_->pic_order_cnt_type() == 1) {
        spdlog::warn("this slice has a poc type of 1");
        if (!sps_->delta_pic_order_always_zero()) {
            delta_pic_order_cnt_0_ = rbsp_data_->read_se();
            spdlog::warn("delta_pic_order_cnt_0: {}", delta_pic_order_cnt_0_);

            if (bottom_field_pic_order_in_frame_present()) {
                delta_pic_order_cnt_1_ = rbsp_data_->read_se();
                spdlog::warn("delta_pic_order_cnt_1: {}", delta_pic_order_cnt_1_);
            } else
                spdlog::warn("delta_pic_order_cnt_1 is not present");
        } else
            spdlog::warn("delta_pic_order for poc type 1 is always_zero");
    }

    if (pps_->redundant_pic_cnt_present()) {
        redundant_pic_cnt_ = rbsp_data_->read_ue();
        spdlog::error("redundant_pic_cnt_present {}", redundant_pic_cnt_);
        return -1;
    }

    if (is_B_slice()) {
        direct_spatial_mv_pred_flag_ = rbsp_data_->read_u1();
        spdlog::warn("this slice is B slice, direct_spatial_mv_pred_flag: {}", static_cast<bool>(direct_spatial_mv_pred_flag_));
    } else
        spdlog::warn("this slice is not B slice, direct_spatial_mv_pred_flag is false");

    if (is_P_slice() || is_SP_slice() || is_B_slice()) {

        spdlog::warn("this slice has ref pic list");

        // update final_num_ref_idx_l0_active_minus1_ final_num_ref_idx_l1_active_minus1_ from pps
        final_num_ref_idx_l0_active_minus1_ = pps_->num_ref_idx_l0_default_active_minus1();
        spdlog::trace("num_ref_idx_l0_active_minus1 from default {}", final_num_ref_idx_l0_active_minus1_);

        if (is_B_slice()) {
            final_num_ref_idx_l1_active_minus1_ = pps_->num_ref_idx_l1_default_active_minus1();
            spdlog::trace("num_ref_idx_l1_active_minus1 from default {}", final_num_ref_idx_l1_active_minus1_);
        }

        num_ref_idx_active_override_flag_ = rbsp_data_->read_u1();
        spdlog::info("num_ref_idx_active_override_flag {}", static_cast<bool>(num_ref_idx_active_override_flag_));

        if (num_ref_idx_active_override_flag_) {
            num_ref_idx_l0_active_minus1_ = rbsp_data_->read_ue();
            spdlog::info("num_ref_idx_l0_active_minus1 will be updated {}", num_ref_idx_l0_active_minus1_);

            final_num_ref_idx_l0_active_minus1_ = num_ref_idx_l0_active_minus1_;

            if (is_B_slice()) {
                num_ref_idx_l1_active_minus1_ = rbsp_data_->read_ue();
                spdlog::info("num_ref_idx_l1_active_minus1 will be updated {}", num_ref_idx_l1_active_minus1_);

                final_num_ref_idx_l1_active_minus1_ = num_ref_idx_l1_active_minus1_;
            }
        }

        spdlog::warn("final num_ref_idx_l0_active_minus1 {}", final_num_ref_idx_l0_active_minus1_);

        if (is_B_slice())
            spdlog::warn("final num_ref_idx_l1_active_minus1 {}", final_num_ref_idx_l1_active_minus1_);

    } else
        spdlog::warn("this slice is I slice, does not have ref pic list");

    if (rbsp_data_->nal_unit_type() == 20 || rbsp_data_->nal_unit_type() == 21) {
        spdlog::error("ref_pic_list_mvc_modification is unsupported");
        return -1;
    } else {
        if (is_I_slice() || is_SI_slice()) {
            spdlog::warn("this slice is I/SI slice, no need to parse ref_pic_list_modification");
        } else
            spdlog::warn("this slice is P/B slice, need to parse ref_pic_list_modification:");
        parse_ref_pic_list_modification();
    }

    if ((pps_->weighted_pred() && (is_P_slice() || is_SP_slice()))
        || (pps_->weighted_bipred() == 1 && is_B_slice())) {
        // spdlog::error("pred_weight_table is unsupported");
        // return -1;
        spdlog::warn("this slice has pred_weight_table");
        parse_pred_weight_table();
    } else
        spdlog::warn("this slice does not have pred_weight_table");

    if (rbsp_data_->nal_ref_idc() != 0) {
        // dec_ref_pic_marking
        spdlog::warn("this is a ref picture, will parse dec_ref_pic_marking");
        parse_dec_ref_pic_marking();
    } else
        spdlog::warn("this is non-ref picture, no need to parse dec_ref_pic_marking");

    if (pps_->entropy_coding_mode()
        && !is_I_slice()
        && !is_SI_slice()) {
        cabac_init_idc_ = rbsp_data_->read_ue();
        spdlog::warn("this slice has cabac_init_idc: {}", cabac_init_idc_);
    } else
        spdlog::warn("this slice does not have cabac_init_idc");

    slice_qp_delta_ = rbsp_data_->read_se();
    spdlog::info("slice_qp_delta {}", slice_qp_delta_);

    if (is_SP_slice() || is_SI_slice()) {
        if (is_SP_slice())
            sp_for_switch_flag_ = rbsp_data_->read_u1();
        slice_qs_delta_ = rbsp_data_->read_se();
    }

    spdlog::info("sp_for_switch_flag {}", (bool)sp_for_switch_flag_);
    if (slice_qs_delta_ != INT32_MIN) {
        spdlog::info("has slice_qs_delta, {}", slice_qs_delta_);
    }

    if (pps_->deblocking_filter_control_present()) {
        spdlog::warn("will read disable_deblocking_filter_idc as pps.deblocking_filter_control_present is true");

        disable_deblocking_filter_idc_ = rbsp_data_->read_ue();
        spdlog::trace("disable_deblocking_filter_idc: {}", disable_deblocking_filter_idc_);

        if (disable_deblocking_filter_idc_ != 1) {
            spdlog::trace("disable_deblocking_filter_idc != 1");

            slice_alpha_c0_offset_div2_ = rbsp_data_->read_se();
            spdlog::trace("slice_alpha_c0_offset_div2: {}", slice_alpha_c0_offset_div2_);

            slice_beta_offset_div2_ = rbsp_data_->read_se();
            spdlog::trace("slice_beta_offset_div2: {}", slice_beta_offset_div2_);
        }
    } else
        spdlog::warn("will not read disable_deblocking_filter_idc as pps.deblocking_filter_control_present is false");

    if (pps_->num_slice_groups_minus1() > 0) {
        spdlog::error("num_slice_groups_minus1 > 0 is unsupported.");
        return -1;
    }

    spdlog::warn("********end parse slice header********");
    return 0;
}

void Slice::log_header()
{
    const char* space = "  ";
    std::cout << "slice header parse result:" << std::endl;
    std::cout << space << "first_mb_in_slice " << first_mb_in_slice_ << std::endl;
    std::cout << space << "slice_type " << slice_type_str(slice_type_enum_) << std::endl;
    std::cout << space << "pic_parameter_set_id " << pic_parameter_set_id_ << std::endl;
    if (colour_plane_id_ >= 0)
        std::cout << space << "colour_plane_id " << colour_plane_id_ << std::endl;
    std::cout << space << "frame_num " << frame_num_ << std::endl;
    if (field_pic_flag_) {
        if (bottom_field_flag_)
            std::cout << space << "this slice is bottom field" << std::endl;
        else
            std::cout << space << "this slice is top field" << std::endl;
    } else
        std::cout << space << "this slice is frame" << std::endl;

    if (idr_pic_id_ >= 0)
        std::cout << space << "idr_pic_id " << idr_pic_id_ << std::endl;

    if (sps_->pic_order_cnt_type() == 0) {
        std::cout << space << "poc type is 0" << std::endl;
        std::cout << space << space << "pic_order_cnt_lsb " << pic_order_cnt_lsb_ << std::endl;
        std::cout << space << space << "delta_pic_order_cnt_bottom " << delta_pic_order_cnt_bottom_ << std::endl;
    } else if (sps_->pic_order_cnt_type() == 1) {
        std::cout << space << "poc type is 1" << std::endl;
        std::cout << space << space << "delta_pic_order_cnt_0 " << delta_pic_order_cnt_0_ << std::endl;
        std::cout << space << space << "delta_pic_order_cnt_1 " << delta_pic_order_cnt_1_ << std::endl;
    } else {
        std::cout << space << "poc type is 2 is unsupported" << std::endl;
    }

    if (redundant_pic_cnt_ >= 0)
        std::cout << space << "redundant_pic_cnt " << redundant_pic_cnt_ << std::endl;

    std::cout << space << "direct_spatial_mv_pred_flag " << direct_spatial_mv_pred_flag_ << std::endl;

    std::cout << space << "num_ref_idx_active_override_flag " << num_ref_idx_active_override_flag_ << std::endl;
    std::cout << space << "num_ref_idx_l0_active_minus1 " << num_ref_idx_l0_active_minus1_ << std::endl;
    std::cout << space << "num_ref_idx_l1_active_minus1 " << num_ref_idx_l1_active_minus1_ << std::endl;

    std::cout << space << "ref_pic_list_modification_flag_l0 " << ref_pic_list_modification_flag_l0_ << std::endl;
    std::cout << space << space << "ref_pic_list_modification_l0 size " << ref_pic_list_modification_l0_.size() << std::endl;
    std::cout << space << "ref_pic_list_modification_flag_l1 " << ref_pic_list_modification_flag_l1_ << std::endl;
    std::cout << space << space << "ref_pic_list_modification_l1 size " << ref_pic_list_modification_l1_.size() << std::endl;

    if (rbsp_data_->idr_pic_flag()) {
        std::cout << space << "no_output_of_prior_pics_flag " << no_output_of_prior_pics_flag_ << std::endl;
        std::cout << space << "long_term_reference_flag " << long_term_reference_flag_ << std::endl;
    } else {
        std::cout << space << "adaptive_ref_pic_marking_mode_flag " << adaptive_ref_pic_marking_mode_flag_ << std::endl;
        if (adaptive_ref_pic_marking_mode_flag_)
            std::cout << space << space << "memory_management_control_operation_list size " << memory_management_control_operation_list_.size() << std::endl;
    }

    if (cabac_init_idc_ >= 0)
        std::cout << space << "cabac_init_idc " << cabac_init_idc_ << std::endl;

    std::cout << space << "slice_qp_delta " << slice_qp_delta_ << std::endl;
    std::cout << space << "sp_for_switch_flag " << sp_for_switch_flag_ << std::endl;
    std::cout << space << "slice_qs_delta " << slice_qs_delta_ << std::endl;

    std::cout << space << "disable_deblocking_filter_idc " << disable_deblocking_filter_idc_ << std::endl;
    std::cout << space << "slice_alpha_c0_offset_div2 " << slice_alpha_c0_offset_div2_ << std::endl;
    std::cout << space << "slice_beta_offset_div2 " << slice_beta_offset_div2_ << std::endl;
}

void Slice::cal_poc(VideoDecoder* dec)
{
    switch (sps_->pic_order_cnt_type()) {
    case 0: {
        cal_poc_0(dec->prev_ref_pic_poc());
        break;
    }
    case 1: {
        cal_poc_1(dec->prev_frame_num());
        break;
    }
    case 2: {
        cal_poc_2(dec->prev_frame_num());
        break;
    }
    default:
        assert(false);
        break;
    }
}

void Slice::cal_poc_0(const std::pair<int, int>& prev_ref_pic_poc)
{
    // value copy
    auto [prevPicOrderCntMsb, prevPicOrderCntLsb] = prev_ref_pic_poc;

    if (rbsp_data_->idr_pic_flag()) {
        prevPicOrderCntMsb = 0;
        prevPicOrderCntLsb = 0;
    }

    int MaxPicOrderCntLsb = sps_->MaxPicOrderCntLsb();

    if ((pic_order_cnt_lsb_ < prevPicOrderCntLsb)
        && ((prevPicOrderCntLsb - pic_order_cnt_lsb_) >= (MaxPicOrderCntLsb / 2)))
        PicOrderCntMsb_ = prevPicOrderCntMsb + MaxPicOrderCntLsb;
    else if ((pic_order_cnt_lsb_ > prevPicOrderCntLsb)
        && ((pic_order_cnt_lsb_ - prevPicOrderCntLsb) > (MaxPicOrderCntLsb / 2)))
        PicOrderCntMsb_ = prevPicOrderCntMsb - MaxPicOrderCntLsb;
    else
        PicOrderCntMsb_ = prevPicOrderCntMsb;

    if (!is_bottom_field_)
        TopFieldOrderCnt_ = PicOrderCntMsb_ + pic_order_cnt_lsb_;

    if (!is_top_field_) {
        if (!field_pic_flag_)
            BottomFieldOrderCnt_ = TopFieldOrderCnt_ + delta_pic_order_cnt_bottom_;
        else
            BottomFieldOrderCnt_ = PicOrderCntMsb_ + pic_order_cnt_lsb_;
    }

    if (is_frame_) {
        spdlog::warn("FrameNume {}, TopFieldOrderCnt {}, BottomFieldOrderCnt {}",
            frame_num_, TopFieldOrderCnt_, BottomFieldOrderCnt_);
    } else {
        // TODO
    }
}

// TODO: when should we update this?
// after decode ref pic marking?
void Slice::update_prev_poc_0(std::pair<int, int>& prev_ref_pic_poc)
{
    // value ref
    auto& [prevPicOrderCntMsb, prevPicOrderCntLsb] = prev_ref_pic_poc;

    if (rbsp_data_->nal_ref_idc()) {
        // thi cond is unnecessary
        if (has_mm_op_5_) {
            // poc should already minus tmp_poc
            if (!is_bottom_field_) {
                prevPicOrderCntMsb = 0;
                prevPicOrderCntLsb = TopFieldOrderCnt_;
            } else {
                prevPicOrderCntMsb = 0;
                prevPicOrderCntLsb = 0;
            }
        } else {
            prevPicOrderCntMsb = PicOrderCntMsb_;
            prevPicOrderCntLsb = pic_order_cnt_lsb_;
        }
    } else
        spdlog::error("update_pre_poc_0 from a non ref pic");
}

void Slice::cal_poc_1(const std::pair<int, int>& prev_frame_num)
{
    // value copy
    auto [prevFrameNum, prevFrameNumOffset] = prev_frame_num;

    int MaxFrameNum = sps_->MaxFrameNum();
    int absFrameNum = 0;
    int num_ref_frames_in_pic_order_cnt_cycle = sps_->num_ref_frames_in_pic_order_cnt_cycle();
    int picOrderCntCycleCnt, frameNumInPicOrderCntCycle;
    int expectedPicOrderCnt;
    int ExpectedDeltaPerPicOrderCntCycle = sps_->ExpectedDeltaPerPicOrderCntCycle();
    std::vector<int>& offset_for_ref_frame = sps_->offset_for_ref_frame();
    int offset_for_non_ref_pic = sps_->offset_for_non_ref_pic();
    int offset_for_top_to_bottom_field = sps_->offset_for_top_to_bottom_field();

    if (rbsp_data_->idr_pic_flag())
        FrameNumOffset_ = 0;
    else if (prevFrameNum > frame_num_)
        FrameNumOffset_ = prevFrameNumOffset + MaxFrameNum;
    else
        FrameNumOffset_ = prevFrameNumOffset;

    if (num_ref_frames_in_pic_order_cnt_cycle != 0)
        absFrameNum = FrameNumOffset_ + frame_num_;
    else
        absFrameNum = 0;

    // if not ref frame, go back to the ref frame
    if (rbsp_data_->nal_ref_idc() == 0 && absFrameNum > 0)
        absFrameNum = absFrameNum - 1;

    if (absFrameNum > 0) {
        picOrderCntCycleCnt = (absFrameNum - 1) / num_ref_frames_in_pic_order_cnt_cycle;
        frameNumInPicOrderCntCycle = (absFrameNum - 1) % num_ref_frames_in_pic_order_cnt_cycle;

        expectedPicOrderCnt = picOrderCntCycleCnt * ExpectedDeltaPerPicOrderCntCycle;
        for (int i = 0; i <= frameNumInPicOrderCntCycle; i++)
            expectedPicOrderCnt = expectedPicOrderCnt + offset_for_ref_frame[i];
    } else {
        expectedPicOrderCnt = 0;
    }

    if (rbsp_data_->nal_ref_idc() == 0)
        expectedPicOrderCnt = expectedPicOrderCnt + offset_for_non_ref_pic;

    if (!field_pic_flag_) {
        TopFieldOrderCnt_ = expectedPicOrderCnt + delta_pic_order_cnt_0_;
        BottomFieldOrderCnt_ = TopFieldOrderCnt_ + offset_for_top_to_bottom_field + delta_pic_order_cnt_1_;

    } else if (!bottom_field_flag_)
        TopFieldOrderCnt_ = expectedPicOrderCnt + delta_pic_order_cnt_0_;
    else
        BottomFieldOrderCnt_ = expectedPicOrderCnt + offset_for_top_to_bottom_field + delta_pic_order_cnt_1_;
}

// every time when decode a slice, we should update prev_frame_num
void Slice::update_prev_frame_num_1_or_2(std::pair<int, int>& prev_frame_num)
{
    // value ref
    auto& [prevFrameNum, prevFrameNumOffset] = prev_frame_num;

    prevFrameNum = frame_num_;
    if (has_mm_op_5_)
        prevFrameNumOffset = 0;
    else
        prevFrameNumOffset = FrameNumOffset_;
}

void Slice::cal_poc_2(const std::pair<int, int>& prev_frame_num)
{
    // value copy
    auto [prevFrameNum, prevFrameNumOffset] = prev_frame_num;

    int MaxFrameNum = sps_->MaxFrameNum();
    int tempPicOrderCnt;

    if (rbsp_data_->idr_pic_flag())
        FrameNumOffset_ = 0;
    else if (prevFrameNum > frame_num_)
        FrameNumOffset_ = prevFrameNumOffset + MaxFrameNum;
    else
        FrameNumOffset_ = prevFrameNumOffset;

    if (rbsp_data_->idr_pic_flag())
        tempPicOrderCnt = 0;
    else if (rbsp_data_->nal_ref_idc() == 0)
        tempPicOrderCnt = 2 * (FrameNumOffset_ + frame_num_) - 1;
    else
        tempPicOrderCnt = 2 * (FrameNumOffset_ + frame_num_);

    if (!field_pic_flag_) {
        TopFieldOrderCnt_ = tempPicOrderCnt;
        BottomFieldOrderCnt_ = tempPicOrderCnt;
    } else if (bottom_field_flag_)
        BottomFieldOrderCnt_ = tempPicOrderCnt;
    else
        TopFieldOrderCnt_ = tempPicOrderCnt;
}

bool Slice::at_least_one_short_term_ref()
{
    return frame_ref_status_ == RefStatus::ShortTerm
        || top_field_ref_status_ == RefStatus::ShortTerm
        || bottom_field_ref_status_ == RefStatus::ShortTerm;
}

bool Slice::at_least_one_long_term_ref()
{
    return frame_ref_status_ == RefStatus::LongTerm
        || top_field_ref_status_ == RefStatus::LongTerm
        || bottom_field_ref_status_ == RefStatus::LongTerm;
}

bool Slice::at_least_one_ref()
{
    return (at_least_one_short_term_ref() && !at_least_one_long_term_ref())
        || (!at_least_one_short_term_ref() && at_least_one_long_term_ref());
}

bool Slice::check_ref_status()
{
    // a frame cannot have a field in short term, and another in long term

    if (frame_ref_status_ == RefStatus::ShortTerm
        && top_field_ref_status_ == RefStatus::ShortTerm
        && bottom_field_ref_status_ == RefStatus::ShortTerm)
        return true;

    if (frame_ref_status_ == RefStatus::LongTerm
        && top_field_ref_status_ == RefStatus::LongTerm
        && bottom_field_ref_status_ == RefStatus::LongTerm)
        return true;

    if (frame_ref_status_ == RefStatus::NonRef
        && top_field_ref_status_ == RefStatus::ShortTerm
        && bottom_field_ref_status_ == RefStatus::NonRef)
        return true;

    if (frame_ref_status_ == RefStatus::NonRef
        && top_field_ref_status_ == RefStatus::NonRef
        && bottom_field_ref_status_ == RefStatus::ShortTerm)
        return true;

    if (frame_ref_status_ == RefStatus::NonRef
        && top_field_ref_status_ == RefStatus::LongTerm
        && bottom_field_ref_status_ == RefStatus::NonRef)
        return true;

    if (frame_ref_status_ == RefStatus::NonRef
        && top_field_ref_status_ == RefStatus::NonRef
        && bottom_field_ref_status_ == RefStatus::LongTerm)
        return true;

    if (frame_ref_status_ == RefStatus::NonRef
        && top_field_ref_status_ == RefStatus::NonRef
        && bottom_field_ref_status_ == RefStatus::NonRef)
        return true;

    return false;
}

int Slice::decoding_process_for_picture_numbers(std::shared_ptr<Slice> current_slice)
{
    int FrameNum = frame_num_;
    int frame_num = current_slice->FrameNum();
    int MaxFrameNum = sps_->MaxFrameNum();

    int FrameNumWrap;

    int ShortTerm_Or_LongTerm_PicNum;

    bool field_pic_flag = current_slice->field_pic_flag();
    bool bottom_field_flag = current_slice->bottom_field_flag();

    if (current_slice->slice_field_frame_type() != SliceFieldFrameType::Frame
        || slice_field_frame_type() != SliceFieldFrameType::Frame) {
        spdlog::error("only frame or MBAFF mode is supported");
        assert(false);
    }

    // we do not distinguish between frame or field
    // for a ShortTerm-ref frame(with frame_ref_status_ = ShortTerm,
    // top_field_ref_status_ = ShortTerm, bottom_field_ref_status_ = ShortTerm),
    // if one of its field is marked as NonRef (for example, the top field),
    // then frame_ref_status_ is NonRef, top_field_ref_status_ is NonRef,
    // but bottom_field_ref_status_ is still ShortTerm
    if (at_least_one_short_term_ref()) {
        if (FrameNum > frame_num)
            FrameNumWrap = FrameNum - MaxFrameNum;
        else
            FrameNumWrap = FrameNum;
    }

    if (!field_pic_flag) {
        if (at_least_one_short_term_ref()) {
            // short term and long term are mutual-exclusive
            ShortTerm_Or_LongTerm_PicNum = FrameNumWrap;
        } else
            ShortTerm_Or_LongTerm_PicNum = LongTermFrameIdx_;
    } else {
        // TODO
    }

    return ShortTerm_Or_LongTerm_PicNum;
}

// TODO: how to handle complementary field pair?
int Slice::PicOrderCnt()
{
    int poc;
    if (is_frame_)
        poc = std::min(TopFieldOrderCnt_, BottomFieldOrderCnt_);
    else if (is_top_field_)
        poc = TopFieldOrderCnt_;
    else if (is_bottom_field_)
        poc = BottomFieldOrderCnt_;
    return poc;
}

void Slice::update_prev_frame_num_or_prev_ref_pic_poc(VideoDecoder* vdec)
{
    if (sps_->pic_order_cnt_type() == 0
        && is_reference_slice()
        && at_least_one_ref()) // at_least_one_ref() more reliable ?
        update_prev_poc_0(vdec->prev_ref_pic_poc());
    else if (sps_->pic_order_cnt_type() == 1
        || sps_->pic_order_cnt_type() == 2)
        update_prev_frame_num_1_or_2(vdec->prev_frame_num());
}

int Slice::PicOrderCntOrLongTermPicNum()
{
    if (at_least_one_short_term_ref())
        return PicOrderCnt();
    else
        return LongTermFrameIdx_; // TODO PAFF mode may have problems here
}

void Slice::set_ref_list_P(std::vector<std::tuple<int, std::shared_ptr<Slice>>> ref_list_P_0)
{
    // keep a init version of ref_list_P_0
    ref_list_P_0_init_ = std::move(ref_list_P_0);

    // copy ref_list_P_0_init_ to ref_list_P_0_
    ref_list_P_0_.insert(ref_list_P_0_.end(), ref_list_P_0_init_.begin(), ref_list_P_0_init_.end());

    // update the ref list by num_ref_idx_l0_active_minus1
    // if size() > num_ref_idx_l0_active_minus1() + 1, shrink
    // else, insert default initialized item, which is std::tuple<int, std::shared_ptr<Slice>>>{}
    ref_list_P_0_.resize(num_ref_idx_l0_active_minus1() + 1);

    // add a dummy entry for ref pic list modification
    ref_list_P_0_.push_back({});
}

void Slice::set_ref_list_B(
    std::pair<std::vector<std::tuple<int, std::shared_ptr<Slice>>>,
        std::vector<std::tuple<int, std::shared_ptr<Slice>>>>
        ref_list_B_pair)
{
    ref_list_B_0_init_ = std::move(ref_list_B_pair.first);
    ref_list_B_1_init_ = std::move(ref_list_B_pair.second);

    // for list 0
    ref_list_B_0_.insert(ref_list_B_0_.end(), ref_list_B_0_init_.begin(), ref_list_B_0_init_.end());

    // update the ref list by num_ref_idx_l0_active_minus1
    ref_list_B_0_.resize(num_ref_idx_l0_active_minus1() + 1);

    // add a dummy entry for modification
    ref_list_B_0_.push_back({});

    // for list 1
    ref_list_B_1_.insert(ref_list_B_1_.end(), ref_list_B_1_init_.begin(), ref_list_B_1_init_.end());

    // update the ref list by num_ref_idx_l1_active_minus1
    ref_list_B_1_.resize(num_ref_idx_l1_active_minus1() + 1);

    // add a dummy entry for modification
    ref_list_B_1_.push_back({});
}

void Slice::modification_process_for_reference_picture_lists()
{
    if (ref_pic_list_modification_flag_l0_) {
        int refIdxL0 = 0;
        std::vector<std::tuple<int, std::shared_ptr<Slice>>>*ref_list_0_ptr, *ref_list_0_init_ptr;
        if (is_B_slice()) {
            ref_list_0_init_ptr = &ref_list_B_0_init_;
            ref_list_0_ptr = &ref_list_B_0_;
        } else {
            ref_list_0_init_ptr = &ref_list_P_0_init_;
            ref_list_0_ptr = &ref_list_P_0_;
        }
        auto& ref_list_0 = *ref_list_0_ptr;
        auto& ref_list_0_init = *ref_list_0_init_ptr;

        for (auto& [modification_of_pic_nums_idc, pic_num] : ref_pic_list_modification_l0_) {
            if (modification_of_pic_nums_idc == 0 || modification_of_pic_nums_idc == 1)
                refIdxL0 = modification_process_of_reference_picture_lists_for_short_term_reference_pictures(
                    refIdxL0,
                    ref_list_0,
                    ref_list_0_init,
                    picNumL0Pred_,
                    num_ref_idx_l0_active_minus1(),
                    modification_of_pic_nums_idc,
                    pic_num);
            else
                refIdxL0 = modification_process_of_reference_picture_lists_for_long_term_reference_pictures(
                    refIdxL0,
                    ref_list_0,
                    ref_list_0_init,
                    num_ref_idx_l0_active_minus1(),
                    pic_num);
        }
    }

    auto ref_pic_log = [](const char* message,
                           const char* str,
                           const std::vector<std::tuple<int, std::shared_ptr<Slice>>>& value) {
        spdlog::warn(message);
        for (auto& [pic_num, ref_slice] : value) {
            spdlog::warn("  {} {}", str, pic_num);
        }
    };

    auto check_ref_pic_list = [](const std::vector<std::tuple<int, std::shared_ptr<Slice>>>& value) {
        for (auto& [pic_num, ref_slice] : value) {
            if (!ref_slice) {
                spdlog::error("after ref pic modification, ref slice is null");
                assert(false);
            }
        }
    };

    // check ref pic list 0 status
    // after modification, there should not any 'non-exsiting' ref pic
    if (is_B_slice()) {
        ref_list_B_0_.pop_back(); // remove dummy element

        check_ref_pic_list(ref_list_B_0_);

        // poc has been replaced with pic num
        ref_pic_log("ref list B 0:", "short-term or long-term picnum", ref_list_B_0_);

    } else {
        ref_list_P_0_.pop_back(); // remove dummy element

        check_ref_pic_list(ref_list_P_0_);

        ref_pic_log("ref list P 0:", "short-term or long-term picnum", ref_list_P_0_);
    }

    if (ref_pic_list_modification_flag_l1_) {
        int refIdxL1 = 0;
        auto& ref_list_1 = ref_list_B_1_;
        auto& ref_list_1_init = ref_list_B_1_init_;
        for (auto& [modification_of_pic_nums_idc, pic_num] : ref_pic_list_modification_l1_) {
            if (modification_of_pic_nums_idc == 0 || modification_of_pic_nums_idc == 1)
                refIdxL1 = modification_process_of_reference_picture_lists_for_short_term_reference_pictures(
                    refIdxL1,
                    ref_list_1,
                    ref_list_1_init,
                    picNumL1Pred_,
                    num_ref_idx_l1_active_minus1(),
                    modification_of_pic_nums_idc,
                    pic_num);
            else
                refIdxL1 = modification_process_of_reference_picture_lists_for_long_term_reference_pictures(
                    refIdxL1,
                    ref_list_1,
                    ref_list_1_init,
                    num_ref_idx_l1_active_minus1(),
                    pic_num);
        }
    }

    // check ref pic list 1 status
    if (is_B_slice()) {
        ref_list_B_1_.pop_back(); // remove dummy element

        check_ref_pic_list(ref_list_B_1_);

        ref_pic_log("ref list B 1:", "short-term or long-term picnum", ref_list_B_1_);
    }
}

// only update first (num_ref_idx_lX_active_minus1 + 1) element?
int Slice::modification_process_of_reference_picture_lists_for_short_term_reference_pictures(
    int refIdxLX,
    std::vector<std::tuple<int, std::shared_ptr<Slice>>>& RefPicListX,
    std::vector<std::tuple<int, std::shared_ptr<Slice>>>& RefPicListX_init,
    int& picNumLXPred,
    int num_ref_idx_lX_active_minus1,
    int modification_of_pic_nums_idc,
    int abs_diff_pic_num_minus1)
{
    int picNumLXNoWrap, picNumLX;

    // if first time, init picNumLXPred to CurrPicNum()
    if (picNumLXPred == INT32_MIN)
        picNumLXPred = CurrPicNum();

    if (modification_of_pic_nums_idc == 0) {
        if (picNumLXPred - (abs_diff_pic_num_minus1 + 1) < 0)
            picNumLXNoWrap = picNumLXPred - (abs_diff_pic_num_minus1 + 1) + MaxPicNum();
        else
            picNumLXNoWrap = picNumLXPred - (abs_diff_pic_num_minus1 + 1);
    } else {
        if (picNumLXPred + (abs_diff_pic_num_minus1 + 1) >= MaxPicNum())
            picNumLXNoWrap = picNumLXPred + (abs_diff_pic_num_minus1 + 1) - MaxPicNum();
        else
            picNumLXNoWrap = picNumLXPred + (abs_diff_pic_num_minus1 + 1);
    }

    // update picNumLXPred
    picNumLXPred = picNumLXNoWrap;

    if (picNumLXNoWrap > CurrPicNum())
        picNumLX = picNumLXNoWrap - MaxPicNum();
    else
        picNumLX = picNumLXNoWrap;

    // find target short term pic from RefPicListX_init
    auto it = std::find_if(RefPicListX_init.begin(), RefPicListX_init.end(),
        [picNumLX](const std::tuple<int, std::shared_ptr<Slice>>& value) {
            auto& [pic_num, ref_slice] = value;
            if (ref_slice->at_least_one_short_term_ref() && pic_num == picNumLX)
                return true;
            return false;
        });

    if (it == RefPicListX_init.end()) {
        spdlog::error("cannot find short term ref pic by pic num in modification_process_of_reference_picture_lists_for_short_term_reference_pictures");
        assert(false);
    }

    // TODO: still has questions about this process

    for (int cIdx = num_ref_idx_lX_active_minus1 + 1; cIdx > refIdxLX; cIdx--)
        RefPicListX[cIdx] = RefPicListX[cIdx - 1];

    RefPicListX[refIdxLX++] = *it;

    int nIdx = refIdxLX;
    auto PicNumF = [](const std::tuple<int, std::shared_ptr<Slice>>& value) {
        auto& [pic_num, ref_slice] = value;
        // ref_slice may be null
        if (ref_slice && ref_slice->at_least_one_short_term_ref())
            return pic_num;
        return INT32_MAX; // note: this should be ok
    };
    for (int cIdx = refIdxLX; cIdx <= num_ref_idx_lX_active_minus1 + 1; cIdx++) {
        if (PicNumF(RefPicListX[cIdx]) != picNumLX)
            RefPicListX[nIdx++] = RefPicListX[cIdx];
    }

    // always reset the last element
    // maybe no need to do this?
    RefPicListX[num_ref_idx_lX_active_minus1 + 1] = {};

    return refIdxLX; // refIdxLX is already incremented
}

int Slice::modification_process_of_reference_picture_lists_for_long_term_reference_pictures(
    int refIdxLX,
    std::vector<std::tuple<int, std::shared_ptr<Slice>>>& RefPicListX,
    std::vector<std::tuple<int, std::shared_ptr<Slice>>>& RefPicListX_init,
    int num_ref_idx_lX_active_minus1,
    int long_term_pic_num)
{

    for (int cIdx = num_ref_idx_lX_active_minus1 + 1; cIdx > refIdxLX; cIdx--)
        RefPicListX[cIdx] = RefPicListX[cIdx - 1];

    // find target long term pic
    auto it = std::find_if(RefPicListX_init.begin(), RefPicListX_init.end(),
        [long_term_pic_num](const std::tuple<int, std::shared_ptr<Slice>>& value) {
            auto& [pic_num, ref_slice] = value;
            if (ref_slice->at_least_one_long_term_ref() && pic_num == long_term_pic_num)
                return true;
            return false;
        });

    if (it == RefPicListX_init.end()) {
        spdlog::error("cannot find short term ref pic by pic num in modification_process_of_reference_picture_lists_for_long_term_reference_pictures");
        assert(false);
    }

    RefPicListX[refIdxLX++] = *it;

    int nIdx = refIdxLX;

    auto LongTermPicNumF = [](const std::tuple<int, std::shared_ptr<Slice>>& value) {
        auto& [pic_num, ref_slice] = value;
        // ref_slice may be null
        if (ref_slice && ref_slice->at_least_one_long_term_ref())
            return pic_num;
        return INT32_MAX; // note: this should be ok
    };

    for (int cIdx = refIdxLX; cIdx <= num_ref_idx_lX_active_minus1 + 1; cIdx++)
        if (LongTermPicNumF(RefPicListX[cIdx]) != long_term_pic_num)
            RefPicListX[nIdx++] = RefPicListX[cIdx];

    // always reset the last element
    // maybe no need to do this?
    RefPicListX[num_ref_idx_lX_active_minus1 + 1] = {};

    return refIdxLX;
}

void Slice::adaptive_memory_control_decoded_reference_picture_marking_process(VideoDecoder* vdec)
{
    std::vector<std::shared_ptr<Slice>>& ref_slices = vdec->ref_slices();
    // memory_management_control_operation_list_
    for (auto& mm_ctrl : memory_management_control_operation_list_) {
        switch (std::get<0>(mm_ctrl)) {
        case 1: {
            // marking_process_of_a_short_term_reference_picture_as_unused_for_reference

            // memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
            // difference_of_pic_nums_minus1, 0);
            int difference_of_pic_nums_minus1 = std::get<1>(mm_ctrl);
            int picNumX = CurrPicNum() - (difference_of_pic_nums_minus1 + 1);
            vdec->mark_short_term_as_unref_by_picNumX(picNumX, shared_from_this());
            break;
        }
        case 2: {
            // Marking process of a long-term reference picture as "unused for reference"

            // memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
            // long_term_pic_num, 0);

            int long_term_pic_num = std::get<1>(mm_ctrl);

            vdec->mark_long_term_as_unref_by_long_term_pic_num(long_term_pic_num, shared_from_this());
            break;
        }
        case 3: {
            // Assignment process of a LongTermFrameIdx to a short-term reference picture

            // memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
            // difference_of_pic_nums_minus1,
            // long_term_frame_idx);

            int difference_of_pic_nums_minus1 = std::get<1>(mm_ctrl);
            int long_term_frame_idx = std::get<2>(mm_ctrl);
            int picNumX = CurrPicNum() - (difference_of_pic_nums_minus1 + 1);

            vdec->mark_short_term_as_long_term(picNumX, long_term_frame_idx, shared_from_this());
            break;
        }
        case 4: {
            // Decoding process for MaxLongTermFrameIdx

            // memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
            // max_long_term_frame_idx_plus1,
            // 0);

            int max_long_term_frame_idx_plus1 = std::get<1>(mm_ctrl);
            vdec->set_max_long_term_frame_idx_and_mark_long_term_as_unref(max_long_term_frame_idx_plus1, shared_from_this());
            break;
        }
        case 5: {
            // Marking process of all reference pictures as "unused for reference" and
            // setting MaxLongTermFrameIdx to "no long-term frame indices"

            // memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
            //     0,
            //     0);
            vdec->mark_all_unref_and_set_max_long_term_frame_idx_to_none();
            break;
        }
        case 6: {
            // Process for assigning a long-term frame index to the current picture

            // memory_management_control_operation_list_.emplace_back(memory_management_control_operation,
            //     long_term_frame_idx,
            //     0);
            int long_term_frame_idx = std::get<1>(mm_ctrl);

            vdec->possiblely_mark_long_term_as_unref_by_long_term_pic_num(long_term_frame_idx, shared_from_this());

            mark_as_long_or_short_term_ref(MarkScope::All, true /* bool long_term */);

            set_long_term_frame_idx(long_term_frame_idx);
            break;
        }
        default: {
            assert(false);
            break;
        }
        }
    }
}

int Slice::NextMbAddress(int CurrMbAddr)
{
    // TODO: slice group
    return CurrMbAddr + 1;
}

int Slice::parse_slice_data(VideoDecoder* decoder)
{
    // allocate memory for yuv data
    int PicWidthInMbs = sps_->PicWidthInMbs();
    int FrameHeightInMbs = sps_->FrameHeightInMbs();
    int PicHeightInMbs = is_frame_ ? FrameHeightInMbs : FrameHeightInMbs / 2;
    int size_y = PicWidthInMbs * 16 * PicHeightInMbs * 16;
    int size_u_or_v = PicWidthInMbs * sps_->MbWidthC() * PicHeightInMbs * sps_->MbHeightC();

    yuv_data_ = std::vector<int>(size_y + size_u_or_v * 2, 0);
    y_data_ = yuv_data_.data();
    u_data_ = y_data_ + size_y;
    v_data_ = u_data_ + size_u_or_v;

    // set init QP_Y_PREV_
    QP_Y_PREV_ = SliceQPY();

    // start parse macro block

    if (pps_->entropy_coding_mode()) {
        while (!rbsp_data_->byte_aligned())
            rbsp_data_->read_u1();
    }

    int CurrMbAddr = first_mb_in_slice_ * (1 + (MbaffFrameFlag() ? 1 : 0));
    int moreDataFlag = 1,
        prevMbSkipped = 0,
        mb_skip_run = 0;

    // only consider frame mode
    // TODO: revisit the initial value of mb_field_decoding_flag_
    mb_field_decoding_flag_ = field_pic_flag_;

    do {
        if (!is_I_slice() && !is_SI_slice()) {
            if (!pps_->entropy_coding_mode()) {
                mb_skip_run = rbsp_data_->read_ue();
                prevMbSkipped = (mb_skip_run > 0);

                for (int i = 0; i < mb_skip_run; i++) {

                    // TODO: process skipped mb
                    // be care when both top and bottom of a mb pair are skipped
                    //
                    // shall we update_QP_Y_PREV() for skip?

                    CurrMbAddr = NextMbAddress(CurrMbAddr);
                }

                if (mb_skip_run > 0)
                    moreDataFlag = rbsp_data_->more_rbsp_data();
            } else {
                // TODO
                assert(false);
            }
        }

        if (moreDataFlag) {
            if (MbaffFrameFlag() && (CurrMbAddr % 2 == 0 || (CurrMbAddr % 2 == 1 && prevMbSkipped)))
                mb_field_decoding_flag_ = rbsp_data_->read_ue();

            // parse macroblock
            std::shared_ptr<MacroBlock> mb = std::make_shared<MacroBlock>(this, rbsp_data_, CurrMbAddr);
            // add MacroBlock first, though it hasnt been parsed
            add_MacroBlock(CurrMbAddr, mb);
            mb->parse_MacroBlock();
        }

        if (!pps_->entropy_coding_mode())
            moreDataFlag = rbsp_data_->more_rbsp_data();
        else
            assert(false); // TODO

        CurrMbAddr = NextMbAddress(CurrMbAddr);

    } while (moreDataFlag);

    write_yuv("");

    return 0;
}

void Slice::add_MacroBlock(int CurrMbAddr, std::shared_ptr<MacroBlock> mb)
{
    auto it = mb_map_.find(CurrMbAddr);
    if (it != mb_map_.end()) {
        mb_map_.erase(it);
        spdlog::warn("will remove an exist mb and insert a new mb");
    }

    mb_map_.insert(std::make_pair(CurrMbAddr, mb));
}

// 6.4.1 Inverse macroblock scanning process
std::pair<int, int> Slice::Inverse_macroblock_scanning_process(int mbAddr)
{
    // If MbaffFrameFlag is equal to 0
    int x = InverseRasterScan(mbAddr, 16, 16, sps_->PicWidthInSamplesL(), 0);
    int y = InverseRasterScan(mbAddr, 16, 16, sps_->PicWidthInSamplesL(), 1);

    // else TODO

    return { x, y };
}

// 6.4.3 Inverse 4x4 luma block scanning process
std::pair<int, int> Slice::Inverse_4x4_luma_block_scanning_process(int luma4x4BlkIdx)
{
    int x = InverseRasterScan(luma4x4BlkIdx / 4, 8, 8, 16, 0) + InverseRasterScan(luma4x4BlkIdx % 4, 4, 4, 8, 0);
    int y = InverseRasterScan(luma4x4BlkIdx / 4, 8, 8, 16, 1) + InverseRasterScan(luma4x4BlkIdx % 4, 4, 4, 8, 1);
    return { x, y };
}

// TODO: what if 4:2:2 and 4:4:4 format?
// it seems that 6.4.7 only works for 4:2:0 and 4:2:2，4:4:4 chroma should use 6.4.3
//
// 6.4.7 Inverse 4x4 chroma block scanning process
std::pair<int, int> Slice::Inverse_4x4_chroma_block_scanning_process(int chroma4x4BlkIdx)
{
    int x = InverseRasterScan(chroma4x4BlkIdx, 4, 4, 8, 0);
    int y = InverseRasterScan(chroma4x4BlkIdx, 4, 4, 8, 1);
    return { x, y };
}

// 6.4.8 Derivation process of the availability for macroblock addresses
//
bool Slice::mb_availability(int mbAddr, int CurrMbAddr)
{
    if (mbAddr < 0 || mbAddr > CurrMbAddr)
        return false;

    // slice group is not considered

    return true;
}

// for simplicity, if mbAddrN < 0 or mbAddrN > CurrMbAddr, then unavailable
//
// 6.4.9 Derivation process for neighbouring macroblock addresses and their availability
std::tuple<int, int, int, int>
Slice::Derivation_process_for_neighbouring_macroblock_addresses_and_their_availability(int CurrMbAddr)
{
    int mbAddrA = INT32_MIN,
        mbAddrB = INT32_MIN,
        mbAddrC = INT32_MIN,
        mbAddrD = INT32_MIN;

    if (CurrMbAddr % sps_->PicWidthInMbs() != 0)
        mbAddrA = CurrMbAddr - 1;

    mbAddrB = CurrMbAddr - sps_->PicWidthInMbs();

    if ((CurrMbAddr + 1) % sps_->PicWidthInMbs() != 0)
        mbAddrC = CurrMbAddr - sps_->PicWidthInMbs() + 1;

    if (CurrMbAddr % sps_->PicWidthInMbs() != 0)
        mbAddrD = CurrMbAddr - sps_->PicWidthInMbs() - 1;

    // INT32_MIN is unavailable.
    // TODO: less than 0 may be more suitable?
    mbAddrA = mb_availability(mbAddrA, CurrMbAddr) ? mbAddrA : INT32_MIN;
    mbAddrB = mb_availability(mbAddrB, CurrMbAddr) ? mbAddrB : INT32_MIN;
    mbAddrC = mb_availability(mbAddrC, CurrMbAddr) ? mbAddrC : INT32_MIN;
    mbAddrD = mb_availability(mbAddrD, CurrMbAddr) ? mbAddrD : INT32_MIN;

    return { mbAddrA, mbAddrB, mbAddrC, mbAddrD };
}

/*
N    xD               yD
A    −1               0
B    0                −1
C    predPartWidth    −1
D    −1               −1
*/

// 6.4.11.1 Derivation process for neighbouring macroblocks
std::pair<int, int>
Slice::Derivation_process_for_neighbouring_macroblocks(int CurrMbAddr, bool is_luma)
{
    int mbAddrA = std::get<0>(Derivation_process_for_neighbouring_locations(CurrMbAddr, -1, 0, is_luma));
    int mbAddrB = std::get<0>(Derivation_process_for_neighbouring_locations(CurrMbAddr, 0, -1, is_luma));

    return { mbAddrA, mbAddrB };
}

// 6.4.11.4 Derivation process for neighbouring 4x4 luma blocks
std::tuple<int, int, int, int>
Slice::Derivation_process_for_neighbouring_4x4_luma_blocks(int CurrMbAddr, int luma4x4BlkIdx)
{
    int mbAddrA = INT32_MIN;
    int luma4x4BlkIdxA = INT32_MIN;
    int mbAddrB = INT32_MIN;
    int luma4x4BlkIdxB = INT32_MIN;

    auto func = [this, CurrMbAddr, luma4x4BlkIdx](int xD, int yD) -> std::pair<int, int> {
        auto [x, y] = Inverse_4x4_luma_block_scanning_process(luma4x4BlkIdx);
        int xN = x + xD;
        int yN = y + yD;
        auto [mbAddrN, xW, yW] = Derivation_process_for_neighbouring_locations(CurrMbAddr, xN, yN, true);
        if (mbAddrN == INT32_MIN) // is this safe?
            return { INT32_MIN, INT32_MIN };
        int ret_luma4x4BlkIdx = Derivation_process_for_4x4_luma_block_indices(xW, yW);
        return { mbAddrN, ret_luma4x4BlkIdx };
    };

    // A
    auto tmpA = func(-1, 0);
    mbAddrA = tmpA.first;
    luma4x4BlkIdxA = tmpA.second;

    // B
    auto tmpB = func(0, -1);
    mbAddrB = tmpB.first;
    luma4x4BlkIdxB = tmpB.second;

    return { mbAddrA, luma4x4BlkIdxA, mbAddrB, luma4x4BlkIdxB };
}

// this function works for 4:2:2 and 4:4:4 only if Inverse_4x4_chroma_block_scanning_process()
// and Derivation_process_for_4x4_chroma_block_indices() work for 4:2:2 and 4:4:4
//
// 6.4.11.5 Derivation process for neighbouring 4x4 chroma blocks
std::tuple<int, int, int, int>
Slice::Derivation_process_for_neighbouring_4x4_chroma_blocks(int CurrMbAddr, int chroma4x4BlkIdx)
{
    int mbAddrA = INT32_MIN;
    int chroma4x4BlkIdxA = INT32_MIN;
    int mbAddrB = INT32_MIN;
    int chroma4x4BlkIdxB = INT32_MIN;

    auto func = [this, CurrMbAddr, chroma4x4BlkIdx](int xD, int yD) -> std::pair<int, int> {
        auto [x, y] = Inverse_4x4_chroma_block_scanning_process(chroma4x4BlkIdx);
        int xN = x + xD;
        int yN = y + yD;
        auto [mbAddrN, xW, yW] = Derivation_process_for_neighbouring_locations(CurrMbAddr, xN, yN, false);
        if (mbAddrN == INT32_MIN) // is this safe?
            return { INT32_MIN, INT32_MIN };
        int ret_chroma4x4BlkIdx = Derivation_process_for_4x4_chroma_block_indices(xW, yW);
        return { mbAddrN, ret_chroma4x4BlkIdx };
    };

    // A
    auto tmpA = func(-1, 0);
    mbAddrA = tmpA.first;
    chroma4x4BlkIdxA = tmpA.second;

    // B
    auto tmpB = func(0, -1);
    mbAddrB = tmpB.first;
    chroma4x4BlkIdxB = tmpB.second;

    return { mbAddrA, chroma4x4BlkIdxA, mbAddrB, chroma4x4BlkIdxB };
}

// 6.4.13.1 Derivation process for 4x4 luma block indices
int Slice::Derivation_process_for_4x4_luma_block_indices(int xP, int yP)
{
    int luma4x4BlkIdx = 8 * (yP / 8) + 4 * (xP / 8) + 2 * ((yP % 8) / 4) + ((xP % 8) / 4);
    return luma4x4BlkIdx;
}

// this process may only work for 4:2:0 and 4:2:2
// 4:4:4 should use 6.4.13.1, but ITU Rec does not
// specify this
//
// 6.4.13.2 Derivation process for 4x4 chroma block indices
int Slice::Derivation_process_for_4x4_chroma_block_indices(int xP, int yP)
{
    int chroma4x4BlkIdx = 2 * (yP / 4) + (xP / 4);
    return chroma4x4BlkIdx;
}

// 6.4.12.1 Specification for neighbouring locations in fields and non-MBAFF frames
// mbAddrN xW yW
std::tuple<int, int, int>
Slice::Derivation_process_for_neighbouring_locations(int CurrMbAddr,
    int xN,
    int yN,
    bool is_luma)
{
    int maxW, maxH;
    int mbAddrN = INT32_MIN;
    int xW = INT32_MIN,
        yW = INT32_MIN;

    if (is_luma) {
        maxW = 16;
        maxH = 16;
    } else {
        maxW = sps_->MbWidthC();
        maxH = sps_->MbHeightC();
    }

    // only consider frame mode

    auto [mbAddrA,
        mbAddrB,
        mbAddrC,
        mbAddrD]
        = Derivation_process_for_neighbouring_macroblock_addresses_and_their_availability(CurrMbAddr);

    if (xN < 0 && yN < 0)
        mbAddrN = mbAddrD;
    else if (xN < 0 && yN >= 0 && yN <= maxH - 1)
        mbAddrN = mbAddrA;
    else if (xN >= 0 && xN <= maxW - 1 && yN < 0)
        mbAddrN = mbAddrB;
    else if (xN >= 0 && xN <= maxW - 1 && yN >= 0 && yN <= maxH - 1)
        mbAddrN = CurrMbAddr;
    else if (xN > maxW - 1 && yN < 0)
        mbAddrN = mbAddrC;
    else {
        // not available
        // assert(false);
    }

    if (mbAddrN != INT32_MIN) { // is this safe? yes
        xW = (xN + maxW) % maxW;
        yW = (yN + maxH) % maxH;
    }
    return { mbAddrN, xW, yW };
}

MacroBlock& Slice::get_mb_by_addr(int addr)
{
    if (mb_map_.find(addr) == mb_map_.end()) {
        assert(false);
    }
    return *mb_map_[addr];
}

// be care of the meaning of 'x' and 'y'
// x is width coordinate
// y is height coordinate
//
int Slice::get_constructed_luma(int x, int y)
{
    int PicWidthL = sps_->PicWidthInSamplesL();

    return y_data_[x + y * PicWidthL];
}

int Slice::get_constructed_chroma(int idx, int x, int y)
{
    int PicWidthC = sps_->PicWidthInSamplesC();

    return idx == 0 ? u_data_[x + y * PicWidthC] : v_data_[x + y * PicWidthC];
}

void Slice::set_constructed_luma(int x, int y, int value)
{
    int PicWidthL = sps_->PicWidthInSamplesL();

    y_data_[x + y * PicWidthL] = value;
}

void Slice::set_constructed_chroma(int idx, int x, int y, int value)
{
    int PicWidthC = sps_->PicWidthInSamplesC();

    if (idx == 0)
        u_data_[x + y * PicWidthC] = value;
    else
        v_data_[x + y * PicWidthC] = value;
}

void Slice::write_yuv(std::string file_name)
{
    if (!file_name.size())
        file_name = std::to_string(PicOrderCnt()) + pic_type_str() + ".yuv420p";

    FILE* f = fopen(file_name.c_str(), "wb");
    if (!f) {
        assert(false);
    }

    std::vector<int8_t> tmp(yuv_data_.size());
    for (int i = 0; i < yuv_data_.size(); i++)
        tmp[i] = yuv_data_[i];

    int ret = fwrite(tmp.data(), 1, tmp.size(), f);
    assert(ret == tmp.size());
    fclose(f);
}