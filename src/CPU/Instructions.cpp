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
    nes->memory[0x100 + sp] = accumulator;
    sp--;
    return clockCycles;
}

int CPU::PHP(int clockCycles)
{
    processorStatus.set(static_cast<uint8_t>(Flags::breakCommand));
    nes->memory[0x100 + sp] = static_cast<uint8_t>(processorStatus.to_ulong());
    sp--;
    processorStatus.reset(static_cast<uint8_t>(Flags::breakCommand));
    return clockCycles;
}

int CPU::PLA(int clockCycles)
{
    sp++;
    accumulator = nes->memory[0x100 + sp];
    setZN(accumulator);
    return clockCycles;
}

int CPU::PLP(int clockCycles)
{
    sp++;
    processorStatus = nes->memory[0x100 + sp] | 0x20;
    processorStatus.reset(static_cast<uint8_t>(Flags::breakCommand));
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
    
    bool overflow = std::bitset<8>(value).test(6);

    if (overflow) {
        processorStatus.set(static_cast<size_t>(Flags::overflowFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::overflowFlag));
    }

    if (result == 0) {
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
 * Arithmetic
*/

int CPU::ADC(uint8_t value, int clockCycles)
{
    uint16_t sum = accumulator + value;

    bool carryIsSet = processorStatus.test(static_cast<size_t>(Flags::carryFlag));

    if (carryIsSet) {
        sum++;
    }

    if (sum > 255) {  // Set carry flag if overflow occurred
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
    }

    bool overflow = ~((accumulator ^ value) & 0x80) & ((accumulator ^ sum) & 0x80);

    if (overflow) {  // Set overflow flag if twos complement overflow
        processorStatus.set(static_cast<size_t>(Flags::overflowFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::overflowFlag));
    }

    accumulator = sum & 0xFF;

    setZN(accumulator);

    return clockCycles;
}

int CPU::CMP(uint8_t value, int clockCycles)
{
    uint8_t result = accumulator - value;

    if (accumulator >= value) {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
    }
    
    setZN(result);

    return clockCycles;
}

int CPU::CPX(uint8_t value, int clockCycles)
{
    uint8_t result = indexX - value;

    if (indexX >= value) {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
    }
    
    setZN(result);
    
    return clockCycles;
}

int CPU::CPY(uint8_t value, int clockCycles)
{
    uint8_t result = indexY - value;

    if (indexY >= value) {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
    }
    
    setZN(result);

    return clockCycles;
}

int CPU::SBC(uint8_t value, int clockCycles)
{
    int16_t result = accumulator - value;

    bool carryIsClear = !processorStatus.test(static_cast<size_t>(Flags::carryFlag));

    if (carryIsClear) {
        result--;
    }

    if (result < 0) {  // Reset carry flag if overflow occurs
        processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
    } else {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    }

    bool overflow = ((accumulator ^ value) & 0x80) & ((accumulator ^ (result & 0xFF)) & 0x80);

    if (overflow) {  // Set overflow flag if twos complement overflow
        processorStatus.set(static_cast<size_t>(Flags::overflowFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::overflowFlag));
    }

    accumulator = result & 0xFF;

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
    bool carry = (value >> 7) == 1;  // Test bit 7 of input value

    if (carry) {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
    }

    value <<= 1;

    setZN(value);

    return clockCycles;
}

int CPU::LSR(uint8_t &value, int clockCycles)
{
    bool carry = (value & 0x01) == 1;  // Test bit 0 of input value

    if (carry) {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
    }

    value >>= 1;

    setZN(value);

    return clockCycles;
}

int CPU::ROL(uint8_t &value, int clockCycles)
{
    bool carry = (value >> 7) == 1;  // Test bit 7 of input value

    value <<= 1;
    value |= processorStatus[0];

    if (carry) {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
    }

    setZN(value);

    return clockCycles;
}

int CPU::ROR(uint8_t &value, int clockCycles)
{
    bool carry = (value & 0x01) == 1;  // Test bit 0 of input value

    value >>= 1;
    value |= (processorStatus[0] << 7);

    if (carry) {
        processorStatus.set(static_cast<size_t>(Flags::carryFlag));
    } else {
        processorStatus.reset(static_cast<size_t>(Flags::carryFlag));
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
    nes->memory[0x100 + sp] = (pc >> 8) & 0xFF;
    sp--;
    nes->memory[0x100 + sp] = pc & 0xFF;
    sp--;
    pc = address;
    return clockCycles;
}

int CPU::RTS(int clockCycles)
{
    sp++;
    uint8_t loByte = nes->memory[0x100 + sp];
    sp++;
    uint8_t hiByte = nes->memory[0x100 + sp];
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

/**
 * System Functions
*/

int CPU::BRK(int clockCycles)
{
    // Push pc to stack
    pc++;
    nes->memory[0x100 + sp] = (pc >> 8) & 0xFF;
    sp--;
    nes->memory[0x100 + sp] = pc & 0xFF;
    sp--;

    processorStatus.set(static_cast<uint8_t>(Flags::breakCommand));

    // Push processor status to stack
    nes->memory[0x100 + sp] = static_cast<uint8_t>(processorStatus.to_ulong());
    sp--;

    processorStatus.reset(static_cast<uint8_t>(Flags::breakCommand));
    processorStatus.set(static_cast<uint8_t>(Flags::interruptDisable));
    
    uint8_t lowByte = nes->memory[0xFFFE];
    uint8_t highByte = nes->memory[0xFFFF];

    pc = (highByte << 8) | lowByte; 

    return clockCycles;
}

int CPU::NOP(int clockCycles)
{
    return clockCycles;
}

int CPU::RTI(int clockCycles)
{
    // Get processor status from stack
    sp++;
    processorStatus = nes->memory[0x100 + sp] | 0x20;
    processorStatus.reset(static_cast<uint8_t>(Flags::breakCommand));

    // Get pc from stack
    sp++;
    uint8_t lowByte = nes->memory[0x100 + sp];
    sp++;
    uint8_t highByte = nes->memory[0x100 + sp];
    pc = (highByte << 8) | lowByte;

    return clockCycles;
}