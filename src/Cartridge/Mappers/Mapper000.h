#pragma once

#include <array>

#include "Mapper.h"

/**
 *  Mapper 0: NROM
 *  https://www.nesdev.org/wiki/NROM
*/

class Mapper000 : public Mapper
{
public:
    Mapper000(Cartridge &data);

    uint8_t chrRead(uint16_t address) override;
    void chrWrite(uint16_t address, uint8_t value) override;
    uint8_t prgRead(uint16_t address) override;
    void prgWrite(uint16_t address, uint8_t value) override;
};