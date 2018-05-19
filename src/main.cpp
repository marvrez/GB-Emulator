#include "cartridge.h"
#include "gameboy.h"

#include <iostream>
#include <cstdio>
#include <memory>
enum REGNAME {
    CONTROLLER_REG = 0,
    SERIAL_DATA,
    SERIAL_CONTROL,
    DIVIDER = 0x4,
    TIMA_COUNT,
    TIMA_RELOAD,
    TIMA_CONTROL,
    IF = 0xF,
    SM1_SWEEP,
    SM1_LENGTH,
    SM1_ENVELOPE,
    SM1_FREQ_LO,
    SM1_FREQ_HI,
    SM2_LENGTH = 0x16,
    SM2_ENVELOPE,
    SM2_FREQ_LO,
    SM2_FREQ_HI,
    SM3_CONTROL,
    SM3_LENGTH,
    SM3_SELECT,
    SM3_FREQ_LO,
    SM3_FREQ_HI,
    SM4_LENGTH = 0x20,
    SM4_ENVELOPE,
    SM4_POLY,
    SM4_COUNTER,
    S_CHANNEL_CONTROL,
    S_SELECT,
    S_CONTROL,
    S_WAVE_RAM = 0x30,
    LCDC_CONTROL = 0x40,
    LCDC_STAT,
    SCY,
    SCX,
    LY,
    LYC,
    DMA,
    BGP,
    OBP0,
    OBP1,
    WY,
    WX,
    IE = 0xFF
};

int main(int argc, char* argv[])
{
    std::cout << "0x" << std::hex << (int)S_CONTROL << std::endl;
    srand(time(nullptr));
    if(argc < 2) {
        std::cout << BOLDRED << "[ERROR]: " << RESET;
        std::cout << "Please provide a ROM file to run" << std::endl;
        return 0;
    }

    std::unique_ptr<Cartridge> cartridge = std::make_unique<Cartridge>(argv[1]);
    GameBoy gameboy(cartridge);
    gameboy.run();

    return 0;
}
