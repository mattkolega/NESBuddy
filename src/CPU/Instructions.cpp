#include "CPU.h"

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