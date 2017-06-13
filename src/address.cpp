#include "address.h"

Address::Address(u16 location) : address(location) 
{
}

Address::Address(const WordRegister& wr) : address(wr.getValue()) 
{
}

u16 Address::getValue() const {
    return address;
}

bool Address::isInRange(const Address& low, const Address& high) const {
    return low.getValue() <= this->getValue() 
      && this->getValue() <= high.getValue();
}

bool Address::operator==(u16 other) const {
    return address == other;
}

Address Address::operator+(u32 other) const {
    u16 new_addr = static_cast<u16>(address + other);
        return Address(new_addr);
}

Address Address::operator-(u32 other) const {
    u16 new_addr = static_cast<u16>(address - other);
        return Address(new_addr);
}
