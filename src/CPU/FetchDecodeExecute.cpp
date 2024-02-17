#include "CPU.h"

#include "../NES.h"

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
        case 0x00:
            return CPU::BRK(7);

        case 0x01:
            return CPU::ORA(nes->memory[getIndexedIndirectAddress()], 6);

        case 0x05:
            return CPU::ORA(nes->memory[getZeroPageAddress()], 3);

        case 0x06:
            return CPU::ASL(nes->memory[getZeroPageAddress()], 5);

        case 0x08:
            return CPU::PHP(3);

        case 0x09:
            return CPU::ORA(getImmediateValue(), 2);

        case 0x0A:
            return CPU::ASL(accumulator, 2);

        case 0x0D:
            return CPU::ORA(nes->memory[getAbsoluteAddress()], 4);

        case 0x0E:
            return CPU::ASL(nes->memory[getAbsoluteAddress()], 6);

        case 0x10:
            return CPU::BPL(getRelativeOffset(), 2);

        case 0x11:
            return CPU::ORA(nes->memory[getIndirectIndexedAddress()], 5);

        case 0x15:
            return CPU::ORA(nes->memory[getZeroPageXAddress()], 4);

        case 0x16:
            return CPU::ASL(nes->memory[getZeroPageXAddress()], 6);

        case 0x18:
            return CPU::CLC(2);

        case 0x19:
            return CPU::ORA(nes->memory[getAbsoluteYAddress()], 4);
        
        case 0x1D:
            return CPU::ORA(nes->memory[getAbsoluteXAddress()], 4);

        case 0x1E:
            return CPU::ASL(nes->memory[getAbsoluteXAddress()], 7);

        case 0x20:
            return CPU::JSR(getAbsoluteAddress(), 6);

        case 0x21:
            return CPU::AND(nes->memory[getIndexedIndirectAddress()], 6);

        case 0x24:
            return CPU::BIT(nes->memory[getZeroPageAddress()], 3);

        case 0x25:
            return CPU::AND(nes->memory[getZeroPageAddress()], 3);

        case 0x26:
            return CPU::ROL(nes->memory[getZeroPageAddress()], 5);

        case 0x28:
            return CPU::PLP(4);

        case 0x29:
            return CPU::AND(getImmediateValue(), 2);

        case 0x2A:
            return CPU::ROL(accumulator, 2);

        case 0x2C:
            return CPU::BIT(nes->memory[getAbsoluteAddress()], 4);

        case 0x2D:
            return CPU::AND(nes->memory[getAbsoluteAddress()], 4);

        case 0x2E:
            return CPU::ROL(nes->memory[getAbsoluteAddress()], 6);

        case 0x30:
            return CPU::BMI(getRelativeOffset(), 2);

        case 0x31:
            return CPU::AND(nes->memory[getIndirectIndexedAddress()], 5);

        case 0x35:
            return CPU::AND(nes->memory[getZeroPageXAddress()], 4);

        case 0x36:
            return CPU::ROL(nes->memory[getZeroPageXAddress()], 6);

        case 0x38:
            return CPU::SEC(2);

        case 0x39:
            return CPU::AND(nes->memory[getAbsoluteYAddress()], 4);

        case 0x3D:
            return CPU::AND(nes->memory[getAbsoluteXAddress()], 4);

        case 0x3E:
            return CPU::ROL(nes->memory[getAbsoluteXAddress()], 7);

        case 0x40:
            return CPU::RTI(6);

        case 0x41:
            return CPU::EOR(nes->memory[getIndexedIndirectAddress()], 6);

        case 0x45:
            return CPU::EOR(nes->memory[getZeroPageAddress()], 3);

        case 0x46:
            return CPU::LSR(nes->memory[getZeroPageAddress()], 5);

        case 0x48:
            return CPU::PHA(3);

        case 0x49:
            return CPU::EOR(getImmediateValue(), 2);

        case 0x4A:
            return CPU::LSR(accumulator, 2);

        case 0x4C:
            return CPU::JMP(getAbsoluteAddress(), 3);

        case 0x4D:
            return CPU::EOR(nes->memory[getAbsoluteAddress()], 4);

        case 0x4E:
            return CPU::LSR(nes->memory[getAbsoluteAddress()], 6);

        case 0x50:
            return CPU::BVC(getRelativeOffset(), 2);

        case 0x51:
            return CPU::EOR(nes->memory[getIndirectIndexedAddress()], 5);

        case 0x55:
            return CPU::EOR(nes->memory[getZeroPageXAddress()], 4);

        case 0x56:
            return CPU::LSR(nes->memory[getZeroPageXAddress()], 6);

        case 0x58:
            return CPU::CLI(2);

        case 0x59:
            return CPU::EOR(nes->memory[getAbsoluteYAddress()], 4);

        case 0x5D:
            return CPU::EOR(nes->memory[getAbsoluteXAddress()], 4);

        case 0x5E:
            return CPU::LSR(nes->memory[getAbsoluteXAddress()], 7);

        case 0x60:
            return CPU::RTS(6);

        case 0x61:
            return CPU::ADC(nes->memory[getIndexedIndirectAddress()], 6);

        case 0x65:
            return CPU::ADC(nes->memory[getZeroPageAddress()], 3);

        case 0x66:
            return CPU::ROR(nes->memory[getZeroPageAddress()], 5);

        case 0x68:
            return CPU::PLA(4);

        case 0x69:
            return CPU::ADC(getImmediateValue(), 2);

        case 0x6A:
            return CPU::ROR(accumulator, 2);

        case 0x6C:
            return CPU::JMP(getIndirectAddress(), 5);
        
        case 0x6D:
            return CPU::ADC(nes->memory[getAbsoluteAddress()], 4);

        case 0x6E:
            return CPU::ROR(nes->memory[getAbsoluteAddress()], 6);

        case 0x70:
            return CPU::BVS(getRelativeOffset(), 2);

        case 0x71:
            return CPU::ADC(nes->memory[getIndirectIndexedAddress()], 5);

        case 0x75:
            return CPU::ADC(nes->memory[getZeroPageXAddress()], 4);

        case 0x76:
            return CPU::ROR(nes->memory[getZeroPageXAddress()], 6);

        case 0x78:
            return CPU::SEI(2);

        case 0x79:
            return CPU::ADC(nes->memory[getAbsoluteYAddress()], 4);

        case 0x7D:
            return CPU::ADC(nes->memory[getAbsoluteXAddress()], 4);

        case 0x7E:
            return CPU::ROR(nes->memory[getAbsoluteXAddress()], 7);

        case 0x81:
            return CPU::STA(getIndexedIndirectAddress(), 6);

        case 0x84:
            return CPU::STY(getZeroPageAddress(), 3);

        case 0x85:
            return CPU::STA(getZeroPageAddress(), 3);

        case 0x86:
            return CPU::STX(getZeroPageAddress(), 3);

        case 0x88:
            return CPU::DEY(2);

        case 0x8A:
            return CPU::TXA(2);

        case 0x8C:
            return CPU::STY(getAbsoluteAddress(), 4);

        case 0x8D:
            return CPU::STA(getAbsoluteAddress(), 4);

        case 0x8E:
            return CPU::STX(getAbsoluteAddress(), 4);

        case 0x90:
            return CPU::BCC(getRelativeOffset(), 2);

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
            return CPU::TXS(2);

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

        case 0xB0:
            return CPU::BCS(getRelativeOffset(), 2);

        case 0xB1:
            return CPU::LDA(nes->memory[getIndirectIndexedAddress()], 5);

        case 0xB4:
            return CPU::LDY(nes->memory[getZeroPageXAddress()], 4);

        case 0xB5:
            return CPU::LDA(nes->memory[getZeroPageXAddress()], 4);

        case 0xB6:
            return CPU::LDX(nes->memory[getZeroPageYAddress()], 4);

        case 0xB8:
            return CPU::CLV(2);

        case 0xB9:
            return CPU::LDA(nes->memory[getAbsoluteYAddress()], 4);

        case 0xBA:
            return CPU::TSX(2);

        case 0xBC:
            return CPU::LDY(nes->memory[getAbsoluteXAddress()], 4);

        case 0xBD:
            return CPU::LDA(nes->memory[getAbsoluteXAddress()], 4);

        case 0xBE:
            return CPU::LDX(nes->memory[getAbsoluteYAddress()], 4);

        case 0xC0:
            return CPU::CPY(getImmediateValue(), 2);

        case 0xC1:
            return CPU::CMP(nes->memory[getIndexedIndirectAddress()], 6);

        case 0xC4:
            return CPU::CPY(nes->memory[getZeroPageAddress()], 3);

        case 0xC5:
            return CPU::CMP(nes->memory[getZeroPageAddress()], 3);

        case 0xC6:
            return CPU::DEC(getZeroPageAddress(), 5);

        case 0xC8:
            return CPU::INY(2);

        case 0xC9:
            return CPU::CMP(getImmediateValue(), 2);

        case 0xCA:
            return CPU::DEX(2);

        case 0xCC:  
            return CPU::CPY(nes->memory[getAbsoluteAddress()], 4);

        case 0xCD:
            return CPU::CMP(nes->memory[getAbsoluteAddress()], 4);

        case 0xCE:
            return CPU::DEC(getAbsoluteAddress(), 6);

        case 0xD0:
            return CPU::BNE(getRelativeOffset(), 2);

        case 0xD1:
            return CPU::CMP(nes->memory[getIndirectIndexedAddress()], 5);

        case 0xD5:
            return CPU::CMP(nes->memory[getZeroPageXAddress()], 4);

        case 0xD6:
            return CPU::DEC(getZeroPageXAddress(), 6);

        case 0xD8:
            return CPU::CLD(2);
        
        case 0xD9:
            return CPU::CMP(nes->memory[getAbsoluteYAddress()], 4);

        case 0xDD:
            return CPU::CMP(nes->memory[getAbsoluteXAddress()], 4);

        case 0xDE:
            return CPU::DEC(getAbsoluteXAddress(), 7);

        case 0xE0:
            return CPU::CPX(getImmediateValue(), 2);

        case 0xE1:
            return CPU::SBC(nes->memory[getIndexedIndirectAddress()], 6);

        case 0xE4:
            return CPU::CPX(nes->memory[getZeroPageAddress()], 3);

        case 0xE5:
            return CPU::SBC(nes->memory[getZeroPageAddress()], 3);

        case 0xE6:
            return CPU::INC(getZeroPageAddress(), 5);

        case 0xE8:
            return CPU::INX(2);

        case 0xE9:
            return CPU::SBC(getImmediateValue(), 2);

        case 0xEA:
            return CPU::NOP(2);

        case 0xEC:
            return CPU::CPX(nes->memory[getAbsoluteAddress()], 4);

        case 0xED:
            return CPU::SBC(nes->memory[getAbsoluteAddress()], 4);

        case 0xEE:
            return CPU::INC(getAbsoluteAddress(), 6);

        case 0xF0:
            return CPU::BEQ(getRelativeOffset(), 2);

        case 0xF1:
            return CPU::SBC(nes->memory[getIndirectIndexedAddress()], 5);

        case 0xF5:
            return CPU::SBC(nes->memory[getZeroPageXAddress()], 4);

        case 0xF6:
            return CPU::INC(getZeroPageXAddress(), 6);

        case 0xF8:
            return CPU::SED(2);

        case 0xF9:
            return CPU::SBC(nes->memory[getAbsoluteYAddress()], 4);

        case 0xFD:
            return CPU::SBC(nes->memory[getAbsoluteXAddress()], 4);

        case 0xFE:
            return CPU::INC(getAbsoluteXAddress(), 7);

        default:
            return 0;
    }
}