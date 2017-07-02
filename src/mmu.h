#ifndef MMU_H
#define MMU_H

#include "address.h"
#include "cartridge.h"
#include "mbc.h"

#include <vector>
#include <memory>
#include <string>


class MMU {
public:
    MMU(std::unique_ptr<Cartridge>& c);
    ~MMU();
    void reset();

    void writeByte(const Address& address, u8 byte);
    u8 readByte(const Address& address);
private:
    std::unique_ptr<MBC> mbc;
    std::vector<u8> wram, vram, voam, zram;
    Cartridge* cartridge;
    std::string saveFilename;
};

#endif
