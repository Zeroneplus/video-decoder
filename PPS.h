
#pragma once

#include "NalUnit.h"

class Pps {
public:
    Pps() = default;
    Pps(std::shared_ptr<NalUnit::RbspData> rbsp);
    int parse();

private:
    std::shared_ptr<NalUnit::RbspData> rbsp_data;
};