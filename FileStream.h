
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include <string>
#include <memory>

#include "InputStream.h"

class FileStream: public InputStream {
public:
    FileStream(std::string filename);
    ~FileStream();
    std::shared_ptr<NalUnit> get_nal_unit() override;

private:
    int read_from_stream();
    std::string file_name;
    FILE *file_pointer;
    int file_pos;
    int buffer_size;
    int current_data_size;
    char *buffer_addr;
    static constexpr read_size = 1024;
};