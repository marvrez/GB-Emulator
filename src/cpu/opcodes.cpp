#include "cpu.h"

void CPU::OPCode0x00() { OPCode_NOP(); }
void CPU::OPCode0x01() { OPCode_LD(BC); }
void CPU::OPCode0x02() { OPCode_LD(Address(BC), A); }
void CPU::OPCode0x03() { OPCode_INC(BC); }
void CPU::OPCode0x04() { OPCode_INC(B); }
void CPU::OPCode0x05() { OPCode_DEC(B); }
void CPU::OPCode0x06() { OPCode_LD(B); }
void CPU::OPCode0x07() { OPCode_RLC(); }
void CPU::OPCode0x08() { OPCode_LD(Address(getWordFromPC()), SP); }
void CPU::OPCode0x09() { OPCode_ADD_HL(BC); }
void CPU::OPCode0x0A() { OPCode_LD(A, Address(BC)); }
void CPU::OPCode0x0B() { OPCode_DEC(BC); }
void CPU::OPCode0x0C() { OPCode_INC(C); }
void CPU::OPCode0x0D() { OPCode_DEC(C); }
void CPU::OPCode0x0E() { OPCode_LD(C); }
void CPU::OPCode0x0F() { OPCode_RRC(); }
void CPU::OPCode0x10() { OPCode_STOP(); }
void CPU::OPCode0x11() { OPCode_LD(DE); }
void CPU::OPCode0x12() { OPCode_LD(Address(DE), A); }
void CPU::OPCode0x13() { OPCode_INC(DE); }
void CPU::OPCode0x14() { OPCode_INC(D); }
void CPU::OPCode0x15() { OPCode_DEC(D); }
void CPU::OPCode0x16() { OPCode_LD(D); }
void CPU::OPCode0x17() { OPCode_RL(); }
void CPU::OPCode0x18() { OPCode_JR(); }
void CPU::OPCode0x19() { OPCode_ADD_HL(DE); }
void CPU::OPCode0x1A() { OPCode_LD(A, Address(DE)); }
void CPU::OPCode0x1B() { OPCode_DEC(DE); }
void CPU::OPCode0x1C() { OPCode_INC(E); }
void CPU::OPCode0x1D() { OPCode_DEC(E); }
void CPU::OPCode0x1E() { OPCode_LD(E); }
void CPU::OPCode0x1F() { OPCode_RR(); }
void CPU::OPCode0x20() { OPCode_JR(Condition::NZ); }
void CPU::OPCode0x21() { OPCode_LD(HL); }
void CPU::OPCode0x22() { OPCode_LDI(Address(HL), A); }
void CPU::OPCode0x23() { OPCode_INC(HL); }
void CPU::OPCode0x24() { OPCode_INC(H); }
void CPU::OPCode0x25() { OPCode_DEC(H); }
void CPU::OPCode0x26() { OPCode_LD(H); }
void CPU::OPCode0x27() { OPCode_DAA(); }
void CPU::OPCode0x28() { OPCode_JR(Condition::Z); }
void CPU::OPCode0x29() { OPCode_ADD_HL(HL); }
void CPU::OPCode0x2A() { OPCode_LDI(A, Address(HL)); }
void CPU::OPCode0x2B() { OPCode_DEC(HL); }
void CPU::OPCode0x2C() { OPCode_INC(L); }
void CPU::OPCode0x2D() { OPCode_DEC(L); }
void CPU::OPCode0x2E() { OPCode_LD(L); }
void CPU::OPCode0x2F() { OPCode_CPL(); }
void CPU::OPCode0x30() { OPCode_JR(Condition::NC); }
void CPU::OPCode0x31() { OPCode_LD(SP); }
void CPU::OPCode0x32() { OPCode_LDD(Address(HL), A); }
void CPU::OPCode0x33() { OPCode_INC(SP); }
void CPU::OPCode0x34() { OPCode_INC(Address(HL)); }
void CPU::OPCode0x35() { OPCode_DEC(Address(HL)); }
void CPU::OPCode0x36() { OPCode_LD(Address(HL)); }
void CPU::OPCode0x37() { OPCode_SCF(); }
void CPU::OPCode0x38() { OPCode_JR(Condition::C); }
void CPU::OPCode0x39() { OPCode_ADD_HL(SP); }
void CPU::OPCode0x3A() { OPCode_LDD(A, Address(HL)); }
void CPU::OPCode0x3B() { OPCode_DEC(SP); }
void CPU::OPCode0x3C() { OPCode_INC(A); }
void CPU::OPCode0x3D() { OPCode_DEC(A); }
void CPU::OPCode0x3E() { OPCode_LD(A); }
void CPU::OPCode0x3F() { OPCode_CCF(); }
void CPU::OPCode0x40() { OPCode_LD(B, B); }
void CPU::OPCode0x41() { OPCode_LD(B, C); }
void CPU::OPCode0x42() { OPCode_LD(B, D); }
void CPU::OPCode0x43() { OPCode_LD(B, E); }
void CPU::OPCode0x44() { OPCode_LD(B, H); }
void CPU::OPCode0x45() { OPCode_LD(B, L); }
void CPU::OPCode0x46() { OPCode_LD(B, Address(HL)); }
void CPU::OPCode0x47() { OPCode_LD(B, A); }
void CPU::OPCode0x48() { OPCode_LD(C, B); }
void CPU::OPCode0x49() { OPCode_LD(C, C); }
void CPU::OPCode0x4A() { OPCode_LD(C, D); }
void CPU::OPCode0x4B() { OPCode_LD(C, E); }
void CPU::OPCode0x4C() { OPCode_LD(C, H); }
void CPU::OPCode0x4D() { OPCode_LD(C, L); }
void CPU::OPCode0x4E() { OPCode_LD(C, Address(HL)); }
void CPU::OPCode0x4F() { OPCode_LD(C, A); }
void CPU::OPCode0x50() { OPCode_LD(D, B); }
void CPU::OPCode0x51() { OPCode_LD(D, C); }
void CPU::OPCode0x52() { OPCode_LD(D, D); }
void CPU::OPCode0x53() { OPCode_LD(D, E); }
void CPU::OPCode0x54() { OPCode_LD(D, H); }
void CPU::OPCode0x55() { OPCode_LD(D, L); }
void CPU::OPCode0x56() { OPCode_LD(D, Address(HL)); }
void CPU::OPCode0x57() { OPCode_LD(D, A); }
void CPU::OPCode0x58() { OPCode_LD(E, B); }
void CPU::OPCode0x59() { OPCode_LD(E, C); }
void CPU::OPCode0x5A() { OPCode_LD(E, D); }
void CPU::OPCode0x5B() { OPCode_LD(E, E); }
void CPU::OPCode0x5C() { OPCode_LD(E, H); }
void CPU::OPCode0x5D() { OPCode_LD(E, L); }
void CPU::OPCode0x5E() { OPCode_LD(E, Address(HL)); }
void CPU::OPCode0x5F() { OPCode_LD(E, A); }
void CPU::OPCode0x60() { OPCode_LD(H, B); }
void CPU::OPCode0x61() { OPCode_LD(H, C); }
void CPU::OPCode0x62() { OPCode_LD(H, D); }
void CPU::OPCode0x63() { OPCode_LD(H, E); }
void CPU::OPCode0x64() { OPCode_LD(H, H); }
void CPU::OPCode0x65() { OPCode_LD(H, L); }
void CPU::OPCode0x66() { OPCode_LD(H, Address(HL)); }
void CPU::OPCode0x67() { OPCode_LD(H, A); }
void CPU::OPCode0x68() { OPCode_LD(L, B); }
void CPU::OPCode0x69() { OPCode_LD(L, C); }
void CPU::OPCode0x6A() { OPCode_LD(L, D); }
void CPU::OPCode0x6B() { OPCode_LD(L, E); }
void CPU::OPCode0x6C() { OPCode_LD(L, H); }
void CPU::OPCode0x6D() { OPCode_LD(L, L); }
void CPU::OPCode0x6E() { OPCode_LD(L, Address(HL)); }
void CPU::OPCode0x6F() { OPCode_LD(L, A); }
void CPU::OPCode0x70() { OPCode_LD(Address(HL), B); }
void CPU::OPCode0x71() { OPCode_LD(Address(HL), C); }
void CPU::OPCode0x72() { OPCode_LD(Address(HL), D); }
void CPU::OPCode0x73() { OPCode_LD(Address(HL), E); }
void CPU::OPCode0x74() { OPCode_LD(Address(HL), H); }
void CPU::OPCode0x75() { OPCode_LD(Address(HL), L); }
void CPU::OPCode0x76() { OPCode_HALT(); }
void CPU::OPCode0x77() { OPCode_LD(Address(HL), A); }
void CPU::OPCode0x78() { OPCode_LD(A, B); }
void CPU::OPCode0x79() { OPCode_LD(A, C); }
void CPU::OPCode0x7A() { OPCode_LD(A, D); }
void CPU::OPCode0x7B() { OPCode_LD(A, E); }
void CPU::OPCode0x7C() { OPCode_LD(A, H); }
void CPU::OPCode0x7D() { OPCode_LD(A, L); }
void CPU::OPCode0x7E() { OPCode_LD(A, Address(HL)); }
void CPU::OPCode0x7F() { OPCode_LD(A, A); }
void CPU::OPCode0x80() { OPCode_ADD(C); }
void CPU::OPCode0x81() { OPCode_ADD(B); }
void CPU::OPCode0x82() { OPCode_ADD(D); }
void CPU::OPCode0x83() { OPCode_ADD(E); }
void CPU::OPCode0x84() { OPCode_ADD(H); }
void CPU::OPCode0x85() { OPCode_ADD(L); }
void CPU::OPCode0x86() { OPCode_ADD(Address(HL)); }
void CPU::OPCode0x87() { OPCode_ADD(A); }
void CPU::OPCode0x88() { OPCode_ADC(B); }
void CPU::OPCode0x89() { OPCode_ADC(C); }
void CPU::OPCode0x8A() { OPCode_ADC(D); }
void CPU::OPCode0x8B() { OPCode_ADC(E); }
void CPU::OPCode0x8C() { OPCode_ADC(H); }
void CPU::OPCode0x8D() { OPCode_ADC(L); }
void CPU::OPCode0x8E() { OPCode_ADC(Address(HL)); }
void CPU::OPCode0x8F() { OPCode_ADC(A); }
void CPU::OPCode0x90() { OPCode_SUB(B); }
void CPU::OPCode0x91() { OPCode_SUB(C); }
void CPU::OPCode0x92() { OPCode_SUB(D); }
void CPU::OPCode0x93() { OPCode_SUB(E); }
void CPU::OPCode0x94() { OPCode_SUB(H); }
void CPU::OPCode0x95() { OPCode_SUB(L); }
void CPU::OPCode0x96() { OPCode_SUB(Address(HL)); }
void CPU::OPCode0x97() { OPCode_SUB(A); }
void CPU::OPCode0x98() { OPCode_SBC(B); }
void CPU::OPCode0x99() { OPCode_SBC(C); }
void CPU::OPCode0x9A() { OPCode_SBC(D); }
void CPU::OPCode0x9B() { OPCode_SBC(E); }
void CPU::OPCode0x9C() { OPCode_SBC(H); }
void CPU::OPCode0x9D() { OPCode_SBC(L); }
void CPU::OPCode0x9E() { OPCode_SBC(Address(HL)); }
void CPU::OPCode0x9F() { OPCode_SBC(A); }
void CPU::OPCode0xA0() { OPCode_AND(B); }
void CPU::OPCode0xA1() { OPCode_AND(C); }
void CPU::OPCode0xA2() { OPCode_AND(D); }
void CPU::OPCode0xA3() { OPCode_AND(E); }
void CPU::OPCode0xA4() { OPCode_AND(H); }
void CPU::OPCode0xA5() { OPCode_AND(L); }
void CPU::OPCode0xA6() { OPCode_AND(Address(HL)); }
void CPU::OPCode0xA7() { OPCode_AND(A); }
void CPU::OPCode0xA8() { OPCode_XOR(B); }
void CPU::OPCode0xA9() { OPCode_XOR(C); }
void CPU::OPCode0xAA() { OPCode_XOR(D); }
void CPU::OPCode0xAB() { OPCode_XOR(E); }
void CPU::OPCode0xAC() { OPCode_XOR(H); }
void CPU::OPCode0xAD() { OPCode_XOR(L); }
void CPU::OPCode0xAE() { OPCode_XOR(Address(HL)); }
void CPU::OPCode0xAF() { OPCode_XOR(A); }
void CPU::OPCode0xB0() { OPCode_OR(B); }
void CPU::OPCode0xB1() { OPCode_OR(C); }
void CPU::OPCode0xB2() { OPCode_OR(D); }
void CPU::OPCode0xB3() { OPCode_OR(E); }
void CPU::OPCode0xB4() { OPCode_OR(H); }
void CPU::OPCode0xB5() { OPCode_OR(L); }
void CPU::OPCode0xB6() { OPCode_OR(Address(HL)); }
void CPU::OPCode0xB7() { OPCode_OR(A); }
void CPU::OPCode0xB8() { OPCode_CP(B); }
void CPU::OPCode0xB9() { OPCode_CP(C); }
void CPU::OPCode0xBA() { OPCode_CP(D); }
void CPU::OPCode0xBB() { OPCode_CP(E); }
void CPU::OPCode0xBC() { OPCode_CP(H); }
void CPU::OPCode0xBD() { OPCode_CP(L); }
void CPU::OPCode0xBE() { OPCode_CP(Address(HL)); }
void CPU::OPCode0xBF() { OPCode_CP(A); }
void CPU::OPCode0xC0() { OPCode_RET(Condition::NZ); }
void CPU::OPCode0xC1() { OPCode_POP(BC); }
void CPU::OPCode0xC2() { OPCode_JP(Condition::NZ); }
void CPU::OPCode0xC3() { OPCode_JP(); }
void CPU::OPCode0xC4() { OPCode_CALL(Condition::NZ); }
void CPU::OPCode0xC5() { OPCode_PUSH(BC); }
void CPU::OPCode0xC6() { OPCode_ADD(); }
void CPU::OPCode0xC7() { OPCode_RST(0x00); } //rst1
void CPU::OPCode0xC8() { OPCode_RET(Condition::Z); }
void CPU::OPCode0xC9() { OPCode_RET(); }
void CPU::OPCode0xCA() { OPCode_JP(Condition::Z); }
void CPU::OPCode0xCB() { /* External Ops */ }
void CPU::OPCode0xCC() { OPCode_CALL(Condition::Z); }
void CPU::OPCode0xCD() { OPCode_CALL(); }
void CPU::OPCode0xCE() { OPCode_ADC(); }
void CPU::OPCode0xCF() { OPCode_RST(0x08); } //rst2
void CPU::OPCode0xD0() { OPCode_RET(Condition::NC); }
void CPU::OPCode0xD1() { OPCode_POP(DE); }
void CPU::OPCode0xD2() { OPCode_JP(Condition::NC); }
void CPU::OPCode0xD3() { /* Not used */ }
void CPU::OPCode0xD4() { OPCode_CALL(Condition::NC); }
void CPU::OPCode0xD5() { OPCode_PUSH(DE); }
void CPU::OPCode0xD6() { OPCode_SUB(); }
void CPU::OPCode0xD7() { OPCode_RST(0x10); } //rst3
void CPU::OPCode0xD8() { OPCode_RET(Condition::C); }
void CPU::OPCode0xD9() { OPCode_RETI(); }
void CPU::OPCode0xDA() { OPCode_JP(Condition::C); }
void CPU::OPCode0xDB() { /* Not used */ }
void CPU::OPCode0xDC() { OPCode_CALL(Condition::C); }
void CPU::OPCode0xDD() { /* Not used */ }
void CPU::OPCode0xDE() { OPCode_SBC(); }
void CPU::OPCode0xDF() { OPCode_RST(0x18); } //rst4
void CPU::OPCode0xE0() { OPCode_LDH_TO_DATA(); }
void CPU::OPCode0xE1() { OPCode_POP(HL); }
void CPU::OPCode0xE2() { OPCode_LDH_TO_C(); }
void CPU::OPCode0xE3() { /* Not used */ }
void CPU::OPCode0xE4() { /* Not used */ }
void CPU::OPCode0xE5() { OPCode_PUSH(HL); }
void CPU::OPCode0xE6() { OPCode_AND(); }
void CPU::OPCode0xE7() { OPCode_RST(0x20); } //rst5
void CPU::OPCode0xE8() { OPCode_ADD_SP(); }
void CPU::OPCode0xE9() { OPCode_JP(Address(HL)); }
void CPU::OPCode0xEA() { OPCode_LD_TO_ADDRESS(A); }
void CPU::OPCode0xEB() { /* Not used */ }
void CPU::OPCode0xEC() { /* Not used */ }
void CPU::OPCode0xED() { /* Not used */ }
void CPU::OPCode0xEE() { OPCode_XOR(); }
void CPU::OPCode0xEF() { OPCode_RST(0x28); } //rst6
void CPU::OPCode0xF0() { OPCode_LDH_TO_A(); }
void CPU::OPCode0xF1() { OPCode_POP(AF); }
void CPU::OPCode0xF2() { OPCode_LDH_C_TO_A(); }
void CPU::OPCode0xF3() { OPCode_DI(); }
void CPU::OPCode0xF4() { /* Not used */ }
void CPU::OPCode0xF5() { OPCode_PUSH(AF); }
void CPU::OPCode0xF6() { OPCode_OR(); }
void CPU::OPCode0xF7() { OPCode_RST(0x30); } //rst7
void CPU::OPCode0xF8() { OPCode_LDHL(); }
void CPU::OPCode0xF9() { OPCode_LD(SP, HL); }
void CPU::OPCode0xFA() { OPCode_LD_FROM_ADDRESS(A); }
void CPU::OPCode0xFB() { OPCode_EI(); }
void CPU::OPCode0xFC() { /* Not used */ }
void CPU::OPCode0xFD() { /* Not used */ }
void CPU::OPCode0xFE() { OPCode_CP(); }
void CPU::OPCode0xFF() { OPCode_RST(0x38); } //rst8
