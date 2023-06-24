
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <utility>

#include "NalUnit.h"

NalUnit::NalUnit(int size)
    : buf(malloc(size))
    , data_size(size)
{
}

NalUnit::~NalUnit()
{
    if (buf)
        free(buf);
}

void NalUnit::copy(void* addr)
{
    memcpy(buf, addr, data_size);
}

NalUnit::RbspData::RbspData(std::vector<uint8_t> input_buf)
    : buf(std::move(input_buf))
{
    if (buf.empty())
        std::cout << "the Rbsp Data is empty" << std::endl;
}

std::shared_ptr<NalUnit::RbspData> NalUnit::parse()
{
    uint8_t* start = static_cast<uint8_t*>(buf);
    int size = data_size;
    int increment = 0;

    while (*start == 0) {
        ++start;
        ++increment;
        --size;
    }

    if (*start == 1) {
        ++start;
        ++increment;
        --size;
    } else {
        std::cout << "cannot find 0x01" << std::endl;
        return nullptr;
    }

    if (size <= 0) {
        std::cout << "after remove the start code, data size <= 0" << std::endl;
        return nullptr;
    }

    if (increment != 3 && increment != 4) {
        std::cout << "the size of the start code is not 3 or 4" << std::endl;
        return nullptr;
    }

    if (size >= 2 && *(start + size - 1) == 3
        && *(start + size - 2) == 0) {
        --size;
        std::cout << "will remove the last byte for 0x00 0x03 in cabac mode" << std::endl;
    }

    std::vector<uint8_t> tmp(size);
    int k = -1;
    int pre = 0;
    for (int i = 0; i < size; i++) {
        tmp[++k] = *(start + i);
        if (i >= 2
            && i - 2 >= pre
            && *(start + i) == 3
            && *(start + i - 1) == 0
            && *(start + i - 2) == 0) {
            k -= 3;
            pre = i + 2;
        }
    }
    tmp.resize(k + 1);
//    uint8_t* v = tmp.data();
//    int ss = k + 1;
//    std::cout << "ss size is " << ss << std::endl;

    std::shared_ptr<RbspData> res(new RbspData(std::move(tmp)));
    return res;
}