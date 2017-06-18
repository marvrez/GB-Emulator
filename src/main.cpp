#include <iostream>
#include <cstdio>

#include "cartridge.h"

#include <memory>

int main(int argc, char* argv[])
{
    if(argc < 2) {
        std::cout << BOLDRED << "[ERROR]: " << RESET;
        std::cout << "Please provide a ROM file to run" << std::endl;
        return 0;
    }

    std::unique_ptr<Cartridge> cartridge = std::make_unique<Cartridge>(argv[1]);



    return 0;
}
