#include "register.h"

void ByteRegister::setValue(const u8 newVal) {
    this->val = newVal;
}

void ByteRegister::reset() {
    this->val = 0;
}

u8 ByteRegister::getValue() const {
    return this->val;
}

bool ByteRegister::checkBit(u8 bit) const {
    return BitOperations::checkBit(val, bit);
}

void ByteRegister::setBitTo(u8 bit, bool set) {
    val = BitOperations::setBitTo(val, bit, set);
}

void ByteRegister::increment() {
    ++val;
}

void ByteRegister::decrement() {
    --val;
}

bool ByteRegister::operator==(const u8 other) const {
    return val == other;
}


void FlagRegister::setValue(const u8 newVal) {
    val = newVal & 0xF0;
}

void FlagRegister::setFlagZero(bool set) {
    setBitTo(7, set);
}

void FlagRegister::setFlagSubtract(bool set) {
    setBitTo(6, set);
}

void FlagRegister::setFlagHalfCarry(bool set) {
    setBitTo(5, set);
}

void FlagRegister::setFlagCarry(bool set) {
    setBitTo(4, set);
}

bool FlagRegister::checkFlagZero() const {
    return checkBit(7);
}

bool FlagRegister::checkFlagSubtract() const {
    return checkBit(6);
}

bool FlagRegister::checkFlagHalfCarry() const {
    return checkBit(5);
}

bool FlagRegister::checkFlagCarry() const {
    return checkBit(4);
}

u8 FlagRegister::getFlagZero() const {
    return static_cast<u8>(checkFlagZero() ? 1 : 0);
}

u8 FlagRegister::getFlagSubtract() const {
    return static_cast<u8>(checkFlagSubtract() ? 1 : 0);
}

u8 FlagRegister::getFlagHalfCarry() const {
    return static_cast<u8>(checkFlagHalfCarry() ? 1 : 0);
}

u8 FlagRegister::getFlagCarry() const {
    return static_cast<u8>(checkFlagCarry() ? 1 : 0);
}


void WordRegister::setValue(const u16 newVal) {
    val = newVal;
}

u16 WordRegister::getValue() const {
    return val;
}

u8 WordRegister::getLowValue() const {
    return static_cast<u8>(val);
}

u8 WordRegister::getHighValue() const {
    return static_cast<u8>((val) >> 8);
}

void WordRegister::increment() {
    ++val;
}
void WordRegister::decrement() {
    --val;
}


RegisterPair::RegisterPair(ByteRegister* high, ByteRegister* low) :
    lowByte(low), highByte(high)
{
}

void RegisterPair::setValue(const u16 newVal) {
    lowByte->setValue(static_cast<u8>(newVal));
    highByte->setValue(static_cast<u8>((newVal) >> 8));
}

u8 RegisterPair::getLowValue() const {
    return lowByte->getValue();
}

u8 RegisterPair::getHighValue() const {
    return highByte->getValue();
}

ByteRegister* RegisterPair::getHighRegister() const {
    return this->highByte;
}

ByteRegister* RegisterPair::getLowRegister() const {
    return this->lowByte;
}

u16 RegisterPair::getValue() const {
    return BitOperations::composeBytes(highByte->getValue(),
                                        lowByte->getValue());
}

void RegisterPair::increment() {
    this->setValue(this->getValue() + 1);
}

void RegisterPair::decrement() {
    this->setValue(this->getValue() - 1);
}
