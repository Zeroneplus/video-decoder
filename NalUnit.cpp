
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <utility>

#include "NalUnit.h"

int nal_unit_type_to_int(enum NalUnitType nal_unit_type)
{
    int ret = 0;
    switch (nal_unit_type) {
    case NalUnitType::Unspecified:
        ret = 0;
        break;
    case NalUnitType::SliceWithoutPartition:
        ret = 1;
        break;
    case NalUnitType::SliceWithPartition_A:
        ret = 2;
        break;
    case NalUnitType::SliceWithPartition_B:
        ret = 3;
        break;
    case NalUnitType::SliceWithPartition_C:
        ret = 4;
        break;
    case NalUnitType::IDR:
        ret = 5;
        break;
    case NalUnitType::SEI:
        ret = 6;
        break;
    case NalUnitType::SPS:
        ret = 7;
        break;
    case NalUnitType::PPS:
        ret = 8;
        break;
    case NalUnitType::AccessUnitDelimiter:
        ret = 9;
        break;
    case NalUnitType::EndOfSequence:
        ret = 10;
        break;
    case NalUnitType::EndOfStream:
        ret = 11;
        break;
    case NalUnitType::FillerData:
        ret = 12;
        break;
    case NalUnitType::SPSExtension:
        ret = 13;
        break;
    case NalUnitType::PrefixNALUnit:
        ret = 14;
        break;
    case NalUnitType::SubsetSPS:
        ret = 15;
        break;
    case NalUnitType::Reserve16:
        ret = 16;
        break;
    case NalUnitType::Reserve17:
        ret = 17;
        break;
    case NalUnitType::Reserve18:
        ret = 18;
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}

const char* nal_unit_type_to_char(enum NalUnitType nal_unit_type)
{
    const char* ret = nullptr;
    switch (nal_unit_type) {
    case NalUnitType::Unspecified:
        ret = "Unspecified";
        break;
    case NalUnitType::SliceWithoutPartition:
        ret = "SliceWithoutPartition";
        break;
    case NalUnitType::SliceWithPartition_A:
        ret = "SliceWithPartition_A";
        break;
    case NalUnitType::SliceWithPartition_B:
        ret = "SliceWithPartition_B";
        break;
    case NalUnitType::SliceWithPartition_C:
        ret = "SliceWithPartition_C";
        break;
    case NalUnitType::IDR:
        ret = "IDR";
        break;
    case NalUnitType::SEI:
        ret = "SEI";
        break;
    case NalUnitType::SPS:
        ret = "SPS";
        break;
    case NalUnitType::PPS:
        ret = "PPS";
        break;
    case NalUnitType::AccessUnitDelimiter:
        ret = "AccessUnitDelimiter";
        break;
    case NalUnitType::EndOfSequence:
        ret = "EndOfSequence";
        break;
    case NalUnitType::EndOfStream:
        ret = "EndOfStream";
        break;
    case NalUnitType::FillerData:
        ret = "FillerData";
        break;
    case NalUnitType::SPSExtension:
        ret = "SPSExtension";
        break;
    case NalUnitType::PrefixNALUnit:
        ret = "PrefixNALUnit";
        break;
    case NalUnitType::SubsetSPS:
        ret = "SubsetSPS";
        break;
    case NalUnitType::Reserve16:
        ret = "Reserve16";
        break;
    case NalUnitType::Reserve17:
        ret = "Reserve17";
        break;
    case NalUnitType::Reserve18:
        ret = "Reserve18";
        break;
    default:
        ret = "Unknown NalUnitType";
        break;
    }
    return ret;
}

enum NalUnitType int_to_nal_unit_type(int num)
{
    enum NalUnitType ret = NalUnitType::Unspecified;
    switch (num) {
    case 0:
        ret = NalUnitType::Unspecified;
        break;
    case 1:
        ret = NalUnitType::SliceWithoutPartition;
        break;
    case 2:
        ret = NalUnitType::SliceWithPartition_A;
        break;
    case 3:
        ret = NalUnitType::SliceWithPartition_B;
        break;
    case 4:
        ret = NalUnitType::SliceWithPartition_C;
        break;
    case 5:
        ret = NalUnitType::IDR;
        break;
    case 6:
        ret = NalUnitType::SEI;
        break;
    case 7:
        ret = NalUnitType::SPS;
        break;
    case 8:
        ret = NalUnitType::PPS;
        break;
    case 9:
        ret = NalUnitType::AccessUnitDelimiter;
        break;
    case 10:
        ret = NalUnitType::EndOfSequence;
        break;
    case 11:
        ret = NalUnitType::EndOfStream;
        break;
    case 12:
        ret = NalUnitType::FillerData;
        break;
    case 13:
        ret = NalUnitType::SPSExtension;
        break;
    case 14:
        ret = NalUnitType::PrefixNALUnit;
        break;
    case 15:
        ret = NalUnitType::SubsetSPS;
        break;
    case 16:
        ret = NalUnitType::Reserve16;
        break;
    case 17:
        ret = NalUnitType::Reserve17;
        break;
    case 18:
        ret = NalUnitType::Reserve18;
        break;
    default:
        ret = NalUnitType::Unspecified;
        break;
    }
    return ret;
}

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
    , start(buf.data())
    , p(buf.data())
    , end(start + buf.size())
    , bits_left(8)
    , nal_ref_idc_(-1)
    , nal_unit_type_(NalUnitType::Unspecified)
    , idr_pic_flag_(false)
{
    if (buf.empty())
        std::cout << "the Rbsp Data is empty" << std::endl;
}

uint32_t NalUnit::RbspData::read_u1()
{
    uint32_t r = 0;
    if (!eof()) {
        bits_left--;
        r = ((*p) >> bits_left) & 0x01;
        if (bits_left == 0) {
            p++;
            bits_left = 8;
        }
    } else {
        std::cout << "read_u1 after eof" << std::endl;
    }
    return r;
}

void NalUnit::RbspData::skip_u1()
{
    if (!eof()) {
        bits_left--;
        if (bits_left == 0) {
            p++;
            bits_left = 8;
        }
    } else {
        std::cout << "skip_u1 after eof" << std::endl;
    }
}

uint32_t NalUnit::RbspData::peek_u1()
{
    uint32_t r = 0;
    int shift = bits_left - 1;
    if (!eof()) {
        r = ((*p) >> shift) & 0x01;
    } else {
        std::cout << "skip_u1 after eof" << std::endl;
    }
    return r;
}

uint32_t NalUnit::RbspData::read_u(int n)
{
    uint32_t r = 0;
    for (int i = 0; i < n; i++) {
        r |= (read_u1() << (n - 1 - i));
    }
    return r;
}

void NalUnit::RbspData::skip_u(int n)
{
    for (int i = 0; i < n; i++) {
        skip_u1();
    }
}

uint32_t NalUnit::RbspData::read_ue()
{
    int r = 0;
    int i = 0;
    while ((read_u1() == 0) && (i < 32) && !eof())
        i++;
    if (eof()) {
        std::cout << "read_ue after eof" << std::endl;
        return r;
    }
    r = read_u(i);
    r += (1 << i) - 1;
    return r;
}

int32_t NalUnit::RbspData::read_se()
{
    int32_t r = read_ue();
    if (r & 0x01)
        r = (r + 1) / 2;
    else
        r = -(r / 2);
    return r;
}

void NalUnit::RbspData::parse_nal_header()
{
    int forbidden_zero_bit = read_u1();

    if (forbidden_zero_bit)
        std::cout << "forbidden_zero_bit is not equal to 0." << std::endl;

    nal_ref_idc_ = read_u(2);
    nal_unit_type_ = int_to_nal_unit_type(read_u(5));

    idr_pic_flag_ = (nal_unit_type_ == 5) ? true : false;

    if (nal_unit_type_ == 14 || nal_unit_type_ == 20) {
        std::cout << "This nal_unit_type is currently unsupported." << std::endl;
    }

    // std::cout << "forbidden_zero_bit " << forbidden_zero_bit << ", "
    //           << "nal_ref_idc " << nal_ref_idc_ << ", "
    //           << "nal_unit_type " << nal_unit_type_to_char(nal_unit_type_) << "." << std::endl;
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
        if (i - 2 >= pre
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