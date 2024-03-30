#pragma once

#include <array>
#include <fstream>
#include <optional>
#include <string>

#include "Cartridge.h"

enum class FileFormat
{
    iNES,
    NES20,
    invalid
};

enum class PeripheralType
{
    standardController,
    unsupported
};

enum class SystemType
{
    nes,
    playchoice,
    vsSystem,
};

struct NESHeader
{
    uint16_t mapperId;
    Nametable nametable;
    bool trainerPresent;
    uint16_t prgROMBanks;
    uint16_t chrROMBanks;
    uint16_t prgRAMSize;
    uint16_t chrRAMSize;
    Region gameRegion;
    PeripheralType defaultPeripheral;
    SystemType targetSystem;
};

namespace ROMParser 
{
    std::optional<Cartridge> openBinaryFile();
}