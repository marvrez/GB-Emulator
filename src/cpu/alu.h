#ifndef ALU_H
#define ALU_H

#include "../register.h"
class ALU {
public:
    //8-bit ALU for GameBoy
    ALU() = delete;
    ALU(ByteRegister* A, FlagRegister* F);

    //arithmetic operations
    void adc(u8 value);
    void add(u8 val1, u8 val2);
    void sub(u8 value);
    void sbc(const u8 value);

    //logic operations
    void _and(u8 value);
    void _or(u8 value);
    void _xor(u8 value);

    void bit(const u8 bit, const u8 value);
    void cp(const u8 value);

    //rotatoes
    u8 rl(u8 value);
    u8 rlc(u8 value);
    u8 rr(u8 value);
    u8 rrc(u8 value);

    //shifterinos
    u8 sla(u8 value);
    u8 sra(u8 value);
    u8 srl(u8 value);

    /*
    //misc
    void swap(u8 value);
    void scf();
    void ccf();
    void cpl();
    void daa();

    void inc(ByteRegister& reg);
    void dec(ByteRegister& reg);
    */

private:
    FlagRegister* F; //flag
    ByteRegister* A; //accumulator
};

#endif
