#include "tile.h"

using BitOperations::bitValue;

GBColor get_color(u8 pixel_value) {
    switch (pixel_value) {
        case 0: return GBColor::Color0;
        case 1: return GBColor::Color1;
        case 2: return GBColor::Color2;
        case 3: return GBColor::Color3;
    }
}

Tile::Tile(Address& tile_address, MMU* mmu) {
    /* Set the whole framebuffer to be black */
    for (uint x = 0; x < TILE_WIDTH_PX; x++)
        for (uint y = 0; y < TILE_HEIGHT_PX; y++)
            buffer[pixel_index(x, y)] = GBColor::Color0;

    for (uint tile_line = 0; tile_line < TILE_HEIGHT_PX; tile_line++) {
        /* 2 (bytes per line of pixels) * y (lines) */
        uint index_into_tile = 2 * tile_line;
        Address line_start = tile_address + index_into_tile;

        u8 pixels_1 = mmu->readByte(line_start);
        u8 pixels_2 = mmu->readByte(line_start + 1);

        std::vector<u8> pixel_line = get_pixel_line(pixels_1, pixels_2);

        for (uint x = 0; x < TILE_WIDTH_PX; x++) {
            buffer[pixel_index(x, tile_line)] = get_color(pixel_line[x]);
        }
    }
}

GBColor Tile::get_pixel(uint x, uint y) const {
    return buffer[pixel_index(x, y)];
}

std::vector<u8> Tile::get_pixel_line(u8 byte1, u8 byte2) const {
    using BitOperations::bitValue;

    std::vector<u8> pixel_line;
    for (u8 i = 0; i < 8; i++) {
        u8 color_value = static_cast<u8>((bitValue(byte2, 7-i) << 1) | bitValue(byte1, 7-i));
        pixel_line.push_back(color_value);
    }

    return pixel_line;
}

inline uint Tile::pixel_index(uint x, uint y) {
    return (y * TILE_HEIGHT_PX) + x;
}
