#pragma once

#include <array>
#include <cstdint>

#include "CPU/CPU.h"

class NES
{
public:
    NES();
private:
    std::array<uint8_t, 65536> memory {};
    CPU cpu;

    friend class CPU;
};