#include <format>
#include <fstream>
#include <sstream>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "../src/CPU/State.h"
#include "../src/NES.h"

using json = nlohmann::json;

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
            nes.setMemoryValue(ramItem[0], ramItem[1]);
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
            expectedMem << "\tAddr: " << ramItem[0] << " Val: " << ramItem[1] << "\n";
            actualMem << "\tAddr: " << ramItem[0] << " Val: " << +nes.getMemoryValue(ramItem[0]) << "\n";
            if (ramItem[1] != nes.getMemoryValue(ramItem[0])) {
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

TEST_CASE("Opcode $00") { testOpcode("00.json"); }
TEST_CASE("Opcode $01") { testOpcode("01.json"); }
TEST_CASE("Opcode $05") { testOpcode("05.json"); }
TEST_CASE("Opcode $06") { testOpcode("06.json"); }
TEST_CASE("Opcode $08") { testOpcode("08.json"); }
TEST_CASE("Opcode $09") { testOpcode("09.json"); }
TEST_CASE("Opcode $0a") { testOpcode("0a.json"); }
TEST_CASE("Opcode $0d") { testOpcode("0d.json"); }
TEST_CASE("Opcode $0e") { testOpcode("0e.json"); }

TEST_CASE("Opcode $10") { testOpcode("10.json"); }
TEST_CASE("Opcode $11") { testOpcode("11.json"); }
TEST_CASE("Opcode $15") { testOpcode("15.json"); }
TEST_CASE("Opcode $16") { testOpcode("16.json"); }
TEST_CASE("Opcode $18") { testOpcode("18.json"); }
TEST_CASE("Opcode $19") { testOpcode("19.json"); }
TEST_CASE("Opcode $1d") { testOpcode("1d.json"); }
TEST_CASE("Opcode $1e") { testOpcode("1e.json"); }

TEST_CASE("Opcode $20") { testOpcode("20.json"); }
TEST_CASE("Opcode $21") { testOpcode("21.json"); }
TEST_CASE("Opcode $24") { testOpcode("24.json"); }
TEST_CASE("Opcode $25") { testOpcode("25.json"); }
TEST_CASE("Opcode $26") { testOpcode("26.json"); }
TEST_CASE("Opcode $28") { testOpcode("28.json"); }
TEST_CASE("Opcode $29") { testOpcode("29.json"); }
TEST_CASE("Opcode $2a") { testOpcode("2a.json"); }
TEST_CASE("Opcode $2c") { testOpcode("2c.json"); }
TEST_CASE("Opcode $2d") { testOpcode("2d.json"); }
TEST_CASE("Opcode $2e") { testOpcode("2e.json"); }

TEST_CASE("Opcode $30") { testOpcode("30.json"); }
TEST_CASE("Opcode $31") { testOpcode("31.json"); }
TEST_CASE("Opcode $35") { testOpcode("35.json"); }
TEST_CASE("Opcode $36") { testOpcode("36.json"); }
TEST_CASE("Opcode $38") { testOpcode("38.json"); }
TEST_CASE("Opcode $39") { testOpcode("39.json"); }
TEST_CASE("Opcode $3d") { testOpcode("3d.json"); }
TEST_CASE("Opcode $3e") { testOpcode("3e.json"); }

TEST_CASE("Opcode $40") { testOpcode("40.json"); }
TEST_CASE("Opcode $41") { testOpcode("41.json"); }
TEST_CASE("Opcode $45") { testOpcode("45.json"); }
TEST_CASE("Opcode $46") { testOpcode("46.json"); }
TEST_CASE("Opcode $48") { testOpcode("48.json"); }
TEST_CASE("Opcode $49") { testOpcode("49.json"); }
TEST_CASE("Opcode $4a") { testOpcode("4a.json"); }
TEST_CASE("Opcode $4c") { testOpcode("4c.json"); }
TEST_CASE("Opcode $4d") { testOpcode("4d.json"); }
TEST_CASE("Opcode $4e") { testOpcode("4e.json"); }

TEST_CASE("Opcode $50") { testOpcode("50.json"); }
TEST_CASE("Opcode $51") { testOpcode("51.json"); }
TEST_CASE("Opcode $55") { testOpcode("55.json"); }
TEST_CASE("Opcode $56") { testOpcode("56.json"); }
TEST_CASE("Opcode $58") { testOpcode("58.json"); }
TEST_CASE("Opcode $59") { testOpcode("59.json"); }
TEST_CASE("Opcode $5d") { testOpcode("5d.json"); }
TEST_CASE("Opcode $5e") { testOpcode("5e.json"); }

TEST_CASE("Opcode $60") { testOpcode("60.json"); }
TEST_CASE("Opcode $61") { testOpcode("61.json"); }
TEST_CASE("Opcode $65") { testOpcode("65.json"); }
TEST_CASE("Opcode $66") { testOpcode("66.json"); }
TEST_CASE("Opcode $68") { testOpcode("68.json"); }
TEST_CASE("Opcode $69") { testOpcode("69.json"); }
TEST_CASE("Opcode $6a") { testOpcode("6a.json"); }
TEST_CASE("Opcode $6c") { testOpcode("6c.json"); }
TEST_CASE("Opcode $6d") { testOpcode("6d.json"); }
TEST_CASE("Opcode $6e") { testOpcode("6e.json"); }

TEST_CASE("Opcode $70") { testOpcode("70.json"); }
TEST_CASE("Opcode $71") { testOpcode("71.json"); }
TEST_CASE("Opcode $75") { testOpcode("75.json"); }
TEST_CASE("Opcode $76") { testOpcode("76.json"); }
TEST_CASE("Opcode $78") { testOpcode("78.json"); }
TEST_CASE("Opcode $79") { testOpcode("79.json"); }
TEST_CASE("Opcode $7d") { testOpcode("7d.json"); }
TEST_CASE("Opcode $7e") { testOpcode("7e.json"); }

TEST_CASE("Opcode $81") { testOpcode("81.json"); }
TEST_CASE("Opcode $84") { testOpcode("84.json"); }
TEST_CASE("Opcode $85") { testOpcode("85.json"); }
TEST_CASE("Opcode $86") { testOpcode("86.json"); }
TEST_CASE("Opcode $88") { testOpcode("88.json"); }
TEST_CASE("Opcode $8a") { testOpcode("8a.json"); }
TEST_CASE("Opcode $8c") { testOpcode("8c.json"); }
TEST_CASE("Opcode $8d") { testOpcode("8d.json"); }
TEST_CASE("Opcode $8e") { testOpcode("8e.json"); }

TEST_CASE("Opcode $90") { testOpcode("90.json"); }
TEST_CASE("Opcode $91") { testOpcode("91.json"); }
TEST_CASE("Opcode $94") { testOpcode("94.json"); }
TEST_CASE("Opcode $95") { testOpcode("95.json"); }
TEST_CASE("Opcode $96") { testOpcode("96.json"); }
TEST_CASE("Opcode $98") { testOpcode("98.json"); }
TEST_CASE("Opcode $99") { testOpcode("99.json"); }
TEST_CASE("Opcode $9a") { testOpcode("9a.json"); }
TEST_CASE("Opcode $9d") { testOpcode("9d.json"); }

TEST_CASE("Opcode $a0") { testOpcode("a0.json"); }
TEST_CASE("Opcode $a1") { testOpcode("a1.json"); }
TEST_CASE("Opcode $a2") { testOpcode("a2.json"); }
TEST_CASE("Opcode $a4") { testOpcode("a4.json"); }
TEST_CASE("Opcode $a5") { testOpcode("a5.json"); }
TEST_CASE("Opcode $a6") { testOpcode("a6.json"); }
TEST_CASE("Opcode $a8") { testOpcode("a8.json"); }
TEST_CASE("Opcode $a9") { testOpcode("a9.json"); }
TEST_CASE("Opcode $aa") { testOpcode("aa.json"); }
TEST_CASE("Opcode $ac") { testOpcode("ac.json"); }
TEST_CASE("Opcode $ad") { testOpcode("ad.json"); }
TEST_CASE("Opcode $ae") { testOpcode("ae.json"); }

TEST_CASE("Opcode $b0") { testOpcode("b0.json"); }
TEST_CASE("Opcode $b1") { testOpcode("b1.json"); }
TEST_CASE("Opcode $b4") { testOpcode("b4.json"); }
TEST_CASE("Opcode $b5") { testOpcode("b5.json"); }
TEST_CASE("Opcode $b6") { testOpcode("b6.json"); }
TEST_CASE("Opcode $b8") { testOpcode("b8.json"); }
TEST_CASE("Opcode $b9") { testOpcode("b9.json"); }
TEST_CASE("Opcode $ba") { testOpcode("ba.json"); }
TEST_CASE("Opcode $bc") { testOpcode("bc.json"); }
TEST_CASE("Opcode $bd") { testOpcode("bd.json"); }
TEST_CASE("Opcode $be") { testOpcode("be.json"); }

TEST_CASE("Opcode $c0") { testOpcode("c0.json"); }
TEST_CASE("Opcode $c1") { testOpcode("c1.json"); }
TEST_CASE("Opcode $c4") { testOpcode("c4.json"); }
TEST_CASE("Opcode $c5") { testOpcode("c5.json"); }
TEST_CASE("Opcode $c6") { testOpcode("c6.json"); }
TEST_CASE("Opcode $c8") { testOpcode("c8.json"); }
TEST_CASE("Opcode $c9") { testOpcode("c9.json"); }
TEST_CASE("Opcode $ca") { testOpcode("ca.json"); }
TEST_CASE("Opcode $cc") { testOpcode("cc.json"); }
TEST_CASE("Opcode $cd") { testOpcode("cd.json"); }
TEST_CASE("Opcode $ce") { testOpcode("ce.json"); }

TEST_CASE("Opcode $d0") { testOpcode("d0.json"); }
TEST_CASE("Opcode $d1") { testOpcode("d1.json"); }
TEST_CASE("Opcode $d5") { testOpcode("d5.json"); }
TEST_CASE("Opcode $d6") { testOpcode("d6.json"); }
TEST_CASE("Opcode $d8") { testOpcode("d8.json"); }
TEST_CASE("Opcode $d9") { testOpcode("d9.json"); }
TEST_CASE("Opcode $dd") { testOpcode("dd.json"); }
TEST_CASE("Opcode $de") { testOpcode("de.json"); }

TEST_CASE("Opcode $e0") { testOpcode("e0.json"); }
TEST_CASE("Opcode $e1") { testOpcode("e1.json"); }
TEST_CASE("Opcode $e4") { testOpcode("e4.json"); }
TEST_CASE("Opcode $e5") { testOpcode("e5.json"); }
TEST_CASE("Opcode $e6") { testOpcode("e6.json"); }
TEST_CASE("Opcode $e8") { testOpcode("e8.json"); }
TEST_CASE("Opcode $e9") { testOpcode("e9.json"); }
TEST_CASE("Opcode $ea") { testOpcode("ea.json"); }
TEST_CASE("Opcode $ec") { testOpcode("ec.json"); }
TEST_CASE("Opcode $ed") { testOpcode("ed.json"); }
TEST_CASE("Opcode $ee") { testOpcode("ee.json"); }

TEST_CASE("Opcode $f0") { testOpcode("f0.json"); }
TEST_CASE("Opcode $f1") { testOpcode("f1.json"); }
TEST_CASE("Opcode $f5") { testOpcode("f5.json"); }
TEST_CASE("Opcode $f6") { testOpcode("f6.json"); }
TEST_CASE("Opcode $f8") { testOpcode("f8.json"); }
TEST_CASE("Opcode $f9") { testOpcode("f9.json"); }
TEST_CASE("Opcode $fd") { testOpcode("fd.json"); }
TEST_CASE("Opcode $fe") { testOpcode("fe.json"); }