#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "cartridge_util.h"
#include "address.h"

#include <string>
#include <vector>

class Cartridge {
public:
    Cartridge(std::string filename);

    std::string getFilename() const;
    const std::vector<u8>& getData() const;
    CartridgeType getType()  const;
    std::string getGametitle() const;
private:
    std::string filename;
    std::string gametitle;

    std::vector<u8> data;

    /* Cartridge information */
    CartridgeType cartridgeType;
    ROMSize romSize;
    RAMSize ramSize;
    u8 version;
};

#endif
