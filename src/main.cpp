#include <iostream>
#include <cstdio>

#include "incl/color_utils.h"
#include "register.h"
#include "cpu/alu.h"

int main(int argc, char** argv)
{
    if(argc < 2) {
        std::cout << BOLDRED << "[ERROR]: " << RESET;
        std::cout << "Please provide a ROM file to run" << std::endl;
        return 0;
    }

    ByteRegister A, H, L;
    FlagRegister F;
    RegisterPair HL(&H, &L);

    ALU alu(&A, &F, &HL);

    alu.add(A.getValue(),0xFE);

    printf("ZERO: %d HALF-CARRY: %d CARRY: %d SUBTRACT: %d\n", F.getFlagZero(), F.getFlagHalfCarry(), F.getFlagCarry(), F.getFlagSubtract());
    printf("A-VALUE: 0x%X\n", A.getValue());

    alu.sub(0xFF);
    printf("ZERO: %d HALF-CARRY: %d CARRY: %d SUBTRACT: %d\n", F.getFlagZero(), F.getFlagHalfCarry(), F.getFlagCarry(), F.getFlagSubtract());
    printf("A-VALUE: 0x%X\n", A.getValue());

    alu.add(A.getValue(), 0x01);
    printf("ZERO: %d HALF-CARRY: %d CARRY: %d SUBTRACT: %d\n", F.getFlagZero(), F.getFlagHalfCarry(), F.getFlagCarry(), F.getFlagSubtract());
    printf("A-VALUE: 0x%X\n", A.getValue());


    return 0;
}
