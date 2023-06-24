
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <memory>
#include <string>

#include "FileStream.h"
#include "NalUnit.h"

FileStream::FileStream(std::string filename)
    : file_name(std::move(filename))
    , file_pointer(nullptr)
    , buffer_size(0)
    , current_data_size(0)
    , buffer_addr(nullptr)
{
    int ret = 0;

    file_pointer = fopen(file_name.c_str(), "rb");

    if (!file_pointer) {
        std::cout << "error when open input file" << file_name;
        abort();
    }
}

FileStream::~FileStream()
{
    if (buffer_addr)
        free(buffer_addr);
    if (file_pointer)
        fclose(file_pointer);
}

int FileStream::read_from_stream()
{
    char* cur_pos = nullptr;
    int ret;

    if (!file_pointer) {
        std::cout << "no input file when read from stream";
        return -1;
    }

    if (current_data_size > buffer_size)
        return -1;
    else if (buffer_size - current_data_size < read_size) {
        buffer_addr = (char*)realloc(buffer_addr, buffer_size + read_size);
        if (!buffer_addr) {
            std::cout << "realloc error";
            return -1;
        }
        buffer_size += read_size;
    }

    cur_pos = buffer_addr + current_data_size;

    ret = fread(cur_pos, 1, read_size, file_pointer);

    if (ret > 0)
        current_data_size += ret;
    else
        return -1;

    return 1;
}

std::pair<int, int> FileStream::find_start_code(int start)
{
    int offset = start;
    int ret = -1, next = -1;
    while (offset < current_data_size) {
        if ((offset - start + 1 >= 3)
            && (uint8_t)buffer_addr[offset] == 1
            && (uint8_t)buffer_addr[offset - 1] == 0
            && (uint8_t)buffer_addr[offset - 2] == 0) {
            ret = offset - 2;
            next = offset + 1;
            if ((offset - start + 1 >= 4)
                && (uint8_t)buffer_addr[offset - 3] == 0)
                ret--;
            break;
        }
        offset++;
    }
    return std::make_pair(ret, next);
}

std::shared_ptr<NalUnit> FileStream::get_nal_unit()
{
    // TODO: the last data after eof should be sent
    int ret = 0, first_start, second_start, next_search;
    int len;
    std::pair<int, int> found_result;
    std::shared_ptr<NalUnit> res;

    while (true) {
        found_result = find_start_code(0);
        if (found_result.first < 0 || found_result.second < 0) {
            if (read_from_stream() < 0)
                return nullptr;
            continue;
        }
        first_start = found_result.first;
        next_search = found_result.second;
        break;
    }
    if (first_start) {
        std::cout << "first start for first search is not 0";
        return nullptr;
    }

    while (true) {
        found_result = find_start_code(next_search);
        if (found_result.first < 0 || found_result.second < 0) {
            if (read_from_stream() < 0)
                return nullptr;
            continue;
        }
        second_start = found_result.first;
        break;
    }
    len = second_start - first_start;
    res = std::make_shared<NalUnit>(len);
    res->copy(buffer_addr);
    memmove(buffer_addr, buffer_addr + len, current_data_size - len);
    current_data_size -= len;
    return res;
}