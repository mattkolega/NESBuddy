#pragma once

#include <array>
#include <cstdint>

#include "Cartridge/Mappers/Mapper.h"
#include "Cartridge/Cartridge.h"
#include "CPU/CPU.h"
#include "PPU/PPU.h"

using PatternTableTiles = std::array<std::array<std::array<int, 8>, 8>, 512>;

class NES
{
public:
    NES();
    NES(CPUState &initialState);

    uint8_t cpuRead(uint16_t address);
    void cpuWrite(uint16_t address, uint8_t value);
    uint8_t ppuRead(uint16_t address);
    void ppuWrite(uint16_t address, uint8_t value);

    void tickCPU();

    CPUState getCPUState();

    PatternTableTiles getPatternTableTiles();

private:
    std::array<uint8_t, 64 * 1024> memory {};

    CPU cpu;
    PPU ppu;
    
    Cartridge cartridge;
    std::unique_ptr<Mapper> mapper;
    
    friend class CPU;
    friend class PPU;
};