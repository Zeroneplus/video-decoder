
#pragma once

#include <memory>

class NalUnit;

class InputStream {
public:
    virtual std::shared_ptr<NalUnit> get_nal_unit() = 0;
};