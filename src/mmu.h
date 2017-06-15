#ifndef MMU_H
#define MMU_H

#include "address.h"

class MMU {
public:
  void writeByte(const Address& address, u8 byte);
  u8 readByte(const Address& address);
private:
};

#endif
