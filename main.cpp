

#include <iostream>
#include <memory>

#include "FileStream.h"
#include "VideoDecoder.h"

int main(int argn, char** argv)
{
    if (argn < 2) {
        std::cout << "too less args" << std::endl;
        return -1;
    }

    const char* file = argv[1];
    std::shared_ptr<InputStream> st = std::make_shared<FileStream>(file);
    std::shared_ptr<NalUnit> nal;
    std::shared_ptr<NalUnit::RbspData> rbsp;
    VideoDecoder vdec;
    enum NalUnitType nal_unit_type;
    bool has_meet_IDR;
    int ret = 0;
    while ((nal = st->get_nal_unit())) {
        // std::cout << "the size of nal is " << nal->size() << std::endl;
        rbsp = nal->parse();
        // std::cout << "the size of rbsp is " << rbsp->size() << std::endl;
        rbsp->parse_nal_header();
        nal_unit_type = rbsp->nal_type();
        switch (nal_unit_type) {
        case NalUnitType::SEI:
            break;
        case NalUnitType::SPS:
            ret = vdec.add_sps(std::move(rbsp));
            if (ret < 0) {
                std::cout << "vdec.add_sps error" << std::endl;
                goto end;
            }
            break;
        case NalUnitType::PPS:
            ret = vdec.add_pps(std::move(rbsp));
            if (ret < 0) {
                std::cout << "vdec.add_pps error" << std::endl;
                goto end;
            }
            break;
        case NalUnitType::IDR:
            ret = vdec.add_slice(std::move(rbsp));
            if (ret < 0) {
                std::cout << "vdec.add_IDR_slice error" << std::endl;
                goto end;
            }
            if (!has_meet_IDR)
                has_meet_IDR = true;
            else
                goto end;
            break;
        case NalUnitType::SliceWithoutPartition:
            ret = vdec.add_slice(std::move(rbsp));
            if (ret < 0) {
                std::cout << "vdec.add_SliceWithoutPartition error" << std::endl;
                goto end;
            }
            goto end; // 
            break;
        default:
            /* ignore other nal */
            break;
        }
    }
end:
    return 0;
}