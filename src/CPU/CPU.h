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

    /**
     * Instructions
    */

     /* Load/Store Operations */
    void LDA();  // Load accumulator
    void LDX();  // Load x register
    void LDY();  // Load y register
    void STA();  // Store accumulator
    void STX();  // Store x register
    void STY();  // Store y register

    /* Register Transfers */
    void TAX();  // Transfer accumulator to x
    void TAY();  // Transfer accumulator to y
    void TXA();  // Transfer x to accumulator
    void TYA();  // Transfer y to accumulator

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
    void CLC();  // Clear carry flag
    void CLD();  // Clear decimal mode
    void CLI();  // Clear interrupt disable
    void CLV();  // Clear overflow flag
    void SEC();  // Set carry flag
    void SED();  // Set decimal flag
    void SEI();  // Set interrupt disable

    /* System Functions */
    void BRK();  // Force interrupt
    void NOP();  // No operation
    void RTI();  // Return from interrupt
};