#include "cartridge.h"

#include <fstream>
#include <iostream>

Cartridge::Cartridge(std::string filename) : filename(filename) {
    using std::ifstream;
    using std::ios;

    std::cout << BOLDWHITE << "Opening file: " << RESET << filename << std::endl;

    ifstream file(filename.c_str(), ios::binary  | ios::in);
    if (!file.good()) {
        std::cout << BOLDRED <<  "[ERROR] " << RESET;
        std::cout << "Cannot read ROM from file " << filename << std::endl;
        exit(1);
    }

    char byte;
    while (file.read(&byte, 1)) data.push_back(byte);
    file.close();

    std::cout << YELLOW << "Loaded: " << RESET << data.size() / 1024 << "KB" << std::endl ;

    char name[TITLE_LENGTH] = {0};
    for (u8 i = 0; i < TITLE_LENGTH; i++)
        name[i] = static_cast<char>(this->data[Header::TITLE + i]);

    this->gametitle = std::string(name);

    this->cartridgeType = getCartridgeType(data[Header::CARTRIDGE_TYPE]);
    this->version = data[Header::VERSION_NUMBER];
    this->romSize = getRomSize(data[Header::ROM_SIZE]);
    this->ramSize = getRamSize(data[Header::RAM_SIZE]);

    std::cout << CYAN << "Title:\t\t" << RESET << gametitle << ", version " << int(version) << std::endl;
    std::cout << CYAN << "Cartridge:\t\t" << RESET << describeCartridge(cartridgeType) << std::endl;
    std::cout << CYAN << "ROM Size:\t\t" << RESET << describeROMSize(romSize) << std::endl;
    std::cout << CYAN << "RAM Size:\t\t" << RESET << describeRamSize(ramSize) << std::endl;
}

std::string Cartridge::getFilename() const {
    return this->filename;
}

const std::vector<u8>& Cartridge::getData() const {
    return this->data;
}

CartridgeType Cartridge::getType() const {
    return this->cartridgeType;
}
