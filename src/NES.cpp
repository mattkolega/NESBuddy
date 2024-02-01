#include "NES.h"

NES::NES()
{
    cpu.connectToNes(this);
}