#include "PPU.h"

#include <iostream>
#include <array>

#include "../NES.h"

Colour::Colour(int r, int g, int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

Colour::Colour(int hexcode)
{
    r = hexcode >> 16 | 0xFF;
    g = hexcode >> 8  | 0xFF;
    b = hexcode       | 0xFF;
}

PPU::PPU()
{
}

void PPU::connectToNes(NES *nes)
{
    this->nes = nes;
}

uint8_t PPU::registerRead(int registerNum)
{
    switch(registerNum) {
        case 0:
            return static_cast<uint8_t>(ppuctrl.to_ulong());
        case 1:
            return static_cast<uint8_t>(ppumask.to_ulong());
        case 2:
            return static_cast<uint8_t>(ppustatus.to_ulong());
        case 3:
            return oamaddr;
        case 4:
            return oamdata;
        case 5:
            return ppuscroll;
        case 6:
            return ppuaddr;
        case 7:
            return ppudata;
    }
}

void PPU::registerWrite(int registerNum, uint8_t value)
{
    switch(registerNum) {
        case 0:
            ppuctrl = value;
        case 1:
            ppumask = value;
        case 2:
            ppustatus = value;
        case 3:
            oamaddr = value;
        case 4:
            oamdata = value;
        case 5:
            ppuscroll = value;
        case 6:
            ppuaddr = value;
        case 7:
            ppudata = value;
    }
}