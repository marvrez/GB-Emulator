#include "framebuffer.h"


FrameBuffer::FrameBuffer(u32 width, u32 height) :
    width(width),
    height(height),
    buffer(height,std::vector<Color>(width, Color::White))
{
}

void FrameBuffer::setPixel(u32 x, u32 y, Color color) {
    buffer[y][x] = color;
}

Color FrameBuffer::getPixel(u32 x, u32 y) const {
    return buffer[y][x];
}


