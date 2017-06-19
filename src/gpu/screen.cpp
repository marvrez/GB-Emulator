#include "screen.h"

Screen::Screen(std::string gametitle, bool wholeFramebuffer, u32 magnification) :
    magnification(magnification),
    wholeFramebuffer(wholeFramebuffer),
    logicalHeight(wholeFramebuffer ? FrameBuffer::FRAMEBUFFER_SIZE : GAMEBOY_HEIGHT),
    logicalWidth(wholeFramebuffer ? FrameBuffer::FRAMEBUFFER_SIZE : GAMEBOY_WIDTH),
    width(logicalWidth*magnification),
    height(logicalHeight*magnification),
    pixelSize(magnification),
    renderWindow(sf::VideoMode(width, height), gametitle, sf::Style::Titlebar | sf::Style::Close)
{
    renderWindow.setFramerateLimit(60);
    renderWindow.setVerticalSyncEnabled(60);

    image.create(width, height);
}

void Screen::draw(const FrameBuffer &buffer, u32 scrollX, u32 scrollY, const BGPalette &bgPalette) {
    renderWindow.clear(sf::Color::White);

    sf::Event event;

    while(renderWindow.pollEvent(event)) {
        if(event.type == sf::Event::Closed)
            renderWindow.close();
    }

    setPixelsOnImage(buffer, scrollX, scrollY, bgPalette);
    texture.loadFromImage(image);
    sprite.setTexture(texture, true);
    renderWindow.draw(sprite);

    renderWindow.display();
}

bool Screen::isOpen() {
    return renderWindow.isOpen();
}

void Screen::setPixelsOnImage(const FrameBuffer& buffer, u32 scrollX, u32 scrollY, const BGPalette& bgPalette) {
    for (u32 y = 0; y < logicalHeight; ++y) {
         for (u32 x = 0; x < logicalWidth; ++x) {
             u32 bufferY = wholeFramebuffer ? y : (scrollY + y) % FrameBuffer::FRAMEBUFFER_SIZE;
             u32 bufferX = wholeFramebuffer ? x : (scrollX + x) % FrameBuffer::FRAMEBUFFER_SIZE;

             Color color = getColor(buffer.getPixel(bufferX, bufferY), bgPalette);
             this->setLargePixel(x, y, getSFColor(color));
         }
     }
}

void Screen::setLargePixel(u32 x, u32 y, sf::Color color) {
    for(u32 h = 0; h < pixelSize; ++h)
        for(u32 w = 0; w < pixelSize; ++w)
            image.setPixel((x*pixelSize) + w, (y*pixelSize) + h, color);
}

Color Screen::getColor(GBColor color, const BGPalette& bgPalette) {
    switch (color) {
        case GBColor::Color0: return bgPalette.color0;
        case GBColor::Color1: return bgPalette.color1;
        case GBColor::Color2: return bgPalette.color2;
        case GBColor::Color3: return bgPalette.color3;
    }
}

sf::Color Screen::getSFColor(Color color) {
    switch (color) {
        case Color::White: 		return sf::Color::White;
        case Color::LightGray: 	return sf::Color(170, 170, 170);
        case Color::DarkGray: 	return sf::Color(85, 85, 85);
        case Color::Black: 		return sf::Color::Black;
    }
}
