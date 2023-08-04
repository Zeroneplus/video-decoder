
#include <stdlib.h>
#include <string.h>

#include <cassert>
#include <iostream>
#include <utility>

#include "NalUnit.h"

struct CodedBlockPatternMap {
    int codeNum = -1;
    int Intra_4x4_or_Intra_8x8 = -1;
    int Inter = -1;
};

static bool disable_over_read_warn = false;

static const CodedBlockPatternMap chroma_array_1_2_me[] = {
    { 0, 47, 0 },
    { 1, 31, 16 },
    { 2, 15, 1 },
    { 3, 0, 2 },
    { 4, 23, 4 },
    { 5, 27, 8 },
    { 6, 29, 32 },
    { 7, 30, 3 },
    { 8, 7, 5 },
    { 9, 11, 10 },
    { 10, 13, 12 },
    { 11, 14, 15 },
    { 12, 39, 47 },
    { 13, 43, 7 },
    { 14, 45, 11 },
    { 15, 46, 13 },
    { 16, 16, 14 },
    { 17, 3, 6 },
    { 18, 5, 9 },
    { 19, 10, 31 },
    { 20, 12, 35 },
    { 21, 19, 37 },
    { 22, 21, 42 },
    { 23, 26, 44 },
    { 24, 28, 33 },
    { 25, 35, 34 },
    { 26, 37, 36 },
    { 27, 42, 40 },
    { 28, 44, 39 },
    { 29, 1, 43 },
    { 30, 2, 45 },
    { 31, 4, 46 },
    { 32, 8, 17 },
    { 33, 17, 18 },
    { 34, 18, 20 },
    { 35, 20, 24 },
    { 36, 24, 19 },
    { 37, 6, 21 },
    { 38, 9, 26 },
    { 39, 22, 28 },
    { 40, 25, 23 },
    { 41, 32, 27 },
    { 42, 33, 29 },
    { 43, 34, 30 },
    { 44, 36, 22 },
    { 45, 40, 25 },
    { 46, 38, 38 },
    { 47, 41, 41 }
};

// TODO
// static const CodedBlockPatternMap chroma_array_0_3 [] = {};

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
    } else if (!disable_over_read_warn) {
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
        std::cout << "peek_u1 after eof" << std::endl;
    }
    return r;
}

uint32_t NalUnit::RbspData::peek_u(int n)
{
    uint8_t* tmp_p = p;
    int tmp_bits_left = bits_left;

    disable_over_read_warn = true;
    int ret = read_u(n);
    disable_over_read_warn = false;

    p = tmp_p;
    bits_left = tmp_bits_left;

    return ret;
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

uint32_t NalUnit::RbspData::read_te(int range)
{
    uint32_t b = 0;
    int ret;

    if (range <= 0)
        return 0;

    if (range == 1) {
        b = read_u1();
        return b == 0 ? 1 : 0;
    }

    ret = read_ue();

    assert(ret <= range);

    return ret;
}

int NalUnit::RbspData::read_me(bool is_Inter)
{
    // only consider ChromaArrayType is equal to 1 or 2
    uint32_t t = read_ue();

    assert(t == chroma_array_1_2_me[t].codeNum);

    return is_Inter ? chroma_array_1_2_me[t].Inter : chroma_array_1_2_me[t].Intra_4x4_or_Intra_8x8;
}

uint32_t NalUnit::RbspData::read_level_prefix()
{
    int leadingZeroBits = -1;
    int level_prefix;

    for (int b = 0; !b; leadingZeroBits++)
        b = read_u1();
    level_prefix = leadingZeroBits;
    return level_prefix;
}

bool NalUnit::RbspData::more_rbsp_data()
{
    if (eof())
        return false;

    if (p < end - 1)
        return true;

    int mask = (1 << bits_left) - 1;
    int s = 1 << (bits_left - 1);

    if (s == ((*p) & mask))
        return false;
    else
        return true;
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
    int k = 0;
    for (int i = 0; i < size; i++) {
        if (i + 2 < size && *(start + i) == 0
            && *(start + i + 1) == 0
            && *(start + i + 2) == 3) {
            tmp[k++] = 0;
            tmp[k++] = 0;
            i += 2;
        } else
            tmp[k++] = *(start + i);
    }

    tmp.resize(k);
    //    uint8_t* v = tmp.data();
    //    int ss = k + 1;
    //    std::cout << "ss size is " << ss << std::endl;

    std::shared_ptr<RbspData> res(new RbspData(std::move(tmp)));
    return res;
}