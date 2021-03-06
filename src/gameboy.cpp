#include "gameboy.h"

#include "incl/color_utils.h"

#include <chrono>
#include <thread>
using namespace std::literals;

GameBoy::GameBoy(std::unique_ptr<Cartridge>& cartridge) :
    mmu(std::make_shared<MMU>(cartridge)),
    cpu(std::make_unique<CPU>(mmu)),
    gpu(std::make_unique<GPU>(mmu, screen)),
    keypad(std::make_shared<Keypad>(mmu)),
    screen(std::make_shared<Screen>(cartridge->getGametitle())),
    timer(std::make_unique<Timer>(mmu)),
    apu(std::make_unique<APU>(mmu))
{
    screen->setKeypad(keypad);
    std::cout << YELLOW << "Gameboy initialized" << RESET << std::endl;
}

void GameBoy::run() {
    u32 elapsedCycles = 0;
    while (screen->isOpen()) {
        auto cycles = cpu->tick();
        elapsedCycles += cycles.cycles;
        gpu->tick(cycles);
        keypad->tick();
        timer->tick(cycles);
        //apu->tick(cycles);
    }
}
