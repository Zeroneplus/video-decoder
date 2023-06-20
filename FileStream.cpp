class FileStream: public InputStream {
public:
    FileStream() = default;
    FileStream(std::string filename);
    ~FileStream();
    std::shared_ptr<NalUnit> get_nal_unit() override;

private:
    std::string file_name;
    FILE *file_pointer;
    int file_pos;
    int buffer_size;
    char *buffer_addr;
    static constexpr read_size = 1024;
};

#include <iostream>
#include <utility>
#include <memory>

FileStream::(std::string filename):
    file_name(std::move(filename)),
    file_pointer(nullptr),
    file_pos(0),
    buffer_size(0),
    buffer_addr(nullptr)
{
    int ret = 0;

    if ((file_pointer = fopen(file_name.c_str(), "rb")) == nullptr)
    {
        std::cout << "error when open input file" << file_name;
    }
}

int FileStream::read_from_stream() {
    char *cur_pos = nullptr;
    int ret;
    if (!file_pointer)
    {
        std::cout << "no input file when read from stream" << file_name;
        return -1;
    }
    buffer_addr = realloc(buffer_addr, read_size);
    cur_pos = buffer_addr + buffer_size;

    ret = fread(cur_pos, 1, read_size, file_pointer);

if (ret > 0) {
    buffer_size += ret;
    file_pos += ret;
}

    if (ret <= 0)
        return ret;
    return 1;
}

int FileStream::find_start_code(int offset) {

    while(offset < buffer_size){
        if (offset >= 2) {

        }
        else if (offset >= 3) {

        }
    }

}

std::shared_ptr<NalUnit> FileStream::get_nal_unit() {
    bool found_first_start_code = false;
    bool found_second_start_code = false;
    int ret = 0;
    while(true){
        if (!buffer_addr)
            ret = read_from_stream();
        if (ret <= 0)
            break;
        
    }

    if (found_first_start_code && found_second_start_code)
        return make_shared<NalUnit>();
    else if (ret < 0 && found_first_start_code)
        return make_shared<NalUnit>();
}