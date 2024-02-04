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

uint8_t CPU::getValue(AddrMode addressMode)
{
    switch(addressMode) 
    {
        case AddrMode::absolute:
            uint8_t byteOne = nes->memory[pc];
            pc++;
            uint8_t byteTwo = nes->memory[pc];
            pc++;
            uint16_t address = (byteTwo << 8) | byteOne;
            return nes->memory[address];

        case AddrMode::accumulator:
            return accumulator;

        case AddrMode::immediate:
            uint8_t value = pc;
            pc++;
            return value;
        
        case AddrMode::indirect:
            uint8_t byteOne = nes->memory[pc];
            pc++;
            uint8_t byteTwo = nes->memory[pc];
            pc++;
            uint16_t address = (byteTwo << 8) | byteOne;
            return address;

        case AddrMode::zeroPage:
            uint8_t address = nes->memory[pc];
            pc++;
            return nes->memory[address];

        case AddrMode::absoluteX:
            uint8_t byteOne = nes->memory[pc];
            pc++;
            uint8_t byteTwo = nes->memory[pc];
            pc++;
            uint16_t address = ((byteTwo << 8) | byteOne) + indexX;
            return nes->memory[address];

        case AddrMode::absoluteY:
            uint8_t byteOne = nes->memory[pc];
            pc++;
            uint8_t byteTwo = nes->memory[pc];
            pc++;
            uint16_t address = ((byteTwo << 8) | byteOne) + indexY;
            return nes->memory[address];

        case AddrMode::indexIndirect:
            uint8_t address = (nes->memory[pc] + indexX);
            pc++;
            uint8_t byteOne = nes->memory[address];
            address++;
            uint8_t byteTwo = nes->memory[address];
            address++;
            uint16_t targetAddress = (byteTwo << 8) | byteOne;
            return nes->memory[targetAddress];

        case AddrMode::indirectIndex:
            uint16_t address = (nes->memory[pc] + indexY);
            pc++;
            uint8_t byteOne = nes->memory[address];
            address++;
            uint8_t byteTwo = nes->memory[address];
            address++;
            uint16_t targetAddress = (byteTwo << 8) | byteOne;
            return nes->memory[targetAddress];

        case AddrMode::zeroPageX:
            uint8_t address = nes->memory[pc] + indexX;
            pc++;
            return nes->memory[address];
            
        case AddrMode::zeroPageY:
            uint8_t address = nes->memory[pc] + indexY;
            pc++;
            return nes->memory[address];

        default:
            break;
    }
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