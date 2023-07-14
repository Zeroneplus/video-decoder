
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

    spdlog::trace("ref_pic_list_modification_{}:", idx);
    if (list->size() > 0) {
        for (int i = 0; i < list->size(); i++) {
            spdlog::trace("  modification_op {}, {}",
                std::get<0>((*list)[i]), std::get<1>((*list)[i]));
        }
    } else
        spdlog::trace("  null", idx);

    return ret;
}

int Slice::parse_ref_pic_list_modification()
{
    int ret = 0;

    if (!is_I_slice() && !is_SI_slice()) {
        ref_pic_list_modification_flag_l0_ = rbsp_data_->read_u1();
        spdlog::info("ref_pic_list_modification_flag_l0 {}", (bool)ref_pic_list_modification_flag_l0_);
        if (ref_pic_list_modification_flag_l0_) {
            spdlog::trace("will parse ref_pic_list_modification 0");
            ret = parse_single_ref_pic_list_modification(0);
        }
    }
    if (is_B_slice()) {
        ref_pic_list_modification_flag_l1_ = rbsp_data_->read_u1();
        spdlog::info("ref_pic_list_modification_flag_l1 {}", (bool)ref_pic_list_modification_flag_l1_);
        if (ref_pic_list_modification_flag_l1_) {
            spdlog::trace("will parse ref_pic_list_modification 1");
            ret = parse_single_ref_pic_list_modification(1);
        }
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
        spdlog::trace("{}: this slice is a idr pic", __func__);

        no_output_of_prior_pics_flag_ = rbsp_data_->read_u1();
        spdlog::trace("no_output_of_prior_pics_flag {}", (bool)no_output_of_prior_pics_flag_);

        long_term_reference_flag_ = rbsp_data_->read_u1();
        spdlog::trace("long_term_reference_flag {}", (bool)long_term_reference_flag_);
    } else {
        spdlog::trace("{}: this slice is a non-idr ref pic", __func__);

        adaptive_ref_pic_marking_mode_flag_ = rbsp_data_->read_u1();
        spdlog::trace("adaptive_ref_pic_marking_mode_flag {}", (bool)adaptive_ref_pic_marking_mode_flag_);

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

            spdlog::trace("dec_ref_pic_marking:");
            for (int i = 0; i < memory_management_control_operation_list_.size(); i++) {
                spdlog::trace("  memory_management_control_operation {}, {}, {}",
                    std::get<0>(memory_management_control_operation_list_[i]),
                    std::get<1>(memory_management_control_operation_list_[i]),
                    std::get<2>(memory_management_control_operation_list_[i]));
            }
        }
    }
    return 0;
}

int Slice::parse_slice_header(VideoDecoder* decoder)
{
    spdlog::info("**********parse slice header**********");

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

    if (sps_->separate_colour_plane()) {
        colour_plane_id_ = rbsp_data_->read_u(2);
        spdlog::warn("this slice has colour_plane_id {}, check if it is a 4:4:4 chroma format", colour_plane_id_);
    } else
        spdlog::warn("this slice has no colour_plane_id");

    frame_num_ = rbsp_data_->read_u(sps_->frame_num_bits());
    spdlog::warn("frame_num: {}", frame_num_);

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
        spdlog::warn("this slice is not an idr pic, does not have idr_pic_id");

    if (sps_->pic_order_cnt_type() == 0) {
        spdlog::warn("this slice has a poc type of 0");

        pic_order_cnt_lsb_ = rbsp_data_->read_u(sps_->poc_lsb_bits());
        spdlog::warn("pic_order_cnt_lsb: {}", pic_order_cnt_lsb_);

        if (bottom_field_pic_order_in_frame_present()) {
            delta_pic_order_cnt_bottom_ = rbsp_data_->read_se();
            spdlog::warn("delta_pic_order_cnt_bottom present: {}", delta_pic_order_cnt_bottom_);
        }
    }

    if (sps_->pic_order_cnt_type() == 1) {
        spdlog::warn("this slice has a poc type of 1");
        if (!sps_->delta_pic_order_always_zero()) {
            delta_pic_order_cnt_0_ = rbsp_data_->read_se();
            spdlog::warn("delta_pic_order_cnt_0: {}", delta_pic_order_cnt_0_);

            if (bottom_field_pic_order_in_frame_present()) {
                delta_pic_order_cnt_1_ = rbsp_data_->read_se();
                spdlog::warn("delta_pic_order_cnt_1: {}", delta_pic_order_cnt_1_);
            }
        }
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
            spdlog::warn("this slice is P/B slice, need to parse ref_pic_list_modification");
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

    spdlog::info("********end parse slice header********");
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