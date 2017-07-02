#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "cpu/cpu.h"
#include "mmu.h"
#include "gpu/gpu.h"
#include "gpu/screen.h"
#include "keypad.h"

#include <memory>

const u32 CYCLES_PER_FRAME = 70224;

class GameBoy {
public:
    GameBoy(std::unique_ptr<Cartridge>& cartridge);
    void run();

private:
    std::shared_ptr<Screen> screen;
    std::shared_ptr<MMU> mmu;
    std::unique_ptr<CPU> cpu;
    std::unique_ptr<GPU> gpu;
    std::shared_ptr<Keypad> keypad;
    //std::unique_ptr<APU> apu; //TODO..
};

#endif
