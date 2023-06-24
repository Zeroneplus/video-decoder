
#pragma once

#include <memory>
#include <vector>

class NalUnit {
public:
    class RbspData {
    public:
        RbspData(std::vector<uint8_t> input_buf);
        int size()
        {
            return buf.size();
        }

    private:
        std::vector<uint8_t> buf;
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
