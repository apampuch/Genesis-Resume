#include "color.h"
#include "sections.h"
#include "text.h"
#include "resources.h"
#include "vdp_manage.h"

int counter = 0;
u16 dyeColor = RGB24_TO_VDPCOLOR(0xFF0000);

void loadSkillsSection(Section* s);
void updateSkillsSection(Section* s);

void setupSkillsSection(Section* s)
{
    s->selected[0] = 0;
    s->selected[1] = 0;

    s->maxes[0] = 0;
    s->maxes[1] = 0;

    s->loadFunc = &loadSkillsSection;
    s->updateFunc = &updateSkillsSection;
}

void loadSkillsSection(Section* s)
{
    // DEBUG STRING
    sprintf(s->DBG_STR, "%s", "Skills Section");

    // clear vdp and map
    resetVDPStack();

    // print skills
    const int START_ROW = 2;
    for (u16 i = 0; i <= 22; i++)
    {
        VDP_drawTextBG(VDP_BG_A, skillsText[i], 2, i + START_ROW);
    }

    // setup background
    u16 kbLoc = loadFreeVDPSpace(&Keyboard);

    VDP_fillTileMapRectInc(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, kbLoc), 0, 0, 40, 28);

    // hide cursor
    SPR_setVisibility(cursor, HIDDEN);
}

void updateSkillsSection(Section* s)
{
    counter++;
    if (counter % 6 == 0)
    {
        dyeColor = smoothRotateHue(dyeColor, PAL_getColor(8));
        colorizeRange(1,9, dyeColor);
    }
}