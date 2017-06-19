#ifndef MBC_H
#define MBC_H

#include "util.h"
#include "address.h"

#include <string>
#include <vector>
#include <ctime>


class MBC {
public:
    virtual ~MBC() {}
    virtual u8 readROM(const Address& address) = 0;
    virtual u8 readRAM(const Address& address) = 0;
    virtual void writeROM(const Address& address, u8 value) = 0;
    virtual void writeRAM(const Address& address, u8 value) = 0;
    virtual void save(std::string) {}
    virtual void load(std::string) {}
};

class MBC0 : public MBC {
public:
    MBC0(const std::vector<u8> &rom);
    u8 readROM(const Address& address);
    u8 readRAM(const Address& address);
    void writeROM(const Address& address, u8 value);
    void writeRAM(const Address& address, u8 value);
protected:
    std::vector<u8> rom, ram;
};

class MBC1 : public MBC {
public:
    MBC1(const std::vector<u8> &rom);
    u8 readROM(const Address& address);
    u8 readRAM(const Address& address);
    void writeROM(const Address& address, u8 value);
    void writeRAM(const Address& address, u8 value);
protected:
    std::vector<u8> rom, ram;
    int rombank;
    int rambank;
    bool extram_on;
    bool ram_mode;
};

class MBC3 : public MBC {
public:
    MBC3(const std::vector<u8> &rom);
    u8 readROM(const Address& address);
    u8 readRAM(const Address& address);
    void writeROM(const Address& address, u8 value);
    void writeRAM(const Address& address, u8 value);
    void save(std::string filename);
    void load(std::string filename);
protected:
    std::vector<u8> rom, ram, rtc;
    int rombank;
    int rambank;
    bool extram_on;
    bool rtc_lock;
    time_t rtczero;

    void calc_rtczero();
    void calc_rtcregs();
};

class MBC5 : public MBC {
public:
    MBC5(const std::vector<u8> &rom);
    u8 readROM(const Address& address);
    u8 readRAM(const Address& address);
    void writeROM(const Address& address, u8 value);
    void writeRAM(const Address& address, u8 value);
    void save(std::string filename);
    void load(std::string filename);
protected:
    std::vector<u8> rom, ram;
    int rombank;
    int rambank;
    bool extram_on;
};

#endif
