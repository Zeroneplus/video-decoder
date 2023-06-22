

#include <iostream>
#include <memory>

#include "FileStream.h"
#include "NalUnit.h"

int main(int argn, char** argv)
{
    //    if (argn < 2){
    //        std::cout << "too less args" << std::endl;
    //        return -1;
    //    }
    const char* file = "/home/cdzhang/open_source_proj/video-decoder/test/trailer.h264";
    std::shared_ptr<InputStream> st = std::make_shared<FileStream>(file);
    std::shared_ptr<NalUnit> nal;
    while (nal = st->get_nal_unit()) {
        std::cout << "the size of nal is " << nal->size() << std::endl;
    }
}