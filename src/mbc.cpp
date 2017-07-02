#include "mbc.h"

#include "cartridge_util.h"
#include "incl/color_utils.h"

#include <string>
#include <fstream>

MBC0::MBC0(const std::vector<u8>& rom) : rom(rom) {
    ram.resize(0x2000);
    std::cout << BOLDYELLOW << "MBC0 created" << RESET << std::endl;
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

MBC1::MBC1(const std::vector<u8>& rom) :
    rom(rom),
    rombank(1),
    rambank(0),
    extram_on(false),
    ram_mode(false)
{
    ram.resize(0x10000);
    std::cout << BOLDYELLOW << "MBC1 created" << RESET << std::endl;
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

/********************************/

MBC3::MBC3(const std::vector<u8>& rom) :
    rom(rom),
    rombank(1),
    rambank(0),
    extram_on(false),
    rtc_lock(false)
{
    int ramsize = 0;
    switch (rom[Header::RAM_SIZE]) {
    case 0x00:
        ramsize = 0; break; //none
    case 0x01:
        ramsize = 0x800; break; // 2 kB
    case 0x02:
        ramsize = 0x2000; break; // 8 kB
    case 0x03:
        ramsize = 0x8000; break; // 32 kB
    }

    ram.resize(ramsize);
    rtc.resize(5);
    std::cout << BOLDYELLOW << "MBC3 created" << RESET << std::endl;
}

u8 MBC3::readROM(const Address& address) {
    u16 addressVal = address.getValue();
    if (addressVal < 0x4000) return rom[addressVal];
    addressVal &= 0x3FFF;
    return rom[rombank * 0x4000 + static_cast<s32>(addressVal)];
}

u8 MBC3::readRAM(const Address& address) {
    u16 addressVal = address.getValue();
    if (!extram_on) return 0;
    if (rambank <= 3) {
        addressVal &= 0x1FFF;
        return ram[rambank * 0x2000 + static_cast<s32>(addressVal)];
    }
    else return rtc[rambank - 0x08];
}

void MBC3::writeROM(const Address& address, u8 value) {
    u16 addressVal = address.getValue();
    switch (addressVal & 0xF000) {
    case 0x0000: case 0x1000:
        extram_on = (value == 0x0A);
        break;
    case 0x2000: case 0x3000:
        value &= 0x7F;
        rombank = (value == 0x00) ? 0x01 : value;
        break;
    case 0x4000: case 0x5000:
        rambank = value;
        break;
    case 0x6000: case 0x7000:
        switch (value) {
        case 0x00:
            rtc_lock = false;
            break;
        case 0x01:
            if (rtc_lock == false) calc_rtcregs();
            rtc_lock = true;
            break;
        }
        break;
    }
}

void MBC3::writeRAM(const Address& address, u8 value) {
    u16 addressVal = address.getValue();
    if (!extram_on) return;

    if (rambank <= 3) {
        addressVal &= 0x1FFF;
        ram[rambank * 0x2000 + static_cast<s32>(addressVal)] = value;
    }
    else {
        rtc[rambank - 0x8] = value;
        calc_rtczero();
    }
}

void MBC3::save(std::string filename) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    char head[] = "GB_MBC";
    file.write(head, sizeof head);
    file.write(reinterpret_cast<char*>(&rom[Header::CARTRIDGE_TYPE]), 1);

    for (u8 i = 0; i < 5; ++i)
        file.write(reinterpret_cast<char*>(&rtc[i]), 1);

    unsigned long long rtc = rtczero;
    file.write(reinterpret_cast<char*>(&rtc), 8);

    for (unsigned i = 0; i < ram.size(); ++i)
        file.write(reinterpret_cast<char*>(&ram[i]), 1);
    file.close();
}

void MBC3::load(std::string filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) return;

    char head[7];
    char ver;

    file.read(head, 7);
    file.read(&ver, 1);
    if (strcmp(head, "GB_MBC") != 0) {
        file.close();
        return;
    }

    char byte;
    rtc.clear();
    for (int i = 0; i < 5; ++i) {
        file.read(&byte, 1);
        rtc.push_back(byte);
    }

    unsigned long long rtc;
    file.read(reinterpret_cast<char*>(&rtc), 8);
    rtczero = rtc;

    ram.clear();
    while (file.read(&byte, 1)) ram.push_back(byte);
    file.close();

    calc_rtcregs();
}

void MBC3::calc_rtczero() {
    time_t difftime = time(nullptr);
    long long days;
    difftime -= rtc[0];
    difftime -= rtc[1] * 60;
    difftime -= rtc[2] * 3600;
    days = rtc[4] & 0x1;
    days = days << 8 | rtc[3];
    difftime -= days * 3600 * 24;
    rtczero = difftime;
}

void MBC3::calc_rtcregs() {
    if (rtc[4] & 0x40) return;

    time_t difftime = time(nullptr) - rtczero;
    rtc[0] = difftime % 60;
    rtc[1] = (difftime / 60) % 60;
    rtc[2] = (difftime / 3600) % 24;
    long long days = (difftime / (3600*24));
    rtc[3] = days & 0xFF;
    rtc[4] = (rtc[4] & 0xFE) | ((days >> 8) & 0x1);

    if (days >= 512) {
        rtc[4] |= 0x80;
        calc_rtczero();
    }
}

/********************************/

MBC5::MBC5(const std::vector<u8>& rom) :
    rom(rom),
    rombank(1),
    rambank(0),
    extram_on(false)
{
    ram.resize(0x20000);
    std::cout << BOLDYELLOW << "MBC5 created" << RESET << std::endl;
}

u8 MBC5::readROM(const Address& address) {
    u16 addressVal = address.getValue();
    if (addressVal < 0x4000) return rom[addressVal];
    addressVal &= 0x3FFF;

    return rom[rombank * 0x4000 | addressVal];
}

u8 MBC5::readRAM(const Address& address) {
    if (!extram_on) return 0;

    u16 addressVal = address.getValue();
    addressVal &= 0x1FFF;
    return ram[rambank * 0x2000 | addressVal];
}

void MBC5::writeROM(const Address& address, u8 value) {
    u16 addressVal = address.getValue();
    switch (addressVal & 0xF000) {
    case 0x0000:
    case 0x1000:
        extram_on = (value == 0x0A);
        break;
    case 0x2000:
        rombank = (rombank & 0x100) | (value);
        break;
    case 0x3000:
        value &= 0x1;
        rombank = (rombank & 0xFF) | (static_cast<s32>(value) << 8);
        break;
    case 0x4000:
    case 0x5000:
        rambank = value & 0x0F;
        break;
    }
}

void MBC5::writeRAM(const Address& address, u8 value) {
    if (!extram_on) return;

    u16 addressVal = address.getValue();
    addressVal &= 0x1FFF;
    ram[rambank * 0x2000 | addressVal] = value;
}

void MBC5::save(std::string filename) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    for (unsigned i = 0; i < ram.size(); ++i)
        file.write(reinterpret_cast<char*>(&ram[i]), 1);
    file.close();
}

void MBC5::load(std::string filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) return;

    char byte;
    ram.clear();
    while (file.read(&byte, 1))
        ram.push_back(byte);
    file.close();
}
