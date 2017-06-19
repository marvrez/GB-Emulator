#include "mbc.h"

#include <string>
#include <fstream>

MBC0::MBC0(const std::vector<u8>& rom) {
    this->rom = rom;
    ram.resize(0x2000);
}

u8 MBC0::readROM(const Address& address) {
    return rom[address.getValue()];
}

u8 MBC0::readRAM(const Address& address) {
    return ram[address.getValue()];
}

void MBC0::writeROM(const Address& address, u8 value) {}

void MBC0::writeRAM(const Address& address, u8 value) {
    ram[address.getValue()] = value;
}

/********************************/

MBC1::MBC1(const std::vector<u8>& rom) {
    this->rom = rom;
    ram.resize(0x10000);
    rombank = 1;
    rambank = 0;
    extram_on = false;
    ram_mode = false;
}

u8 MBC1::readROM(const Address& address) {
    u16 addressVal = address.getValue();
    if (addressVal < 0x4000) return rom[addressVal];
    addressVal &= 0x3FFF;
    int the_rombank = (ram_mode) ? rombank & 0x1F : rombank;
    return rom[the_rombank * 0x4000 | addressVal];
}

u8 MBC1::readRAM(const Address& address) {
    if (!extram_on || !ram_mode) return 0;
    u16 addressVal = address.getValue();
    addressVal &= 0x1FFF;
    return ram[rambank * 0x2000 | addressVal];
}

void MBC1::writeROM(const Address& address, u8 value) {
    u16 addressVal = address.getValue();
    switch (addressVal & 0xF000) {
    case 0x0000:
    case 0x1000:
        extram_on = (value == 0x0A);
        break;
    case 0x2000:
    case 0x3000:
        value &= 0x1F;
        if (value == 0) value = 1;
        rombank = (rombank & 0x60) | value;
        break;
    case 0x4000:
    case 0x5000:
        value &= 0x03;
        if (ram_mode) rambank = value;
        else rombank = (value << 5) | (rombank & 0x1F);
        break;
    case 0x6000:
    case 0x7000:
        ram_mode = value & 1;
        break;
    }
}

void MBC1::writeRAM(const Address& address, u8 value) {
    if (!extram_on || !ram_mode) return;
    if (!ram_mode) rambank = 0;
    u16 addressVal = address.getValue();
    addressVal &= 0x1FFF;
    ram[rambank * 0x2000 | addressVal] = value;
}
