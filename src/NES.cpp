#include "NES.h"

#include "CPU/State.h"

NES::NES()
{
    cpu.connectToNes(this);
}

NES::NES(CPUState &initialState) : cpu(initialState)
{
    cpu.connectToNes(this);
}

uint8_t NES::getMemoryValue(uint16_t address)
{
    return memory[address];
}

void NES::setMemoryValue(uint16_t address, uint8_t value)
{
    memory[address] = value;
}

CPUState NES::getCPUState()
{
    return cpu.getState();
}