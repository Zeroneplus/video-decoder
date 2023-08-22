

#include <cassert>
#include <iostream>
#include <memory>

#include "spdlog/spdlog.h"

#include "FileStream.h"
#include "VideoDecoder.h"

int main(int argn, char** argv)
{
    spdlog::set_level(spdlog::level::err);

    if (argn < 2) {
        spdlog::error("Too less args");
        spdlog::error("Usage: {} filename", argv[0]);
        return -1;
    }

    const char* file = argv[1];
    std::shared_ptr<InputStream> st = std::make_shared<FileStream>(file);

    VideoDecoder vdec;

    bool has_meet_IDR;

    int ret = 0;

    std::shared_ptr<NalUnit> nal;
    std::shared_ptr<NalUnit::RbspData> rbsp;
    enum NalUnitType nal_unit_type;

    int ctrl_num = 5;

    while ((nal = st->get_nal_unit())) {
        spdlog::warn("============= start new nal =============", nal->size());

        // spdlog::trace("The size of split nal is {}", nal->size());

        rbsp = nal->parse();
        // spdlog::trace("The size of rbsp from nal is {}", rbsp->size());

        rbsp->parse_nal_header();
        nal_unit_type = rbsp->nal_unit_type();
        spdlog::warn("The nal type of rbsp is {}", nal_unit_type_to_char(nal_unit_type));

        switch (nal_unit_type) {
        case NalUnitType::SEI:
            spdlog::trace("Currently we will ignore SEI slice");
            break;
        case NalUnitType::SPS:
            spdlog::trace("Will add a sps slice to our video decoder");
            ret = vdec.add_sps(std::move(rbsp));
            if (ret < 0) {
                spdlog::error("Some error occur when add a sps slice to video decoder");
                goto end;
            }
            break;
        case NalUnitType::PPS:
            spdlog::trace("Will add a pps slice to our video decoder");
            ret = vdec.add_pps(std::move(rbsp));
            if (ret < 0) {
                spdlog::error("Some error occur when add a pps slice to video decoder");
                goto end;
            }
            break;
        case NalUnitType::IDR:
            spdlog::trace("Will add an IDR slice to our video decoder");
            ret = vdec.add_slice(std::move(rbsp));
            if (ret < 0) {
                spdlog::error("Some error occur when add a IDR slice to video decoder");
                goto end;
            }

            // now we will only decode a GOP
            if (!has_meet_IDR)
                has_meet_IDR = true;
            else
                goto end;

            break;
        case NalUnitType::SliceWithoutPartition:
            spdlog::trace("Will add a SliceWithoutPartition to our video decoder");
            ret = vdec.add_slice(std::move(rbsp));
            if (ret < 0) {
                spdlog::error("Some error occur when add a SliceWithoutPartition to video decoder");
                goto end;
            }
            if (--ctrl_num == 0) {
                goto end; // tmp
            }
            break;
        default:
            /* ignore other nal */
            spdlog::warn("Ignore an unknown slice");
            break;
        }
    }
end:
    return 0;
}