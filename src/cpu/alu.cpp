#include "alu.h"

#include "../util.h"

using BitOperations::checkBit;
using BitOperations::clearBit;
using BitOperations::setBit;
using BitOperations::setBitTo;
using BitOperations::composeNibbles;

ALU::ALU(ByteRegister* A, FlagRegister* F)
    :A(A), F(F)
{
}

void ALU::adc(u8 value) {
    u8 carry = F->getFlagCarry();
    u32 fullResult = A->getValue() + value + carry;
    u8 result = static_cast<u8>(fullResult);

    A->setValue(result);

    F->setFlagZero(result == 0);
    F->setFlagSubtract(0);
    F->setFlagHalfCarry(((A->getValue() & 0xF) + (value & 0xF) + carry) > 0xF);
    F->setFlagCarry(fullResult > 0xFF);
}

void ALU::add(u8 val1, u8 val2) {
    u16 result = val1 + val2;

    A->setValue(static_cast<u8>(result));

    F->setFlagZero(A->getValue() == 0);
    F->setFlagSubtract(0);
    F->setFlagHalfCarry((val1 & 0xF) + (val2 & 0xF) > 0xF);
    F->setFlagCarry((result & 0x100) != 0);
}

void ALU::sub(u8 value) {
    u8 regValue = A->getValue();
    u8 result = static_cast<u8>(regValue - value);

    A->setValue(result);

    F->setFlagZero(A->getValue() == 0);
    F->setFlagSubtract(1);
    F->setFlagHalfCarry(((regValue & 0x0F) - (value & 0x0F)) < 0);
    F->setFlagCarry(regValue < value);
}

void ALU::sbc(const u8 value) {
    u8 carry = F->getFlagCarry();
    u8 regValue = A->getValue();

    int fullResult = regValue - value - carry;
    u8 result = static_cast<u8>(fullResult);

    A->setValue(result);

    F->setFlagZero(result == 0);
    F->setFlagSubtract(1);
    F->setFlagHalfCarry(((regValue & 0x0F) - (value & 0x0F) - carry) < 0);
    F->setFlagCarry(fullResult < 0);
}

void ALU::_and(u8 value) {
    A->setValue(A->getValue() & value);

    F->setFlagZero(A->getValue() == 0);
    F->setFlagHalfCarry(1);
    F->setFlagCarry(0);
    F->setFlagSubtract(0);
}

void ALU::_or(u8 value) {
    A->setValue(A->getValue() | value);

    F->setFlagZero(A->getValue() == 0);
    F->setFlagHalfCarry(0);
    F->setFlagCarry(0);
    F->setFlagSubtract(0);
}

void ALU::_xor(u8 value) {
    A->setValue(A->getValue() ^ value);

    F->setFlagZero(A->getValue() == 0);
    F->setFlagHalfCarry(0);
    F->setFlagCarry(0);
    F->setFlagSubtract(0);
}

void ALU::bit(const u8 bit, const u8 value) {
    F->setFlagZero(!checkBit(value,bit));
    F->setFlagHalfCarry(1);
    F->setFlagSubtract(0);
}

void ALU::cp(const u8 value) {
    u8 regValue = A->getValue();
    u8 result = static_cast<u8> (regValue - value);

    F->setFlagZero(result == 0);
    F->setFlagHalfCarry(((regValue & 0x0F) - (value & 0x0F)) < 0);
    F->setFlagCarry(regValue < value);
    F->setFlagSubtract(1);
}

u8 ALU::rl(u8 value) {
    u8 result = static_cast<u8>(value << 1) | F->getFlagCarry();

    F->setFlagCarry(checkBit(value,7));
    F->setFlagZero(result == 0);
    F->setFlagSubtract(0);
    F->setFlagHalfCarry(0);
    return result;
}

u8 ALU::rlc(u8 value) {
    u8 result = static_cast<u8>(value << 1) | checkBit(value,7);

    F->setFlagCarry(checkBit(value,7));
    F->setFlagZero(result == 0);
    F->setFlagHalfCarry(0);
    F->setFlagSubtract(0);

    return result;
}

u8 ALU::rr(u8 value) {
    u8 result = static_cast<u8>(value >> 1) | (F->getFlagCarry() << 7);

    F->setFlagCarry(checkBit(value,0));
    F->setFlagZero(result == 0);
    F->setFlagSubtract(0);
    F->setFlagHalfCarry(0);

    return result;
}

u8 ALU::rrc(u8 value) {
    u8 result = static_cast<u8>((value >> 1) | (checkBit(value,0) << 7));

    F->setFlagCarry(checkBit(value,0));
    F->setFlagZero(result == 0);
    F->setFlagSubtract(0);
    F->setFlagHalfCarry(0);

    return result;
}

u8 ALU::sla(u8 value) {
    u8 result = static_cast<u8>(value << 1);

    F->setFlagZero(result == 0);
    F->setFlagCarry(checkBit(value, 7));
    F->setFlagHalfCarry(0);
    F->setFlagSubtract(0);

    return result;
}

u8 ALU::sra(u8 value) {
    u8 result = setBitTo(static_cast<u8>(value >> 1), 7, checkBit(value, 7)); //msb doesn't change

    F->setFlagZero(result == 0);
    F->setFlagCarry(checkBit(value,0));
    F->setFlagHalfCarry(0);
    F->setFlagSubtract(0);

    return result;
}

u8 ALU::srl(u8 value) {
    u8 result = (value >> 1);

    F->setFlagCarry(checkBit(value,0));//lsb
    F->setFlagZero(result == 0);
    F->setFlagHalfCarry(0);
    F->setFlagSubtract(0);

    return result;
}

u8 ALU::swap(u8 value) {
    u8 result = composeNibbles(value & 0x0F, (value & 0xF0) >> 4);//compose lower and upper nibbles

    F->setFlagZero(result == 0);
    F->setFlagSubtract(0);
    F->setFlagHalfCarry(0);
    F->setFlagCarry(0);

    return result;
}

void ALU::scf() {
    F->setFlagCarry(1);
    F->setFlagSubtract(0);
    F->setFlagHalfCarry(0);
}

void ALU::ccf() {
    F->setFlagCarry(!F->checkFlagCarry());
    F->setFlagSubtract(0);
    F->setFlagHalfCarry(0);
}

void ALU::cpl() {
    A->setValue(~(A->getValue()));

    F->setFlagSubtract(1);
    F->setFlagHalfCarry(1);
}

void ALU::daa() {
    u8 regValue = A->getValue();
    u16 adjust = F->checkFlagCarry() ? 0x60 : 0x00;

    if (F->checkFlagHalfCarry() || (!F->checkFlagSubtract() && ((regValue & 0x0F) > 0x09)))
        adjust |= 0x06;

    if (F->checkFlagCarry() || (!F->checkFlagSubtract() && (regValue > 0x99)))
        adjust |= 0x60;

    regValue = F->checkFlagSubtract() ? static_cast<u8>(regValue-adjust)
                                      : static_cast<u8>(regValue+adjust);

    F->setFlagCarry(adjust >= 0x60);
    F->setFlagHalfCarry(0);
    F->setFlagZero(regValue == 0);

    A->setValue(static_cast<u8>(regValue));
}

void ALU::inc(ByteRegister& reg) {
    reg.increment();

    F->setFlagZero(reg.getValue() == 0);
    F->setFlagSubtract(0);
    F->setFlagHalfCarry((reg.getValue() & 0x0F) == 0x00);
}

void ALU::dec(ByteRegister& reg) {
    reg.decrement();

    F->setFlagZero(reg.getValue() == 0);
    F->setFlagSubtract(1);
    F->setFlagHalfCarry((reg.getValue() & 0x0F) == 0x0F);
}
