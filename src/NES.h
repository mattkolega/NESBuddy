#pragma once

#include <array>
#include <cstdint>

#include "Cartridge/Mappers/Mapper.h"
#include "Cartridge/Cartridge.h"
#include "CPU/CPU.h"

class NES
{
public:
    NES();
    NES(CPUState &initialState);

    uint8_t memoryRead(uint16_t address);
    void memoryWrite(uint16_t address, uint8_t value);

    void tickCPU();

    CPUState getCPUState();

private:
    std::array<uint8_t, 64 * 1024> memory {};

    CPU cpu;
    
    Cartridge cartridge;
    std::unique_ptr<Mapper> mapper;
    
    friend class CPU;
};