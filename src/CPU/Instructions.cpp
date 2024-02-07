#include "CPU.h"

#include "../NES.h"

/**
 * Load/Store Operations
*/

int CPU::LDA(uint8_t value, int clockCycles)
{
    accumulator = value;
    setZN(accumulator);
    return clockCycles;
}

int CPU::LDX(uint8_t value, int clockCycles)
{
    indexX = value;
    setZN(indexX);
    return clockCycles;
}

int CPU::LDY(uint8_t value, int clockCycles)
{
    indexY = value;
    setZN(indexY);
    return clockCycles;
}

int CPU::STA(uint16_t address, int clockCycles)
{
    nes->memory[address] = accumulator;
    return clockCycles;
}

int CPU::STX(uint16_t address, int clockCycles)
{
    nes->memory[address] = indexX;
    return clockCycles;
}

int CPU::STY(uint16_t address, int clockCycles)
{
    nes->memory[address] = indexY;
    return clockCycles;
}

/**
 * Stack Operations
*/

int CPU::PHA(int clockCycles)
{
    nes->memory[sp] = accumulator;
    sp--;
    return clockCycles;
}

int CPU::PHP(int clockCycles)
{
    nes->memory[sp] = static_cast<uint8_t>(processorStatus.to_ulong());
    return clockCycles;
}

int CPU::PLA(int clockCycles)
{
    sp++;
    accumulator = nes->memory[sp];
    setZN(accumulator);
    return clockCycles;
}

int CPU::PLP(int clockCycles)
{
    sp++;
    processorStatus = nes->memory[sp];
    return clockCycles;
}

int CPU::TSX(int clockCycles)
{
    indexX = sp;
    setZN(indexX);
    return clockCycles;
}

int CPU::TXS(int clockCycles)
{
    sp = indexX;
    return clockCycles;
}

/**
 * Logical
*/

int CPU::AND(uint8_t value, int clockCycles)
{
    accumulator &= value;
    setZN(accumulator);
    return clockCycles;
}

int CPU::BIT(uint8_t value, int clockCycles)
{   
    uint8_t result = accumulator & value;
    setZN(result);
    
    bool overflow = std::bitset<8>(result).test(6);

    if (overflow) {
        processorStatus.set(static_cast<size_t>(Flags::overflowFlag));
    }

    return clockCycles;
}

int CPU::EOR(uint8_t value, int clockCycles)
{
    accumulator ^= value;
    setZN(accumulator);
    return clockCycles;
}

int CPU::ORA(uint8_t value, int clockCycles)
{
    accumulator |= value;
    setZN(accumulator);
    return clockCycles;
}

/**
 * Increments and Decrements
*/

int CPU::DEC(uint16_t address, int clockCycles)
{
    nes->memory[address]--;
    setZN(nes->memory[address]);
    return clockCycles;
}

int CPU::DEX(int clockCycles)
{
    indexX--;
    setZN(indexX);
    return clockCycles;
}

int CPU::DEY(int clockCycles)
{
    indexY--;
    setZN(indexY);
    return clockCycles;
}

int CPU::INC(uint16_t address, int clockCycles)
{   
    nes->memory[address]++;
    setZN(nes->memory[address]);
    return clockCycles;
}

int CPU::INX(int clockCycles)
{
    indexX++;
    setZN(indexX);
    return clockCycles;
}

int CPU::INY(int clockCycles)
{
    indexY++;
    setZN(indexY);
    return clockCycles;
}

/**
 * Shifts
*/

int CPU::ASL(uint8_t &value, int clockCycles)
{
    bool carry = processorStatus.test(static_cast<size_t>(value >> 7));

    if (carry) {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    }

    value <<= 1;

    setZN(value);

    return clockCycles;
}

int CPU::LSR(uint8_t &value, int clockCycles)
{
    bool carry = processorStatus.test(static_cast<size_t>(value & 0x1));

    if (carry) {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    }

    value >>= 1;

    setZN(value);

    return clockCycles;
}

int CPU::ROL(uint8_t &value, int clockCycles)
{
    bool carry = processorStatus.test(static_cast<size_t>(value >> 7));

    value <<= 1;
    value |= processorStatus[0];

    if (carry) {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    }

    setZN(value);

    return clockCycles;
}

int CPU::ROR(uint8_t &value, int clockCycles)
{
    bool carry = processorStatus.test(static_cast<size_t>(value & 0x1));

    value >>= 1;
    value |= (processorStatus[7] << 7);

    if (carry) {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    }

    setZN(value);

    return clockCycles;
}

/**
 * Jumps and Calls
*/

int CPU::JMP(uint16_t address, int clockCycles)
{
    pc = address;
    return clockCycles;
}

int CPU::JSR(uint16_t address, int clockCycles)
{
    pc--;
    nes->memory[sp] = (pc >> 8) & 0xFF;
    sp--;
    nes->memory[sp] = pc & 0xFF;
    sp--;
    pc = address;
    return clockCycles;
}

int CPU::RTS(int clockCycles)
{
    sp++;
    uint8_t loByte = nes->memory[sp];
    sp++;
    uint8_t hiByte = nes->memory[sp];
    uint16_t address = (hiByte << 8) | loByte; 
    address++;
    pc = address;
    return clockCycles;
}

/**
 * Branches
*/

int CPU::BCC(int8_t offset, int clockCycles)
{
    bool carryIsClear = !processorStatus.test(static_cast<size_t>(Flags::carryFlag));

    if (carryIsClear) {
        pc += offset;
    }

    return carryIsClear ? clockCycles+1 : clockCycles;
}

int CPU::BCS(int8_t offset, int clockCycles)
{
    bool carryIsSet = processorStatus.test(static_cast<size_t>(Flags::carryFlag));

    if (carryIsSet) {
        pc += offset;
    }

    return carryIsSet ? clockCycles+1 : clockCycles;
}

int CPU::BEQ(int8_t offset, int clockCycles)
{   
    bool isEqual = processorStatus.test(static_cast<size_t>(Flags::zeroFlag));

    if (isEqual) {
        pc += offset;
    }

    return isEqual ? clockCycles+1 : clockCycles;
}

int CPU::BMI(int8_t offset, int clockCycles)
{
    bool isMinus = processorStatus.test(static_cast<size_t>(Flags::negativeFlag));

    if (isMinus) {
        pc += offset;
    }

    return isMinus ? clockCycles+1 : clockCycles;
}

int CPU::BNE(int8_t offset, int clockCycles)
{
    bool isNotEqual = !processorStatus.test(static_cast<size_t>(Flags::zeroFlag));

    if (isNotEqual) {
        pc += offset;
    }

    return isNotEqual ? clockCycles+1 : clockCycles;
}

int CPU::BPL(int8_t offset, int clockCycles)
{   
    bool isPositive = !processorStatus.test(static_cast<size_t>(Flags::negativeFlag));

    if (isPositive) {
        pc += offset;
    }

    return isPositive ? clockCycles+1 : clockCycles;
}

int CPU::BVC(int8_t offset, int clockCycles)
{
    bool overflowIsClear = !processorStatus.test(static_cast<size_t>(Flags::overflowFlag));

    if (overflowIsClear) {
        pc += offset;
    }

    return overflowIsClear ? clockCycles+1 : clockCycles;
}

int CPU::BVS(int8_t offset, int clockCycles)
{
    bool overflowIsSet = processorStatus.test(static_cast<size_t>(Flags::overflowFlag));

    if (overflowIsSet) {
        pc += offset;
    }

    return overflowIsSet ? clockCycles+1 : clockCycles;
}

/**
 * Register Transfers
*/

int CPU::TAX(int clockCycles)
{
    indexX = accumulator;
    setZN(indexX);
    return clockCycles;
}

int CPU::TAY(int clockCycles)
{
    indexY = accumulator;
    setZN(indexY);
    return clockCycles;
}

int CPU::TXA(int clockCycles)
{
    accumulator = indexX;
    setZN(accumulator);
    return clockCycles;
}

int CPU::TYA(int clockCycles)
{
    accumulator = indexY;
    setZN(accumulator);
    return clockCycles;
}

/**
 * Status Flag Changes
*/

int CPU::CLC(int clockCycles)
{
    processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
    return clockCycles;
}

int CPU::CLD(int clockCycles)
{
    processorStatus.reset(static_cast<size_t>(Flags::decimalMode));
    return clockCycles;
}

int CPU::CLI(int clockCycles)
{
    processorStatus.reset(static_cast<size_t>(Flags::interruptDisable));
    return clockCycles;
}

int CPU::CLV(int clockCycles)
{
    processorStatus.reset(static_cast<size_t>(Flags::overflowFlag));
    return clockCycles;
}

int CPU::SEC(int clockCycles)
{
    processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    return clockCycles;
}

int CPU::SED(int clockCycles)
{
    processorStatus.set(static_cast<size_t>(Flags::decimalMode));
    return clockCycles;
}

int CPU::SEI(int clockCycles)
{
    processorStatus.set(static_cast<size_t>(Flags::interruptDisable));
    return clockCycles;
}