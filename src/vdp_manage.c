#include "vdp_manage.h"

// pointer to last available part of VDP
u16 VDPStack = TILE_USERINDEX;

u16 getVDPStack()
{
    return VDPStack;
}

// Called in by each setup function to do resetting.
void resetVDPStack()
{
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