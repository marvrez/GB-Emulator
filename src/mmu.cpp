#include "mmu.h"
#include <iostream>

#include "boot_roms.h"

MMU::MMU(std::unique_ptr<Cartridge>& c) : cartridge(c.get()) {
    this->reset();

    std::vector<u8> rom = cartridge->getData();
    switch (cartridge->getType()) {
    case CartridgeType::ROMOnly:
        mbc = std::make_unique<MBC0>(rom);
        break;
    case CartridgeType::MBC1:
        mbc = std::make_unique<MBC1>(rom);
        break;
    case CartridgeType::MBC3:
        mbc = std::make_unique<MBC3>(rom);
        break;
    case CartridgeType::MBC5:
        mbc = std::make_unique<MBC5>(rom);
        break;
    default:
        std::cout << RED << "[ERROR]" << RESET << "Unknown cartridge type!" << std::endl;
        exit(1);
        break;
    }

    this->saveFilename = cartridge->getGametitle() + ".gbsave";
    mbc->load(saveFilename);
}

MMU::~MMU() {
    if(mbc.get() != nullptr) {
        mbc->save(this->saveFilename);
        std::cout << BOLDMAGENTA << "Current gamestate was succesfully saved to: " << RESET << saveFilename << std::endl;
    }
}

void MMU::reset() {
    wram.resize(0x2000, 0);
    vram.resize(0x2000, 0);
    voam.resize(0xA0, 0);
    zram.resize(0x100, 0);

    // Memory is documented to reset to these values
    zram[0xFF00&0xFF] = 0x00; // Reset Joystick to all unpressed
    zram[0xFF05&0xFF] = 0x00;
    zram[0xFF06&0xFF] = 0x00;
    zram[0xFF07&0xFF] = 0x00;
    zram[0xFF10&0xFF] = 0x80;
    zram[0xFF11&0xFF] = 0xBF;
    zram[0xFF12&0xFF] = 0xF3;
    zram[0xFF14&0xFF] = 0xBF;
    zram[0xFF16&0xFF] = 0x3F;
    zram[0xFF17&0xFF] = 0x00;
    zram[0xFF19&0xFF] = 0xBF;
    zram[0xFF1A&0xFF] = 0x7F;
    zram[0xFF1B&0xFF] = 0xFF;
    zram[0xFF1C&0xFF] = 0x9F;
    zram[0xFF1E&0xFF] = 0xBF;
    zram[0xFF20&0xFF] = 0xFF;
    zram[0xFF21&0xFF] = 0x00;
    zram[0xFF22&0xFF] = 0x00;
    zram[0xFF23&0xFF] = 0xBF;
    zram[0xFF24&0xFF] = 0x77;
    zram[0xFF25&0xFF] = 0xF3;
    zram[0xFF26&0xFF] = 0xF1;
    zram[0xFF40&0xFF] = 0x91;
    zram[0xFF41&0xFF] = 0x85;
    zram[0xFF42&0xFF] = 0x00;
    zram[0xFF43&0xFF] = 0x00;
    zram[0xFF45&0xFF] = 0x00;
    zram[0xFF47&0xFF] = 0xFC;
    zram[0xFF48&0xFF] = 0xFF;
    zram[0xFF49&0xFF] = 0xFF;
    zram[0xFF4A&0xFF] = 0x00;
    zram[0xFF4B&0xFF] = 0x00;
    zram[0xFFFF&0xFF] = 0x00;
}

u8 MMU::readByte(const Address& address) {
    u16 addressValue = address.getValue();
    /*
    if (address.isInRange(0x0, 0x7FFF)) {
          if (address.isInRange(0x0, 0xFF) && readByte(0xFF50) != 0x1) {
              return bootRomDMG[addressValue];
          }
          return mbc->readROM(address);
    }*/
    switch(addressValue & 0xF000) {
    // ROM bank 0
    case 0x0000: case 0x1000: case 0x2000: case 0x3000:
    // ROM bank 1
    case 0x4000: case 0x5000: case 0x6000: case 0x7000:
        return mbc->readROM(address);

    // VRAM
    case 0x8000: case 0x9000:
        return vram[addressValue & 0x1FFF];

    // External RAM
    case 0xA000: case 0xB000:
        return mbc->readRAM(address);

    // Work RAM and echo
    case 0xC000: case 0xD000: case 0xE000:
        return wram[addressValue & 0x1FFF];

    // Everything else
    case 0xF000:
        switch(addressValue & 0x0F00) {
        // Echo RAM
        case 0x000: case 0x100: case 0x200: case 0x300:
        case 0x400: case 0x500: case 0x600: case 0x700:
        case 0x800: case 0x900: case 0xA00: case 0xB00:
        case 0xC00: case 0xD00:
            return wram[addressValue & 0x1FFF];

        // OAM
        case 0xE00:
            return addressValue < 0xFEA0 ? voam[addressValue & 0xFF] : 0;

        // Zeropage RAM, I/O
        case 0xF00:
            return zram[addressValue & 0xFF];

        }
    }

    return 0;
}

void MMU::writeByte(const Address& address, u8 byte) {
    u16 addressValue = address.getValue();
    if (address == 0xFF02 && byte == 0x81) {
                std::cout << this->readByte(0xFF01);
    }

    switch(addressValue & 0xF000) {
    // bios and ROM bank 0
    case 0x0000: case 0x1000: case 0x2000: case 0x3000:
    // ROM bank 1
    case 0x4000: case 0x5000: case 0x6000: case 0x7000:
        mbc->writeROM(address, byte);
        break;

    // VRAM
    case 0x8000: case 0x9000:
        vram[addressValue & 0x1FFF] = byte;
        break;

    // External RAM(cartridge RAM)
    case 0xA000: case 0xB000:
        mbc->writeRAM(address, byte);
        break;

    // Internal work RAM and echo
    case 0xC000: case 0xD000: case 0xE000:
        wram[addressValue & 0x1FFF] = byte;
        break;

    // Everything else
    case 0xF000:
        switch(addressValue & 0x0F00) {
        // Echo RAM
        case 0x000: case 0x100: case 0x200: case 0x300:
        case 0x400: case 0x500: case 0x600: case 0x700:
        case 0x800: case 0x900: case 0xA00: case 0xB00:
        case 0xC00: case 0xD00:
            wram[addressValue & 0x1FFF] = byte;
            break;

        // OAM
        case 0xE00:
            if(addressValue < 0xFEA0) voam[addressValue & 0xFF] = byte;
            break;

        // Zeropage RAM, I/O - audio, keypad, gpu
        case 0xF00:
            zram[addressValue & 0xFF] = byte;
            break;
        }

        break;
    }
}
