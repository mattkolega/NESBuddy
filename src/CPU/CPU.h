#pragma once

#include <bitset>
#include <cstdint>

class NES;

enum class Flags : unsigned char
{
    carryFlag         = 0,
    zeroFlag          = 1,
    interruptDisable  = 2,
    decimalMode       = 3,
    breakCommand      = 4,
    overflowFlag      = 6,
    negativeFlag      = 7,
};

class CPU
{
public:
    CPU();
    ~CPU();

    void tick();
private:
    NES *nes { nullptr };

    /* Registers */
    uint16_t pc {};          // Program Counter
    uint8_t sp {};           // Stack Pointer
    uint8_t accumulator {};  // Accumulator
    uint8_t indexX {};       // Index Register X
    uint8_t indexY {};       // Index Register Y

    /* Flags */
    std::bitset<8> processorStatus { 0b0010'0000 };
private:
    void connectToNes(NES *nes);

    uint8_t fetchInstruct();
    void decodeAndExecuteInstruct();
};