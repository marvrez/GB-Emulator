#ifndef TILE_H
#define TILE_H

#include "../address.h"
#include "../util.h"
#include "../mmu.h"

const uint TILES_PER_LINE = 32;
const uint TILE_HEIGHT_PX = 8;
const uint TILE_WIDTH_PX = 8;

const Address TILE_SET_ZERO_LOCATION = 0x8000;
const Address TILE_SET_ONE_LOCATION = 0x8800;

const Address TILE_MAP_ZERO_LOCATION = 0x9800;
const Address TILE_MAP_ONE_LOCATION = 0x9C00;

/* A single tile contains 8 lines, each of which is two bytes */
const uint TILES_BYTES = 2 * 8;

class Tile {
public:
    Tile(Address& address, MMU* mmu);

    GBColor get_pixel(u32 x, u32 y) const;

private:
    static uint pixel_index(u32 x, u32 y);
    std::vector<u8> get_pixel_line(u8 byte1, u8 byte2) const;

    GBColor buffer[TILE_HEIGHT_PX * TILE_WIDTH_PX];
};

#endif
