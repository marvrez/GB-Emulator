#include "gpu.h"
#include "screen.h"

GPU::GPU(std::shared_ptr<MMU>& mmu, std::shared_ptr<Screen>& screen) :
    mmu(mmu),
    screen(screen),
    buffer(FrameBuffer::FRAMEBUFFER_SIZE, FrameBuffer::FRAMEBUFFER_SIZE),
    mode(VideoMode::HBlank),
    cycleCounter(0),
    scanlineTransfered(false)
{
    for(int i = 0; i < 4; ++i)
        bgPalette[i] = obj0Palette[i] = obj1Palette[i] = Color::White;
}

void GPU::tick(Cycles cycle) {
    preprocessRAM();

    updated = false;
    if(!lcdEnabled()) return;
    cycleCounter += cycle.cycles;

    switch(mode) {
    case VideoMode::ReadOAM:
        if (cycleCounter >= 80) {
            cycleCounter -= 80;
            mode = VideoMode::ReadVRAM;
            scanlineTransfered = false;
        }
        break;
    case VideoMode::ReadVRAM:
        if (cycleCounter >= 12 && !scanlineTransfered) {
            draw();
            scanlineTransfered = true;
        }
        if (cycleCounter >= 172) {
            cycleCounter -= 172;
            mode = VideoMode::HBlank;
        }
        break;
    case VideoMode::HBlank:
        if (cycleCounter >= 204) {
            cycleCounter -= 204;

            line.increment();
            if (line.getValue() == 144) {
                mode = VideoMode::VBlank;
                updated = true; //fire VBLANK signal
            }
            else mode = VideoMode::ReadOAM;
        }
        break;
    case VideoMode::VBlank:
        if (cycleCounter >= 456) {
            cycleCounter -= 456;

            line.increment();
            // stay 10 iterations in this loop, but increase line the first 9 times
            if (line.getValue() == 154) {
                renderFrame();
                mode = VideoMode::ReadOAM;
                line.reset();
            }
        }
        break;
    }

    postprocessRAM();
}

bool GPU::isUpdated() {
    return this->updated;
}

FrameBuffer* GPU::getBuffer() {
    return &buffer;
}

void GPU::setVSyncCallback(void(*Callback)()) {
    VSyncCallback = Callback;
}

bool GPU::bgEnabled() const {
    return LCDControl.checkBit(0);
}

bool GPU::spritesEnabled() const {
    return LCDControl.checkBit(1);
}

bool GPU::spritesLarge() const {
    return LCDControl.checkBit(2);
}

u16 GPU::bgTileMapLocation() const {
    if(LCDControl.checkBit(3)) return TILE_MAP_ONE_LOCATION.getValue();
    else return TILE_MAP_ZERO_LOCATION.getValue();
}

u16 GPU::bgTileDataLocation() const {
    if(LCDControl.checkBit(4)) return TILE_SET_ZERO_LOCATION.getValue();
    else return TILE_SET_ONE_LOCATION.getValue();
}

bool GPU::windowEnabled() const {
    return LCDControl.checkBit(5);
}

u16 GPU::windowTileMapLocation() const {
    if(LCDControl.checkBit(6)) return TILE_MAP_ONE_LOCATION.getValue();
    else return TILE_MAP_ZERO_LOCATION.getValue();
}

bool GPU::lcdEnabled() const {
    return LCDControl.checkBit(7);
}

void GPU::draw() {
    drawBG();
    drawWindow();
    drawSprites();
}

void GPU::drawBG() {
    if (!bgEnabled()) return;

    s32 bgX = xScroll.getValue();
    s32 bgY = yScroll.getValue() + line.getValue();

    Address tileMapLocation = bgTileMapLocation();
    Address tileSetLocation = bgTileDataLocation();

    s32 tileY = (bgY >> 3) & 0x1F;
    s32 tileX;

    for (int x = 0; x < GAMEBOY_WIDTH; ++x, ++bgX) {
        tileX = (bgX >> 3) & 0x1F;

        Address tileAddress = 0;
        if (LCDControl.checkBit(4)) {
            u8 tilenr = mmu->readByte(tileMapLocation + tileY * 32 + tileX);
            tileAddress = 0x8000 + tilenr * 16;
        } else {
            // Signed!
            s8 tilenr = mmu->readByte(tileMapLocation + tileY * 32 + tileX);
            tileAddress = 0x9000 + tilenr * 16;
        }

        u8 byte1 = mmu->readByte(tileAddress + ((bgY & 0x07) * 2));
        u8 byte2 = mmu->readByte(tileAddress + ((bgY & 0x07) * 2) + 1);

        u8 xbit = bgX & 0x07, pixelVal = 0;
        pixelVal |= (byte1 & (0x01 << (7 - xbit))) ? 0x1 : 0;
        pixelVal |= (byte2 & (0x01 << (7 - xbit))) ? 0x2 : 0;
        Color color = bgPalette[pixelVal];

        buffer.setPixel(x, line.getValue(), color);
    }
}

void GPU::drawWindow() {
    if (!windowEnabled()) return;
    int winY = line.getValue() - WindowYPosition.getValue();
    if (winY < 0) return;

    int winX = -(WindowXPosition.getValue() - 7);

    Address tileMapLocation = windowTileMapLocation();
    Address tileSetLocation = bgTileDataLocation();

    int tileY = (winY >> 3) & 0x1F;
    int tileX;

    for (int x = 0; x < GAMEBOY_WIDTH; ++x, ++winX) {
        if (winX < 0) continue;
        tileX = (winX >> 3) & 0x1F;

        Address tileAddress = 0;
        if (LCDControl.checkBit(4)) {
            u8 tilenr = mmu->readByte(tileMapLocation + tileY * 32 + tileX);
            tileAddress = 0x8000 + tilenr * 16;
        } else {
            // Signed!
            s8 tilenr = mmu->readByte(tileMapLocation + tileY * 32 + tileX);
            tileAddress = 0x9000 + tilenr * 16;
        }

        u8 byte1 = mmu->readByte(tileAddress + ((winY & 0x07) * 2));
        u8 byte2 = mmu->readByte(tileAddress + ((winY & 0x07) * 2) + 1);

        u8 xbit = winX % 8;
        u8 pixelVal = 0;
        pixelVal |= (byte1 & (0x80 >> xbit)) ? 0x1 : 0;
        pixelVal |= (byte2 & (0x80 >> xbit)) ? 0x2 : 0;
        Color color = bgPalette[pixelVal];

        buffer.setPixel(x, line.getValue(), color);
    }
}

void GPU::drawSprites() {
    if (!spritesEnabled()) return;

    s32 spriteHeight = spritesLarge() ? 16 : 8;
    s32 numSprites = 0, lineVal = line.getValue();

    std::vector<int> spriteY(40), spriteX(40), spriteTile(40);
    std::vector<OAM_Flags> spriteFlags(40);

    for (int i = 0; i < 40; ++i) {
        Address spriteAddress = OAM_BASE_ADDRESS + i*4;

        spriteY[i] 			= mmu->readByte(spriteAddress + 0) - 16;
        spriteX[i] 			= mmu->readByte(spriteAddress + 1) - 8;
        spriteTile[i] 		= mmu->readByte(spriteAddress + 2) & (spriteHeight == 16 ? 0xFE : 0xFF);
        spriteFlags[i].byte = mmu->readByte(spriteAddress + 3);

        if (lineVal >= spriteY[i] && lineVal < spriteY[i] + spriteHeight)
            numSprites++;
    }

    for (int i = 39; i >= 0; --i) {
        if (lineVal < spriteY[i] || lineVal >= spriteY[i] + spriteHeight)
            continue;

        if (numSprites-- > 10) continue;

        if (spriteX[i] < -7 || spriteX[i] >= GAMEBOY_WIDTH)
            continue;

        s32 tileY = lineVal - spriteY[i];
        if (spriteFlags[i].flags.yFlip)
            tileY = (spriteHeight - 1) - tileY;

        Address tileAddress = TILE_SET_ZERO_LOCATION + spriteTile[i] * TILES_BYTES + tileY * 2;
        u8 byte1 = mmu->readByte(tileAddress);
        u8 byte2 = mmu->readByte(tileAddress + 1);

        for (int x = 0; x < 8; ++x) {
            if (spriteX[i] + x < 0 || spriteX[i] + x >= GAMEBOY_WIDTH)
                continue;

            s32 pixelVal = 0;
            pixelVal |= (byte1 & (0x01 << (spriteFlags[i].flags.xFlip ? x : 7 - x))) ? 1 : 0;
            pixelVal |= (byte2 & (0x01 << (spriteFlags[i].flags.xFlip ? x : 7 - x))) ? 2 : 0;

            // pixelVal = 0 is always transparant
            if (pixelVal == 0) continue;
            // if belowbg, then only draw on bg[0]
            if (spriteFlags[i].flags.belowBg && buffer.getPixel(spriteX[i] + x, lineVal) != bgPalette[0])
                continue;

            Color color = spriteFlags[i].flags.pal1 ? obj1Palette[pixelVal] : obj0Palette[pixelVal];
            buffer.setPixel(spriteX[i] + x, lineVal, color);
        }
    }
}

void GPU::preprocessRAM() {
    prevMode = mode;
    oldLineValue = line.getValue();

    //Updates the registers..
    LCDControl.setValue(mmu->readByte(LCDC_ADDRESS));
    LCDControllerStatus.setValue(mmu->readByte(LCDCS_ADDRESS));
    yScroll.setValue(mmu->readByte(SCROLLY_ADDRESS));
    xScroll.setValue(mmu->readByte(SCROLLX_ADDRESS));
    line.setValue(mmu->readByte(LINE_ADDRESS));
    LYCompare.setValue(mmu->readByte(LYC_ADDRESS));
    WindowYPosition.setValue(mmu->readByte(WINYPOS_ADDRESS));
    WindowXPosition.setValue(mmu->readByte(WINXPOS_ADDRESS));

    DMATransfer.setValue(mmu->readByte(DMA_TRANSFER_ADDRESS));
    if(DMATransfer.getValue() != 0) {
        u16 baseAddr = DMATransfer.getValue() << 8;
        for(u8 offset = 0; offset < 0xA0; ++offset)
            mmu->writeByte(Address(OAM_BASE_ADDRESS + offset), mmu->readByte(Address(baseAddr + offset)) );
        mmu->writeByte(DMA_TRANSFER_ADDRESS, 0);
    }

    u8 oldRegValue;

    oldRegValue = BGPalette.getValue();
    BGPalette.setValue(mmu->readByte(BGPALETTE_ADDRESS));
    if (oldRegValue != BGPalette.getValue()) {
        for(int i = 0; i < 4; ++i) {
            switch((BGPalette.getValue() >> (2*i)) & 3) {
            case 0: bgPalette[i] = Color::White; break;
            case 1: bgPalette[i] = Color::LightGray; break;
            case 2: bgPalette[i] = Color::DarkGray; break;
            case 3: bgPalette[i] = Color::Black; break;
            }
        }
    }

    oldRegValue = ObjPalette0.getValue();
    ObjPalette0.setValue(mmu->readByte(OBJ0_PALETTE_ADDRESS));
    if (oldRegValue != ObjPalette0.getValue()) {
        for(int i = 0; i < 4; ++i) {
            switch((ObjPalette0.getValue() >> (2*i)) & 3) {
            case 0: obj0Palette[i] = Color::White; break;
            case 1: obj0Palette[i] = Color::LightGray; break;
            case 2: obj0Palette[i] = Color::DarkGray; break;
            case 3: obj0Palette[i] = Color::Black; break;
            }
        }
    }

    oldRegValue = ObjPalette1.getValue();
    ObjPalette1.setValue(mmu->readByte(OBJ1_PALETTE_ADDRESS));
    if (oldRegValue != ObjPalette1.getValue()) {
        for(int i = 0; i < 4; ++i) {
            switch((ObjPalette1.getValue() >> (2*i)) & 3) {
            case 0: obj1Palette[i] = Color::White; break;
            case 1: obj1Palette[i] = Color::LightGray; break;
            case 2: obj1Palette[i] = Color::DarkGray; break;
            case 3: obj1Palette[i] = Color::Black; break;
            }
        }
    }

}

void GPU::postprocessRAM() {
    //update zram
    mmu->writeByte(LINE_ADDRESS, line.getValue());

    u8 modeFlag = static_cast<u8>(this->mode);
    u8 lcdstatValue = LCDControllerStatus.getValue();
    lcdstatValue &= 0xF8;
    lcdstatValue |= (line.getValue() == LYCompare.getValue() ? 0x4 : 0) | (modeFlag & 0x3);

    mmu->writeByte(LCDCS_ADDRESS, lcdstatValue);

    bool lcdstat = false;
    if (line.getValue() != oldLineValue)
        if ((lcdstatValue & 0x40) && line.getValue() == LYCompare.getValue())
            lcdstat = true;

    if (mode != prevMode) {
        if ((lcdstatValue & 0x20) && modeFlag == 2) lcdstat = true;
        if ((lcdstatValue & 0x10) && modeFlag == 1) lcdstat = true;
        if ((lcdstatValue & 0x08) && modeFlag == 0) lcdstat = true;
    }

    if (lcdstat || updated) {
        u8 interruptFlags = mmu->readByte(CPU::INTERRUPT_FLAG);
        interruptFlags |= (lcdstat ? 0x2 : 0) | (updated ? 0x1 : 0);
        mmu->writeByte(CPU::INTERRUPT_FLAG, interruptFlags);
    }
}

BGPalette GPU::getBGPalette() const {
    return {bgPalette[0], bgPalette[1], bgPalette[2], bgPalette[3]};
}

void GPU::renderFrame() {
    screen->draw(buffer, xScroll.getValue(), yScroll.getValue(), getBGPalette());
}
