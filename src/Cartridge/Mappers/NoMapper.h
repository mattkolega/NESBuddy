#pragma once

#include <array>

#include "Mapper.h"

/**
 *  This class is intented to be used for testing with reading and writing
 *  to memory with no extra functionality.
 *  Not to be used for actual NES emulation.
 *  Not to be confused with Mapper 0 which is commonly described as no mapper.
*/

class NoMapper : public Mapper
{
public:
    NoMapper(Cartridge &data);

    uint8_t chrRead(uint16_t address) override;
    void chrWrite(uint16_t address, uint8_t value) override;
    uint8_t prgRead(uint16_t address) override;
    void prgWrite(uint16_t address, uint8_t value) override;
};