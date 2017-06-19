#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../util.h"

#include <vector>

class FrameBuffer {
public:
    FrameBuffer(u32 width, u32 height);

    void setPixel(u32 x, u32 y, GBColor color);
    GBColor getPixel(u32 x, u32 y) const;
private:
    u32 width, height;

    std::vector<std::vector<GBColor> > buffer;
};

#endif
