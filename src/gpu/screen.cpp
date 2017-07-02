#include "screen.h"

Screen::Screen(/*std::shared_ptr<Keypad>& keypad,*/ std::string gametitle, u32 magnification) :
    //keypad(keypad),
    magnification(magnification),
    logicalHeight(GAMEBOY_HEIGHT),
    logicalWidth(GAMEBOY_WIDTH),
    width(logicalWidth * magnification),
    height(logicalHeight * magnification),
    pixelSize(magnification),
    renderWindow(sf::VideoMode(width, height), gametitle, sf::Style::Titlebar | sf::Style::Close)
{
    renderWindow.setFramerateLimit(60);
    renderWindow.setVerticalSyncEnabled(true);

    image.create(width, height);
}

void Screen::draw(const FrameBuffer &buffer) {
    renderWindow.clear(sf::Color::White);

    sf::Event event;
    while(renderWindow.pollEvent(event)) {
        if(event.type == sf::Event::Closed)
            renderWindow.close();
        else if(event.type == sf::Event::KeyPressed) {
            KeypadKey key = SFKeyToGBKey(event.key.code);
            if(key != KeypadKey::NONE)
                keypad->keyDown(key);
        }
        else if(event.type == sf::Event::KeyReleased) {
            KeypadKey key = SFKeyToGBKey(event.key.code);
            if(key != KeypadKey::NONE)
                keypad->keyUp(key);
        }

    }

    setPixelsOnImage(buffer);
    texture.loadFromImage(image);
    sprite.setTexture(texture, true);
    renderWindow.draw(sprite);

    renderWindow.display();
}

bool Screen::isOpen() {
    return renderWindow.isOpen();
}

void Screen::setKeypad(std::shared_ptr<Keypad> keypad) {
    this->keypad = keypad;
}

void Screen::setPixelsOnImage(const FrameBuffer& buffer) {
    for (u32 y = 0; y < logicalHeight; ++y) {
         for (u32 x = 0; x < logicalWidth; ++x) {
             u32 bufferY = y % FrameBuffer::FRAMEBUFFER_SIZE;
             u32 bufferX = x % FrameBuffer::FRAMEBUFFER_SIZE;

             Color color = buffer.getPixel(bufferX, bufferY);
             this->setLargePixel(x, y, getSFMLColor(color));
         }
     }
}

void Screen::setLargePixel(u32 x, u32 y, sf::Color color) {
    for(u32 h = 0; h < pixelSize; ++h)
        for(u32 w = 0; w < pixelSize; ++w)
            this->image.setPixel((x*pixelSize) + w, (y*pixelSize) + h, color);
}

KeypadKey Screen::SFKeyToGBKey(sf::Keyboard::Key key) {
    KeypadKey result = KeypadKey::NONE;
    switch (key) {
    case sf::Keyboard::Key::Return:
        result = KeypadKey::START; break;
    case sf::Keyboard::Key::Space:
        result = KeypadKey::SELECT; break;
    case sf::Keyboard::Key::Left:
        result = KeypadKey::LEFT; break;
    case sf::Keyboard::Key::Right:
        result = KeypadKey::RIGHT; break;
    case sf::Keyboard::Key::Down:
        result = KeypadKey::DOWN; break;
    case sf::Keyboard::Key::Up:
        result = KeypadKey::UP; break;
    case sf::Keyboard::Key::X:
        result = KeypadKey::A; break;
    case sf::Keyboard::Key::Z:
        result = KeypadKey::B; break;
    default: break;
    }
    return result;
}

sf::Color Screen::getSFMLColor(Color color) {
    switch (color) {
        case Color::White: 		return sf::Color::White;
        case Color::LightGray: 	return sf::Color(170, 170, 170);
        case Color::DarkGray: 	return sf::Color(85, 85, 85);
        case Color::Black: 		return sf::Color::Black;
    }
}
