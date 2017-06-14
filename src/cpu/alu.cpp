#include "alu.h"

#include "../util.h"

using BitOperations::checkBit;
using BitOperations::clearBit;
using BitOperations::setBit;
using BitOperations::setBitTo;

ALU::ALU(ByteRegister* A, FlagRegister* F)
    :A(A), F(F)
{
}

void ALU::adc(u8 value) {
    u8 carry = F->getFlagCarry();
    u16 fullResult = A->getValue() + value + carry;
    u8 result = static_cast<u8>(fullResult);

    A->setValue(result);

    F->setFlagZero(result == 0);
    F->setFlagSubtract(0);
    F->setFlagHalfCarry(((A->getValue() & 0x0F) + (value & 0x0F) + carry) > 0x0F);
    F->setFlagCarry(fullResult > 0xFF);
}

void ALU::add(u8 val1, u8 val2) {
    u16 result = val1 + val2;

    A->setValue(static_cast<u8>(result));

    F->setFlagZero(A->getValue() == 0);
    F->setFlagSubtract(0);
    F->setFlagHalfCarry((val1 & 0x0F) + (val2 & 0x0F) > 0x0F);
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
    F->setFlagHalfCarry(0);
    F->setFlagSubtract(1);
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
    u8 result = static_cast<u8>((value << 1) | checkBit(value,7));

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

    F->setFlagCarry(checkBit(value,0));
    F->setFlagZero(result == 0);
    F->setFlagHalfCarry(0);
    F->setFlagSubtract(0);

    return result;
}
