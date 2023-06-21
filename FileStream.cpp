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

#include <iostream>
#include <utility>
#include <memory>

FileStream::(std::string filename):
    file_name(std::move(filename)),
    file_pointer(nullptr),
    file_pos(0),
    buffer_size(0),
    current_data_size(0),
    buffer_addr(nullptr),
{
    int ret = 0;

    file_pointer = fopen(file_name.c_str(), "rb");

    if (!file_pointer)
    {
        std::cout << "error when open input file" << file_name;
        abort();
    }
}

FileStream::~FileStream() {
    if (buffer_addr)
        free(buffer_addr);
    if (file_pointer)
        fclose(file_pointer);
}

int FileStream::read_from_stream() {
    char *cur_pos = nullptr;
    int ret;

    if (!file_pointer)
    {
        std::cout << "no input file when read from stream";
        return -1;
    }
    if (current_data_size > buffer_size)
        return -1;
    else if (buffer_size - current_data_size < read_size)
    {
        buffer_addr = realloc(buffer_addr, buffer_size + read_size);
        if (!buffer_addr) {
            std::cout << "realloc error";
            return -1;
        }
        buffer_size += read_size;
    }

    cur_pos = buffer_addr + current_data_size;

    ret = fread(cur_pos, 1, read_size, file_pointer);

    current_data_size += read_size;

    return 1;
}

std::pair<int, int> FileStream::find_start_code(int start) {
    int offset = start;
    int ret = -1, next = -1;
    while(offset < current_data_size){
        if (   (offset - start + 1 >= 3)
            && (uint8_t)buffer_addr[offset] == 3
            && (uint8_t)buffer_addr[offset - 1] == 0
            && (uint8_t)buffer_addr[offset - 2] == 0) {
            ret = offset - 2;
            next = offset + 1;
            if (   (offset - start + 1 >= 4)
                && (uint8_t)buffer_addr[offset - 3] == 0)
                ret--;
            break;
        }
    }
    return make_pair<int, int>(ret, next);
}

std::shared_ptr<NalUnit> FileStream::get_nal_unit() {
    int ret = 0, first_start, second_start;
    std::pair<int, int> found_result;

    while(true){
        ret = read_from_stream();
        if (ret < 0)
            return nullptr;

        found_result = find_start_code(0);
        if (found_result.first < 0 || found_result.second < 0)
            continue;
        first_start = found_result.first;

        found_result = find_start_code(found_result.second);
        if (found_result.first < 0 || found_result.second < 0)
            continue;
        second_start = found_result.first;
        break;
    }
    return nullptr;
}