#include "cpu.h"

#include "opcode_cycles.h"

CPU::CPU(std::shared_ptr<MMU> mmu) :
    alu(std::make_unique<ALU>(&A,&F)), mmu(mmu), AF(&A,&F), BC(&B,&C), DE(&D, &E), HL(&H,&L)
{
    InitOPCodeFunctors();
}

Cycles CPU::tick() {
    handleInterrupts();
    if(halted) return OPCodeMachineCycles[0x00];
    return executeOPCode(getByteFromPC(), PC.getValue()); //getopcode from PC and execute..
}

Cycles CPU::executeOPCode(u8 opcode, u16 OPCodePC) {
    branchTaken = false;
    if (opcode == 0xCB) return executeCBOPCode(getByteFromPC(), OPCodePC);
    else return executeNormalOPCode(opcode, OPCodePC);
}

void CPU::handleInterrupts() {
    /*
    if (interruptsEnabled) {
        u8 firedInterrupts = interruptFlag.getValue() & interruptEnabled.getValue();

        if (!firedInterrupts) return;

        push(PC);

        bool handledInterrupt = false;

        handledInterrupt = handleInterrupt(0, Interrupts::VBLANK, firedInterrupts);
        if (handledInterrupt) return;

        handledInterrupt = handleInterrupt(1, Interrupts::LCDC_STATUS, firedInterrupts);
        if (handledInterrupt) return;

        handledInterrupt = handleInterrupt(2, Interrupts::TIMER, firedInterrupts);
        if (handledInterrupt) return;

        handledInterrupt = handleInterrupt(3, Interrupts::SERIAL, firedInterrupts);
        if (handledInterrupt) return;

        handledInterrupt = handleInterrupt(4, Interrupts::JOYPAD, firedInterrupts);
        if (handledInterrupt) return;
    }
    */
    using BitOperations::checkBit;

    if (interruptsEnabled == false && halted == false)
        return;
    u8 firedInterrupts = interruptFlag.getValue() & interruptEnabled.getValue();
    if(!firedInterrupts) return;
    for (unsigned interruptBit = 0; interruptBit < 5; ++interruptBit) {
        if (checkBit(firedInterrupts, interruptBit)) {
            halted = false;
            if (interruptsEnabled) {
                interruptsEnabled = false;
                interruptFlag.setBitTo(interruptBit, 0);
                OPCode_RST(0x0040 | (interruptBit << 3));
                return;
            }
            return;
        }
    }
}

bool CPU::isCondition(Condition condition) {
    bool shouldBranch;

    switch (condition) {
        case Condition::NZ:
            shouldBranch = !F.checkFlagZero();
            break;
        case Condition::Z:
            shouldBranch = F.checkFlagZero();
            break;
        case Condition::NC:
            shouldBranch = !F.checkFlagCarry();
            break;
        case Condition::C:
            shouldBranch = F.checkFlagCarry();
            break;
    }

    //remember branch so correct cycles are to be used
    return branchTaken = shouldBranch;
}

u8 CPU::getByteFromPC() { //fetch opcode
    u8 byte = mmu->readByte(Address(PC));
    PC.increment();
    return byte;
}

s8 CPU::getSignedByteFromPC() {
    return static_cast<s8>(getByteFromPC());
}

u16 CPU::getWordFromPC() {
    u8 low = getByteFromPC();
    u8 high = getByteFromPC();
    return BitOperations::composeBytes(high, low);
}

void CPU::push(const IWordRegister& reg) {
    SP.decrement();
    mmu->writeByte(Address(SP), reg.getHighValue());
    SP.decrement();
    mmu->writeByte(Address(SP), reg.getLowValue());
}

void CPU::pop(IWordRegister& reg) {
    u8 low = mmu->readByte(Address(SP));
    SP.increment();
    u8 high = mmu->readByte(Address(SP));
    SP.increment();

    u16 value = BitOperations::composeBytes(high, low);
    reg.setValue(value);
}

Cycles CPU::executeNormalOPCode(u8 opcode, u16 OPCodePC) {
    OPCptr* opcodeTable = OPCodes;
    (this->*opcodeTable[opcode])(); //execute opcode
    return !branchTaken ? //return number of cycles taken
            OPCodeMachineCycles[opcode]
          : OPCodeBranchMachineCycles[opcode];
}

Cycles CPU::executeCBOPCode(u8 opcode, u16 OPCodePC) {
    OPCptr* opcodeTable = OPCodesCB;
    (this->*opcodeTable[opcode])(); //execute opcode
    return OPCodeCBMachineCycles[opcode]; //return number of cycles taken
}

bool CPU::isHalted() const {
    return this->halted;
}

void CPU::InitOPCodeFunctors() {
    OPCodes[0x00] = &CPU::OPCode0x00;
    OPCodes[0x01] = &CPU::OPCode0x01;
    OPCodes[0x02] = &CPU::OPCode0x02;
    OPCodes[0x03] = &CPU::OPCode0x03;
    OPCodes[0x04] = &CPU::OPCode0x04;
    OPCodes[0x05] = &CPU::OPCode0x05;
    OPCodes[0x06] = &CPU::OPCode0x06;
    OPCodes[0x07] = &CPU::OPCode0x07;
    OPCodes[0x08] = &CPU::OPCode0x08;
    OPCodes[0x09] = &CPU::OPCode0x09;
    OPCodes[0x0A] = &CPU::OPCode0x0A;
    OPCodes[0x0B] = &CPU::OPCode0x0B;
    OPCodes[0x0C] = &CPU::OPCode0x0C;
    OPCodes[0x0D] = &CPU::OPCode0x0D;
    OPCodes[0x0E] = &CPU::OPCode0x0E;
    OPCodes[0x0F] = &CPU::OPCode0x0F;

    OPCodes[0x10] = &CPU::OPCode0x10;
    OPCodes[0x11] = &CPU::OPCode0x11;
    OPCodes[0x12] = &CPU::OPCode0x12;
    OPCodes[0x13] = &CPU::OPCode0x13;
    OPCodes[0x14] = &CPU::OPCode0x14;
    OPCodes[0x15] = &CPU::OPCode0x15;
    OPCodes[0x16] = &CPU::OPCode0x16;
    OPCodes[0x17] = &CPU::OPCode0x17;
    OPCodes[0x18] = &CPU::OPCode0x18;
    OPCodes[0x19] = &CPU::OPCode0x19;
    OPCodes[0x1A] = &CPU::OPCode0x1A;
    OPCodes[0x1B] = &CPU::OPCode0x1B;
    OPCodes[0x1C] = &CPU::OPCode0x1C;
    OPCodes[0x1D] = &CPU::OPCode0x1D;
    OPCodes[0x1E] = &CPU::OPCode0x1E;
    OPCodes[0x1F] = &CPU::OPCode0x1F;

    OPCodes[0x20] = &CPU::OPCode0x20;
    OPCodes[0x21] = &CPU::OPCode0x21;
    OPCodes[0x22] = &CPU::OPCode0x22;
    OPCodes[0x23] = &CPU::OPCode0x23;
    OPCodes[0x24] = &CPU::OPCode0x24;
    OPCodes[0x25] = &CPU::OPCode0x25;
    OPCodes[0x26] = &CPU::OPCode0x26;
    OPCodes[0x27] = &CPU::OPCode0x27;
    OPCodes[0x28] = &CPU::OPCode0x28;
    OPCodes[0x29] = &CPU::OPCode0x29;
    OPCodes[0x2A] = &CPU::OPCode0x2A;
    OPCodes[0x2B] = &CPU::OPCode0x2B;
    OPCodes[0x2C] = &CPU::OPCode0x2C;
    OPCodes[0x2D] = &CPU::OPCode0x2D;
    OPCodes[0x2E] = &CPU::OPCode0x2E;
    OPCodes[0x2F] = &CPU::OPCode0x2F;

    OPCodes[0x30] = &CPU::OPCode0x30;
    OPCodes[0x31] = &CPU::OPCode0x31;
    OPCodes[0x32] = &CPU::OPCode0x32;
    OPCodes[0x33] = &CPU::OPCode0x33;
    OPCodes[0x34] = &CPU::OPCode0x34;
    OPCodes[0x35] = &CPU::OPCode0x35;
    OPCodes[0x36] = &CPU::OPCode0x36;
    OPCodes[0x37] = &CPU::OPCode0x37;
    OPCodes[0x38] = &CPU::OPCode0x38;
    OPCodes[0x39] = &CPU::OPCode0x39;
    OPCodes[0x3A] = &CPU::OPCode0x3A;
    OPCodes[0x3B] = &CPU::OPCode0x3B;
    OPCodes[0x3C] = &CPU::OPCode0x3C;
    OPCodes[0x3D] = &CPU::OPCode0x3D;
    OPCodes[0x3E] = &CPU::OPCode0x3E;
    OPCodes[0x3F] = &CPU::OPCode0x3F;

    OPCodes[0x40] = &CPU::OPCode0x40;
    OPCodes[0x41] = &CPU::OPCode0x41;
    OPCodes[0x42] = &CPU::OPCode0x42;
    OPCodes[0x43] = &CPU::OPCode0x43;
    OPCodes[0x44] = &CPU::OPCode0x44;
    OPCodes[0x45] = &CPU::OPCode0x45;
    OPCodes[0x46] = &CPU::OPCode0x46;
    OPCodes[0x47] = &CPU::OPCode0x47;
    OPCodes[0x48] = &CPU::OPCode0x48;
    OPCodes[0x49] = &CPU::OPCode0x49;
    OPCodes[0x4A] = &CPU::OPCode0x4A;
    OPCodes[0x4B] = &CPU::OPCode0x4B;
    OPCodes[0x4C] = &CPU::OPCode0x4C;
    OPCodes[0x4D] = &CPU::OPCode0x4D;
    OPCodes[0x4E] = &CPU::OPCode0x4E;
    OPCodes[0x4F] = &CPU::OPCode0x4F;

    OPCodes[0x50] = &CPU::OPCode0x50;
    OPCodes[0x51] = &CPU::OPCode0x51;
    OPCodes[0x52] = &CPU::OPCode0x52;
    OPCodes[0x53] = &CPU::OPCode0x53;
    OPCodes[0x54] = &CPU::OPCode0x54;
    OPCodes[0x55] = &CPU::OPCode0x55;
    OPCodes[0x56] = &CPU::OPCode0x56;
    OPCodes[0x57] = &CPU::OPCode0x57;
    OPCodes[0x58] = &CPU::OPCode0x58;
    OPCodes[0x59] = &CPU::OPCode0x59;
    OPCodes[0x5A] = &CPU::OPCode0x5A;
    OPCodes[0x5B] = &CPU::OPCode0x5B;
    OPCodes[0x5C] = &CPU::OPCode0x5C;
    OPCodes[0x5D] = &CPU::OPCode0x5D;
    OPCodes[0x5E] = &CPU::OPCode0x5E;
    OPCodes[0x5F] = &CPU::OPCode0x5F;

    OPCodes[0x60] = &CPU::OPCode0x60;
    OPCodes[0x61] = &CPU::OPCode0x61;
    OPCodes[0x62] = &CPU::OPCode0x62;
    OPCodes[0x63] = &CPU::OPCode0x63;
    OPCodes[0x64] = &CPU::OPCode0x64;
    OPCodes[0x65] = &CPU::OPCode0x65;
    OPCodes[0x66] = &CPU::OPCode0x66;
    OPCodes[0x67] = &CPU::OPCode0x67;
    OPCodes[0x68] = &CPU::OPCode0x68;
    OPCodes[0x69] = &CPU::OPCode0x69;
    OPCodes[0x6A] = &CPU::OPCode0x6A;
    OPCodes[0x6B] = &CPU::OPCode0x6B;
    OPCodes[0x6C] = &CPU::OPCode0x6C;
    OPCodes[0x6D] = &CPU::OPCode0x6D;
    OPCodes[0x6E] = &CPU::OPCode0x6E;
    OPCodes[0x6F] = &CPU::OPCode0x6F;

    OPCodes[0x70] = &CPU::OPCode0x70;
    OPCodes[0x71] = &CPU::OPCode0x71;
    OPCodes[0x72] = &CPU::OPCode0x72;
    OPCodes[0x73] = &CPU::OPCode0x73;
    OPCodes[0x74] = &CPU::OPCode0x74;
    OPCodes[0x75] = &CPU::OPCode0x75;
    OPCodes[0x76] = &CPU::OPCode0x76;
    OPCodes[0x77] = &CPU::OPCode0x77;
    OPCodes[0x78] = &CPU::OPCode0x78;
    OPCodes[0x79] = &CPU::OPCode0x79;
    OPCodes[0x7A] = &CPU::OPCode0x7A;
    OPCodes[0x7B] = &CPU::OPCode0x7B;
    OPCodes[0x7C] = &CPU::OPCode0x7C;
    OPCodes[0x7D] = &CPU::OPCode0x7D;
    OPCodes[0x7E] = &CPU::OPCode0x7E;
    OPCodes[0x7F] = &CPU::OPCode0x7F;

    OPCodes[0x80] = &CPU::OPCode0x80;
    OPCodes[0x81] = &CPU::OPCode0x81;
    OPCodes[0x82] = &CPU::OPCode0x82;
    OPCodes[0x83] = &CPU::OPCode0x83;
    OPCodes[0x84] = &CPU::OPCode0x84;
    OPCodes[0x85] = &CPU::OPCode0x85;
    OPCodes[0x86] = &CPU::OPCode0x86;
    OPCodes[0x87] = &CPU::OPCode0x87;
    OPCodes[0x88] = &CPU::OPCode0x88;
    OPCodes[0x89] = &CPU::OPCode0x89;
    OPCodes[0x8A] = &CPU::OPCode0x8A;
    OPCodes[0x8B] = &CPU::OPCode0x8B;
    OPCodes[0x8C] = &CPU::OPCode0x8C;
    OPCodes[0x8D] = &CPU::OPCode0x8D;
    OPCodes[0x8E] = &CPU::OPCode0x8E;
    OPCodes[0x8F] = &CPU::OPCode0x8F;

    OPCodes[0x90] = &CPU::OPCode0x90;
    OPCodes[0x91] = &CPU::OPCode0x91;
    OPCodes[0x92] = &CPU::OPCode0x92;
    OPCodes[0x93] = &CPU::OPCode0x93;
    OPCodes[0x94] = &CPU::OPCode0x94;
    OPCodes[0x95] = &CPU::OPCode0x95;
    OPCodes[0x96] = &CPU::OPCode0x96;
    OPCodes[0x97] = &CPU::OPCode0x97;
    OPCodes[0x98] = &CPU::OPCode0x98;
    OPCodes[0x99] = &CPU::OPCode0x99;
    OPCodes[0x9A] = &CPU::OPCode0x9A;
    OPCodes[0x9B] = &CPU::OPCode0x9B;
    OPCodes[0x9C] = &CPU::OPCode0x9C;
    OPCodes[0x9D] = &CPU::OPCode0x9D;
    OPCodes[0x9E] = &CPU::OPCode0x9E;
    OPCodes[0x9F] = &CPU::OPCode0x9F;

    OPCodes[0xA0] = &CPU::OPCode0xA0;
    OPCodes[0xA1] = &CPU::OPCode0xA1;
    OPCodes[0xA2] = &CPU::OPCode0xA2;
    OPCodes[0xA3] = &CPU::OPCode0xA3;
    OPCodes[0xA4] = &CPU::OPCode0xA4;
    OPCodes[0xA5] = &CPU::OPCode0xA5;
    OPCodes[0xA6] = &CPU::OPCode0xA6;
    OPCodes[0xA7] = &CPU::OPCode0xA7;
    OPCodes[0xA8] = &CPU::OPCode0xA8;
    OPCodes[0xA9] = &CPU::OPCode0xA9;
    OPCodes[0xAA] = &CPU::OPCode0xAA;
    OPCodes[0xAB] = &CPU::OPCode0xAB;
    OPCodes[0xAC] = &CPU::OPCode0xAC;
    OPCodes[0xAD] = &CPU::OPCode0xAD;
    OPCodes[0xAE] = &CPU::OPCode0xAE;
    OPCodes[0xAF] = &CPU::OPCode0xAF;

    OPCodes[0xB0] = &CPU::OPCode0xB0;
    OPCodes[0xB1] = &CPU::OPCode0xB1;
    OPCodes[0xB2] = &CPU::OPCode0xB2;
    OPCodes[0xB3] = &CPU::OPCode0xB3;
    OPCodes[0xB4] = &CPU::OPCode0xB4;
    OPCodes[0xB5] = &CPU::OPCode0xB5;
    OPCodes[0xB6] = &CPU::OPCode0xB6;
    OPCodes[0xB7] = &CPU::OPCode0xB7;
    OPCodes[0xB8] = &CPU::OPCode0xB8;
    OPCodes[0xB9] = &CPU::OPCode0xB9;
    OPCodes[0xBA] = &CPU::OPCode0xBA;
    OPCodes[0xBB] = &CPU::OPCode0xBB;
    OPCodes[0xBC] = &CPU::OPCode0xBC;
    OPCodes[0xBD] = &CPU::OPCode0xBD;
    OPCodes[0xBE] = &CPU::OPCode0xBE;
    OPCodes[0xBF] = &CPU::OPCode0xBF;

    OPCodes[0xC0] = &CPU::OPCode0xC0;
    OPCodes[0xC1] = &CPU::OPCode0xC1;
    OPCodes[0xC2] = &CPU::OPCode0xC2;
    OPCodes[0xC3] = &CPU::OPCode0xC3;
    OPCodes[0xC4] = &CPU::OPCode0xC4;
    OPCodes[0xC5] = &CPU::OPCode0xC5;
    OPCodes[0xC6] = &CPU::OPCode0xC6;
    OPCodes[0xC7] = &CPU::OPCode0xC7;
    OPCodes[0xC8] = &CPU::OPCode0xC8;
    OPCodes[0xC9] = &CPU::OPCode0xC9;
    OPCodes[0xCA] = &CPU::OPCode0xCA;
    OPCodes[0xCB] = &CPU::OPCode0xCB;
    OPCodes[0xCC] = &CPU::OPCode0xCC;
    OPCodes[0xCD] = &CPU::OPCode0xCD;
    OPCodes[0xCE] = &CPU::OPCode0xCE;
    OPCodes[0xCF] = &CPU::OPCode0xCF;

    OPCodes[0xD0] = &CPU::OPCode0xD0;
    OPCodes[0xD1] = &CPU::OPCode0xD1;
    OPCodes[0xD2] = &CPU::OPCode0xD2;
    OPCodes[0xD3] = &CPU::OPCode0xD3;
    OPCodes[0xD4] = &CPU::OPCode0xD4;
    OPCodes[0xD5] = &CPU::OPCode0xD5;
    OPCodes[0xD6] = &CPU::OPCode0xD6;
    OPCodes[0xD7] = &CPU::OPCode0xD7;
    OPCodes[0xD8] = &CPU::OPCode0xD8;
    OPCodes[0xD9] = &CPU::OPCode0xD9;
    OPCodes[0xDA] = &CPU::OPCode0xDA;
    OPCodes[0xDB] = &CPU::OPCode0xDB;
    OPCodes[0xDC] = &CPU::OPCode0xDC;
    OPCodes[0xDD] = &CPU::OPCode0xDD;
    OPCodes[0xDE] = &CPU::OPCode0xDE;
    OPCodes[0xDF] = &CPU::OPCode0xDF;

    OPCodes[0xE0] = &CPU::OPCode0xE0;
    OPCodes[0xE1] = &CPU::OPCode0xE1;
    OPCodes[0xE2] = &CPU::OPCode0xE2;
    OPCodes[0xE3] = &CPU::OPCode0xE3;
    OPCodes[0xE4] = &CPU::OPCode0xE4;
    OPCodes[0xE5] = &CPU::OPCode0xE5;
    OPCodes[0xE6] = &CPU::OPCode0xE6;
    OPCodes[0xE7] = &CPU::OPCode0xE7;
    OPCodes[0xE8] = &CPU::OPCode0xE8;
    OPCodes[0xE9] = &CPU::OPCode0xE9;
    OPCodes[0xEA] = &CPU::OPCode0xEA;
    OPCodes[0xEB] = &CPU::OPCode0xEB;
    OPCodes[0xEC] = &CPU::OPCode0xEC;
    OPCodes[0xED] = &CPU::OPCode0xED;
    OPCodes[0xEE] = &CPU::OPCode0xEE;
    OPCodes[0xEF] = &CPU::OPCode0xEF;

    OPCodes[0xF0] = &CPU::OPCode0xF0;
    OPCodes[0xF1] = &CPU::OPCode0xF1;
    OPCodes[0xF2] = &CPU::OPCode0xF2;
    OPCodes[0xF3] = &CPU::OPCode0xF3;
    OPCodes[0xF4] = &CPU::OPCode0xF4;
    OPCodes[0xF5] = &CPU::OPCode0xF5;
    OPCodes[0xF6] = &CPU::OPCode0xF6;
    OPCodes[0xF7] = &CPU::OPCode0xF7;
    OPCodes[0xF8] = &CPU::OPCode0xF8;
    OPCodes[0xF9] = &CPU::OPCode0xF9;
    OPCodes[0xFA] = &CPU::OPCode0xFA;
    OPCodes[0xFB] = &CPU::OPCode0xFB;
    OPCodes[0xFC] = &CPU::OPCode0xFC;
    OPCodes[0xFD] = &CPU::OPCode0xFD;
    OPCodes[0xFE] = &CPU::OPCode0xFE;
    OPCodes[0xFF] = &CPU::OPCode0xFF;

    OPCodesCB[0x00] = &CPU::OPCodeCB0x00;
    OPCodesCB[0x01] = &CPU::OPCodeCB0x01;
    OPCodesCB[0x02] = &CPU::OPCodeCB0x02;
    OPCodesCB[0x03] = &CPU::OPCodeCB0x03;
    OPCodesCB[0x04] = &CPU::OPCodeCB0x04;
    OPCodesCB[0x05] = &CPU::OPCodeCB0x05;
    OPCodesCB[0x06] = &CPU::OPCodeCB0x06;
    OPCodesCB[0x07] = &CPU::OPCodeCB0x07;
    OPCodesCB[0x08] = &CPU::OPCodeCB0x08;
    OPCodesCB[0x09] = &CPU::OPCodeCB0x09;
    OPCodesCB[0x0A] = &CPU::OPCodeCB0x0A;
    OPCodesCB[0x0B] = &CPU::OPCodeCB0x0B;
    OPCodesCB[0x0C] = &CPU::OPCodeCB0x0C;
    OPCodesCB[0x0D] = &CPU::OPCodeCB0x0D;
    OPCodesCB[0x0E] = &CPU::OPCodeCB0x0E;
    OPCodesCB[0x0F] = &CPU::OPCodeCB0x0F;

    OPCodesCB[0x10] = &CPU::OPCodeCB0x10;
    OPCodesCB[0x11] = &CPU::OPCodeCB0x11;
    OPCodesCB[0x12] = &CPU::OPCodeCB0x12;
    OPCodesCB[0x13] = &CPU::OPCodeCB0x13;
    OPCodesCB[0x14] = &CPU::OPCodeCB0x14;
    OPCodesCB[0x15] = &CPU::OPCodeCB0x15;
    OPCodesCB[0x16] = &CPU::OPCodeCB0x16;
    OPCodesCB[0x17] = &CPU::OPCodeCB0x17;
    OPCodesCB[0x18] = &CPU::OPCodeCB0x18;
    OPCodesCB[0x19] = &CPU::OPCodeCB0x19;
    OPCodesCB[0x1A] = &CPU::OPCodeCB0x1A;
    OPCodesCB[0x1B] = &CPU::OPCodeCB0x1B;
    OPCodesCB[0x1C] = &CPU::OPCodeCB0x1C;
    OPCodesCB[0x1D] = &CPU::OPCodeCB0x1D;
    OPCodesCB[0x1E] = &CPU::OPCodeCB0x1E;
    OPCodesCB[0x1F] = &CPU::OPCodeCB0x1F;

    OPCodesCB[0x20] = &CPU::OPCodeCB0x20;
    OPCodesCB[0x21] = &CPU::OPCodeCB0x21;
    OPCodesCB[0x22] = &CPU::OPCodeCB0x22;
    OPCodesCB[0x23] = &CPU::OPCodeCB0x23;
    OPCodesCB[0x24] = &CPU::OPCodeCB0x24;
    OPCodesCB[0x25] = &CPU::OPCodeCB0x25;
    OPCodesCB[0x26] = &CPU::OPCodeCB0x26;
    OPCodesCB[0x27] = &CPU::OPCodeCB0x27;
    OPCodesCB[0x28] = &CPU::OPCodeCB0x28;
    OPCodesCB[0x29] = &CPU::OPCodeCB0x29;
    OPCodesCB[0x2A] = &CPU::OPCodeCB0x2A;
    OPCodesCB[0x2B] = &CPU::OPCodeCB0x2B;
    OPCodesCB[0x2C] = &CPU::OPCodeCB0x2C;
    OPCodesCB[0x2D] = &CPU::OPCodeCB0x2D;
    OPCodesCB[0x2E] = &CPU::OPCodeCB0x2E;
    OPCodesCB[0x2F] = &CPU::OPCodeCB0x2F;

    OPCodesCB[0x30] = &CPU::OPCodeCB0x30;
    OPCodesCB[0x31] = &CPU::OPCodeCB0x31;
    OPCodesCB[0x32] = &CPU::OPCodeCB0x32;
    OPCodesCB[0x33] = &CPU::OPCodeCB0x33;
    OPCodesCB[0x34] = &CPU::OPCodeCB0x34;
    OPCodesCB[0x35] = &CPU::OPCodeCB0x35;
    OPCodesCB[0x36] = &CPU::OPCodeCB0x36;
    OPCodesCB[0x37] = &CPU::OPCodeCB0x37;
    OPCodesCB[0x38] = &CPU::OPCodeCB0x38;
    OPCodesCB[0x39] = &CPU::OPCodeCB0x39;
    OPCodesCB[0x3A] = &CPU::OPCodeCB0x3A;
    OPCodesCB[0x3B] = &CPU::OPCodeCB0x3B;
    OPCodesCB[0x3C] = &CPU::OPCodeCB0x3C;
    OPCodesCB[0x3D] = &CPU::OPCodeCB0x3D;
    OPCodesCB[0x3E] = &CPU::OPCodeCB0x3E;
    OPCodesCB[0x3F] = &CPU::OPCodeCB0x3F;

    OPCodesCB[0x40] = &CPU::OPCodeCB0x40;
    OPCodesCB[0x41] = &CPU::OPCodeCB0x41;
    OPCodesCB[0x42] = &CPU::OPCodeCB0x42;
    OPCodesCB[0x43] = &CPU::OPCodeCB0x43;
    OPCodesCB[0x44] = &CPU::OPCodeCB0x44;
    OPCodesCB[0x45] = &CPU::OPCodeCB0x45;
    OPCodesCB[0x46] = &CPU::OPCodeCB0x46;
    OPCodesCB[0x47] = &CPU::OPCodeCB0x47;
    OPCodesCB[0x48] = &CPU::OPCodeCB0x48;
    OPCodesCB[0x49] = &CPU::OPCodeCB0x49;
    OPCodesCB[0x4A] = &CPU::OPCodeCB0x4A;
    OPCodesCB[0x4B] = &CPU::OPCodeCB0x4B;
    OPCodesCB[0x4C] = &CPU::OPCodeCB0x4C;
    OPCodesCB[0x4D] = &CPU::OPCodeCB0x4D;
    OPCodesCB[0x4E] = &CPU::OPCodeCB0x4E;
    OPCodesCB[0x4F] = &CPU::OPCodeCB0x4F;

    OPCodesCB[0x50] = &CPU::OPCodeCB0x50;
    OPCodesCB[0x51] = &CPU::OPCodeCB0x51;
    OPCodesCB[0x52] = &CPU::OPCodeCB0x52;
    OPCodesCB[0x53] = &CPU::OPCodeCB0x53;
    OPCodesCB[0x54] = &CPU::OPCodeCB0x54;
    OPCodesCB[0x55] = &CPU::OPCodeCB0x55;
    OPCodesCB[0x56] = &CPU::OPCodeCB0x56;
    OPCodesCB[0x57] = &CPU::OPCodeCB0x57;
    OPCodesCB[0x58] = &CPU::OPCodeCB0x58;
    OPCodesCB[0x59] = &CPU::OPCodeCB0x59;
    OPCodesCB[0x5A] = &CPU::OPCodeCB0x5A;
    OPCodesCB[0x5B] = &CPU::OPCodeCB0x5B;
    OPCodesCB[0x5C] = &CPU::OPCodeCB0x5C;
    OPCodesCB[0x5D] = &CPU::OPCodeCB0x5D;
    OPCodesCB[0x5E] = &CPU::OPCodeCB0x5E;
    OPCodesCB[0x5F] = &CPU::OPCodeCB0x5F;

    OPCodesCB[0x60] = &CPU::OPCodeCB0x60;
    OPCodesCB[0x61] = &CPU::OPCodeCB0x61;
    OPCodesCB[0x62] = &CPU::OPCodeCB0x62;
    OPCodesCB[0x63] = &CPU::OPCodeCB0x63;
    OPCodesCB[0x64] = &CPU::OPCodeCB0x64;
    OPCodesCB[0x65] = &CPU::OPCodeCB0x65;
    OPCodesCB[0x66] = &CPU::OPCodeCB0x66;
    OPCodesCB[0x67] = &CPU::OPCodeCB0x67;
    OPCodesCB[0x68] = &CPU::OPCodeCB0x68;
    OPCodesCB[0x69] = &CPU::OPCodeCB0x69;
    OPCodesCB[0x6A] = &CPU::OPCodeCB0x6A;
    OPCodesCB[0x6B] = &CPU::OPCodeCB0x6B;
    OPCodesCB[0x6C] = &CPU::OPCodeCB0x6C;
    OPCodesCB[0x6D] = &CPU::OPCodeCB0x6D;
    OPCodesCB[0x6E] = &CPU::OPCodeCB0x6E;
    OPCodesCB[0x6F] = &CPU::OPCodeCB0x6F;

    OPCodesCB[0x70] = &CPU::OPCodeCB0x70;
    OPCodesCB[0x71] = &CPU::OPCodeCB0x71;
    OPCodesCB[0x72] = &CPU::OPCodeCB0x72;
    OPCodesCB[0x73] = &CPU::OPCodeCB0x73;
    OPCodesCB[0x74] = &CPU::OPCodeCB0x74;
    OPCodesCB[0x75] = &CPU::OPCodeCB0x75;
    OPCodesCB[0x76] = &CPU::OPCodeCB0x76;
    OPCodesCB[0x77] = &CPU::OPCodeCB0x77;
    OPCodesCB[0x78] = &CPU::OPCodeCB0x78;
    OPCodesCB[0x79] = &CPU::OPCodeCB0x79;
    OPCodesCB[0x7A] = &CPU::OPCodeCB0x7A;
    OPCodesCB[0x7B] = &CPU::OPCodeCB0x7B;
    OPCodesCB[0x7C] = &CPU::OPCodeCB0x7C;
    OPCodesCB[0x7D] = &CPU::OPCodeCB0x7D;
    OPCodesCB[0x7E] = &CPU::OPCodeCB0x7E;
    OPCodesCB[0x7F] = &CPU::OPCodeCB0x7F;

    OPCodesCB[0x80] = &CPU::OPCodeCB0x80;
    OPCodesCB[0x81] = &CPU::OPCodeCB0x81;
    OPCodesCB[0x82] = &CPU::OPCodeCB0x82;
    OPCodesCB[0x83] = &CPU::OPCodeCB0x83;
    OPCodesCB[0x84] = &CPU::OPCodeCB0x84;
    OPCodesCB[0x85] = &CPU::OPCodeCB0x85;
    OPCodesCB[0x86] = &CPU::OPCodeCB0x86;
    OPCodesCB[0x87] = &CPU::OPCodeCB0x87;
    OPCodesCB[0x88] = &CPU::OPCodeCB0x88;
    OPCodesCB[0x89] = &CPU::OPCodeCB0x89;
    OPCodesCB[0x8A] = &CPU::OPCodeCB0x8A;
    OPCodesCB[0x8B] = &CPU::OPCodeCB0x8B;
    OPCodesCB[0x8C] = &CPU::OPCodeCB0x8C;
    OPCodesCB[0x8D] = &CPU::OPCodeCB0x8D;
    OPCodesCB[0x8E] = &CPU::OPCodeCB0x8E;
    OPCodesCB[0x8F] = &CPU::OPCodeCB0x8F;

    OPCodesCB[0x90] = &CPU::OPCodeCB0x90;
    OPCodesCB[0x91] = &CPU::OPCodeCB0x91;
    OPCodesCB[0x92] = &CPU::OPCodeCB0x92;
    OPCodesCB[0x93] = &CPU::OPCodeCB0x93;
    OPCodesCB[0x94] = &CPU::OPCodeCB0x94;
    OPCodesCB[0x95] = &CPU::OPCodeCB0x95;
    OPCodesCB[0x96] = &CPU::OPCodeCB0x96;
    OPCodesCB[0x97] = &CPU::OPCodeCB0x97;
    OPCodesCB[0x98] = &CPU::OPCodeCB0x98;
    OPCodesCB[0x99] = &CPU::OPCodeCB0x99;
    OPCodesCB[0x9A] = &CPU::OPCodeCB0x9A;
    OPCodesCB[0x9B] = &CPU::OPCodeCB0x9B;
    OPCodesCB[0x9C] = &CPU::OPCodeCB0x9C;
    OPCodesCB[0x9D] = &CPU::OPCodeCB0x9D;
    OPCodesCB[0x9E] = &CPU::OPCodeCB0x9E;
    OPCodesCB[0x9F] = &CPU::OPCodeCB0x9F;

    OPCodesCB[0xA0] = &CPU::OPCodeCB0xA0;
    OPCodesCB[0xA1] = &CPU::OPCodeCB0xA1;
    OPCodesCB[0xA2] = &CPU::OPCodeCB0xA2;
    OPCodesCB[0xA3] = &CPU::OPCodeCB0xA3;
    OPCodesCB[0xA4] = &CPU::OPCodeCB0xA4;
    OPCodesCB[0xA5] = &CPU::OPCodeCB0xA5;
    OPCodesCB[0xA6] = &CPU::OPCodeCB0xA6;
    OPCodesCB[0xA7] = &CPU::OPCodeCB0xA7;
    OPCodesCB[0xA8] = &CPU::OPCodeCB0xA8;
    OPCodesCB[0xA9] = &CPU::OPCodeCB0xA9;
    OPCodesCB[0xAA] = &CPU::OPCodeCB0xAA;
    OPCodesCB[0xAB] = &CPU::OPCodeCB0xAB;
    OPCodesCB[0xAC] = &CPU::OPCodeCB0xAC;
    OPCodesCB[0xAD] = &CPU::OPCodeCB0xAD;
    OPCodesCB[0xAE] = &CPU::OPCodeCB0xAE;
    OPCodesCB[0xAF] = &CPU::OPCodeCB0xAF;

    OPCodesCB[0xB0] = &CPU::OPCodeCB0xB0;
    OPCodesCB[0xB1] = &CPU::OPCodeCB0xB1;
    OPCodesCB[0xB2] = &CPU::OPCodeCB0xB2;
    OPCodesCB[0xB3] = &CPU::OPCodeCB0xB3;
    OPCodesCB[0xB4] = &CPU::OPCodeCB0xB4;
    OPCodesCB[0xB5] = &CPU::OPCodeCB0xB5;
    OPCodesCB[0xB6] = &CPU::OPCodeCB0xB6;
    OPCodesCB[0xB7] = &CPU::OPCodeCB0xB7;
    OPCodesCB[0xB8] = &CPU::OPCodeCB0xB8;
    OPCodesCB[0xB9] = &CPU::OPCodeCB0xB9;
    OPCodesCB[0xBA] = &CPU::OPCodeCB0xBA;
    OPCodesCB[0xBB] = &CPU::OPCodeCB0xBB;
    OPCodesCB[0xBC] = &CPU::OPCodeCB0xBC;
    OPCodesCB[0xBD] = &CPU::OPCodeCB0xBD;
    OPCodesCB[0xBE] = &CPU::OPCodeCB0xBE;
    OPCodesCB[0xBF] = &CPU::OPCodeCB0xBF;

    OPCodesCB[0xC0] = &CPU::OPCodeCB0xC0;
    OPCodesCB[0xC1] = &CPU::OPCodeCB0xC1;
    OPCodesCB[0xC2] = &CPU::OPCodeCB0xC2;
    OPCodesCB[0xC3] = &CPU::OPCodeCB0xC3;
    OPCodesCB[0xC4] = &CPU::OPCodeCB0xC4;
    OPCodesCB[0xC5] = &CPU::OPCodeCB0xC5;
    OPCodesCB[0xC6] = &CPU::OPCodeCB0xC6;
    OPCodesCB[0xC7] = &CPU::OPCodeCB0xC7;
    OPCodesCB[0xC8] = &CPU::OPCodeCB0xC8;
    OPCodesCB[0xC9] = &CPU::OPCodeCB0xC9;
    OPCodesCB[0xCA] = &CPU::OPCodeCB0xCA;
    OPCodesCB[0xCB] = &CPU::OPCodeCB0xCB;
    OPCodesCB[0xCC] = &CPU::OPCodeCB0xCC;
    OPCodesCB[0xCD] = &CPU::OPCodeCB0xCD;
    OPCodesCB[0xCE] = &CPU::OPCodeCB0xCE;
    OPCodesCB[0xCF] = &CPU::OPCodeCB0xCF;

    OPCodesCB[0xD0] = &CPU::OPCodeCB0xD0;
    OPCodesCB[0xD1] = &CPU::OPCodeCB0xD1;
    OPCodesCB[0xD2] = &CPU::OPCodeCB0xD2;
    OPCodesCB[0xD3] = &CPU::OPCodeCB0xD3;
    OPCodesCB[0xD4] = &CPU::OPCodeCB0xD4;
    OPCodesCB[0xD5] = &CPU::OPCodeCB0xD5;
    OPCodesCB[0xD6] = &CPU::OPCodeCB0xD6;
    OPCodesCB[0xD7] = &CPU::OPCodeCB0xD7;
    OPCodesCB[0xD8] = &CPU::OPCodeCB0xD8;
    OPCodesCB[0xD9] = &CPU::OPCodeCB0xD9;
    OPCodesCB[0xDA] = &CPU::OPCodeCB0xDA;
    OPCodesCB[0xDB] = &CPU::OPCodeCB0xDB;
    OPCodesCB[0xDC] = &CPU::OPCodeCB0xDC;
    OPCodesCB[0xDD] = &CPU::OPCodeCB0xDD;
    OPCodesCB[0xDE] = &CPU::OPCodeCB0xDE;
    OPCodesCB[0xDF] = &CPU::OPCodeCB0xDF;

    OPCodesCB[0xE0] = &CPU::OPCodeCB0xE0;
    OPCodesCB[0xE1] = &CPU::OPCodeCB0xE1;
    OPCodesCB[0xE2] = &CPU::OPCodeCB0xE2;
    OPCodesCB[0xE3] = &CPU::OPCodeCB0xE3;
    OPCodesCB[0xE4] = &CPU::OPCodeCB0xE4;
    OPCodesCB[0xE5] = &CPU::OPCodeCB0xE5;
    OPCodesCB[0xE6] = &CPU::OPCodeCB0xE6;
    OPCodesCB[0xE7] = &CPU::OPCodeCB0xE7;
    OPCodesCB[0xE8] = &CPU::OPCodeCB0xE8;
    OPCodesCB[0xE9] = &CPU::OPCodeCB0xE9;
    OPCodesCB[0xEA] = &CPU::OPCodeCB0xEA;
    OPCodesCB[0xEB] = &CPU::OPCodeCB0xEB;
    OPCodesCB[0xEC] = &CPU::OPCodeCB0xEC;
    OPCodesCB[0xED] = &CPU::OPCodeCB0xED;
    OPCodesCB[0xEE] = &CPU::OPCodeCB0xEE;
    OPCodesCB[0xEF] = &CPU::OPCodeCB0xEF;

    OPCodesCB[0xF0] = &CPU::OPCodeCB0xF0;
    OPCodesCB[0xF1] = &CPU::OPCodeCB0xF1;
    OPCodesCB[0xF2] = &CPU::OPCodeCB0xF2;
    OPCodesCB[0xF3] = &CPU::OPCodeCB0xF3;
    OPCodesCB[0xF4] = &CPU::OPCodeCB0xF4;
    OPCodesCB[0xF5] = &CPU::OPCodeCB0xF5;
    OPCodesCB[0xF6] = &CPU::OPCodeCB0xF6;
    OPCodesCB[0xF7] = &CPU::OPCodeCB0xF7;
    OPCodesCB[0xF8] = &CPU::OPCodeCB0xF8;
    OPCodesCB[0xF9] = &CPU::OPCodeCB0xF9;
    OPCodesCB[0xFA] = &CPU::OPCodeCB0xFA;
    OPCodesCB[0xFB] = &CPU::OPCodeCB0xFB;
    OPCodesCB[0xFC] = &CPU::OPCodeCB0xFC;
    OPCodesCB[0xFD] = &CPU::OPCodeCB0xFD;
    OPCodesCB[0xFE] = &CPU::OPCodeCB0xFE;
    OPCodesCB[0xFF] = &CPU::OPCodeCB0xFF;
}
