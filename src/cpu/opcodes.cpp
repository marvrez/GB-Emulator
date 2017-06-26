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
void CPU::OPCode0x10() { OPCode_HALT();/*OPCode_STOP();*/ }
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
void CPU::OPCode0x80() { OPCode_ADD(B); }
void CPU::OPCode0x81() { OPCode_ADD(C); }
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


//CB OPCodes
void CPU::OPCodeCB0x00() { OPCode_RLC(B); }
void CPU::OPCodeCB0x01() { OPCode_RLC(C); }
void CPU::OPCodeCB0x02() { OPCode_RLC(D); }
void CPU::OPCodeCB0x03() { OPCode_RLC(E); }
void CPU::OPCodeCB0x04() { OPCode_RLC(H); }
void CPU::OPCodeCB0x05() { OPCode_RLC(L); }
void CPU::OPCodeCB0x06() { OPCode_RLC(Address(HL)); }
void CPU::OPCodeCB0x07() { OPCode_RLC(A); }
void CPU::OPCodeCB0x08() { OPCode_RRC(B); }
void CPU::OPCodeCB0x09() { OPCode_RRC(C); }
void CPU::OPCodeCB0x0A() { OPCode_RRC(D); }
void CPU::OPCodeCB0x0B() { OPCode_RRC(E); }
void CPU::OPCodeCB0x0C() { OPCode_RRC(H); }
void CPU::OPCodeCB0x0D() { OPCode_RRC(L); }
void CPU::OPCodeCB0x0E() { OPCode_RRC(Address(HL)); }
void CPU::OPCodeCB0x0F() { OPCode_RRC(A); }
void CPU::OPCodeCB0x10() { OPCode_RL(B); }
void CPU::OPCodeCB0x11() { OPCode_RL(C); }
void CPU::OPCodeCB0x12() { OPCode_RL(D); }
void CPU::OPCodeCB0x13() { OPCode_RL(E); }
void CPU::OPCodeCB0x14() { OPCode_RL(H); }
void CPU::OPCodeCB0x15() { OPCode_RL(L); }
void CPU::OPCodeCB0x16() { OPCode_RL(Address(HL)); }
void CPU::OPCodeCB0x17() { OPCode_RL(A); }
void CPU::OPCodeCB0x18() { OPCode_RR(B); }
void CPU::OPCodeCB0x19() { OPCode_RR(C); }
void CPU::OPCodeCB0x1A() { OPCode_RR(D); }
void CPU::OPCodeCB0x1B() { OPCode_RR(E); }
void CPU::OPCodeCB0x1C() { OPCode_RR(H); }
void CPU::OPCodeCB0x1D() { OPCode_RR(L); }
void CPU::OPCodeCB0x1E() { OPCode_RR(Address(HL)); }
void CPU::OPCodeCB0x1F() { OPCode_RR(A); }
void CPU::OPCodeCB0x20() { OPCode_SLA(B); }
void CPU::OPCodeCB0x21() { OPCode_SLA(C); }
void CPU::OPCodeCB0x22() { OPCode_SLA(D); }
void CPU::OPCodeCB0x23() { OPCode_SLA(E); }
void CPU::OPCodeCB0x24() { OPCode_SLA(H); }
void CPU::OPCodeCB0x25() { OPCode_SLA(L); }
void CPU::OPCodeCB0x26() { OPCode_SLA(Address(HL)); }
void CPU::OPCodeCB0x27() { OPCode_SLA(A); }
void CPU::OPCodeCB0x28() { OPCode_SRA(B); }
void CPU::OPCodeCB0x29() { OPCode_SRA(C); }
void CPU::OPCodeCB0x2A() { OPCode_SRA(D); }
void CPU::OPCodeCB0x2B() { OPCode_SRA(E); }
void CPU::OPCodeCB0x2C() { OPCode_SRA(H); }
void CPU::OPCodeCB0x2D() { OPCode_SRA(L); }
void CPU::OPCodeCB0x2E() { OPCode_SRA(Address(HL)); }
void CPU::OPCodeCB0x2F() { OPCode_SRA(A); }
void CPU::OPCodeCB0x30() { OPCode_SWAP(B); }
void CPU::OPCodeCB0x31() { OPCode_SWAP(C); }
void CPU::OPCodeCB0x32() { OPCode_SWAP(D); }
void CPU::OPCodeCB0x33() { OPCode_SWAP(E); }
void CPU::OPCodeCB0x34() { OPCode_SWAP(H); }
void CPU::OPCodeCB0x35() { OPCode_SWAP(L); }
void CPU::OPCodeCB0x36() { OPCode_SWAP(Address(HL)); }
void CPU::OPCodeCB0x37() { OPCode_SWAP(A); }
void CPU::OPCodeCB0x38() { OPCode_SRL(B); }
void CPU::OPCodeCB0x39() { OPCode_SRL(C); }
void CPU::OPCodeCB0x3A() { OPCode_SRL(D); }
void CPU::OPCodeCB0x3B() { OPCode_SRL(E); }
void CPU::OPCodeCB0x3C() { OPCode_SRL(H); }
void CPU::OPCodeCB0x3D() { OPCode_SRL(L); }
void CPU::OPCodeCB0x3E() { OPCode_SRL(Address(HL)); }
void CPU::OPCodeCB0x3F() { OPCode_SRL(A); }
void CPU::OPCodeCB0x40() { OPCode_BIT(0, B); }
void CPU::OPCodeCB0x41() { OPCode_BIT(0, C); }
void CPU::OPCodeCB0x42() { OPCode_BIT(0, D); }
void CPU::OPCodeCB0x43() { OPCode_BIT(0, E); }
void CPU::OPCodeCB0x44() { OPCode_BIT(0, H); }
void CPU::OPCodeCB0x45() { OPCode_BIT(0, L); }
void CPU::OPCodeCB0x46() { OPCode_BIT(0, Address(HL)); }
void CPU::OPCodeCB0x47() { OPCode_BIT(0, A); }
void CPU::OPCodeCB0x48() { OPCode_BIT(1, B); }
void CPU::OPCodeCB0x49() { OPCode_BIT(1, C); }
void CPU::OPCodeCB0x4A() { OPCode_BIT(1, D); }
void CPU::OPCodeCB0x4B() { OPCode_BIT(1, E); }
void CPU::OPCodeCB0x4C() { OPCode_BIT(1, H); }
void CPU::OPCodeCB0x4D() { OPCode_BIT(1, L); }
void CPU::OPCodeCB0x4E() { OPCode_BIT(1, Address(HL)); }
void CPU::OPCodeCB0x4F() { OPCode_BIT(1, A); }
void CPU::OPCodeCB0x50() { OPCode_BIT(2, B); }
void CPU::OPCodeCB0x51() { OPCode_BIT(2, C); }
void CPU::OPCodeCB0x52() { OPCode_BIT(2, D); }
void CPU::OPCodeCB0x53() { OPCode_BIT(2, E); }
void CPU::OPCodeCB0x54() { OPCode_BIT(2, H); }
void CPU::OPCodeCB0x55() { OPCode_BIT(2, L); }
void CPU::OPCodeCB0x56() { OPCode_BIT(2, Address(HL)); }
void CPU::OPCodeCB0x57() { OPCode_BIT(2, A); }
void CPU::OPCodeCB0x58() { OPCode_BIT(3, B); }
void CPU::OPCodeCB0x59() { OPCode_BIT(3, C); }
void CPU::OPCodeCB0x5A() { OPCode_BIT(3, D); }
void CPU::OPCodeCB0x5B() { OPCode_BIT(3, E); }
void CPU::OPCodeCB0x5C() { OPCode_BIT(3, H); }
void CPU::OPCodeCB0x5D() { OPCode_BIT(3, L); }
void CPU::OPCodeCB0x5E() { OPCode_BIT(3, Address(HL)); }
void CPU::OPCodeCB0x5F() { OPCode_BIT(3, A); }
void CPU::OPCodeCB0x60() { OPCode_BIT(4, B); }
void CPU::OPCodeCB0x61() { OPCode_BIT(4, C); }
void CPU::OPCodeCB0x62() { OPCode_BIT(4, D); }
void CPU::OPCodeCB0x63() { OPCode_BIT(4, E); }
void CPU::OPCodeCB0x64() { OPCode_BIT(4, H); }
void CPU::OPCodeCB0x65() { OPCode_BIT(4, L); }
void CPU::OPCodeCB0x66() { OPCode_BIT(4, Address(HL)); }
void CPU::OPCodeCB0x67() { OPCode_BIT(4, A); }
void CPU::OPCodeCB0x68() { OPCode_BIT(5, B); }
void CPU::OPCodeCB0x69() { OPCode_BIT(5, C); }
void CPU::OPCodeCB0x6A() { OPCode_BIT(5, D); }
void CPU::OPCodeCB0x6B() { OPCode_BIT(5, E); }
void CPU::OPCodeCB0x6C() { OPCode_BIT(5, H); }
void CPU::OPCodeCB0x6D() { OPCode_BIT(5, L); }
void CPU::OPCodeCB0x6E() { OPCode_BIT(5, Address(HL)); }
void CPU::OPCodeCB0x6F() { OPCode_BIT(5, A); }
void CPU::OPCodeCB0x70() { OPCode_BIT(6, B); }
void CPU::OPCodeCB0x71() { OPCode_BIT(6, C); }
void CPU::OPCodeCB0x72() { OPCode_BIT(6, D); }
void CPU::OPCodeCB0x73() { OPCode_BIT(6, E); }
void CPU::OPCodeCB0x74() { OPCode_BIT(6, H); }
void CPU::OPCodeCB0x75() { OPCode_BIT(6, L); }
void CPU::OPCodeCB0x76() { OPCode_BIT(6, Address(HL)); }
void CPU::OPCodeCB0x77() { OPCode_BIT(6, A); }
void CPU::OPCodeCB0x78() { OPCode_BIT(7, B); }
void CPU::OPCodeCB0x79() { OPCode_BIT(7, C); }
void CPU::OPCodeCB0x7A() { OPCode_BIT(7, D); }
void CPU::OPCodeCB0x7B() { OPCode_BIT(7, E); }
void CPU::OPCodeCB0x7C() { OPCode_BIT(7, H); }
void CPU::OPCodeCB0x7D() { OPCode_BIT(7, L); }
void CPU::OPCodeCB0x7E() { OPCode_BIT(7, Address(HL)); }
void CPU::OPCodeCB0x7F() { OPCode_BIT(7, A); }
void CPU::OPCodeCB0x80() { OPCode_RES(0, B); }
void CPU::OPCodeCB0x81() { OPCode_RES(0, C); }
void CPU::OPCodeCB0x82() { OPCode_RES(0, D); }
void CPU::OPCodeCB0x83() { OPCode_RES(0, E); }
void CPU::OPCodeCB0x84() { OPCode_RES(0, H); }
void CPU::OPCodeCB0x85() { OPCode_RES(0, L); }
void CPU::OPCodeCB0x86() { OPCode_RES(0, Address(HL)); }
void CPU::OPCodeCB0x87() { OPCode_RES(0, A); }
void CPU::OPCodeCB0x88() { OPCode_RES(1, B); }
void CPU::OPCodeCB0x89() { OPCode_RES(1, C); }
void CPU::OPCodeCB0x8A() { OPCode_RES(1, D); }
void CPU::OPCodeCB0x8B() { OPCode_RES(1, E); }
void CPU::OPCodeCB0x8C() { OPCode_RES(1, H); }
void CPU::OPCodeCB0x8D() { OPCode_RES(1, L); }
void CPU::OPCodeCB0x8E() { OPCode_RES(1, Address(HL)); }
void CPU::OPCodeCB0x8F() { OPCode_RES(1, A); }
void CPU::OPCodeCB0x90() { OPCode_RES(2, B); }
void CPU::OPCodeCB0x91() { OPCode_RES(2, C); }
void CPU::OPCodeCB0x92() { OPCode_RES(2, D); }
void CPU::OPCodeCB0x93() { OPCode_RES(2, E); }
void CPU::OPCodeCB0x94() { OPCode_RES(2, H); }
void CPU::OPCodeCB0x95() { OPCode_RES(2, L); }
void CPU::OPCodeCB0x96() { OPCode_RES(2, Address(HL)); }
void CPU::OPCodeCB0x97() { OPCode_RES(2, A); }
void CPU::OPCodeCB0x98() { OPCode_RES(3, B); }
void CPU::OPCodeCB0x99() { OPCode_RES(3, C); }
void CPU::OPCodeCB0x9A() { OPCode_RES(3, D); }
void CPU::OPCodeCB0x9B() { OPCode_RES(3, E); }
void CPU::OPCodeCB0x9C() { OPCode_RES(3, H); }
void CPU::OPCodeCB0x9D() { OPCode_RES(3, L); }
void CPU::OPCodeCB0x9E() { OPCode_RES(3, Address(HL)); }
void CPU::OPCodeCB0x9F() { OPCode_RES(3, A); }
void CPU::OPCodeCB0xA0() { OPCode_RES(4, B); }
void CPU::OPCodeCB0xA1() { OPCode_RES(4, C); }
void CPU::OPCodeCB0xA2() { OPCode_RES(4, D); }
void CPU::OPCodeCB0xA3() { OPCode_RES(4, E); }
void CPU::OPCodeCB0xA4() { OPCode_RES(4, H); }
void CPU::OPCodeCB0xA5() { OPCode_RES(4, L); }
void CPU::OPCodeCB0xA6() { OPCode_RES(4, Address(HL)); }
void CPU::OPCodeCB0xA7() { OPCode_RES(4, A); }
void CPU::OPCodeCB0xA8() { OPCode_RES(5, B); }
void CPU::OPCodeCB0xA9() { OPCode_RES(5, C); }
void CPU::OPCodeCB0xAA() { OPCode_RES(5, D); }
void CPU::OPCodeCB0xAB() { OPCode_RES(5, E); }
void CPU::OPCodeCB0xAC() { OPCode_RES(5, H); }
void CPU::OPCodeCB0xAD() { OPCode_RES(5, L); }
void CPU::OPCodeCB0xAE() { OPCode_RES(5, Address(HL)); }
void CPU::OPCodeCB0xAF() { OPCode_RES(5, A); }
void CPU::OPCodeCB0xB0() { OPCode_RES(6, B); }
void CPU::OPCodeCB0xB1() { OPCode_RES(6, C); }
void CPU::OPCodeCB0xB2() { OPCode_RES(6, D); }
void CPU::OPCodeCB0xB3() { OPCode_RES(6, E); }
void CPU::OPCodeCB0xB4() { OPCode_RES(6, H); }
void CPU::OPCodeCB0xB5() { OPCode_RES(6, L); }
void CPU::OPCodeCB0xB6() { OPCode_RES(6, Address(HL)); }
void CPU::OPCodeCB0xB7() { OPCode_RES(6, A); }
void CPU::OPCodeCB0xB8() { OPCode_RES(7, B); }
void CPU::OPCodeCB0xB9() { OPCode_RES(7, C); }
void CPU::OPCodeCB0xBA() { OPCode_RES(7, D); }
void CPU::OPCodeCB0xBB() { OPCode_RES(7, E); }
void CPU::OPCodeCB0xBC() { OPCode_RES(7, H); }
void CPU::OPCodeCB0xBD() { OPCode_RES(7, L); }
void CPU::OPCodeCB0xBE() { OPCode_RES(7, Address(HL)); }
void CPU::OPCodeCB0xBF() { OPCode_RES(7, A); }
void CPU::OPCodeCB0xC0() { OPCode_SET(0, B); }
void CPU::OPCodeCB0xC1() { OPCode_SET(0, C); }
void CPU::OPCodeCB0xC2() { OPCode_SET(0, D); }
void CPU::OPCodeCB0xC3() { OPCode_SET(0, E); }
void CPU::OPCodeCB0xC4() { OPCode_SET(0, H); }
void CPU::OPCodeCB0xC5() { OPCode_SET(0, L); }
void CPU::OPCodeCB0xC6() { OPCode_SET(0, Address(HL)); }
void CPU::OPCodeCB0xC7() { OPCode_SET(0, A); }
void CPU::OPCodeCB0xC8() { OPCode_SET(1, B); }
void CPU::OPCodeCB0xC9() { OPCode_SET(1, C); }
void CPU::OPCodeCB0xCA() { OPCode_SET(1, D); }
void CPU::OPCodeCB0xCB() { OPCode_SET(1, E); }
void CPU::OPCodeCB0xCC() { OPCode_SET(1, H); }
void CPU::OPCodeCB0xCD() { OPCode_SET(1, L); }
void CPU::OPCodeCB0xCE() { OPCode_SET(1, Address(HL)); }
void CPU::OPCodeCB0xCF() { OPCode_SET(1, A); }
void CPU::OPCodeCB0xD0() { OPCode_SET(2, B); }
void CPU::OPCodeCB0xD1() { OPCode_SET(2, C); }
void CPU::OPCodeCB0xD2() { OPCode_SET(2, D); }
void CPU::OPCodeCB0xD3() { OPCode_SET(2, E); }
void CPU::OPCodeCB0xD4() { OPCode_SET(2, H); }
void CPU::OPCodeCB0xD5() { OPCode_SET(2, L); }
void CPU::OPCodeCB0xD6() { OPCode_SET(2, Address(HL)); }
void CPU::OPCodeCB0xD7() { OPCode_SET(2, A); }
void CPU::OPCodeCB0xD8() { OPCode_SET(3, B); }
void CPU::OPCodeCB0xD9() { OPCode_SET(3, C); }
void CPU::OPCodeCB0xDA() { OPCode_SET(3, D); }
void CPU::OPCodeCB0xDB() { OPCode_SET(3, E); }
void CPU::OPCodeCB0xDC() { OPCode_SET(3, H); }
void CPU::OPCodeCB0xDD() { OPCode_SET(3, L); }
void CPU::OPCodeCB0xDE() { OPCode_SET(3, Address(HL)); }
void CPU::OPCodeCB0xDF() { OPCode_SET(3, A); }
void CPU::OPCodeCB0xE0() { OPCode_SET(4, B); }
void CPU::OPCodeCB0xE1() { OPCode_SET(4, C); }
void CPU::OPCodeCB0xE2() { OPCode_SET(4, D); }
void CPU::OPCodeCB0xE3() { OPCode_SET(4, E); }
void CPU::OPCodeCB0xE4() { OPCode_SET(4, H); }
void CPU::OPCodeCB0xE5() { OPCode_SET(4, L); }
void CPU::OPCodeCB0xE6() { OPCode_SET(4, Address(HL)); }
void CPU::OPCodeCB0xE7() { OPCode_SET(4, A); }
void CPU::OPCodeCB0xE8() { OPCode_SET(5, B); }
void CPU::OPCodeCB0xE9() { OPCode_SET(5, C); }
void CPU::OPCodeCB0xEA() { OPCode_SET(5, D); }
void CPU::OPCodeCB0xEB() { OPCode_SET(5, E); }
void CPU::OPCodeCB0xEC() { OPCode_SET(5, H); }
void CPU::OPCodeCB0xED() { OPCode_SET(5, L); }
void CPU::OPCodeCB0xEE() { OPCode_SET(5, Address(HL)); }
void CPU::OPCodeCB0xEF() { OPCode_SET(5, A); }
void CPU::OPCodeCB0xF0() { OPCode_SET(6, B); }
void CPU::OPCodeCB0xF1() { OPCode_SET(6, C); }
void CPU::OPCodeCB0xF2() { OPCode_SET(6, D); }
void CPU::OPCodeCB0xF3() { OPCode_SET(6, E); }
void CPU::OPCodeCB0xF4() { OPCode_SET(6, H); }
void CPU::OPCodeCB0xF5() { OPCode_SET(6, L); }
void CPU::OPCodeCB0xF6() { OPCode_SET(6, Address(HL)); }
void CPU::OPCodeCB0xF7() { OPCode_SET(6, A); }
void CPU::OPCodeCB0xF8() { OPCode_SET(7, B); }
void CPU::OPCodeCB0xF9() { OPCode_SET(7, C); }
void CPU::OPCodeCB0xFA() { OPCode_SET(7, D); }
void CPU::OPCodeCB0xFB() { OPCode_SET(7, E); }
void CPU::OPCodeCB0xFC() { OPCode_SET(7, H); }
void CPU::OPCodeCB0xFD() { OPCode_SET(7, L); }
void CPU::OPCodeCB0xFE() { OPCode_SET(7, Address(HL)); }
void CPU::OPCodeCB0xFF() { OPCode_SET(7, A); }
