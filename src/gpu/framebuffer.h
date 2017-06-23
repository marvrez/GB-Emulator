#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../util.h"

#include <vector>


class FrameBuffer {
public:
    FrameBuffer(u32 width, u32 height);

    void setPixel(u32 x, u32 y, Color color);
    Color getPixel(u32 x, u32 y) const;

    static const u32 FRAMEBUFFER_SIZE = 255;
private:
    u32 width, height;

    std::vector<std::vector<Color> > buffer;
};

#endif
