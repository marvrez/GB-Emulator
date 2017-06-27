#include "gameboy.h"

#include "incl/color_utils.h"

#include <chrono>
#include <thread>
using namespace std::literals;

GameBoy::GameBoy(std::unique_ptr<Cartridge>& cartridge) :
    screen(std::make_shared<Screen>(cartridge->getGametitle())),
    mmu(std::make_shared<MMU>(cartridge)),
    cpu(std::make_unique<CPU>(mmu)),
    gpu(std::make_unique<GPU>(mmu, screen))
{
    std::cout << YELLOW << "Gameboy initialized." << RESET << std::endl;
}

void GameBoy::run() {
    u32 elapsedCycles = 0;
    auto start_time = std::chrono::steady_clock::now();
    auto next_frame = start_time + 16.75041876ms;
    while (screen->isOpen()) {
        auto cycles = cpu->tick();
        elapsedCycles += cycles.cycles;
        gpu->tick(cycles);

        /*
        if(gpu->isUpdated()) { //refresh screen
            gpu->renderFrame();
            std::this_thread::sleep_until(next_frame);
            next_frame += 16.75041876ms;
        }*/
    }
}