#ifndef SCREEN_H
#define SCREEN_H

#include "framebuffer.h"
#include "../keypad.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <memory>

class Screen {
public:
    Screen(/*std::shared_ptr<Keypad>& keypad,*/ std::string gametitle, u32 magnification = 5);
    ~Screen() = default;

    void draw(const FrameBuffer& buffer, u32 scrollX, u32 scrollY, const BGPalette& bgPalette) ;
    bool isOpen();
    void setKeypad(std::shared_ptr<Keypad> keypad);

private:
    std::shared_ptr<Keypad> keypad;
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

    KeypadKey SFKeyToGBKey(sf::Keyboard::Key key);

    sf::Color getSFMLColor(Color color);
};

#endif
