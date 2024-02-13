#pragma once

#include <bitset>
#include <cstdint>

enum class Flags : unsigned char
{
    carryFlag         = 0,
    zeroFlag          = 1,
    interruptDisable  = 2,
    decimalMode       = 3,
    breakCommand      = 4,
    overflowFlag      = 6,
    negativeFlag      = 7,
};

class NES;
struct CPUState;

class CPU
{
public:
    CPU();
    CPU(CPUState &initialState);

    void connectToNes(NES *nes);

    void tick();

    CPUState getState();
private:
    NES *nes { nullptr };

    /* Registers */
    uint16_t pc {};          // Program Counter
    uint8_t sp {};           // Stack Pointer
    uint8_t accumulator {};  // Accumulator
    uint8_t indexX {};       // Index Register X
    uint8_t indexY {};       // Index Register Y

    /* Flags */
    std::bitset<8> processorStatus { 0b0010'0000 };
private:
    uint8_t fetchInstruct();
    int decodeAndExecuteInstruct(uint8_t instruction);

    /* Processor Status Helper Functions */
    void setZN(uint8_t value);

    /* Addressing Mode Handlers */
    uint16_t getAbsoluteAddress();
    uint16_t getAbsoluteXAddress();
    uint16_t getAbsoluteYAddress();
    uint8_t getImmediateValue();
    uint16_t getIndirectAddress();
    uint8_t getZeroPageAddress();
    uint8_t getZeroPageXAddress();
    uint8_t getZeroPageYAddress();
    uint16_t getIndexedIndirectAddress();
    uint16_t getIndirectIndexedAddress();
    int8_t getRelativeOffset();
    
    /**
     * Instructions
    */

    /* Load/Store Operations */
    int LDA(uint8_t value, int clockCycles);  // Load accumulator
    int LDX(uint8_t value, int clockCycles);  // Load x register
    int LDY(uint8_t value, int clockCycles);  // Load y register
    int STA(uint16_t address, int clockCycles);  // Store accumulator
    int STX(uint16_t address, int clockCycles);  // Store x register
    int STY(uint16_t address, int clockCycles);  // Store y register

    /* Register Transfers */
    int TAX(int clockCycles);  // Transfer accumulator to x
    int TAY(int clockCycles);  // Transfer accumulator to y
    int TXA(int clockCycles);  // Transfer x to accumulator
    int TYA(int clockCycles);  // Transfer y to accumulator

    /* Stack Operations */
    int PHA(int clockCycles);  // Push accumulator
    int PHP(int clockCycles);  // Push processor status
    int PLA(int clockCycles);  // Pull accumulator
    int PLP(int clockCycles);  // Pull processor status
    int TSX(int clockCycles);  // Transfer stack pointer to x
    int TXS(int clockCycles);  // Transfer x to stack pointer

    /* Logical */
    int AND(uint8_t value, int clockCycles);  // Logical AND
    int BIT(uint8_t value, int clockCycles);  // Bit test
    int EOR(uint8_t value, int clockCycles);  // Exclusive OR
    int ORA(uint8_t value, int clockCycles);  // Logical inclusive OR

    /* Arithmetic */
    int ADC(uint8_t value, int clockCycles);  // Add with carry
    int CMP(uint8_t value, int clockCycles);  // Compare
    int CPX(uint8_t value, int clockCycles);  // Compare x register
    int CPY(uint8_t value, int clockCycles);  // Compare y register
    int SBC(uint8_t value, int clockCycles);  // Subtract with carry

    /* Increments and Decrements */
    int DEC(uint16_t address, int clockCycles);  // Decrement memory
    int DEX(int clockCycles);  // Decrement x register
    int DEY(int clockCycles);  // Decrement y register
    int INC(uint16_t address, int clockCycles);  // Increment memory
    int INX(int clockCycles);  // Increment x register
    int INY(int clockCycles);  // Increment y register
    
    /* Shifts */
    int ASL(uint8_t &value, int clockCycles);  // Arithmetic shift left
    int LSR(uint8_t &value, int clockCycles);  // Logical shift right
    int ROL(uint8_t &value, int clockCycles);  // Rotate left
    int ROR(uint8_t &value, int clockCycles);  // Rotate right

    /* Jumps and Calls */
    int JMP(uint16_t address, int clockCycles);  // Jump
    int JSR(uint16_t address, int clockCycles);  // Jump to subroutine
    int RTS(int clockCycles);  // Return from subroutine

    /* Branches */
    int BCC(int8_t offset, int clockCycles);  // Branch if carry clear
    int BCS(int8_t offset, int clockCycles);  // Branch is carry set
    int BEQ(int8_t offset, int clockCycles);  // Branch if equal
    int BMI(int8_t offset, int clockCycles);  // Branch if minus
    int BNE(int8_t offset, int clockCycles);  // Branch if not equal
    int BPL(int8_t offset, int clockCycles);  // Branch if positive
    int BVC(int8_t offset, int clockCycles);  // Branch if overflow clear
    int BVS(int8_t offset, int clockCycles);  // Branch if overflow set

    /* Status Flag Changes */
    int CLC(int clockCycles);  // Clear carry flag
    int CLD(int clockCycles);  // Clear decimal mode
    int CLI(int clockCycles);  // Clear interrupt disable
    int CLV(int clockCycles);  // Clear overflow flag
    int SEC(int clockCycles);  // Set carry flag
    int SED(int clockCycles);  // Set decimal flag
    int SEI(int clockCycles);  // Set interrupt disable

    /* System Functions */
    int BRK(int clockCycles);  // Force interrupt
    int NOP(int clockCycles);  // No operation
    int RTI(int clockCycles);  // Return from interrupt
};