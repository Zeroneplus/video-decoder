
#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>

enum NalUnitType {
    Unspecified = 0,
    SliceWithoutPartition,
    SliceWithPartition_A,
    SliceWithPartition_B,
    SliceWithPartition_C,
    IDR,
    SEI,
    SPS,
    PPS,
    AccessUnitDelimiter,
    EndOfSequence,
    EndOfStream,
    FillerData,
    SPSExtension,
    PrefixNALUnit,
    SubsetSPS,
    Reserve16,
    Reserve17,
    Reserve18,
};

int nal_unit_type_to_int(enum NalUnitType nal_unit_type);

const char* nal_unit_type_to_char(enum NalUnitType nal_unit_type);

enum NalUnitType int_to_nal_unit_type(int num);

class NalUnit {
public:
    class RbspData {
    public:
        RbspData(std::vector<uint8_t> input_buf);
        int size()
        {
            return buf.size();
        }

        bool byte_aligned()
        {
            return bits_left == 8;
        }

        bool eof()
        {
            return p >= end;
        }

        uint32_t read_u1();

        void skip_u1();

        uint32_t peek_u1();

        uint32_t read_u(int n);

        void skip_u(int n);

        uint32_t read_ue();

        int32_t read_se();

        void parse_nal_header();

        NalUnitType nal_unit_type()
        {
            return nal_unit_type_;
        }
        bool idr_pic_flag()
        {
            return idr_pic_flag_;
        }
        int nal_ref_idc()
        {
            return nal_ref_idc_;
        }

        bool check_trailing_bits()
        {
            if (read_u1() == 1) {
                while (!eof()) {
                    if (read_u1() != 0)
                        return false;
                }
                return true;
            }
            return false;
        }

    private:
        std::vector<uint8_t> buf;
        uint8_t* start;
        uint8_t* p;
        uint8_t* end;
        int bits_left;
        int nal_ref_idc_;
        NalUnitType nal_unit_type_;
        bool idr_pic_flag_;
    };

    NalUnit(int size);
    ~NalUnit();
    void copy(void* addr);
    std::shared_ptr<RbspData> parse();
    int size() { return data_size; }

private:
    void* buf;
    int data_size;
};
