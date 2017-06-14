#include "alu.h"

#include "../util.h"

using BitOperations::checkBit;
using BitOperations::clearBit;
using BitOperations::setBit;

ALU::ALU(ByteRegister* A, FlagRegister* F, RegisterPair* HL)
    :A(A), F(F), HL(HL)
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

void ALU::add_hl(u16 value) {
    u16 regValue = HL->getValue();
    u32 result = regValue + value;

    HL->setValue(static_cast<u16>(result));

    F->setFlagSubtract(0);
    F->setFlagHalfCarry((regValue & 0x0FFF) + (value & 0x0FFF) > 0x0FFF);
    F->setFlagCarry((result & 0x10000) != 0);
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
    F->setFlagHalfCarry(((regValue & 0xf) - (value & 0xf) - carry) < 0);
    F->setFlagCarry(fullResult < 0);
}
