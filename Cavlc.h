
// this code is borrowed from 'h264_video_decoder_demo'
//
// TODO: implement a 'real' huffman decoding algo

#pragma once

#include "NalUnit.h"

int coeff_token_table(int nC,
    int coeff_token,
    int& coeff_token_bit_length,
    int& TrailingOnes,
    int& TotalCoeff);

int get_total_zeros(
    std::shared_ptr<NalUnit::RbspData> rbsp_data,
    int maxNumCoeff,
    int tzVlcIndex,
    int& total_zeros);

int get_run_before(
    std::shared_ptr<NalUnit::RbspData> rbsp_data,
    int zerosLeft,
    int& run_before);