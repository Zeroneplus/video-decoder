
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

int Slice::parse_pred_weight_table()
{
    spdlog::trace("start parse pred_weight_table...");

    luma_log2_weight_denom_ = rbsp_data_->read_ue();
    spdlog::trace("luma_log2_weight_denom is {}", luma_log2_weight_denom_);

    if (sps_->ChromaArrayType() != 0) {
        spdlog::trace("will read chroma_log2_weight_denom");

        chroma_log2_weight_denom_ = rbsp_data_->read_ue();
        spdlog::trace("chroma_log2_weight_denom is {}", chroma_log2_weight_denom_);
    }

    return -1;
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

    return ret;
}

int Slice::parse_ref_pic_list_modification()
{
    int ret = 0;
    if (!is_I_slice() && !is_SI_slice()) {
        ref_pic_list_modification_flag_l0_ = rbsp_data_->read_u1();
        if (ref_pic_list_modification_flag_l0_) {
            ret = parse_single_ref_pic_list_modification(0);
        }
    }
    if (is_B_slice()) {
        ref_pic_list_modification_flag_l1_ = rbsp_data_->read_u1();
        if (ref_pic_list_modification_flag_l1_) {
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
        no_output_of_prior_pics_flag_ = rbsp_data_->read_u1();
        long_term_reference_flag_ = rbsp_data_->read_u1();
    } else {
        adaptive_ref_pic_marking_mode_flag_ = rbsp_data_->read_u1();
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
        }
    }
    return 0;
}

int Slice::parse_slice_header(VideoDecoder* decoder)
{
    spdlog::info("parse slice header...");

    first_mb_in_slice_ = rbsp_data_->read_ue();
    spdlog::info("first_mb_in_slice {}", first_mb_in_slice_);

    slice_type_ = rbsp_data_->read_ue();
    slice_type_enum_ = (enum SliceType)slice_type_;
    spdlog::info("slice type: {}", slice_type_str(slice_type_enum_));

    pic_parameter_set_id_ = rbsp_data_->read_ue();
    spdlog::info("pps id for current slice {}", pic_parameter_set_id_);

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
        spdlog::trace("will read colour_plane_id");
        colour_plane_id_ = rbsp_data_->read_u(2);
    }

    if (colour_plane_id_ != INT32_MIN) {
        spdlog::info("has colour_plane_id, {}", colour_plane_id_);
    }

    frame_num_ = rbsp_data_->read_u(sps_->frame_num_bits());
    spdlog::warn("frame_num {}", frame_num_);

    if (!sps_->frame_mbs_only()) {
        spdlog::trace("will read field_pic_flag");
        field_pic_flag_ = rbsp_data_->read_u1();
        if (field_pic_flag_) {
            spdlog::trace("will read bottom_field_flag");
            bottom_field_flag_ = rbsp_data_->read_u1();
        }
    }

    if (field_pic_flag_) {
        if (bottom_field_flag_)
            spdlog::info("this slice is bottom field");
        else
            spdlog::info("this slice is top field");
    } else
        spdlog::info("this slice is frame");

    if (rbsp_data_->idr_pic_flag()) {
        idr_pic_id_ = rbsp_data_->read_ue();
        spdlog::info("this slice is idr pic, idr_pic_id {}", idr_pic_id_);
    } else
        spdlog::info("this slice is not idr pic");

    if (sps_->pic_order_cnt_type() == 0) {
        spdlog::info("slice poc type 0");

        pic_order_cnt_lsb_ = rbsp_data_->read_u(sps_->poc_lsb_bits());
        spdlog::warn("pic_order_cnt_lsb {}", pic_order_cnt_lsb_);

        if (bottom_field_pic_order_in_frame_present()) {
            delta_pic_order_cnt_bottom_ = rbsp_data_->read_se();
            spdlog::warn("delta_pic_order_cnt_bottom present, {}", delta_pic_order_cnt_bottom_);
        }
    }

    if (sps_->pic_order_cnt_type() == 1
        && !sps_->delta_pic_order_always_zero()) {
        spdlog::info("slice poc type 0");

        delta_pic_order_cnt_0_ = rbsp_data_->read_se();
        spdlog::warn("delta_pic_order_cnt_0 {}", delta_pic_order_cnt_0_);

        if (bottom_field_pic_order_in_frame_present()) {
            delta_pic_order_cnt_1_ = rbsp_data_->read_se();
            spdlog::warn("delta_pic_order_cnt_1 {}", delta_pic_order_cnt_1_);
        }
    }

    if (pps_->redundant_pic_cnt_present()) {
        redundant_pic_cnt_ = rbsp_data_->read_ue();
        spdlog::error("redundant_pic_cnt_present {}", redundant_pic_cnt_);
        return -1;
    }

    if (is_B_slice())
        direct_spatial_mv_pred_flag_ = rbsp_data_->read_u1();

    spdlog::info("(B slice only) direct_spatial_mv_pred_flag {}", static_cast<bool>(direct_spatial_mv_pred_flag_));

    if (is_P_slice() || is_SP_slice() || is_B_slice()) {

        // update final_num_ref_idx_l0_active_minus1_ final_num_ref_idx_l1_active_minus1_ from pps
        final_num_ref_idx_l0_active_minus1_ = pps_->num_ref_idx_l0_default_active_minus1();
        if (is_B_slice())
            final_num_ref_idx_l1_active_minus1_ = pps_->num_ref_idx_l1_default_active_minus1();

        num_ref_idx_active_override_flag_ = rbsp_data_->read_u1();
        spdlog::info("num_ref_idx_active_override_flag {}", static_cast<bool>(num_ref_idx_active_override_flag_));

        if (num_ref_idx_active_override_flag_) {
            num_ref_idx_l0_active_minus1_ = rbsp_data_->read_ue();
            spdlog::info("num_ref_idx_l0_active_minus1 {}", num_ref_idx_l0_active_minus1_);

            final_num_ref_idx_l0_active_minus1_ = num_ref_idx_l0_active_minus1_;

            if (is_B_slice()) {
                num_ref_idx_l1_active_minus1_ = rbsp_data_->read_ue();
                spdlog::info("num_ref_idx_l1_active_minus1 {}", num_ref_idx_l1_active_minus1_);

                final_num_ref_idx_l1_active_minus1_ = num_ref_idx_l1_active_minus1_;
            }
        }
    }

    if (rbsp_data_->nal_unit_type() == 20 || rbsp_data_->nal_unit_type() == 21) {
        spdlog::error("ref_pic_list_mvc_modification is unsupported");
        return -1;
    } else {
        // ref_pic_list_modification
        parse_ref_pic_list_modification();
    }

    if ((pps_->weighted_pred() && (is_P_slice() || is_SP_slice()))
        || (pps_->weighted_bipred() == 1 && is_B_slice())) {
        spdlog::error("pred_weight_table is unsupported");
        return -1;
    }

    if (rbsp_data_->nal_ref_idc() != 0) {
        // dec_ref_pic_marking
        parse_dec_ref_pic_marking();
    }

    if (pps_->entropy_coding_mode()
        && !is_I_slice()
        && !is_SI_slice())
        cabac_init_idc_ = rbsp_data_->read_ue();

    if (cabac_init_idc_ != INT32_MIN)
        spdlog::info("has cabac_init_idc, {}", cabac_init_idc_);

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
        disable_deblocking_filter_idc_ = rbsp_data_->read_ue();
        if (disable_deblocking_filter_idc_ != 1) {
            slice_alpha_c0_offset_div2_ = rbsp_data_->read_se();
            slice_beta_offset_div2_ = rbsp_data_->read_se();
        }
    }

    if (pps_->num_slice_groups_minus1() > 0) {
        spdlog::error("num_slice_groups_minus1 > 0 is unsupported.");
        return -1;
    }
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