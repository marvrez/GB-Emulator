#ifndef UTIL_H
#define UTIL_H

#include <cstdint>

using u8  =  uint8_t;
using u16 =  uint16_t;
using u32 =  uint32_t;
using u64 =  uint64_t;

using s8  =  int8_t;
using s16 =  int16_t;
using s32 =  int32_t;
using s64 =  int64_t;

static const int GAMEBOY_WIDTH = 160;
static const int GAMEBOY_HEIGHT = 144;

static const int CLOCK_RATE = 4194304;

enum class GBColor {
    Color0,
    Color1,
    Color2,
    Color3,
};

enum class Color { //real colors
    White,
    LightGray,
    DarkGray,
    Black,
};

struct BGPalette {
    Color color0 = Color::White, //just default initializing some values..
          color1 = Color::LightGray,
          color2 = Color::DarkGray,
          color3 = Color::Black;
};

class Cycles {
public:
    Cycles(u32 nCycles) : cycles(nCycles) {}

    const u32 cycles;
};


//BIT-FUNCTIONS

namespace BitOperations {

static inline u8 composeBits(const u8 high, const u8 low) {
    return static_cast<u8>(high << 1 | low);
}

static inline u8 composeNibbles(const u8 high, const u8 low) {
    return static_cast<u8>(high << 4 | low);
}

static inline u16 composeBytes(const u8 high, const u8 low) {
    return static_cast<u16>((high << 8) | low);
}

static inline bool checkBit(const u8 value, const u8 bit) {
    return (value & (1 << bit)) != 0;
}

static inline u8 bitValue(const u8 value, const u8 bit) {
    return (value >> bit) & 1;
}

static inline u8 setBit(const u8 value, const u8 bit) {
    u8 value_set = value | (1 << bit);
    return value_set;
}

static inline u8 clearBit(const u8 value, const u8 bit) {
        auto value_cleared = value & ~(1 << bit);
            return static_cast<u8>(value_cleared);
}

static inline u8 setBitTo(const u8 value, const u8 bit, bool bit_on) {
    return bit_on ? setBit(value, bit)
                  : clearBit(value, bit);
}
} //namespace BitOperations

#endif
