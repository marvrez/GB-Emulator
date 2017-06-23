#ifndef GPU_H
#define GPU_H

#include "screen.h"
#include "framebuffer.h"

#include "../mmu.h"
#include "../cpu/cpu.h"
#include "../register.h"
#include "../util.h"

#include <vector>
#include <memory>

const Address TILE_SET_ZERO_LOCATION = 0x8000;
const Address TILE_SET_ONE_LOCATION  = 0x9000;

const Address TILE_MAP_ZERO_LOCATION = 0x9800;
const Address TILE_MAP_ONE_LOCATION  = 0x9C00;

const u16 OAM_BASE_ADDRESS = 0xFE00;


//Addresses for registers
const Address LCDC_ADDRESS = 0xFF40;
const Address LCDCS_ADDRESS = 0xFF41;
const Address SCROLLY_ADDRESS = 0xFF42;
const Address SCROLLX_ADDRESS = 0xFF43;
const Address LINE_ADDRESS = 0xFF44;
const Address LYC_ADDRESS = 0xFF45;
const Address DMA_TRANSFER_ADDRESS = 0xFF46;
const Address BGPALETTE_ADDRESS =  0xFF47;
const Address OBJ0_PALETTE_ADDRESS = 0xFF48;
const Address OBJ1_PALETTE_ADDRESS = 0xFF49;
const Address WINYPOS_ADDRESS = 0xFF4A;
const Address WINXPOS_ADDRESS = 0xFF4B;

const u32 TILES_PER_LINE = 32;
const u32 TILES_BYTES = 16;

//V - cycles
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

union OAM_Flags {
    struct {
        u8 reserved : 4;
        bool pal1 : 1;
        bool xFlip : 1;
        bool yFlip : 1;
        bool belowBg : 1;
    } flags;
    u8 byte;
};

class GPU  {
public:
    GPU(MMU* mmu);

    void tick(Cycles cycle);
    bool isUpdated();

    FrameBuffer* getBuffer();

    void setVSyncCallback(void(*Callback)());
private:
    std::shared_ptr<MMU> mmu;
    FrameBuffer buffer;

    void(*VSyncCallback)();

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

    void preprocessRAM();
    void postprocessRAM();

    struct BGPalette getBGPalette() const;
};

#endif
