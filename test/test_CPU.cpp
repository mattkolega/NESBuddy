#include <format>
#include <fstream>
#include <sstream>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "../src/CPU/State.h"
#include "../src/NES.h"

using json = nlohmann::json;

// Indexes for accessing address/value items from JSON documents
constexpr int address = 0;
constexpr int value   = 1;

void testOpcode(std::string_view testFileName)
{
    std::ifstream f(std::format("./tests/{}", testFileName));

    json tests = json::parse(f);

    for (auto& test : tests) {
        CPUState initialCPUState;
        initialCPUState.accumulator     = test["initial"]["a"];
        initialCPUState.pc              = test["initial"]["pc"];
        initialCPUState.sp              = test["initial"]["s"];
        initialCPUState.indexX          = test["initial"]["x"];
        initialCPUState.indexY          = test["initial"]["y"];
        initialCPUState.processorStatus = test["initial"]["p"];

        NES nes(initialCPUState);

        // Set memory to initial values
        // ramItem[0] is the address and ramItem[1] is the value
        for (auto& ramItem : test["initial"]["ram"]) {
            nes.setMemoryValue(ramItem[address], ramItem[value]);
        }

        // Run CPU for the given instruction
        nes.tickCPU();

        // Get the final state of the CPU
        CPUState endCPUState = nes.getCPUState();

        // Get the expected state of the CPU
        CPUState expectedCPUState;
        expectedCPUState.accumulator     = test["final"]["a"];
        expectedCPUState.pc              = test["final"]["pc"];
        expectedCPUState.sp              = test["final"]["s"];
        expectedCPUState.indexX          = test["final"]["x"];
        expectedCPUState.indexY          = test["final"]["y"];
        expectedCPUState.processorStatus = test["final"]["p"];

        bool ramMatch = true;

        std::ostringstream expectedMem;
        std::ostringstream actualMem;

        // Check if memory contents equal expected values
        for (auto& ramItem : test["final"]["ram"]) {
            expectedMem << "\tAddr: " << ramItem[address] << " Val: " << ramItem[value] << "\n";
            actualMem << "\tAddr: " << ramItem[address] << " Val: " << +nes.getMemoryValue(ramItem[address]) << "\n";
            if (ramItem[value] != nes.getMemoryValue(ramItem[address])) {
                ramMatch = false;
            }
        }

        // Info statements will only be displayed if an assertion fails
        INFO("Operation: " + static_cast<std::string>(test["name"]));
        INFO("Expected CPU State:\n\t" + expectedCPUState.toString() + "\nActual CPU State:\n\t" + endCPUState.toString());
        INFO("Expected Memory State:\n" + expectedMem.str() + "Actual Memory State:\n" + actualMem.str());

        REQUIRE( (expectedCPUState == endCPUState) );
        REQUIRE( ramMatch );
    }
}

TEST_CASE("Opcode $00", "[BRK]") { testOpcode("00.json"); }
TEST_CASE("Opcode $01", "[ORA]") { testOpcode("01.json"); }
TEST_CASE("Opcode $05", "[ORA]") { testOpcode("05.json"); }
TEST_CASE("Opcode $06", "[ASL]") { testOpcode("06.json"); }
TEST_CASE("Opcode $08", "[PHP]") { testOpcode("08.json"); }
TEST_CASE("Opcode $09", "[ORA]") { testOpcode("09.json"); }
TEST_CASE("Opcode $0a", "[ASL]") { testOpcode("0a.json"); }
TEST_CASE("Opcode $0d", "[ORA]") { testOpcode("0d.json"); }
TEST_CASE("Opcode $0e", "[ASL]") { testOpcode("0e.json"); }

TEST_CASE("Opcode $10", "[BPL]") { testOpcode("10.json"); }
TEST_CASE("Opcode $11", "[ORA]") { testOpcode("11.json"); }
TEST_CASE("Opcode $15", "[ORA]") { testOpcode("15.json"); }
TEST_CASE("Opcode $16", "[ASL]") { testOpcode("16.json"); }
TEST_CASE("Opcode $18", "[CLC]") { testOpcode("18.json"); }
TEST_CASE("Opcode $19", "[ORA]") { testOpcode("19.json"); }
TEST_CASE("Opcode $1d", "[ORA]") { testOpcode("1d.json"); }
TEST_CASE("Opcode $1e", "[ASL]") { testOpcode("1e.json"); }

TEST_CASE("Opcode $20", "[JSR]") { testOpcode("20.json"); }
TEST_CASE("Opcode $21", "[AND]") { testOpcode("21.json"); }
TEST_CASE("Opcode $24", "[BIT]") { testOpcode("24.json"); }
TEST_CASE("Opcode $25", "[AND]") { testOpcode("25.json"); }
TEST_CASE("Opcode $26", "[ROL]") { testOpcode("26.json"); }
TEST_CASE("Opcode $28", "[PLP]") { testOpcode("28.json"); }
TEST_CASE("Opcode $29", "[AND]") { testOpcode("29.json"); }
TEST_CASE("Opcode $2a", "[ROL]") { testOpcode("2a.json"); }
TEST_CASE("Opcode $2c", "[BIT]") { testOpcode("2c.json"); }
TEST_CASE("Opcode $2d", "[AND]") { testOpcode("2d.json"); }
TEST_CASE("Opcode $2e", "[ROL]") { testOpcode("2e.json"); }

TEST_CASE("Opcode $30", "[BMI]") { testOpcode("30.json"); }
TEST_CASE("Opcode $31", "[AND]") { testOpcode("31.json"); }
TEST_CASE("Opcode $35", "[AND]") { testOpcode("35.json"); }
TEST_CASE("Opcode $36", "[ROL]") { testOpcode("36.json"); }
TEST_CASE("Opcode $38", "[SEC]") { testOpcode("38.json"); }
TEST_CASE("Opcode $39", "[AND]") { testOpcode("39.json"); }
TEST_CASE("Opcode $3d", "[AND]") { testOpcode("3d.json"); }
TEST_CASE("Opcode $3e", "[ROL]") { testOpcode("3e.json"); }

TEST_CASE("Opcode $40", "[RTI]") { testOpcode("40.json"); }
TEST_CASE("Opcode $41", "[EOR]") { testOpcode("41.json"); }
TEST_CASE("Opcode $45", "[EOR]") { testOpcode("45.json"); }
TEST_CASE("Opcode $46", "[LSR]") { testOpcode("46.json"); }
TEST_CASE("Opcode $48", "[PHA]") { testOpcode("48.json"); }
TEST_CASE("Opcode $49", "[EOR]") { testOpcode("49.json"); }
TEST_CASE("Opcode $4a", "[LSR]") { testOpcode("4a.json"); }
TEST_CASE("Opcode $4c", "[JMP]") { testOpcode("4c.json"); }
TEST_CASE("Opcode $4d", "[EOR]") { testOpcode("4d.json"); }
TEST_CASE("Opcode $4e", "[LSR]") { testOpcode("4e.json"); }

TEST_CASE("Opcode $50", "[BVC]") { testOpcode("50.json"); }
TEST_CASE("Opcode $51", "[EOR]") { testOpcode("51.json"); }
TEST_CASE("Opcode $55", "[EOR]") { testOpcode("55.json"); }
TEST_CASE("Opcode $56", "[LSR]") { testOpcode("56.json"); }
TEST_CASE("Opcode $58", "[CLI]") { testOpcode("58.json"); }
TEST_CASE("Opcode $59", "[EOR]") { testOpcode("59.json"); }
TEST_CASE("Opcode $5d", "[EOR]") { testOpcode("5d.json"); }
TEST_CASE("Opcode $5e", "[LSR]") { testOpcode("5e.json"); }

TEST_CASE("Opcode $60", "[RTS]") { testOpcode("60.json"); }
TEST_CASE("Opcode $61", "[ADC]") { testOpcode("61.json"); }
TEST_CASE("Opcode $65", "[ADC]") { testOpcode("65.json"); }
TEST_CASE("Opcode $66", "[ROR]") { testOpcode("66.json"); }
TEST_CASE("Opcode $68", "[PLA]") { testOpcode("68.json"); }
TEST_CASE("Opcode $69", "[ADC]") { testOpcode("69.json"); }
TEST_CASE("Opcode $6a", "[ROR]") { testOpcode("6a.json"); }
TEST_CASE("Opcode $6c", "[JMP]") { testOpcode("6c.json"); }
TEST_CASE("Opcode $6d", "[ADC]") { testOpcode("6d.json"); }
TEST_CASE("Opcode $6e", "[ROR]") { testOpcode("6e.json"); }

TEST_CASE("Opcode $70", "[BVS]") { testOpcode("70.json"); }
TEST_CASE("Opcode $71", "[ADC]") { testOpcode("71.json"); }
TEST_CASE("Opcode $75", "[ADC]") { testOpcode("75.json"); }
TEST_CASE("Opcode $76", "[ROR]") { testOpcode("76.json"); }
TEST_CASE("Opcode $78", "[SEI]") { testOpcode("78.json"); }
TEST_CASE("Opcode $79", "[ADC]") { testOpcode("79.json"); }
TEST_CASE("Opcode $7d", "[ADC]") { testOpcode("7d.json"); }
TEST_CASE("Opcode $7e", "[ROR]") { testOpcode("7e.json"); }

TEST_CASE("Opcode $81", "[STA]") { testOpcode("81.json"); }
TEST_CASE("Opcode $84", "[STY]") { testOpcode("84.json"); }
TEST_CASE("Opcode $85", "[STA]") { testOpcode("85.json"); }
TEST_CASE("Opcode $86", "[STX]") { testOpcode("86.json"); }
TEST_CASE("Opcode $88", "[DEY]") { testOpcode("88.json"); }
TEST_CASE("Opcode $8a", "[TXA]") { testOpcode("8a.json"); }
TEST_CASE("Opcode $8c", "[STY]") { testOpcode("8c.json"); }
TEST_CASE("Opcode $8d", "[STA]") { testOpcode("8d.json"); }
TEST_CASE("Opcode $8e", "[STX]") { testOpcode("8e.json"); }

TEST_CASE("Opcode $90", "[BCC]") { testOpcode("90.json"); }
TEST_CASE("Opcode $91", "[STA]") { testOpcode("91.json"); }
TEST_CASE("Opcode $94", "[STY]") { testOpcode("94.json"); }
TEST_CASE("Opcode $95", "[STA]") { testOpcode("95.json"); }
TEST_CASE("Opcode $96", "[STX]") { testOpcode("96.json"); }
TEST_CASE("Opcode $98", "[TYA]") { testOpcode("98.json"); }
TEST_CASE("Opcode $99", "[STA]") { testOpcode("99.json"); }
TEST_CASE("Opcode $9a", "[TXS]") { testOpcode("9a.json"); }
TEST_CASE("Opcode $9d", "[STA]") { testOpcode("9d.json"); }

TEST_CASE("Opcode $a0", "[LDY]") { testOpcode("a0.json"); }
TEST_CASE("Opcode $a1", "[LDA]") { testOpcode("a1.json"); }
TEST_CASE("Opcode $a2", "[LDX]") { testOpcode("a2.json"); }
TEST_CASE("Opcode $a4", "[LDY]") { testOpcode("a4.json"); }
TEST_CASE("Opcode $a5", "[LDA]") { testOpcode("a5.json"); }
TEST_CASE("Opcode $a6", "[LDX]") { testOpcode("a6.json"); }
TEST_CASE("Opcode $a8", "[TAY]") { testOpcode("a8.json"); }
TEST_CASE("Opcode $a9", "[LDA]") { testOpcode("a9.json"); }
TEST_CASE("Opcode $aa", "[TAX]") { testOpcode("aa.json"); }
TEST_CASE("Opcode $ac", "[LDY]") { testOpcode("ac.json"); }
TEST_CASE("Opcode $ad", "[LDA]") { testOpcode("ad.json"); }
TEST_CASE("Opcode $ae", "[LDX]") { testOpcode("ae.json"); }

TEST_CASE("Opcode $b0", "[BCS]") { testOpcode("b0.json"); }
TEST_CASE("Opcode $b1", "[LDA]") { testOpcode("b1.json"); }
TEST_CASE("Opcode $b4", "[LDY]") { testOpcode("b4.json"); }
TEST_CASE("Opcode $b5", "[LDA]") { testOpcode("b5.json"); }
TEST_CASE("Opcode $b6", "[LDX]") { testOpcode("b6.json"); }
TEST_CASE("Opcode $b8", "[CLV]") { testOpcode("b8.json"); }
TEST_CASE("Opcode $b9", "[LDA]") { testOpcode("b9.json"); }
TEST_CASE("Opcode $ba", "[TSX]") { testOpcode("ba.json"); }
TEST_CASE("Opcode $bc", "[LDY]") { testOpcode("bc.json"); }
TEST_CASE("Opcode $bd", "[LDA]") { testOpcode("bd.json"); }
TEST_CASE("Opcode $be", "[LDX]") { testOpcode("be.json"); }

TEST_CASE("Opcode $c0", "[CPY]") { testOpcode("c0.json"); }
TEST_CASE("Opcode $c1", "[CMP]") { testOpcode("c1.json"); }
TEST_CASE("Opcode $c4", "[CPY]") { testOpcode("c4.json"); }
TEST_CASE("Opcode $c5", "[CMP]") { testOpcode("c5.json"); }
TEST_CASE("Opcode $c6", "[DEC]") { testOpcode("c6.json"); }
TEST_CASE("Opcode $c8", "[INY]") { testOpcode("c8.json"); }
TEST_CASE("Opcode $c9", "[CMP]") { testOpcode("c9.json"); }
TEST_CASE("Opcode $ca", "[DEX]") { testOpcode("ca.json"); }
TEST_CASE("Opcode $cc", "[CPY]") { testOpcode("cc.json"); }
TEST_CASE("Opcode $cd", "[CMP]") { testOpcode("cd.json"); }
TEST_CASE("Opcode $ce", "[DEC]") { testOpcode("ce.json"); }

TEST_CASE("Opcode $d0", "[BNE]") { testOpcode("d0.json"); }
TEST_CASE("Opcode $d1", "[CMP]") { testOpcode("d1.json"); }
TEST_CASE("Opcode $d5", "[CMP]") { testOpcode("d5.json"); }
TEST_CASE("Opcode $d6", "[DEC]") { testOpcode("d6.json"); }
TEST_CASE("Opcode $d8", "[CLD]") { testOpcode("d8.json"); }
TEST_CASE("Opcode $d9", "[CMP]") { testOpcode("d9.json"); }
TEST_CASE("Opcode $dd", "[CMP]") { testOpcode("dd.json"); }
TEST_CASE("Opcode $de", "[DEC]") { testOpcode("de.json"); }

TEST_CASE("Opcode $e0", "[CPX]") { testOpcode("e0.json"); }
TEST_CASE("Opcode $e1", "[SBC]") { testOpcode("e1.json"); }
TEST_CASE("Opcode $e4", "[CPX]") { testOpcode("e4.json"); }
TEST_CASE("Opcode $e5", "[SBC]") { testOpcode("e5.json"); }
TEST_CASE("Opcode $e6", "[INC]") { testOpcode("e6.json"); }
TEST_CASE("Opcode $e8", "[INX]") { testOpcode("e8.json"); }
TEST_CASE("Opcode $e9", "[SBC]") { testOpcode("e9.json"); }
TEST_CASE("Opcode $ea", "[NOP]") { testOpcode("ea.json"); }
TEST_CASE("Opcode $ec", "[CPX]") { testOpcode("ec.json"); }
TEST_CASE("Opcode $ed", "[SBC]") { testOpcode("ed.json"); }
TEST_CASE("Opcode $ee", "[INC]") { testOpcode("ee.json"); }

TEST_CASE("Opcode $f0", "[BEQ]") { testOpcode("f0.json"); }
TEST_CASE("Opcode $f1", "[SBC]") { testOpcode("f1.json"); }
TEST_CASE("Opcode $f5", "[SBC]") { testOpcode("f5.json"); }
TEST_CASE("Opcode $f6", "[INC]") { testOpcode("f6.json"); }
TEST_CASE("Opcode $f8", "[SED]") { testOpcode("f8.json"); }
TEST_CASE("Opcode $f9", "[SBC]") { testOpcode("f9.json"); }
TEST_CASE("Opcode $fd", "[SBC]") { testOpcode("fd.json"); }
TEST_CASE("Opcode $fe", "[INC]") { testOpcode("fe.json"); }