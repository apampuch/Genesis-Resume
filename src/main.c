#include <genesis.h>
#include <resources.h>
#include "sections.h"
#include "text.h"
#include "color.h"
#include "controller.h"

int main()
{
    int counter = 0;

    // setup font
    PAL_setPaletteColors(0, &Greyscale);
    PAL_setPaletteColors(16, &FontPal);
    VDP_loadFont(&BubbleFont, DMA);
    VDP_setTextPalette(PAL1);

    // VDP_loadTileSet(&Keyboard, TILE_USERINDEX, DMA);
    // VDP_fillTileMapRectInc(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX), 0, 0, 40, 28);

    // print skills
    // const int START_ROW = 2;
    // for (u16 i = 0; i <= 22; i++)
    // {
    //     VDP_drawTextBG(VDP_BG_A, skills[i], 2, i + START_ROW);
    // }

    // colorize palette
    u16 dyeColor = RGB24_TO_VDPCOLOR(0xFF0000);
    
    colorizeRange(1,9, dyeColor);

    // setup debug controls
    // JOY_setEventHandler(colorizeDebugCallback);

    // real shit starts here

    // VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_PLANE);
    // s16 scrollValues[2] = {0, 0};
    // VDP_setHorizontalScrollLine(BG_B, 64, scrollValues, 2, DMA);

    // setup sections
    Section mainSection;
    setupMainSection(&mainSection);
    followSectionLink(&mainSection);

    // clear font data so it can fade in
    VDP_fillTileData(0, TILE_FONTINDEX, 93, TRUE);

    // place text
    VDP_drawText("Skills", 4, 24);
    VDP_drawText("Experience", 28, 24);
    VDP_drawText("Education", 4, 28);
    VDP_drawText("Contact", 28, 28);

    while(1)
    {
        // counter++;
        // if (counter % 6 == 0)
        // {
        //     dyeColor = smoothRotateHue(dyeColor, PAL_getColor(8));
        //     colorizeRange(1,9,dyeColor);
        // }

        SYS_doVBlankProcess();
        currentSection()->updateFunc(&currentSection);
    }
    return (0);
}
