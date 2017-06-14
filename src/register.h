#ifndef REGISTER_H
#define REGISTER_H

#include "util.h"

class ByteRegister {
public:
  ByteRegister() = default;
  ByteRegister(const ByteRegister& br) = delete;
  ByteRegister& operator=(const ByteRegister& br) = delete;

  virtual ~ByteRegister() = default;

  virtual void setValue(u8 newVal);
  void reset();
  u8 getValue() const;

  bool checkBit(u8 bit) const;
  void setBitTo(u8 bit, bool setValue);

  void increment();
  void decrement();

  bool operator==(const u8 other) const;

protected:
  u8 val = 0;
};

class FlagRegister : public ByteRegister {
public:
    //Spacialized behaviour for flag register "F", bits 0-3 are unused
    void setValue(u8 newVal) override;

    //bits 0-3 are unused
    void setFlagZero(bool setValue);
    void setFlagSubtract(bool setValue);
    void setFlagHalfCarry(bool setValue);
    void setFlagCarry(bool setValue);

    bool checkFlagZero() const;
    bool checkFlagSubtract() const;
    bool checkFlagHalfCarry() const;
    bool checkFlagCarry() const;

    u8 getFlagZero() const;
    u8 getFlagSubtract() const;
    u8 getFlagHalfCarry() const;
    u8 getFlagCarry() const;
};

class IWordRegister {
public:
    IWordRegister() = default;
    IWordRegister(const IWordRegister&) = delete;
    IWordRegister& operator=(const IWordRegister&) = delete;
    virtual ~IWordRegister() = default;

    virtual void setValue(u16 newVal) = 0;

    virtual u16 getValue() const = 0;

    virtual u8 getLowValue() const = 0;
    virtual u8 getHighValue() const = 0;

    virtual void increment() = 0;
    virtual void decrement() = 0;
};

class WordRegister : public IWordRegister {
public:
    void setValue(u16 newVal) override;

    u16 getValue() const override;

    u8 getLowValue() const override;
    u8 getHighValue() const override;

    void increment() override;
    void decrement() override;

private:
    u16 val;
};

class RegisterPair : public IWordRegister { //HL, BC, CB, DE, etc..
public:
    RegisterPair() = default;
    RegisterPair(const RegisterPair& rp) = delete;
    RegisterPair& operator=(const RegisterPair& rp) = delete;

    RegisterPair(ByteRegister* high, ByteRegister* low);

    void setValue(u16 newVal) override;

    u16 getValue() const override;

    u8 getLowValue() const override;
    u8 getHighValue() const override;

    ByteRegister* getHighRegister() const;
    ByteRegister* getLowRegister() const;

    void increment() override;
    void decrement() override;

private:
    ByteRegister* lowByte;
    ByteRegister* highByte;
};

#endif
