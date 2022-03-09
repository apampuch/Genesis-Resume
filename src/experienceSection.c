#include "sections.h"
#include "text.h"
#include "vdp_manage.h"
#include <resources.h>

void loadJobSection(Section* s)
{
    resetVDPStack();

    // set skyscraper background
    PAL_setPalette(PAL0, Greyscale.data);
    u16 skyscraperPos = loadFreeVDPSpace(&Skyscraper);
    VDP_fillTileMapRectInc(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, skyscraperPos), 0, 0, 40, 28);

    // hide cursor
    SPR_setVisibility(cursor, HIDDEN);
}

void setupJobSection(Section* s, TextWrapper* t)
{

}

void loadExperienceSection(Section* s)
{
    resetVDPStack();

    // set skyscraper background
    PAL_setPalette(PAL0, Greyscale.data);
    u16 skyscraperPos = loadFreeVDPSpace(&Skyscraper);
    VDP_fillTileMapRectInc(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, skyscraperPos), 0, 0, 40, 28);

    // show cursor
    SPR_setVisibility(cursor, VISIBLE);
}

void updateExperienceSection(Section* s)
{
    
}

void setupExperienceSection(Section* s)
{
    sprintf(s->DBG_STR, "%s", "Experience Section");

    s->loadFunc = &loadExperienceSection;
    s->updateFunc = &updateExperienceSection;

    s->maxes[0] = 0;
    s->maxes[1] = 5;
    
    TextWrapper* texts[6] = { &PySRCGText, &ocrMoeText, &AccentureText, &NijiyaText, &OphtekText, &EquinixText };

    // setup coords and links
    for (u16 i = 0; i < 5; i++)
    {
        u16 y = i * 24 + 36;

        s->linkCoords[0][i][0] = 28;
        s->linkCoords[0][i][1] = y;
        s->links[0][i] = malloc(sizeof(Section));
        setupJobSection(s->links[0][i], texts[i]);
    }
}