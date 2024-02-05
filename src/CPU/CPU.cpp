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

int CPU::decodeAndExecuteInstruct(uint8_t instruction)
{
    switch (instruction)
    {
        case 0x08:
            return CPU::PHP(3);

        case 0x18:
            return CPU::CLC(2);

        case 0x28:
            return CPU::PLP(4);

        case 0x38:
            return CPU::SEC(2);

        case 0x48:
            return CPU::PHA(3);

        case 0x58:
            return CPU::CLI(2);

        case 0x68:
            return CPU::PLA(4);

        case 0x78:
            return CPU::SEI(2);

        case 0x81:
            return CPU::STA(getIndexedIndirectAddress(), 6);

        case 0x84:
            return CPU::STY(getZeroPageAddress(), 3);

        case 0x85:
            return CPU::STA(getZeroPageAddress(), 3);

        case 0x86:
            return CPU::STX(getZeroPageAddress(), 3);

        case 0x8A:
            return CPU::TXA(2);

        case 0x8C:
            return CPU::STY(getAbsoluteAddress(), 4);

        case 0x8D:
            return CPU::STA(getAbsoluteAddress(), 4);

        case 0x8E:
            return CPU::STX(getAbsoluteAddress(), 4);

        case 0x91:
            return CPU::STA(getIndirectIndexedAddress(), 6);

        case 0x94:
            return CPU::STY(getZeroPageXAddress(), 4);

        case 0x95:
            return CPU::STA(getZeroPageXAddress(), 4);

        case 0x96:
            return CPU::STX(getZeroPageYAddress(), 4);

        case 0x98:
            return CPU::TYA(2);

        case 0x99:
            return CPU::STA(getAbsoluteYAddress(), 5);

        case 0x9A:
            return CPU::TSX(2);

        case 0x9D:
            return CPU::STA(getAbsoluteXAddress(), 5);

        case 0xA0:
            return CPU::LDY(getImmediateValue(), 2);

        case 0xA1:
            return CPU::LDA(nes->memory[getIndexedIndirectAddress()], 6);

        case 0xA2:
            return CPU::LDX(getImmediateValue(), 2);

        case 0xA4:
            return CPU::LDY(nes->memory[getZeroPageAddress()], 3);

        case 0xA5:
            return CPU::LDA(nes->memory[getZeroPageAddress()], 3);

        case 0xA6:
            return CPU::LDX(nes->memory[getZeroPageAddress()], 3);

        case 0xA8:
            return CPU::TAY(2);

        case 0xA9:
            return CPU::LDA(getImmediateValue(), 2);

        case 0xAA:
            return CPU::TAX(2);

        case 0xAC:
            return CPU::LDY(nes->memory[getAbsoluteAddress()], 4);
        
        case 0xAD:
            return CPU::LDA(nes->memory[getAbsoluteAddress()], 4);

        case 0xAE:
            return CPU::LDX(nes->memory[getAbsoluteAddress()], 4);

        case 0xB1:
            return CPU::LDA(nes->memory[getIndirectIndexedAddress()], 5);

        case 0xB4:
            return CPU::LDY(nes->memory[getZeroPageYAddress()], 4);

        case 0xB5:
            return CPU::LDA(nes->memory[getZeroPageXAddress()], 4);

        case 0xB6:
            return CPU::LDX(nes->memory[getZeroPageYAddress()], 4);

        case 0xB8:
            return CPU::CLV(2);

        case 0xB9:
            return CPU::LDA(nes->memory[getZeroPageYAddress()], 4);

        case 0xBA:
            return CPU::TSX(2);

        case 0xBC:
            return CPU::LDY(nes->memory[getAbsoluteXAddress()], 4);

        case 0xBE:
            return CPU::LDX(nes->memory[getAbsoluteYAddress()], 4);

        case 0xD8:
            return CPU::CLD(2);

        case 0xF8:
            return CPU::SED(2);

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

uint8_t CPU::getZeroPageAddress()
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

void CPU::setZN(uint8_t value)
{
    if (value == 0) {
        processorStatus.set(static_cast<size_t>(Flags::zeroFlag));
    }

    bool isNegative = std::bitset<8>(value).test(7);

    if (isNegative) {
        processorStatus.set(static_cast<size_t>(Flags::negativeFlag));
    }
}