#ifndef SCREEN_H
#define SCREEN_H

#include "framebuffer.h"

#include <string>

#include <SFML/Graphics.hpp>

class MMU;
class CPU;

class Screen {
public:
    Screen(std::string gametitle, u32 magnification = 5);
    ~Screen() = default;

    void draw(const FrameBuffer& buffer, u32 scrollX, u32 scrollY, const BGPalette& bgPalette) ;
    bool isOpen();

private:
    const u32 magnification;

    const u32 logicalWidth, logicalHeight;
    const u32 width, height;
    const u32 pixelSize;

    sf::RenderWindow renderWindow;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    void setPixelsOnImage(const FrameBuffer& buffer, u32 scrollX, u32 scrollY, const BGPalette& bgPalette);
    void setLargePixel(u32 x, u32 y, sf::Color color);

    sf::Color getSFMLColor(Color color);


};

#endif
