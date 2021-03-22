#ifndef CARTRIDGE_UTIL_H
#define CARTRIDGE_UTIL_H

#include "util.h"
#include "incl/color_utils.h"

#include <string>
#include <iostream>

//utility functions that helps provide information about the cartridge.

const int TITLE_LENGTH = 11;

namespace Header {
const int ENTRY_POINT = 0x100;
const int LOGO = 0x104;
const int TITLE = 0x134;
const int MANUFACTURER_CODE = 0x13F;
const int CGB_FLAG = 0x143;
const int NEW_LICENSE_CODE = 0x144;
const int SGB_FLAG = 0x146;
const int CARTRIDGE_TYPE = 0x147;
const int ROM_SIZE = 0x148;
const int RAM_SIZE = 0x149;
const int DESTINATION_CODE = 0x14A;
const int OLD_LICENSE_CODE = 0x14B;
const int VERSION_NUMBER = 0x14C;
const int HEADER_CHECKSUM = 0x14D;
const int GLOBAL_CHECKSUM = 0x14E;
} // namespace Header

enum class CartridgeType {
    ROMOnly,
    MBC1,
    MBC2,
    MBC3,
    MBC4,
    MBC5,
    Unknown,
};

inline CartridgeType getCartridgeType(u8 type) {
    switch (type) {
        case 0x00: case 0x08: case 0x09:
            return CartridgeType::ROMOnly;

        case 0x01: case 0x02: case 0x03:
            return CartridgeType::MBC1;

        case 0x05: case 0x06:
            return CartridgeType::MBC2;

        case 0x0F: case 0x10: case 0x11: case 0x12: case 0x13:
            return CartridgeType::MBC3;

        case 0x15: case 0x16: case 0x17:
            return CartridgeType::MBC4;

        case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E:
            return CartridgeType::MBC5;

        case 0x0B: case 0x0C: case 0x0D: case 0x20: case 0x22: case 0xFC: case 0xFD: case 0xFE:
            return CartridgeType::Unknown;

        default:
            return CartridgeType::Unknown;
    }
}

inline std::string describeCartridge(CartridgeType type) {
    switch (type) {
         case CartridgeType::ROMOnly:
             return "ROM Only";
         case CartridgeType::MBC1:
             return "MBC1";
         case CartridgeType::MBC2:
             return "MBC2";
         case CartridgeType::MBC3:
             return "MBC3";
         case CartridgeType::MBC4:
             return "MBC4";
         case CartridgeType::MBC5:
             return "MBC5";
         case CartridgeType::Unknown:
             return "Unknown";
     }
    return "Unsupported cartridge type was specified";
}

enum class ROMSize {
    KB32,
    KB64,
    KB128,
    KB256,
    KB512,
    MB1,
    MB2,
    MB4,
    MB1p1,
    MB1p2,
    MB1p5,
};

inline ROMSize getRomSize(u8 sizeCode) {
    switch (sizeCode) {
        case 0x00:
            return ROMSize::KB32;
        case 0x01:
            return ROMSize::KB64;
        case 0x02:
            return ROMSize::KB128;
        case 0x03:
            return ROMSize::KB256;
        case 0x04:
            return ROMSize::KB512;
        case 0x05:
            return ROMSize::MB1;
        case 0x06:
            return ROMSize::MB2;
        case 0x07:
            return ROMSize::MB4;
        case 0x52:
            return ROMSize::MB1p1;
        case 0x53:
            return ROMSize::MB1p2;
        case 0x54:
            return ROMSize::MB1p5;
        default:
            std::cout << BOLDYELLOW << "Unknown ROM size: " << RESET << std::hex << sizeCode << std::endl;
            return ROMSize::KB32;
    }
}

inline std::string describeROMSize(ROMSize size) {
    switch (size) {
        case ROMSize::KB32:
            return "32KB (no ROM banking)";
        case ROMSize::KB64:
            return "64KB (4 banks)";
        case ROMSize::KB128:
            return "128KB (8 banks)";
        case ROMSize::KB256:
            return "256KB (16 banks)";
        case ROMSize::KB512:
            return "512KB (32 banks)";
        case ROMSize::MB1:
            return "1MB (64 banks)";
        case ROMSize::MB2:
            return "2MB (128 banks)";
        case ROMSize::MB4:
            return "4MB (256 banks)";
        case ROMSize::MB1p1:
            return "1.1MB (72 banks)";
        case ROMSize::MB1p2:
            return "1.2MB (80 banks)";
        case ROMSize::MB1p5:
            return "1.5MB (96 banks)";
    }
    return "Unsupported ROM size was specified";
}

enum class RAMSize {
    None,
    KB2,
    KB8,
    KB32,
    KB128,
    KB64,
};

inline RAMSize getRamSize(u8 sizeCode) {
    switch (sizeCode) {
        case 0x00:
            return RAMSize::None;
        case 0x01:
            return RAMSize::KB2;
        case 0x02:
            return RAMSize::KB8;
        case 0x03:
            return RAMSize::KB32;
        case 0x04:
            return RAMSize::KB128;
        case 0x05:
            return RAMSize::KB64;
        default:
            std::cout << BOLDYELLOW << "Unknown RAM size: " << RESET << std::hex << sizeCode << std::endl;
            return RAMSize::None;
    }
}

inline std::string describeRamSize(RAMSize size) {
    switch (size) {
        case RAMSize::None:
            return "No RAM";
        case RAMSize::KB2:
            return "2KB";
        case RAMSize::KB8:
            return "8KB";
        case RAMSize::KB32:
            return "32KB";
        case RAMSize::KB128:
            return "128KB";
        case RAMSize::KB64:
            return "64KB";
    }
    return "Unsupported RAM Size specified";
}

#endif
