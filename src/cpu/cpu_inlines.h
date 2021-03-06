#ifndef CPU_INLINES_H
#define CPU_INLINES_H

#include "../util.h"
#include "../mmu.h"
#include <cstdlib>
#include "cpu.h"

//In this header you will find all opcode helper functions..

/* ADC */
inline void CPU::CPU::OPCode_ADC() {
    alu->adc(getByteFromPC());
}

inline void CPU::OPCode_ADC(const ByteRegister& reg) {
    alu->adc(reg.getValue());
}

inline void CPU::OPCode_ADC(const Address&& addr) {
    alu->adc(mmu->readByte(addr));
}

/* ADD */
inline void CPU::OPCode_ADD() {
    alu->add(A.getValue(), getByteFromPC());
}

inline void CPU::OPCode_ADD(const ByteRegister& reg) {
    alu->add(A.getValue(), reg.getValue());
}

inline void CPU::OPCode_ADD(const Address& addr) {
    alu->add(A.getValue(), mmu->readByte(addr));
}

inline void CPU::OPCode_ADD_HL(u16 value) {
    u16 regValue = HL.getValue();
    u32 result = regValue + value;

    F.setFlagSubtract(0);
    F.setFlagHalfCarry((regValue & 0x0FFF) + (value & 0x0FFF) > 0x0FFF);
    F.setFlagCarry((result & 0x10000) != 0);

    HL.setValue(static_cast<u16>(result));
}

inline void CPU::OPCode_ADD_HL(const IWordRegister& reg) {
    OPCode_ADD_HL(reg.getValue());
}

inline void CPU::OPCode_ADD_SP() {
    u16 spValue = SP.getValue();
    s8 pcValue = getSignedByteFromPC();
    s32 result = static_cast<s32>(spValue + pcValue);

    F.setFlagZero(0);
    F.setFlagSubtract(0);
    F.setFlagHalfCarry(((spValue ^ pcValue ^ (result & 0xFFFF)) & 0x0010) == 0x0010);
    F.setFlagCarry(((spValue ^ pcValue ^ (result & 0xFFFF)) & 0x0100) == 0x0100);

    SP.setValue(static_cast<u16>(result));
}
/* AND */
inline void CPU::OPCode_AND() {
    alu->_and(getByteFromPC());
}

inline void CPU::OPCode_AND(ByteRegister& reg) {
    alu->_and(reg.getValue());
}

inline void CPU::OPCode_AND(Address&& addr) {
    alu->_and(mmu->readByte(addr));
}

/* OR */
inline void CPU::OPCode_OR() {
    alu->_or(getByteFromPC());
}

inline void CPU::OPCode_OR(const ByteRegister& reg) {
    alu->_or(reg.getValue());
}

inline void CPU::OPCode_OR(const Address& addr) {
    alu->_or(mmu->readByte(addr));
}


/* XOR */
inline void CPU::OPCode_XOR() {
    alu->_xor(getByteFromPC());
}

inline void CPU::OPCode_XOR(const ByteRegister& reg) {
    alu->_xor(reg.getValue());
}

inline void CPU::OPCode_XOR(const Address& addr) {
    alu->_xor(mmu->readByte(addr));
}


/* BIT */
inline void CPU::OPCode_BIT(u8 bit, ByteRegister& reg) {
    alu->bit(bit, reg.getValue());
}

inline void CPU::OPCode_BIT(u8 bit, Address&& addr) {
    alu->bit(bit, mmu->readByte(addr));
}


/* CALL */
inline void CPU::OPCode_CALL() {
    u16 address = getWordFromPC();
    push(PC);
    PC.setValue(address);
}

inline void CPU::OPCode_CALL(Condition condition) {
    if (isCondition(condition)) OPCode_CALL();
    else getWordFromPC(); //use unused word
}

/* CCF */
inline void CPU::OPCode_CCF() {
    alu->ccf();
}


/* CP */
inline void CPU::OPCode_CP() {
    alu->cp(getByteFromPC());
}

inline void CPU::OPCode_CP(const ByteRegister& reg) {
    alu->cp(reg.getValue());
}

inline void CPU::OPCode_CP(const Address& addr) {
    alu->cp(mmu->readByte(addr));
}

/* CPL */
inline void CPU::OPCode_CPL() {
    alu->cpl();
}

/* DAA */
inline void CPU::OPCode_DAA() {
    alu->daa();
}

/* DI */
inline void CPU::OPCode_DI() {
    interruptsEnabled = false;
}


/* EI */
inline void CPU::OPCode_EI() {
    interruptsEnabled = true;
}

/* INC */
inline void CPU::OPCode_INC(ByteRegister& reg) {
    alu->inc(reg);
}

inline void CPU::OPCode_INC(IWordRegister& reg) {
    reg.increment();
}

inline void CPU::OPCode_INC(Address&& addr) {
    u8 result = static_cast<u8>(mmu->readByte(addr) + 1);
    mmu->writeByte(addr, result);

    F.setFlagZero(result == 0);
    F.setFlagSubtract(0);
    F.setFlagHalfCarry((result & 0x0F) == 0x00);
}


/* DEC */
inline void CPU::OPCode_DEC(ByteRegister& reg) {
    alu->dec(reg);
}

inline void CPU::OPCode_DEC(IWordRegister& reg) {
    reg.decrement();
}

inline void CPU::OPCode_DEC(Address&& addr) {
    u8 result = static_cast<u8>(mmu->readByte(addr) - 1);
    mmu->writeByte(addr, result);

    F.setFlagZero(result == 0);
    F.setFlagSubtract(1);
    F.setFlagHalfCarry((result & 0x0F) == 0x0F);
}

/* JP */
inline void CPU::OPCode_JP() {
    PC.setValue(getWordFromPC());
}

inline void CPU::OPCode_JP(Condition condition) {
    if (isCondition(condition)) OPCode_JP();
    else getWordFromPC(); //"throw" unused word..
}

inline void CPU::OPCode_JP(const Address& addr) {
    PC.setValue(HL.getValue());
}

/* JR */
inline void CPU::OPCode_JR() {
    s8 offset = getSignedByteFromPC();
    u16 oldValue = PC.getValue();

    u16 newValue = static_cast<u16>(oldValue + offset);
    PC.setValue(newValue);
}

inline void CPU::OPCode_JR(Condition condition) {
    if (isCondition(condition)) OPCode_JR();
    else getSignedByteFromPC();
}


/* HALT */
inline void CPU::OPCode_HALT() {
    halted = true;
}


/* LD */
inline void CPU::OPCode_LD(ByteRegister& reg) {
    reg.setValue(getByteFromPC());
}

inline void CPU::OPCode_LD(ByteRegister& reg, const ByteRegister& reg2) {
    reg.setValue(reg2.getValue());
}

inline void CPU::OPCode_LD(ByteRegister& reg, const Address& address) {
    reg.setValue(mmu->readByte(address));
}


inline void CPU::OPCode_LD(IWordRegister& reg) {
    reg.setValue(getWordFromPC());
}

inline void CPU::OPCode_LD(WordRegister& wordReg, const RegisterPair& regPair) {
    wordReg.setValue(regPair.getValue());
}


inline void CPU::OPCode_LD(const Address& address) {
    mmu->writeByte(address, getByteFromPC());
}

inline void CPU::OPCode_LD(const Address& address, const ByteRegister& byteReg) {
    mmu->writeByte(address, byteReg.getValue());
}

inline void CPU::OPCode_LD(const Address& address, const WordRegister& wordReg) {
    mmu->writeByte(address, wordReg.getLowValue());
    mmu->writeByte(address + 1, wordReg.getHighValue());
}


// (nn), A
inline void CPU::OPCode_LD_FROM_ADDRESS(ByteRegister& reg) {
    reg.setValue(mmu->readByte(getWordFromPC()));
}

inline void CPU::OPCode_LD_TO_ADDRESS(const ByteRegister& reg) {
    mmu->writeByte(Address(getWordFromPC()), reg.getValue());
}


/* LDD */
inline void CPU::OPCode_LDD(ByteRegister& reg, const Address& address) {
    reg.setValue(mmu->readByte(address));
    HL.decrement();
}

inline void CPU::OPCode_LDD(const Address& address, const ByteRegister& reg) {
    mmu->writeByte(address,reg.getValue());
    HL.decrement();
}


/* LDH */
// A, (n)
inline void CPU::OPCode_LDH_TO_A() {
    u8 offset = getByteFromPC();
    u8 value = mmu->readByte(Address(0xFF00 + offset));
    A.setValue(value);
}

// (n), A
inline void CPU::OPCode_LDH_TO_DATA() {
    u8 offset = getByteFromPC();
    mmu->writeByte(Address(0xFF00 + offset), A.getValue());
}

// (reg), A
inline void CPU::OPCode_LDH_TO_C() {
    u8 offset = C.getValue();
    mmu->writeByte(Address(0xFF00 + offset), A.getValue());
}

// A, (reg)
inline void CPU::OPCode_LDH_C_TO_A() {
    A.setValue(mmu->readByte(Address(0xFF00 + C.getValue())));
}


/* LDHL */
inline void CPU::OPCode_LDHL() {
    u16 regValue = SP.getValue();
    s8 pcValue = getSignedByteFromPC();

    s32 result = static_cast<s32>(regValue + pcValue);

    F.setFlagZero(0);
    F.setFlagSubtract(0);
    F.setFlagHalfCarry(((regValue ^ pcValue ^ (result & 0xFFFF)) & 0x0010) == 0x0010);
    F.setFlagCarry(((regValue ^ pcValue ^ (result & 0xFFFF)) & 0x100) == 0x100);

    HL.setValue(static_cast<u16>(result));
}


/* LDI */
inline void CPU::OPCode_LDI(ByteRegister& reg, const Address& address) {
    reg.setValue(mmu->readByte(address));
    HL.increment();
}

inline void CPU::OPCode_LDI(const Address& address, const ByteRegister& reg) {
    mmu->writeByte(address, reg.getValue());
    HL.increment();
}


/* NOP */
inline void CPU::OPCode_NOP() {
    //do nothing.. FeelsWeirdMan
}

/* POP */
inline void CPU::OPCode_POP(RegisterPair& reg) {
    pop(reg);
}


/* PUSH */
inline void CPU::OPCode_PUSH(const RegisterPair& reg) {
    push(reg);
}


/* RES */
inline void CPU::OPCode_RES(u8 bit, ByteRegister& reg) {
    u8 result = BitOperations::clearBit(reg.getValue(), bit);
    reg.setValue(result);
}

inline void CPU::OPCode_RES(u8 bit, Address&& addr) {
    u8 result = BitOperations::clearBit(mmu->readByte(addr), bit);
    mmu->writeByte(addr, result);
}

/* RET */
inline void CPU::OPCode_RET() {
    pop(PC);
}

inline void CPU::OPCode_RET(Condition condition) {
    if (isCondition(condition)) OPCode_RET();
}

/* RETI */
inline void CPU::OPCode_RETI() {
    OPCode_RET();
    OPCode_EI();
}

/* RL */
inline void CPU::OPCode_RL() {
    OPCode_RL(A);
    F.setFlagZero(0);
}

inline void CPU::OPCode_RL(ByteRegister& reg) {
    u8 result = alu->rl(reg.getValue());
    reg.setValue(result);
}

inline void CPU::OPCode_RL(Address&& addr) {
    u8 result = alu->rl(mmu->readByte(addr));
    mmu->writeByte(addr, result);
}


/* RLC */
inline void CPU::OPCode_RLC() {
    OPCode_RLC(A);
    F.setFlagZero(0);
}

inline void CPU::OPCode_RLC(ByteRegister& reg) {
    u8 result = alu->rlc(reg.getValue());
    reg.setValue(result);
}

inline void CPU::OPCode_RLC(Address&& addr) {
    u8 result = alu->rlc(mmu->readByte(addr));
    mmu->writeByte(addr, result);
}

/* RR */
inline void CPU::OPCode_RR() {
    OPCode_RR(A);
    F.setFlagZero(0);
}

inline void CPU::OPCode_RR(ByteRegister& reg) {
    u8 result = alu->rr(reg.getValue());
    reg.setValue(result);
}

inline void CPU::OPCode_RR(Address&& addr) {
    u8 result = alu->rr(mmu->readByte(addr));
    mmu->writeByte(addr, result);
}

/* RRC */
inline void CPU::OPCode_RRC() {
    OPCode_RRC(A);
    F.setFlagZero(0);
}

inline void CPU::OPCode_RRC(ByteRegister& reg) {
    u8 result = alu->rrc(reg.getValue());
    reg.setValue(result);
}

inline void CPU::OPCode_RRC(Address&& addr) {
    u8 result = alu->rrc(mmu->readByte(addr));
    mmu->writeByte(addr, result);
}

/* RST */
inline void CPU::OPCode_RST(u8 offset) {
    push(PC);
    PC.setValue(offset);
}

/* SBC */
inline void CPU::OPCode_SBC() {
    alu->sbc(getByteFromPC());
}

inline void CPU::OPCode_SBC(ByteRegister& reg) {
    alu->sbc(reg.getValue());
}

inline void CPU::OPCode_SBC(Address&& addr) {
    alu->sbc(mmu->readByte(addr));
}


/* SCF */
inline void CPU::OPCode_SCF() {
    alu->scf();
}


/* SET */
inline void CPU::OPCode_SET(u8 bit, ByteRegister& reg) {
    u8 result = BitOperations::setBit(reg.getValue(), bit);
    reg.setValue(result);
}

inline void CPU::OPCode_SET(u8 bit, Address&& addr) {
    u8 result = BitOperations::setBit(mmu->readByte(addr), bit);
    mmu->writeByte(addr, result);
}


/* SLA */
inline void CPU::OPCode_SLA(ByteRegister& reg) {
    u8 result = alu->sla(reg.getValue());
    reg.setValue(result);
}

inline void CPU::OPCode_SLA(Address&& addr) {
    u8 result = alu->sla(mmu->readByte(addr));
    mmu->writeByte(addr, result);
}


/* SRA */
inline void CPU::OPCode_SRA(ByteRegister& reg) {
    u8 result = alu->sra(reg.getValue());
    reg.setValue(result);
}

inline void CPU::OPCode_SRA(Address&& addr) {
    u8 result = alu->sra(mmu->readByte(addr));
    mmu->writeByte(addr, result);
}


/* SRL */
inline void CPU::OPCode_SRL(ByteRegister& reg) {
    u8 result = alu->srl(reg.getValue());
    reg.setValue(result);
}

inline void CPU::OPCode_SRL(Address&& addr) {
    u8 result = alu->srl(mmu->readByte(addr));
    mmu->writeByte(addr, result);
}

/* STOP */
inline void CPU::OPCode_STOP() {
    exit(1); //TODO: ehh... maybe do some real cleanup...
}

/* SUB */
inline void CPU::OPCode_SUB() {
    alu->sub(getByteFromPC());
}

inline void CPU::OPCode_SUB(ByteRegister& reg) {
    alu->sub(reg.getValue());
}

inline void CPU::OPCode_SUB(Address&& addr) {
    alu->sub(mmu->readByte(addr));
}

/* SWAP */
inline void CPU::OPCode_SWAP(ByteRegister& reg) {
    u8 result = alu->swap(reg.getValue());
    reg.setValue(result);
}

inline void CPU::OPCode_SWAP(Address&& addr) {
    u8 result = alu->swap(mmu->readByte(addr));
    mmu->writeByte(addr, result);
}

#endif
