#include "NES.h"

#include <stdexcept>

#include "Cartridge/Parser.h"
#include "Cartridge/Mappers/Mapper.h"
#include "Cartridge/Mappers/Mapper000.h"
#include "Cartridge/Mappers/NoMapper.h"
#include "CPU/State.h"
#include "Logger.h"

NES::NES()
{
    std::optional<Cartridge> cart;

    int fileOpenAttempts = 0;

    while (!cart.has_value()) {
        cart = ROMParser::openBinaryFile();
        if (++fileOpenAttempts == 3) {
            throw std::runtime_error("User has failed to provide .nes file.");
        }
    }

    this->cartridge = cart.value();

    switch (cartridge.mapperId) {
        case 0:
            mapper = std::make_unique<Mapper000>(cartridge);
            break;
        default:
            Logger::printError("Unrecognised/unsupported mapper number in cartridge.");
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

uint8_t NES::memoryRead(uint16_t address)
{
    if (address >= 0x0 && address <= 0x7FFF) {
        return memory[address];
    } else {
        return mapper->prgRead(address - 0x8000);
    }
}

void NES::memoryWrite(uint16_t address, uint8_t value)
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