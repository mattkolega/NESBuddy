#include "Mapper000.h"

#include "../Cartridge.h"
#include "../../Logger.h"

Mapper000::Mapper000(Cartridge &data) : Mapper(data)
{
}

uint8_t Mapper000::chrRead(uint16_t address)
{
    return cartridge.chrROM[address];
}

void Mapper000::chrWrite(uint16_t address, uint8_t value)
{
    Logger::printError("Illegal memory write operation. Mapper 0 doesn't support CHR writes.");
}

uint8_t Mapper000::prgRead(uint16_t address)
{
    if (cartridge.prgROMBanks == 1) {
        return cartridge.prgROM[address];
    } else {
        return cartridge.prgROM[address & 0x3FFF];
    }
}

void Mapper000::prgWrite(uint16_t address, uint8_t value)
{
    Logger::printError("Illegal memory write operation. Mapper 0 doesn't support PRG writes.");
}