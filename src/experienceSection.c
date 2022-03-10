#include "sections.h"
#include "text.h"
#include "vdp_manage.h"
#include <resources.h>

void updateJobSection(Section* s)
{
    // do nothing
}

void loadJobSection(Section* s)
{
    resetVDPStack();

    // set skyscraper background
    PAL_setPalette(PAL0, Greyscale.data);
    u16 skyscraperPos = loadFreeVDPSpace(&Skyscraper);
    VDP_fillTileMapRectInc(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, skyscraperPos), 0, 0, 40, 28);

    // hide cursor
    SPR_setVisibility(cursor, HIDDEN);

    // print job info
    TextWrapper* textPtr = (TextWrapper*)s->extraData[0];
    u16 startRow = (VDP_getScreenHeight() / 16) - (textPtr->length / 2); // start in the center
    for (u16 i = 0; i <= textPtr->length; i++)
    {
        VDP_drawTextBG(VDP_BG_A, textPtr->text[i], 2, i + startRow);
    }
}

void setupJobSection(Section* s, TextWrapper* txt)
{
    s->loadFunc = &loadJobSection;
    s->updateFunc = &updateJobSection;

    s->maxes[0] = 0;
    s->maxes[1] = 0;

    s->extraData[0] = txt; // pass in text
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

    char titles[6][14] = { "PySRCG", "ocr.moe", "Accenture", "Nijiya Market", "Ophtek", "Equinix"};

    // draw text
    for (u16 i = 0; i < 6; i++)
    {
        VDP_drawText(titles[i], LINK_DRAWTEXT_ARGS(s->linkCoords[0][i], 0));
    }
}

void updateExperienceSection(Section* s)
{
    // do nothing
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
    for (u16 i = 0; i < 6; i++)
    {
        u16 y = i * 24 + 36;

        s->linkCoords[0][i][0] = 28;
        s->linkCoords[0][i][1] = y;
        s->links[0][i] = malloc(sizeof(Section));
        setupJobSection(s->links[0][i], texts[i]);
    }
}