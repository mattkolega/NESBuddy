#include "CPU.h"

#include "../NES.h"
#include "State.h"

CPU::CPU() {}

CPU::CPU(CPUState &initialState) {
    pc = initialState.pc;
    sp = initialState.sp;
    accumulator = initialState.accumulator;
    indexX = initialState.indexX;
    indexY = initialState.indexY;
    processorStatus = initialState.processorStatus;
}

void CPU::connectToNes(NES *nes)
{
    this->nes = nes;
}

void CPU::setToPowerUpState()
{
    pc = (nes->cpuRead(0xFFFD) << 8) | nes->cpuRead(0xFFFC);
    accumulator = 0;
    indexX = 0;
    indexY = 0;
    sp = 0xFD;
    processorStatus = 0x34;
}

void CPU::tick()
{
    uint8_t instruction = fetchInstruct();
    decodeAndExecuteInstruct(instruction);
}

CPUState CPU::getState()
{
    CPUState currentState;

    currentState.pc = pc;
    currentState.sp = sp;
    currentState.accumulator = accumulator;
    currentState.indexX = indexX;
    currentState.indexY = indexY;
    currentState.processorStatus = static_cast<uint8_t>(processorStatus.to_ulong());

    return currentState;
}

uint16_t CPU::getAbsoluteAddress()
{
    uint8_t byteOne = nes->cpuRead(pc);
    pc++;
    uint8_t byteTwo = nes->cpuRead(pc);
    pc++;
    return (byteTwo << 8) | byteOne;
}   

uint16_t CPU::getAbsoluteXAddress()
{
    uint8_t byteOne = nes->cpuRead(pc);
    pc++;
    uint8_t byteTwo = nes->cpuRead(pc);
    pc++;
    return ((byteTwo << 8) | byteOne) + indexX;
}

uint16_t CPU::getAbsoluteYAddress()
{
    uint8_t byteOne = nes->cpuRead(pc);
    pc++;
    uint8_t byteTwo = nes->cpuRead(pc);
    pc++;
    return ((byteTwo << 8) | byteOne) + indexY;
}

uint8_t CPU::getImmediateValue()
{
    uint8_t value = nes->cpuRead(pc);
    pc++;
    return value;
}

uint16_t CPU::getIndirectAddress()
{
    uint8_t byteOne = nes->cpuRead(pc);
    pc++;
    uint8_t byteTwo = nes->cpuRead(pc);
    pc++;

    uint16_t absoluteAddress = (byteTwo << 8) | byteOne;
    byteOne = nes->cpuRead(absoluteAddress);

    if ((absoluteAddress & 0x00FF) == 0xFF) {
        absoluteAddress &= 0xFF00;
    } else {
        absoluteAddress++;
    }
    
    byteTwo = nes->cpuRead(absoluteAddress);
    return (byteTwo << 8) | byteOne;
}

uint8_t CPU::getZeroPageAddress()
{
    uint8_t address = nes->cpuRead(pc);
    pc++;
    return address;
}

uint8_t CPU::getZeroPageXAddress()
{
    uint8_t address = nes->cpuRead(pc) + indexX;
    pc++;
    return address;
}

uint8_t CPU::getZeroPageYAddress()
{
    uint8_t address = nes->cpuRead(pc) + indexY;
    pc++;
    return address;
}

uint16_t CPU::getIndexedIndirectAddress()
{
    uint8_t address = (nes->cpuRead(pc) + indexX) & 0xFF;
    pc++;
    uint8_t byteOne = nes->cpuRead(address);
    address++;
    uint8_t byteTwo = nes->cpuRead(address);
    return (byteTwo << 8) | byteOne;
}

uint16_t CPU::getIndirectIndexedAddress()
{
    uint8_t address = nes->cpuRead(pc);
    pc++;
    uint8_t byteOne = nes->cpuRead(address) + indexY;
    int carry  = ((nes->cpuRead(address) + indexY) > 255) ? 1 : 0; 
    address++;
    uint8_t byteTwo = nes->cpuRead(address) + carry;
    return (byteTwo << 8) | byteOne;
}

int8_t CPU::getRelativeOffset()
{
    int8_t offset = nes->cpuRead(pc);
    pc++;
    return offset;
}

void CPU::setZN(uint8_t value)
{
    if (value == 0) {
        processorStatus.set(static_cast<size_t>(Flags::zeroFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::zeroFlag));
    }

    bool isNegative = (value >> 7) == 1;

    if (isNegative) {
        processorStatus.set(static_cast<size_t>(Flags::negativeFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::negativeFlag));
    }
}

void CPU::pushToStack(uint8_t value)
{
    nes->cpuWrite(0x100 + sp, value);
    sp--;
}

uint8_t CPU::popFromStack()
{
    sp++;
    return nes->cpuRead(0x100 + sp);
}