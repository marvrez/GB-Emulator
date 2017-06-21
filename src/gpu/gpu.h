#ifndef GPU_H
#define GPU_H

#include "screen.h"
#include "framebuffer.h"

#include "../mmu.h"
#include "../register.h"
#include "../util.h"

#include <vector>
#include <memory>

const Address TILE_SET_ZERO_LOCATION = 0x8000;
const Address TILE_SET_ONE_LOCATION = 0x8800;

const Address TILE_MAP_ZERO_LOCATION = 0x9800;
const Address TILE_MAP_ONE_LOCATION = 0x9C00;

const u32 TILES_PER_LINE = 32;


const u32 VBlankCycles = 456;
const u32 HBlankCycles = 204;
const u32 ReadingOAMCycles = 80;
const u32 ReadingOAMVRAMCycles = 172;


enum class VideoMode {
    HBlank,
    VBlank,
    ReadOAM,
    ReadVRAM,
};

class GPU  {
public:
    GPU(MMU* mmu);

    void tick(Cycles cycle);
    bool isUpdated();

    FrameBuffer* getBuffer();
private:
    std::shared_ptr<MMU> mmu;
    FrameBuffer buffer;


    VideoMode mode, prevMode;
    u32 cycleCounter;
    u32 oldLineValue;
    bool scanlineTransfered;

    bool updated;

    Color bgPalette[4], obj0Palette[4], obj1Palette[4];

    ByteRegister LCDControl; //registers[0]
    ByteRegister LCDControllerStatus; //registers[1]

    ByteRegister yScroll; //registers[2]
    ByteRegister xScroll; //registers[3]

    ByteRegister line; //registers[4]
    ByteRegister LYCompare; //registers[5]

    ByteRegister DMATransfer; //registers[6]

    ByteRegister BGPalette; //registers[7]
    ByteRegister ObjPalette0; //registers[8]
    ByteRegister ObjPalette1; //registers[9]

    ByteRegister WindowYPosition; //registers[10]
    ByteRegister WindowXPosition; //registers[11]

private:
    bool bgEnabled() const;
    bool spritesEnabled() const;
    bool spritesLarge() const;
    u16  bgTileMapLocation() const;
    u16  bgTileDataLocation() const;
    bool windowEnabled() const;
    u16  windowTileMapLocation() const; //same location as bgTileMapLocation
    bool lcdEnabled() const;

    void draw();
    void drawBG();
    void drawWindow();
    void drawSprites();
    void refreshBuffer();

    void refreshRAM();
    void fireInterrupts();

    BGPalette getBGPalette() const;
};

#endif
