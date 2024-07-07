#include "NES.h"

#include <stdexcept>

#include "Cartridge/Parser.h"
#include "Cartridge/Mappers/Mapper.h"
#include "Cartridge/Mappers/Mapper000.h"
#include "Cartridge/Mappers/NoMapper.h"
#include "CPU/State.h"
#include "Logger.h"

NES::NES()
{
    std::optional<Cartridge> cart;

    int fileOpenAttempts = 0;

    while (!cart.has_value()) {
        cart = ROMParser::openBinaryFile();
        if (++fileOpenAttempts == 3) {
            throw std::runtime_error("User has failed to provide .nes file.");
        }
    }

    this->cartridge = cart.value();

    switch (cartridge.mapperId) {
        case 0:
            mapper = std::make_unique<Mapper000>(cartridge);
            break;
        default:
            Logger::printError("Unrecognised/unsupported mapper number in cartridge.");
            mapper = nullptr;
            break;
    }

    cpu.connectToNes(this);
    cpu.setToPowerUpState();

    ppu.connectToNes(this);
}

NES::NES(CPUState &initialState) : cpu(initialState)
{
    Cartridge cart {};
    cart.prgROM = std::vector<uint8_t>(1024 * 32);
    cart.prgROMBanks = 1;

    cartridge = cart;
    mapper = std::make_unique<NoMapper>(cartridge);

    cpu.connectToNes(this);
}

uint8_t NES::cpuRead(uint16_t address)
{
    if (address >= 0x0000 && address <= 0x1FFF) {
        return memory[address];
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        ppu.registerRead(address % 8);
    } else if (address >= 0x4000 && address <= 0x7FFF) {
        return memory[address];
    } else {
        return mapper->prgRead(address - 0x8000);
    }
}

void NES::cpuWrite(uint16_t address, uint8_t value)
{
    if (address >= 0x0 && address <= 0x1FFF) {
        memory[address] = value;
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        ppu.registerWrite(address % 8, value);
    } else if (address >= 0x4000 && address <= 0x7FFF) {
        memory[address] = value;
    } else {
        mapper->prgWrite(address - 0x8000, value);
    }
}

uint8_t NES::ppuRead(uint16_t address)
{
    if (address >= 0x0 && address <= 0x1FFF) {
        return mapper->chrRead(address);
    }
}

void NES::ppuWrite(uint16_t address, uint8_t value)
{
    if (address >= 0x0 && address <= 0x1FFF) {
        mapper->chrWrite(address, value);
    }
}

PatternTableTiles NES::getPatternTableTiles()
{
    PatternTableTiles tiles;
    int tilesIndex = 0;

    int chrIndex = 0; 

    for (int tileNum = 0; tileNum < 512; tileNum++) {
        std::array<std::array<int, 8>, 8> tile;

        for (int row = 0; row < 8; row++) {
            std::array<int, 8> tileRow;
            uint8_t planeOne = ppuRead(chrIndex);
            uint8_t planeTwo = ppuRead(chrIndex + 8);
            chrIndex++;

            for (int column = 0; column < 8; column++) {
                tileRow[column] = (planeTwo & 0b1) << 1 | (planeOne & 0b1);
                planeOne >>= 1;
                planeTwo >>= 1;
            }

            std::reverse(std::begin(tileRow), std::end(tileRow));  // Reverse array since we started reading from the end of the row
            tile[row] = tileRow;
        }

        tiles[tilesIndex++] = tile; 
        chrIndex += 8;
    }

    return tiles;
}

void NES::tickCPU()
{
    cpu.tick();
}

CPUState NES::getCPUState()
{
    return cpu.getState();
}