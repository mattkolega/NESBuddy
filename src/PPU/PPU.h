#pragma once

#include <array>
#include <bitset>

class Colour {
public:
    int r;
    int g;
    int b;

    Colour(int r, int g, int b);
    Colour(int hexcode);
};

class NES;

class PPU
{
public:
    PPU();

    void connectToNes(NES *nes);

    uint8_t registerRead(int registerNum);
    void registerWrite(int registerNum, uint8_t value);

    void tick();
private:
    NES *nes { nullptr };

    /* Registers */
    std::bitset<8> ppuctrl   {};
    std::bitset<8> ppumask   {};
    std::bitset<8> ppustatus {};
    uint8_t        oamaddr   {};
    uint8_t        oamdata   {};
    uint8_t        ppuscroll {};
    uint8_t        ppuaddr   {};
    uint8_t        ppudata   {};
    uint8_t        oamdma    {};

    std::array<uint8_t, 256> oam {};
};

/**
 * Palette containing all possible display colours
 * Duplicate of BMF Final 3 https://emulation.gametechwiki.com/index.php/Famicom_color_palette
*/
const std::array<Colour, 64> palette {
    Colour(0x686868), Colour(0x001299), Colour(0x1A08AA), Colour(0x51029A),  // $00, $01, $02, $03
    Colour(0x7E0069), Colour(0x8E001C), Colour(0x7E0301), Colour(0x511800),  // $04, $05, $06, $07
    Colour(0x1F3700), Colour(0x014E00), Colour(0x00A500), Colour(0x00501C),  // $08, $09, $0A, $0B
    Colour(0x004061), Colour(0x000000), Colour(0x000000), Colour(0x000000),  // $0C, $0D, $0E, $0F
    Colour(0xB9B9B9), Colour(0x0C5CD7), Colour(0x5035F0), Colour(0x8919E0),  // $10, $11, $12, $13
    Colour(0xBB0CB3), Colour(0xCE0C61), Colour(0xC02B0E), Colour(0x954D01),  // $14, $15, $16, $17
    Colour(0x616F00), Colour(0x1F8B00), Colour(0x01980C), Colour(0x00934B),  // $18, $19, $1A, $1B
    Colour(0x00819B), Colour(0x000000), Colour(0x000000), Colour(0x000000),  // $1C, $1D, $1E, $1F
    Colour(0xFFFFFF), Colour(0x63B4FF), Colour(0x9B91FF), Colour(0xD377FF),  // $20, $21, $22, $23
    Colour(0xEF6AFF), Colour(0xF968C0), Colour(0xF97D6C), Colour(0xED9B2D),  // $24, $25, $26, $27
    Colour(0xBDBD16), Colour(0x7CDA1C), Colour(0x4BE847), Colour(0x35E591),  // $28, $29, $2A, $2B
    Colour(0x3FD9DD), Colour(0x606060), Colour(0x000000), Colour(0x000000),  // $2C, $2D, $2E, $2F
    Colour(0xFFFFFF), Colour(0xACE7FF), Colour(0xD5CDFF), Colour(0xEDBAFF),  // $30, $31, $32, $33
    Colour(0xF8B0FF), Colour(0xFEB0EC), Colour(0xFDBFB5), Colour(0xF9D28E),  // $34, $35, $36, $37
    Colour(0xE8E87C), Colour(0xBBF382), Colour(0x99F7A2), Colour(0x8AF5D0),  // $38, $39, $3A, $3B
    Colour(0x92F4F1), Colour(0xBEBEBE), Colour(0x000000), Colour(0x000000)   // $3C, $3D, $3E, $3F
};