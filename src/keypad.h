#ifndef KEYPAD_H
#define KEYPAD_H

#include "mmu.h"

#include <memory>

constexpr int KEYPAD_MEMADDR = 0xFF00;
enum class KeypadKey {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    A,
    B,
    SELECT,
    START,
    NONE
};

class Keypad {
public:
    Keypad(std::shared_ptr<MMU>& mmu);
    void keyUp(KeypadKey key);
    void keyDown(KeypadKey key);
    void tick();
private:
    std::shared_ptr<MMU> mmu;
    u8 row0, row1;
    bool interrupt;
};

#endif
