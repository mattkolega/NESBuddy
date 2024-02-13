#pragma once

#include <array>
#include <cstdint>

#include "CPU/CPU.h"

class NES
{
public:
    NES();
    NES(CPUState &initialState);

    uint8_t getMemoryValue(uint16_t address);
    void setMemoryValue(uint16_t address, uint8_t value);

    CPUState getCPUState();
private:
    std::array<uint8_t, 64 * 1024> memory {};
    CPU cpu;

    friend class CPU;
};