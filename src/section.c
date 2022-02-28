#include "sections.h"
#include <resources.h>

// this is a stack which keeps track of what link you're on
Section* sectionStack[8];
u8 sectionStackIndex = 0;
s16 verticalScrollValue = 0;

// pointer to last available part of VDP
u16 VDPStack = TILE_USERINDEX;
u16 andrewsResumeTilesLoc;
u16 backgroundLoc;

void followSectionLink(Section* sourceSection, u8 linkIndex)
{
    if (sourceSection->links[linkIndex] != NULL)
    {
        sectionStack[++sectionStackIndex] = sourceSection->links[linkIndex];
        // load section
    }
}

void previousLink()
{
    if (sectionStackIndex > 0)
    {
        sectionStackIndex--;
        // load section
    }
}

// Called in by each setup function to do resetting.
void setupReset()
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

void drawAndrewsResumeLetter(u16 index, u16 x, u16 y)
{
    // calculate index offset
    index = 16 * index + andrewsResumeTilesLoc;
    VDP_fillTileMapRectInc(VDP_BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, index), x, y, 4, 4);
}

void mainSectionUpdate(Section* this)
{
    KLog("Calling!");
    if (verticalScrollValue<0)
    {
        verticalScrollValue++;
        VDP_setVerticalScroll(BG_A, verticalScrollValue);
    }
}

void setupMainSection(Section* s)
{
    setupReset();
    // load text
    andrewsResumeTilesLoc = loadFreeVDPSpace(&AndrewsResume);
    PAL_setPalette(PAL2, AndResPal.data);

    // load background
    backgroundLoc = loadFreeVDPSpace(&PCB);
    VDP_fillTileMapRectInc(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, backgroundLoc), 0, 0, 42, 28);

    // write text
    // line 1 and 2; x and y
    u16 x = 4; u16 y = 10;
    const int INCREMENT_VALUE = 4;

    // andrew's resume
    drawAndrewsResumeLetter(0, x, y);       x += INCREMENT_VALUE;   // a
    drawAndrewsResumeLetter(1, x, y);       x += INCREMENT_VALUE;   // n    
    drawAndrewsResumeLetter(2, x, y);       x += INCREMENT_VALUE;   // d
    drawAndrewsResumeLetter(3, x, y);       x += INCREMENT_VALUE;   // r
    drawAndrewsResumeLetter(4, x, y);       x += INCREMENT_VALUE;   // e
    drawAndrewsResumeLetter(5, x, y);       x += INCREMENT_VALUE;   // w
    VDP_fillTileMapRectInc(VDP_BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, 9*16 + andrewsResumeTilesLoc), x, y, 2, 2); // apostraphe
    x += INCREMENT_VALUE / 2;
    drawAndrewsResumeLetter(6, x, y);                               // s
    x = 7;
    y += 4;
    
    drawAndrewsResumeLetter(3, x, y);       x += INCREMENT_VALUE;   // r
    drawAndrewsResumeLetter(4, x, y);       x += INCREMENT_VALUE;   // e
    drawAndrewsResumeLetter(6, x, y);       x += INCREMENT_VALUE;   // s
    drawAndrewsResumeLetter(7, x, y);       x += INCREMENT_VALUE;   // u
    drawAndrewsResumeLetter(8, x, y);       x += INCREMENT_VALUE;   // m
    drawAndrewsResumeLetter(4, x, y);                               // e

    verticalScrollValue = -96;
    VDP_setVerticalScroll(BG_A, verticalScrollValue);
    s->updateFunc = &mainSectionUpdate;
}