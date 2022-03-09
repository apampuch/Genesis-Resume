#include "vdp_manage.h"

// pointer to last available part of VDP
u16 VDPStack = TILE_USERINDEX;
Sprite* cursor;

u16 getVDPStack()
{
    return VDPStack;
}

// Called in by each setup function to do resetting.
void resetVDPStack()
{
    // clear tile data
    if (VDPStack - TILE_USERINDEX > 0) // check to make sure we don't get rid of stuff we might need
        VDP_fillTileData(0, TILE_USERINDEX, VDPStack - TILE_USERINDEX, TRUE);

    // clear tilemap
    VDP_clearPlane(BG_A, FALSE);
    VDP_clearPlane(BG_B, TRUE);

    // reset scroll    
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setHorizontalScroll(BG_B, 0);
    VDP_setVerticalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_B, 0);

    VDPStack = TILE_USERINDEX;
}

// loads a tilestack and advances the stack pointer
// returns where the tileset was placed
u16 loadFreeVDPSpace(const TileSet* t)
{
    u16 retVal = VDPStack;
    VDP_loadTileSet(t, VDPStack, DMA);
    VDPStack += t->numTile;
    return retVal;
}
