#include "timer.h"

#include "cpu/cpu.h"

constexpr int Divider = 0xFF04;
constexpr int TimerCounter =  0xFF05;
constexpr int TimerModulo = 0xFF06;
constexpr int TimerControl = 0xFF07;

Timer::Timer(std::shared_ptr<MMU> &mmu) :
    mmu(mmu),
    internalCounter(0)
{
}

void Timer::tick(Cycles cycle) {
    bool interrupt = false;
    u8 divider, counter, modulo, control;

    divider = mmu->readByte(Divider); //Divider
    counter = mmu->readByte(TimerCounter); //Timer counter
    modulo = mmu->readByte(TimerModulo); //timer modulo
    control = mmu->readByte(TimerControl); //timer control

    int step = 0;
    switch (control & 0x3) {
        case 0: step = 256; break;
        case 1: step = 4; break;
        case 2: step = 16; break;
        case 3: step = 64; break;
    }

    internalCounter %= 256;

    for (int i = 0; i < cycle.cycles; ++i) {
        internalCounter++;
        if (internalCounter % TIMER_DIVIDER_STEP == 0)
            divider++;

        if ((control & 0x4) == 0) continue;

        if (internalCounter % step == 0) {
            counter++;
            if (counter == 0) {
                counter = modulo;
                interrupt = true;
            }
        }
    }

    mmu->writeByte(Divider, divider);
    mmu->writeByte(TimerCounter, counter);
    mmu->writeByte(TimerModulo, modulo);

    if (interrupt)
        mmu->writeByte(CPU::INTERRUPT_FLAG, mmu->readByte(CPU::INTERRUPT_FLAG) | 0x04);
}
