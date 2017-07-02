#ifndef TIMER_H
#define TIMER_H

#include <memory>
#include "mmu.h"

constexpr int TIMER_DIVIDER_STEP = 64;

class Timer {
public:
    Timer(std::shared_ptr<MMU>& mmu);
    void tick(Cycles cycle);
private:
    std::shared_ptr<MMU> mmu;
    int internalCounter;
};

#endif
