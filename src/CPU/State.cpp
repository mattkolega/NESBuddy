#include "State.h"

bool operator==(CPUState &state1, CPUState &state2)
{
    if (state1.accumulator != state2.accumulator) { return false; }
    if (state1.indexX != state2.indexX) { return false; }
    if (state1.indexY != state2.indexY) { return false; }
    if (state1.pc != state2.pc) { return false; }
    if (state1.sp != state2.sp) { return false; }
    if (state1.processorStatus != state2.processorStatus) { return false; }

    return true;
}