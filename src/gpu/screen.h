#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.hpp>

#include "framebuffer.h"

class MMU;
class CPU;

class Screen {
public:
    virtual ~Screen() = default;

    virtual void draw(const FrameBuffer& buffer, u32 scrollx, u32 scrolly, const BGPalette& bgPalette) = 0;
    virtual bool isOpen() = 0;
};

class SFMLScreen : public Screen {

};

#endif
