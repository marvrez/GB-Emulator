#ifndef ADDRESS_H
#define ADDRESS_H

#include "register.h"
#include "util.h"

class Address {
public:
    Address(u16 location);
    explicit Address(const WordRegister& wr);

    u16 getValue() const;

    bool isInRange(const Address& low, const Address& high) const;

    bool operator==(u16 other) const;
    Address operator+(u32 other) const;
    Address operator-(u32 other) const;

private:
    u16 address;
};

#endif
