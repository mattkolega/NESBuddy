#include "Mapper000.h"

#include "../Cartridge.h"

Mapper000::Mapper000(Cartridge &data) : Mapper(data)
{
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
}