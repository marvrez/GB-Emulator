#include "keypad.h"

#include "cpu/cpu.h"

#include <SFML/Window/Keyboard.hpp>

Keypad::Keypad(std::shared_ptr<MMU> mmu) :
    mmu(mmu),
    row0(0x0F),
    row1(0x0F),
    interrupt(false)
{
}

/**
 * Sets bits for inputs that are released (1 = not pressed).
 */
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

/**
 * Clears bits for inputs that are pressed (0 = pressed).
 */
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

void Keypad::updateInput() {
    static bool left = false;
    static bool right = false;
    static bool up = false;
    static bool down = false;
    static bool a = false;
    static bool b = false;
    static bool start = false;
    static bool select = false;
    // Read button states
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !left) {
        std::cout << "CLICK" << std::endl;
        keyDown(KeypadKey::LEFT);
        left = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && left) {
        std::cout << "RELEASE" << std::endl;
        keyUp(KeypadKey::LEFT);
        left = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !right) {
        keyDown(KeypadKey::RIGHT);
        right = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && right) {
        keyUp(KeypadKey::RIGHT);
        right = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !up) {
        keyDown(KeypadKey::UP);
        up = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && up) {
        keyUp(KeypadKey::UP);
        up = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !down) {
        keyDown(KeypadKey::DOWN);
        down = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && down) {
        keyUp(KeypadKey::DOWN);
        down = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !a) {
        keyDown(KeypadKey::A);
        a = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::X) && a) {
        keyUp(KeypadKey::A);
        a = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !b) {
        keyDown(KeypadKey::B);
        b = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && b) {
        keyUp(KeypadKey::B);
        b = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !start) {
        keyDown(KeypadKey::START);
        start = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && start) {
        keyUp(KeypadKey::START);
        start = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) && !select) {
        keyDown(KeypadKey::SELECT);
        select = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) && select) {
        keyUp(KeypadKey::SELECT);
        select = false;
    }
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
