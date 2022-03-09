#include "sections.h"
#include "text.h"
#include "vdp_manage.h"
#include <resources.h>

void loadContactSection(Section* s)
{
    resetVDPStack();

    // set phone background
    u16 phonePos = loadFreeVDPSpace(&Phone);
    PAL_setPaletteColors(0, &Greyscale);
    VDP_fillTileMapRectInc(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, phonePos), 0, 0, 40, 28);

    // print contact info
    u16 startRow = (VDP_getScreenHeight() / 16) - (ContactText.length / 2); // start in the center
    for (u16 i = 0; i <= ContactText.length; i++)
    {
        VDP_drawTextBG(VDP_BG_A, ContactText.text[i], 6, i + startRow);
    }

    // hide cursor
    SPR_setVisibility(cursor, HIDDEN);
}

void updateContactSection(Section* s)
{
    
}

void setupContactSection(Section* s)
{
    // setup limits
    s->maxes[0] = 0;
    s->maxes[1] = 0;

    s->loadFunc = &loadContactSection;
    s->updateFunc = &updateContactSection;
}
