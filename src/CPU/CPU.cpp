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
            CPU::CLC();
            break;

        case 0x38:
            CPU::SEC();
            break;

        case 0x58:
            CPU::CLI();
            break;

        case 0x78:
            CPU::SEI();
            break;

        case 0x8A:
            CPU::TXA();
            break;

        case 0x98:
            CPU::TYA();
            break;

        case 0xA8:
            CPU::TAY();
            break;

        case 0xAA:
            CPU::TAX();
            break;

        case 0xB8:
            CPU::CLV();
            break;

        case 0xD8:
            CPU::CLD();
            break;

        case 0xF8:
            CPU::SED();
            break;

        default:
            break;
    }
}