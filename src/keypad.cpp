#include "keypad.h"

#include "cpu/cpu.h"

Keypad::Keypad(std::shared_ptr<MMU> &mmu) :
    mmu(mmu),
    row0(0x0F),
    row1(0x0F),
    column(0x30),
    interrupt(false)
{
}

void Keypad::keyUp(KeypadKey key) {
    switch (key) {
    case KeypadKey::RIGHT: 	row1 |= 0x1; break;
    case KeypadKey::LEFT: 	row1 |= 0x2; break;
    case KeypadKey::UP: 	row1 |= 0x4; break;
    case KeypadKey::DOWN: 	row1 |= 0x8; break;
    case KeypadKey::A: 		row0 |= 0x1; break;
    case KeypadKey::B: 		row0 |= 0x2; break;
    case KeypadKey::SELECT: row0 |= 0x4; break;
    case KeypadKey::START: 	row0 |= 0x8; break;
    case KeypadKey::NONE:
    default: break;
    }
}

void Keypad::keyDown(KeypadKey key) {
    switch (key) {
    case KeypadKey::RIGHT: 	row1 &= 0xE; break;
    case KeypadKey::LEFT: 	row1 &= 0xD; break;
    case KeypadKey::UP: 	row1 &= 0xB; break;
    case KeypadKey::DOWN: 	row1 &= 0x7; break;
    case KeypadKey::A: 		row0 &= 0xE; break;
    case KeypadKey::B: 		row0 &= 0xD; break;
    case KeypadKey::SELECT: row0 &= 0xB; break;
    case KeypadKey::START: 	row0 &= 0x7; break;
    case KeypadKey::NONE:
    default: break;
    }
    interrupt = true;
}


void Keypad::tick() {
    u8 byte = mmu->readByte(KEYPAD_MEMADDR);
    byte &= 0x30;
    if (byte & 0x10) byte |= row0;
    if (byte & 0x20) byte |= row1;
    mmu->writeByte(KEYPAD_MEMADDR, byte);

    if (interrupt)
        mmu->writeByte(CPU::INTERRUPT_FLAG, mmu->readByte(CPU::INTERRUPT_FLAG) | 0x10);
    interrupt = false;
}
