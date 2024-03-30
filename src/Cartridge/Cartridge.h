#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

enum class Nametable
{
    verticalArrangement,
    horizontalArrangement
};

enum class Region
{
    ntsc,
    pal,
    global
};

typedef struct Cartridge
{
    int mapperId {};
    Nametable nametable;
    std::optional<std::array<uint8_t, 512>> trainerData {};
    std::vector<uint8_t> prgROM {};
    std::vector<uint8_t> chrROM {};
    uint16_t prgROMBanks {};
    uint16_t chrROMBanks {};
    std::vector<uint8_t> prgRAM {};
    std::vector<uint8_t> chrRAM {};
    Region region;
} Cartridge;