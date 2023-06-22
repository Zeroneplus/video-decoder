
#pragma once

#include <memory>

class NalUnit {
public:
    class RbspData { };

    NalUnit(int size);
    ~NalUnit();
    void copy(void* addr);
    std::shared_ptr<RbspData> parse();
    int size() { return data_size; }

private:
    void* buf;
    int data_size;
};
