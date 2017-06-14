#ifndef ALU_H
#define ALU_H

#include "../register.h"
class ALU {
public:
    ALU() = delete;
    ALU(ByteRegister* A, FlagRegister* F, RegisterPair* HL);

    //arithmetic operations
    void adc(u8 value);
    void add(u8 val1, u8 val2);
    void add_hl(u16 value);
    void sub(u8 value);
    void sbc(const u8 value);
/*
    void _and(u8 value);
    void _or(u8 value);
    void _xor(u8 value);

    void bit(const u8 bit, const u8 value);
    void cp(const u8 value);

    //rotates
    u8 rl(u8 value);
    u8 rlc(u8 value);
    u8 rr(u8 value);
    u8 rrc(u8 value);

    //shifts
    u8 sla(u8 value);
    u8 sra(u8 value);
    u8 srl(u8 value);

    u8 swap(u8 value);
    */

private:
    FlagRegister* F; //flag
    ByteRegister* A; //accumulator
    RegisterPair* HL; //store memory address
};

#endif
