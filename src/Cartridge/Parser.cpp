#include "Parser.h"

#include <nfd.hpp>

#include "../Logger.h"
#include "Mappers/Mapper000.h"

namespace ROMParser 
{
    namespace  // Contains functions which can only be used in openBinaryFile()
    {
        // Opens system file dialog for user to select a ROM file
        std::string getFilePath() 
        {
            NFD::Guard nfdGuard;

            NFD::UniquePath outPath;

            nfdfilteritem_t filterItem[1] = {{"NES Binary File", "nes"}};

            nfdresult_t dialogResult = NFD::OpenDialog(outPath, filterItem, 1);

            if (dialogResult == NFD_CANCEL) {
                Logger::printWarning("File dialog was closed by user.");
                return "";
            } else if (dialogResult != NFD_OKAY) {
                Logger::printError(NFD::GetError());
                return "";
            }

            std::string filepath = outPath.get();

            return filepath;
        }

        FileFormat identifyFileFormat(std::array<uint8_t, 16> &header)
        {
            FileFormat fileFormat;

            // Valid NES header must start with $4E $45 $53 $1A
            if (header[0] == 'N' && header[1] == 'E' && header[2] == 'S' && header[3] == 0x1A) {
                fileFormat = FileFormat::iNES;
            } else {
                fileFormat = FileFormat::invalid;
            }

            if (fileFormat == FileFormat::iNES && (header[7] & 0x0C) == 0x08) {
                fileFormat = FileFormat::NES20;
            }

            return fileFormat;
        }

        // Based on file structure specified at https://www.nesdev.org/wiki/INES
        NESHeader parseiNESHeader(std::array<uint8_t, 16> &headerBytes, std::ifstream &romFile)
        {
            NESHeader header;

            header.prgROMBanks = headerBytes[4];
            header.chrROMBanks = headerBytes[5];

            if (headerBytes[6] & 0x04) {
                header.prgRAMSize = 8192;
            }

            if (header.chrROMBanks == 0) {
                header.chrRAMSize = 8192;
            }
            
            header.nametable = (headerBytes[6] & 0x01) == 0 
                                ? Nametable::verticalArrangement 
                                : Nametable::horizontalArrangement;

            header.trainerPresent = headerBytes[6] & 0x02;

            bool alternativeNametable = headerBytes[6] & 0x08;

            uint8_t mapperLSB = (headerBytes[6] >> 4) & 0x0F;
            uint8_t mapperMSB = (headerBytes[7] >> 4) & 0x0F;
            header.mapperId = (mapperMSB << 4) | mapperLSB;

            switch (headerBytes[7] & 0x03) {
                case 0x00: header.targetSystem = SystemType::nes; break;
                case 0x01: header.targetSystem = SystemType::vsSystem; break;
                case 0x02: header.targetSystem = SystemType::playchoice; break;
            }
            
            header.defaultPeripheral = PeripheralType::standardController;
            header.gameRegion = Region::ntsc;

            return header;
        }

        // Based on file structure specified at https://www.nesdev.org/wiki/NES_2.0
        NESHeader parseNES20Header(std::array<uint8_t, 16> &headerBytes, std::ifstream &romFile)
        {
            NESHeader header;

            uint8_t prgROMBanksLSB = headerBytes[4];
            uint8_t prgROMBanksMSB = headerBytes[9] & 0x0F;
            header.prgROMBanks = (prgROMBanksMSB << 8) | prgROMBanksLSB;

            uint8_t chrROMBanksLSB = headerBytes[5];
            uint8_t chrROMBanksMSB = (headerBytes[9] >> 4) & 0x0F;
            header.chrROMBanks = (chrROMBanksMSB << 8) | chrROMBanksLSB;

            uint8_t prgRAMShiftCount = (headerBytes[10] & 0x0F);
            header.prgRAMSize = prgRAMShiftCount ? (64 << prgRAMShiftCount) : 0;

            uint8_t chrRAMShiftCount = (headerBytes[11] & 0x0F);
            header.chrRAMSize = chrRAMShiftCount ? (64 << chrRAMShiftCount) : 0;

            header.nametable = (headerBytes[6] & 0x01) == 0 
                                ? Nametable::verticalArrangement 
                                : Nametable::horizontalArrangement;

            header.trainerPresent = headerBytes[6] & 0x02;
            bool alternativeNametable = headerBytes[6] & 0x08;

            uint8_t mapperLow = (headerBytes[6] >> 4) & 0x0F;
            uint8_t mapperMiddle = (headerBytes[7] >> 4) & 0x0F;
            uint8_t mapperHigh = headerBytes[8] & 0x0F;
            header.mapperId = (mapperHigh << 8) | (mapperMiddle << 4) | mapperLow;

            switch (headerBytes[7] & 0x03) {
                case 0x00: header.targetSystem = SystemType::nes; break;
                case 0x01: header.targetSystem = SystemType::vsSystem; break;
                case 0x02: header.targetSystem = SystemType::playchoice; break;
            }

            switch (headerBytes[15] & 0x3F) {
                case 0x00: 
                case 0x01: header.defaultPeripheral = PeripheralType::standardController; break;
                default: header.defaultPeripheral = PeripheralType::unsupported; break;
            }

            switch (headerBytes[12] & 0x03) {
                case 0x00: header.gameRegion = Region::ntsc; break;
                case 0x01: header.gameRegion = Region::pal; break;
                case 0x02: header.gameRegion = Region::global; break;
            }
            
            return header;
        }

        std::optional<Cartridge> readFromNesFile(NESHeader &header, std::ifstream &romFile)
        {
            if (header.targetSystem != SystemType::nes) {
                Logger::printError("Unsupported console type is specified in ROM file.");
                return std::nullopt;
            }

            if (header.defaultPeripheral == PeripheralType::unsupported) {
                Logger::printError("Unsupported peripheral type is specified in ROM file.");
                return std::nullopt;
            }

            std::optional<std::array<uint8_t, 512>> trainerData; 
            if (header.trainerPresent) {
                std::array<uint8_t, 512> trainerDataArray;
                romFile.read(reinterpret_cast<char*>(trainerDataArray.data()), 512);
                trainerData = trainerDataArray;
            } else {
                trainerData = std::nullopt;
            }

            size_t numOfPrgROMBytes = 16384 * header.prgROMBanks;
            std::vector<uint8_t> prgROM(numOfPrgROMBytes);
            romFile.read(reinterpret_cast<char*>(prgROM.data()), numOfPrgROMBytes);

            size_t numOfChrROMBytes = 8192 * header.chrROMBanks;
            std::vector<uint8_t> chrROM(numOfChrROMBytes);
            romFile.read(reinterpret_cast<char*>(chrROM.data()), numOfChrROMBytes);

            Cartridge cartridge {
                header.mapperId,
                header.nametable,
                trainerData,
                prgROM,
                chrROM,
                header.prgROMBanks,
                header.chrROMBanks,
                std::vector<uint8_t>(header.prgRAMSize),
                std::vector<uint8_t>(header.chrRAMSize),
                header.gameRegion
            };

            return cartridge;
        }
    }

    std::optional<Cartridge> openBinaryFile()
    {
        const std::string filepath = ROMParser::getFilePath();
        if (filepath == "") return std::nullopt;

        std::ifstream romFile;
        romFile.open(filepath, std::ios::binary | std::ios::in);

        if (!romFile) {
            Logger::printError("ROM file failed to open: " + filepath);
            return std::nullopt;
        }

        Logger::printInfo("Loading .nes file: " + filepath);

        constexpr int headerSize = 16;
        std::array<uint8_t, headerSize> headerBytes;

        romFile.read(reinterpret_cast<char*>(headerBytes.data()), headerSize);
        if (romFile.fail()) {
                Logger::printError("Failure to read from ROM file: " + filepath);
                return std::nullopt;
        }

        FileFormat fileFormat = ROMParser::identifyFileFormat(headerBytes);

        NESHeader header;

        if (fileFormat == FileFormat::iNES) {
            header = ROMParser::parseiNESHeader(headerBytes, romFile);
        }
        else if (fileFormat == FileFormat::NES20) {
            header = ROMParser::parseNES20Header(headerBytes, romFile);
        }
        else if (fileFormat == FileFormat::invalid) {
            Logger::printError("Invalid file format. Please ensure ROM file is a valid .nes file.");
            return std::nullopt;
        }

        return ROMParser::readFromNesFile(header, romFile);
    }
}