#include "CPU.h"

/**
 * Register Transfers
*/

int CPU::TAX(int clockCycles)
{
    indexX = accumulator;

    if (indexX == 0) {
        processorStatus.set(static_cast<size_t>(Flags::zeroFlag));
    }

    bool isNegative = std::bitset<8>(indexX).test(7);

    if (isNegative) {
        processorStatus.set(static_cast<size_t>(Flags::negativeFlag));
    }

    return clockCycles;
}

int CPU::TAY(int clockCycles)
{
    indexY = accumulator;

    if (indexY == 0) {
        processorStatus.set(static_cast<size_t>(Flags::zeroFlag));
    }

    bool isNegative = std::bitset<8>(indexY).test(7);

    if (isNegative) {
        processorStatus.set(static_cast<size_t>(Flags::negativeFlag));
    }

    return clockCycles;
}

int CPU::TXA(int clockCycles)
{
    accumulator = indexX;

    if (accumulator == 0) {
        processorStatus.set(static_cast<size_t>(Flags::zeroFlag));
    }

    bool isNegative = std::bitset<8>(accumulator).test(7);

    if (isNegative) {
        processorStatus.set(static_cast<size_t>(Flags::negativeFlag));
    }

    return clockCycles;
}

int CPU::TYA(int clockCycles)
{
    accumulator = indexY;

    if (accumulator == 0) {
        processorStatus.set(static_cast<size_t>(Flags::zeroFlag));
    }

    bool isNegative = std::bitset<8>(accumulator).test(7);

    if (isNegative) {
        processorStatus.set(static_cast<size_t>(Flags::negativeFlag));
    }   

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