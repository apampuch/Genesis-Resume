#include <genesis.h>
#include <resources.h>
#include "sections.h"
#include "color.h"
#include "controller.h"
#include "vdp_manage.h"

int main()
{
    SPR_initEx(0x50);

    // setup font
    PAL_setPaletteColors(0, &Greyscale);
    PAL_setPaletteColors(16, &FontPal);
    VDP_loadFont(&BubbleFont, DMA);
    VDP_setTextPalette(PAL1);

    // colorize palette
    u16 dyeColor = RGB24_TO_VDPCOLOR(0xFF0000);
    
    colorizeRange(1,9, dyeColor);

    // setup controls
    // JOY_setEventHandler(colorizeDebugCallback); // DEBUG ONLY
    // JOY_setEventHandler(menuControls);

    // setup sections
    Section mainSection;
    setupMainSection(&mainSection);

    // clear font data so it can fade in
    VDP_fillTileData(0, TILE_FONTINDEX, 93, TRUE);

    // place the cursor
    cursor = SPR_addSprite
    (
        &CursorSprite, 
        mainSection.linkCoords[0][0][0],
        mainSection.linkCoords[0][0][1], 
        TILE_ATTR(PAL1, 0, 0, 0)
    );

    // load main section
    followSectionLink(&mainSection);

    Section* curSec;
    while(1)
    {
        SPR_update();
        SYS_doVBlankProcess();
        curSec = currentSection();
        curSec->updateFunc(curSec);
    }
    return (0);
}
