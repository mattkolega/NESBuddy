#include "CPU.h"

#include "../NES.h"

/**
 * Load/Store Operations
*/

int CPU::LDA(uint8_t value, int clockCycles)
{
    accumulator = value;
    setZN(accumulator);
    return clockCycles;
}

int CPU::LDX(uint8_t value, int clockCycles)
{
    indexX = value;
    setZN(indexX);
    return clockCycles;
}

int CPU::LDY(uint8_t value, int clockCycles)
{
    indexY = value;
    setZN(indexY);
    return clockCycles;
}

int CPU::STA(uint16_t address, int clockCycles)
{
    nes->memory[address] = accumulator;
    return clockCycles;
}

int CPU::STX(uint16_t address, int clockCycles)
{
    nes->memory[address] = indexX;
    return clockCycles;
}

int CPU::STY(uint16_t address, int clockCycles)
{
    nes->memory[address] = indexY;
    return clockCycles;
}

/**
 * Stack Operations
*/

int CPU::PHA(int clockCycles)
{
    nes->memory[sp] = accumulator;
    sp--;
    return clockCycles;
}

int CPU::PHP(int clockCycles)
{
    nes->memory[sp] = static_cast<uint8_t>(processorStatus.to_ulong());
    return clockCycles;
}

int CPU::PLA(int clockCycles)
{
    sp++;
    accumulator = nes->memory[sp];
    setZN(accumulator);
    return clockCycles;
}

int CPU::PLP(int clockCycles)
{
    sp++;
    processorStatus = nes->memory[sp];
    return clockCycles;
}

int CPU::TSX(int clockCycles)
{
    indexX = sp;
    setZN(indexX);
    return clockCycles;
}

int CPU::TXS(int clockCycles)
{
    sp = indexX;
    return clockCycles;
}

/**
 * Logical
*/

int CPU::AND(uint8_t value, int clockCycles)
{
    accumulator &= value;
    setZN(accumulator);
    return clockCycles;
}

int CPU::BIT(uint8_t value, int clockCycles)
{   
    uint8_t result = accumulator & value;
    setZN(result);
    
    bool overflow = std::bitset<8>(result).test(6);

    if (overflow) {
        processorStatus.set(static_cast<size_t>(Flags::overflowFlag));
    }

    return clockCycles;
}

int CPU::EOR(uint8_t value, int clockCycles)
{
    accumulator ^= value;
    setZN(accumulator);
    return clockCycles;
}

int CPU::ORA(uint8_t value, int clockCycles)
{
    accumulator |= value;
    setZN(accumulator);
    return clockCycles;
}

/**
 * Register Transfers
*/

int CPU::TAX(int clockCycles)
{
    indexX = accumulator;
    setZN(indexX);
    return clockCycles;
}

int CPU::TAY(int clockCycles)
{
    indexY = accumulator;
    setZN(indexY);
    return clockCycles;
}

int CPU::TXA(int clockCycles)
{
    accumulator = indexX;
    setZN(accumulator);
    return clockCycles;
}

int CPU::TYA(int clockCycles)
{
    accumulator = indexY;
    setZN(accumulator);
    return clockCycles;
}

/**
 * Status Flag Changes
*/

int CPU::CLC(int clockCycles)
{
    processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
    return clockCycles;
}

int CPU::CLD(int clockCycles)
{
    processorStatus.reset(static_cast<size_t>(Flags::decimalMode));
    return clockCycles;
}

int CPU::CLI(int clockCycles)
{
    processorStatus.reset(static_cast<size_t>(Flags::interruptDisable));
    return clockCycles;
}

int CPU::CLV(int clockCycles)
{
    processorStatus.reset(static_cast<size_t>(Flags::overflowFlag));
    return clockCycles;
}

int CPU::SEC(int clockCycles)
{
    processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    return clockCycles;
}

int CPU::SED(int clockCycles)
{
    processorStatus.set(static_cast<size_t>(Flags::decimalMode));
    return clockCycles;
}

int CPU::SEI(int clockCycles)
{
    processorStatus.set(static_cast<size_t>(Flags::interruptDisable));
    return clockCycles;
}