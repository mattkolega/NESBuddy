#include "NES.h"

#include <iostream>

#include "Cartridge/Parser.h"
#include "Cartridge/Mappers/Mapper.h"
#include "Cartridge/Mappers/Mapper000.h"
#include "Cartridge/Mappers/NoMapper.h"
#include "CPU/State.h"

NES::NES()
{
    std::optional<Cartridge> cart;

    while (!cart.has_value()) {
        cart = ROMParser::openBinaryFile();
    }

    this->cartridge = cart.value();

    switch (cartridge.mapperId) {
        case 0:
            mapper = std::make_unique<Mapper000>(cartridge);
            break;
        default:
            std::cerr << "[ERROR]: Unrecognised/unsupported mapper number in cartridge." << std::endl;
            mapper = nullptr;
            break;
    }

    cpu.connectToNes(this);
    cpu.setToPowerUpState();
}

NES::NES(CPUState &initialState) : cpu(initialState)
{
    Cartridge cart {};
    cart.prgROM = std::vector<uint8_t>(1024 * 32);
    cart.prgROMBanks = 1;

    cartridge = cart;
    mapper = std::make_unique<NoMapper>(cartridge);

    cpu.connectToNes(this);
}

uint8_t NES::getMemoryValue(uint16_t address)
{
    if (address >= 0x0 && address <= 0x7FFF) {
        return memory[address];
    } else {
        return mapper->prgRead(address - 0x8000);
    }
}

void NES::setMemoryValue(uint16_t address, uint8_t value)
{
    if (address >= 0x0 && address <= 0x7FFF) {
        memory[address] = value;
    } else {
        mapper->prgWrite(address - 0x8000, value);
    }
}

void NES::tickCPU()
{
    cpu.tick();
}

CPUState NES::getCPUState()
{
    return cpu.getState();
}