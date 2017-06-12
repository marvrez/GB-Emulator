#include "address.h"

Address::Address(u16 location) : addr(location) 
{
}

Address::Address(const RegisterPair& rp) : addr(rp.getValue()) 
{
}

Address::Address(const WordRegister& wr) : addr(wr.getValue()) 
{
}

u16 Address::getValue() const {
    return addr;
}

bool Address::isInRange(const Address& low, const Address& high) const {
    return low.getValue() <= this->getValue() 
      && this->getValue() <= high.getValue();
}

bool Address::operator==(u16 other) const {
    return addr == other;
}

Address Address::operator+(u32 other) const {
    u16 new_addr = static_cast<u16>(addr + other);
        return Address(new_addr);
}

Address Address::operator-(u32 other) const {
    u16 new_addr = static_cast<u16>(addr - other);
        return Address(new_addr);
}
