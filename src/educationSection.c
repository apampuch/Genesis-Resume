#include "sections.h"
#include "vdp_manage.h"

void updateEducationSection(Section* s)
{
    
}

void loadEducationSection(Section* s)
{
    resetVDPStack();

    // loadFreeVDPSpace

    // setup WGU, Chuo, CSUMB
    VDP_drawText("Western Governors University", LINK_DRAWTEXT_ARGS(s->linkCoords[0][0], 0));
    VDP_drawText("Chuo University", LINK_DRAWTEXT_ARGS(s->linkCoords[0][1], 0));
    VDP_drawText("CSU Monterey Bay", LINK_DRAWTEXT_ARGS(s->linkCoords[0][2], 0));
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
}
