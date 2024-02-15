#pragma once

#include <cstdint>
#include <string>

struct CPUState
{
    uint16_t pc {};
    uint8_t sp {};
    uint8_t accumulator {};
    uint8_t indexX {};
    uint8_t indexY {};
    uint8_t processorStatus {};

    std::string toString();
};

bool operator==(CPUState &state1, CPUState &state2);