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
    White,
    LightGray,
    DarkGray,
    Black,
};

enum class Color {
    White,
    LightGray,
    DarkGray,
    Black,
};

struct BGPalette {
    Color White = Color::White,
          LightGray = Color::LightGray,
          DarkGray = Color::DarkGray,
          Black = Color::Black;
};

class Cycles {
public:
    Cycles(u32 nCycles) : cycles(nCycles) {}

    const u32 cycles;
};

#endif
