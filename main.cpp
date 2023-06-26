

#include <iostream>
#include <memory>

#include "FileStream.h"
#include "VideoDecoder.h"

int main(int argn, char** argv)
{
    //    if (argn < 2){
    //        std::cout << "too less args" << std::endl;
    //        return -1;
    //    }
    const char* file = "./test/trailer.h264";
    std::shared_ptr<InputStream> st = std::make_shared<FileStream>(file);
    std::shared_ptr<NalUnit> nal;
    std::shared_ptr<NalUnit::RbspData> rbsp;
    VideoDecoder vdec;
    enum NalUnitType nal_unit_type;
    while ((nal = st->get_nal_unit())) {
        // std::cout << "the size of nal is " << nal->size() << std::endl;
        rbsp = nal->parse();
        // std::cout << "the size of rbsp is " << rbsp->size() << std::endl;
        rbsp->parse_nal_header();
        nal_unit_type = rbsp->nal_type();
        switch (nal_unit_type) {
        case NalUnitType::SliceWithoutPartition:

        case NalUnitType::IDR:

        case NalUnitType::SPS:
            vdec.add_sps(std::move(rbsp));
            goto end;
        case NalUnitType::PPS:

        default:
            /* ignore other nal */
            break;
        }
    }
end:
    return 0;
}