#include "framebuffer.h"


FrameBuffer::FrameBuffer(u32 width, u32 height) :
    width(width),
    height(height),
    buffer(height,std::vector<GBColor>(width, GBColor::Color0))
{
}

void FrameBuffer::setPixel(u32 x, u32 y, GBColor color) {
    buffer[y][x] = color;
}

GBColor FrameBuffer::getPixel(u32 x, u32 y) const {
    return buffer[y][x];
}


