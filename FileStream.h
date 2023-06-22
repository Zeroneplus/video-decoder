
#pragma once

#include <stdio.h>

#include <string>
#include <utility>

#include "InputStream.h"

class FileStream : public InputStream {
public:
    FileStream(std::string filename);
    ~FileStream();
    std::shared_ptr<NalUnit> get_nal_unit() override;

private:
    int read_from_stream();
    std::pair<int, int> find_start_code(int start);
    std::string file_name;
    FILE* file_pointer;
    int buffer_size;
    int current_data_size;
    char* buffer_addr;
    static constexpr int read_size = 1024;
};