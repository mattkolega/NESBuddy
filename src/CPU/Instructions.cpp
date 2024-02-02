#include "CPU.h"

/**
 * Register Transfers
*/

void CPU::TAX()
{
    indexX = accumulator;

    if (indexX == 0) {
        processorStatus.set(static_cast<size_t>(Flags::zeroFlag));
    }

    bool isNegative = std::bitset<8>(indexX).test(7);

    if (isNegative) {
        processorStatus.set(static_cast<size_t>(Flags::negativeFlag));
    }
}

void CPU::TAY()
{
    indexY = accumulator;

    if (indexY == 0) {
        processorStatus.set(static_cast<size_t>(Flags::zeroFlag));
    }

    bool isNegative = std::bitset<8>(indexY).test(7);

    if (isNegative) {
        processorStatus.set(static_cast<size_t>(Flags::negativeFlag));
    }
}

void CPU::TXA()
{
    accumulator = indexX;

    if (accumulator == 0) {
        processorStatus.set(static_cast<size_t>(Flags::zeroFlag));
    }

    bool isNegative = std::bitset<8>(accumulator).test(7);

    if (isNegative) {
        processorStatus.set(static_cast<size_t>(Flags::negativeFlag));
    }
}

void CPU::TYA()
{
    accumulator = indexY;

    if (accumulator == 0) {
        processorStatus.set(static_cast<size_t>(Flags::zeroFlag));
    }

    bool isNegative = std::bitset<8>(accumulator).test(7);

    if (isNegative) {
        processorStatus.set(static_cast<size_t>(Flags::negativeFlag));
    }   
}

/**
 * Status Flag Changes
*/

void CPU::CLC()
{
    processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
}

void CPU::CLD()
{
    processorStatus.reset(static_cast<size_t>(Flags::decimalMode));
}

void CPU::CLI()
{
    processorStatus.reset(static_cast<size_t>(Flags::interruptDisable));
}

void CPU::CLV()
{
    processorStatus.reset(static_cast<size_t>(Flags::overflowFlag));
}

void CPU::SEC()
{
    processorStatus.set(static_cast<size_t>(Flags::carryFlag));
}

void CPU::SED()
{
    processorStatus.set(static_cast<size_t>(Flags::decimalMode));
}

void CPU::SEI()
{
    processorStatus.set(static_cast<size_t>(Flags::interruptDisable));
}