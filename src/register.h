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
    /* Spacialized behaviour for flag register "F"
    *      * (its lower nibble is always 0s */
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

class IWordValue {
public:
    virtual ~IWordValue() = default;

    virtual void set(u16 newVal) = 0;

    virtual u16 getValue() const = 0;

    virtual u8 low() const = 0;
    virtual u8 high() const = 0;
};

class WordRegister : public IWordValue {
public:
    WordRegister() = default;
    WordRegister(const WordRegister& wr) = delete;
    WordRegister& operator=(const WordRegister& wr) = delete;

    void set(u16 newVal) override;

    u16 getValue() const override;

    u8 low() const override;
    u8 high() const override;

    void increment();
    void decrement();

private:
    u16 val;
};

class RegisterPair : public IWordValue { //HL, BC, CB, DE, etc..
public:
    RegisterPair() = default;
    RegisterPair(const RegisterPair& rp) = delete;
    RegisterPair& operator=(const RegisterPair& rp) = delete;

    RegisterPair(ByteRegister& high, ByteRegister& low);

    void set(u16 newVal) override;

    u16 getValue() const override;

    u8 low() const override;
    u8 high() const override;

    void increment();
    void decrement();

private:
    ByteRegister& lowByte;
    ByteRegister& highByte;
};

#endif 
