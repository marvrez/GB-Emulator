#ifndef REGISTER_H
#define REGISTER_H

#include "util.h"

class ByteRegister {
public:
  ByteRegister() = default;
  ByteRegister(const ByteRegister& br) = delete;
  ByteRegister& operator=(const ByteRegister& br) = delete;

  virtual ~ByteRegister() = default;

  virtual void set(u8 newVal);
  void reset();
  u8 getValue() const;

  bool checkBit(u8 bit) const;
  void setBitTo(u8 bit, bool set);

  void increment();
  void decrement();

  bool operator==(const u8 other) const;

protected:
  u8 val;
};

class FlagRegister : public ByteRegister {
public:
    //Spacialized behaviour for flag register "F", bits 0-3 are unused
    void set(u8 newVal) override;

    //bits 0-3 are unused
    void setFlagZero(bool set);
    void setFlagSubtract(bool set);
    void setFlagHalfCarry(bool set);
    void setFlagCarry(bool set);

    bool checkFlagZero() const;
    bool checkFlagSubtract() const;
    bool checkFlagHalfCarry() const;
    bool checkFlagCarry() const;

    u8 getFlagZero() const;
    u8 getFlagSubtract() const;
    u8 getFlagHalfCarry() const;
    u8 getFlagCarry() const;
};

class WordRegister {
public:
    WordRegister() = default;
    WordRegister(const WordRegister& wr) = delete;
    WordRegister& operator=(const WordRegister& wr) = delete;

    WordRegister(ByteRegister& high, ByteRegister& low);

    void set(u16 newVal);

    u16 getValue() const;

    u8 getLowValue() const ;
    u8 getHighValue() const;

    void increment();
    void decrement();

private:
    union {
        u16 raw;
        struct { u8 low, high; };
    } val;
};

using RegisterPair = WordRegister;


#endif 
