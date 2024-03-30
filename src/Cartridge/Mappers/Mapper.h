#pragma once

#include <array>
#include <cstdint>
#include <vector>

typedef struct Cartridge Cartridge;

class Mapper
{
public:
    Mapper(Cartridge &cart);

    virtual uint8_t prgRead(uint16_t address) = 0;
    virtual void prgWrite(uint16_t address, uint8_t value) = 0;

protected:
    Cartridge &cartridge;
};