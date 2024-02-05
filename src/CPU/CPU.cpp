#include "CPU.h"

#include "../NES.h"

CPU::CPU() {}

void CPU::connectToNes(NES *nes)
{
    nes = nes;
}

void CPU::tick()
{
    uint8_t instruction = fetchInstruct();
    decodeAndExecuteInstruct(instruction);
}

uint8_t CPU::fetchInstruct()
{
    uint8_t instruction = nes->memory[pc];
    pc++;
    return instruction;
}

void CPU::decodeAndExecuteInstruct(uint8_t instruction)
{
    switch (instruction)
    {
        case 0x18:
            CPU::CLC(2);
            break;

        case 0x38:
            CPU::SEC(2);
            break;

        case 0x58:
            CPU::CLI(2);
            break;

        case 0x78:
            CPU::SEI(2);
            break;

        case 0x8A:
            CPU::TXA(2);
            break;

        case 0x98:
            CPU::TYA(2);
            break;

        case 0xA8:
            CPU::TAY(2);
            break;

        case 0xAA:
            CPU::TAX(2);
            break;

        case 0xB8:
            CPU::CLV(2);
            break;

        case 0xD8:
            CPU::CLD(2);
            break;

        case 0xF8:
            CPU::SED(2);
            break;

        default:
            break;
    }
}

uint16_t CPU::getAbsoluteAddress()
{
    uint8_t byteOne = nes->memory[pc];
    pc++;
    uint8_t byteTwo = nes->memory[pc];
    pc++;
    return (byteTwo << 8) | byteOne;
}   

uint16_t CPU::getAbsoluteXAddress()
{
    uint8_t byteOne = nes->memory[pc];
    pc++;
    uint8_t byteTwo = nes->memory[pc];
    pc++;
    return ((byteTwo << 8) | byteOne) + indexX;
}

uint16_t CPU::getAbsoluteYAddress()
{
    uint8_t byteOne = nes->memory[pc];
    pc++;
    uint8_t byteTwo = nes->memory[pc];
    pc++;
    return ((byteTwo << 8) | byteOne) + indexY;
}

uint8_t CPU::getImmediateValue()
{
    uint8_t value = pc;
    pc++;
    return value;
}

uint16_t CPU::getIndirectAddress()
{
    uint8_t byteOne = nes->memory[pc];
    pc++;
    uint8_t byteTwo = nes->memory[pc];
    pc++;
    return ((byteTwo << 8) | byteOne);
}

uint8_t CPU::getzeroPageAddress()
{
    uint8_t address = nes->memory[pc];
    pc++;
    return address;
}

uint8_t CPU::getZeroPageXAddress()
{
    uint8_t address = nes->memory[pc] + indexX;
    pc++;
    return address;
}

uint8_t CPU::getZeroPageYAddress()
{
    uint8_t address = nes->memory[pc] + indexY;
    pc++;
    return address;
}

uint16_t CPU::getIndexedIndirectAddress()
{
    uint8_t address = (nes->memory[pc] + indexX) & 0xFF;
    pc++;
    uint8_t byteOne = nes->memory[address];
    address++;
    uint8_t byteTwo = nes->memory[address];
    return (byteTwo << 8) | byteOne;
}

uint16_t CPU::getIndirectIndexedAddress()
{
    uint16_t address = (nes->memory[pc] + indexY);
    pc++;
    uint8_t byteOne = nes->memory[address];
    address++;
    uint8_t byteTwo = nes->memory[address];
    return (byteTwo << 8) | byteOne;
}