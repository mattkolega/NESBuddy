#pragma once

#include <array>
#include <cstdint>

#include "CPU.h"

class NES
{
public:
    NES();
    ~NES();
private:
    std::array<uint8_t, 65536> memory {};
    CPU cpu;
};