#include "mmu.h"

#include "boot_roms.h"

MMU::MMU(std::unique_ptr<Cartridge>& c) : cartridge(std::move(c)) {
    this->reset();
    std::cout << cartridge->getFilename() << "\n";
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

    std::string saveFilename = cartridge->getFilename() + ".gbsave";
    mbc->load(saveFilename);
}

MMU::~MMU() {
    if(mbc.get() != nullptr) mbc->save(cartridge->getFilename());
}

void MMU::reset() {
    wram.resize(0x2000, 0);
    vram.resize(0x2000, 0);
    voam.resize(0xA0, 0);
    zram.resize(0x100, 0);

    zram[0x05] = 0;
    zram[0x06] = 0;
    zram[0x07] = 0;
    zram[0x40] = 0x91;
    zram[0x42] = 0;
    zram[0x43] = 0;
    zram[0x45] = 0;
    zram[0x47] = 0xFC;
    zram[0x48] = 0xFF;
    zram[0x49] = 0xFF;
    zram[0x4A] = 0;
    zram[0x4B] = 0;
    zram[0x0F] = 0;
    zram[0xFF] = 0;
}

u8 MMU::readByte(const Address& address) {
    u16 addressValue = address.getValue();
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

    // External RAM
    case 0xA000: case 0xB000:
        mbc->writeRAM(address, byte);
        break;

    // Work RAM and echo
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

        // Zeropage RAM, I/O
        case 0xF00:
            zram[addressValue & 0xFF] = byte;
            break;
        }

        break;
    }
}
