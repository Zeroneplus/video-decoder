
#include "Slice.h"

Slice::Slice(std::shared_ptr<NalUnit::RbspData> rbsp)
    : rbsp_data(std::move(rbsp))
{
}
