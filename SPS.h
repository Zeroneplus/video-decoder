
#pragma once

#include "NalUnit.h"

class Sps {
public:
    Sps() = default;
    Sps(std::shared_ptr<NalUnit::RbspData> rbsp);
    int parse();

private:
    std::shared_ptr<NalUnit::RbspData> rbsp_data;
};