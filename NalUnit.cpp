
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "NalUnit.h"

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