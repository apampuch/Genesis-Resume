#include <genesis.h>
#include <resources.h>
#include "sections.h"
#include "text.h"
#include "vdp_manage.h"

void updateEducationSection(Section* s)
{
    // do nothing
}

void loadCollegeSection(Section* s)
{
    resetVDPStack();

    // set diploma background
    PAL_setPalette(PAL3, DiplomaPal.data);
    u16 diplomaPos = loadFreeVDPSpace(&Diploma);
    VDP_fillTileMapRectInc(BG_B, TILE_ATTR_FULL(PAL3, 0, 0, 0, diplomaPos), 0, 0, 40, 28);

    // print college info
    TextWrapper* textPtr = (TextWrapper*)s->extraData[0];
    u16 startRow = (VDP_getScreenHeight() / 16) - (textPtr->length / 2); // start in the center
    for (u16 i = 0; i <= textPtr->length; i++)
    {
        VDP_drawTextBG(VDP_BG_A, textPtr->text[i], 2, i + startRow);
    }

    // hide cursor
    SPR_setVisibility(cursor, HIDDEN);
}

void setupCollegeSection(Section* s, TextWrapper* txt)
{
    // nothing to select or anything
    s->selected[0] = 0;
    s->selected[1] = 0;

    s->maxes[0] = 0;
    s->maxes[1] = 0;

    s->extraData[0] = txt; // pass in text
    // pass in background and palette?

    s->updateFunc = &updateEducationSection;
    s->loadFunc = &loadCollegeSection;
}

void loadEducationSection(Section* s)
{
    resetVDPStack();

    // set diploma background
    PAL_setPalette(PAL3, DiplomaPal.data);
    u16 diplomaPos = loadFreeVDPSpace(&Diploma);
    VDP_fillTileMapRectInc(BG_B, TILE_ATTR_FULL(PAL3, 0, 0, 0, diplomaPos), 0, 0, 40, 28);

    // setup WGU, Chuo, CSUMB
    VDP_drawText("Western Governors University", LINK_DRAWTEXT_ARGS(s->linkCoords[0][0], 0));
    VDP_drawText("Chuo University", LINK_DRAWTEXT_ARGS(s->linkCoords[0][1], 0));
    VDP_drawText("CSU Monterey Bay", LINK_DRAWTEXT_ARGS(s->linkCoords[0][2], 0));

    // show cursor
    SPR_setVisibility(cursor, VISIBLE);
}

void setupEducationSection(Section* s)
{
    // DEBUG STRING
    sprintf(s->DBG_STR, "%s", "Education Section");
    s->loadFunc = &loadEducationSection;
    s->updateFunc = &updateEducationSection;

    s->maxes[0] = 0;
    s->maxes[1] = 2;

    s->linkCoords[0][0][0] = 28;
    s->linkCoords[0][0][1] = 60;

    s->linkCoords[0][1][0] = 28;
    s->linkCoords[0][1][1] = 92;

    s->linkCoords[0][2][0] = 28;
    s->linkCoords[0][2][1] = 124;

    s->links[0][0] = malloc(sizeof(Section));
    s->links[0][1] = malloc(sizeof(Section));
    s->links[0][2] = malloc(sizeof(Section));

    // setup WGU, Chuo, and CSUMB sections
    setupCollegeSection(s->links[0][0], &WGUText);
    setupCollegeSection(s->links[0][1], &ChuoText);
    setupCollegeSection(s->links[0][2], &CSUMBText);
}
