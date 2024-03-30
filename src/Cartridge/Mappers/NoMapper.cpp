#include "NoMapper.h"

#include "../Cartridge.h"

NoMapper::NoMapper(Cartridge &data) : Mapper(data)
{
}

uint8_t NoMapper::prgRead(uint16_t address)
{
    return cartridge.prgROM[address];
}

void NoMapper::prgWrite(uint16_t address, uint8_t value)
{
    cartridge.prgROM[address] = value;
}