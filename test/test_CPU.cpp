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