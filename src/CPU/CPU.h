#pragma once

#include <bitset>
#include <cstdint>

class NES;

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

enum class AddrMode
{
    absolute,
    accumulator,
    immediate,
    indirect,
    zeroPage,
    absoluteX,
    absoluteY,
    indexIndirect,
    indirectIndex,
    zeroPageX,
    zeroPageY,
};

class CPU
{
public:
    CPU();

    void connectToNes(NES *nes);

    void tick();
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
    void decodeAndExecuteInstruct(uint8_t instruction);

    uint16_t getAbsoluteAddress();
    uint16_t getAbsoluteXAddress();
    uint16_t getAbsoluteYAddress();
    uint8_t getImmediateValue();
    uint16_t getIndirectAddress();
    uint8_t getzeroPageAddress();
    uint8_t getZeroPageXAddress();
    uint8_t getZeroPageYAddress();
    uint16_t getIndexedIndirectAddress();
    uint16_t getIndirectIndexedAddress();
    
    /**
     * Instructions
    */

    /* Load/Store Operations */
    int LDA(uint8_t value, int clockCycles);  // Load accumulator
    int LDX(uint8_t value, int clockCycles);  // Load x register
    int LDY(uint8_t value, int clockCycles);  // Load y register
    int STA(uint8_t value, int clockCycles);  // Store accumulator
    int STX(uint8_t value, int clockCycles);  // Store x register
    int STY(uint8_t value, int clockCycles);  // Store y register

    /* Register Transfers */
    int TAX(int clockCycles);  // Transfer accumulator to x
    int TAY(int clockCycles);  // Transfer accumulator to y
    int TXA(int clockCycles);  // Transfer x to accumulator
    int TYA(int clockCycles);  // Transfer y to accumulator

    /* Stack Operations */
    void PHA();  // Push accumulator
    void PHP();  // Push processor status
    void PLA();  // Pull accumulator
    void PLP();  // Pull processor status
    void TSX();  // Transfer stack pointer to x
    void TXS();  // Transfer x to stack pointer

    /* Logical */
    void AND();  // Logical AND
    void BIT();  // Bit test
    void EOR();  // Exclusive OR
    void ORA();  // Logical inclusive OR

    /* Arithmetic */
    void ADC();  // Add with carry
    void CMP();  // Compare
    void CPX();  // Compare x register
    void CPY();  // Compare y register
    void SBC();  // Subtract with carry

    /* Increments and Decrements */
    void DEC();  // Decrement memory
    void DEX();  // Decrement x register
    void DEY();  // Decrement y register
    void INC();  // Increment memory
    void INX();  // Increment x register
    void INY();  // Increment y register
    
    /* Shifts */
    void ASL();  // Arithmetic shift left
    void LSR();  // Logical shift right
    void ROL();  // Rotate left
    void ROR();  // Rotate right

    /* Jumps and Calls */
    void JMP();  // Jump
    void JSR();  // Jump to subroutine
    void RTS();  // Return from subroutine

    /* Branches */
    void BCC();  // Branch if carry clear
    void BCS();  // Branch is carry set
    void BEQ();  // Branch if equal
    void BMI();  // Branch if minus
    void BNE();  // Branch if not equal
    void BPL();  // Branch if positive
    void BVC();  // Branch if overflow clear
    void BVS();  // Branch if overflow set

    /* Status Flag Changes */
    int CLC(int clockCycles);  // Clear carry flag
    int CLD(int clockCycles);  // Clear decimal mode
    int CLI(int clockCycles);  // Clear interrupt disable
    int CLV(int clockCycles);  // Clear overflow flag
    int SEC(int clockCycles);  // Set carry flag
    int SED(int clockCycles);  // Set decimal flag
    int SEI(int clockCycles);  // Set interrupt disable

    /* System Functions */
    void BRK();  // Force interrupt
    void NOP();  // No operation
    void RTI();  // Return from interrupt
};