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