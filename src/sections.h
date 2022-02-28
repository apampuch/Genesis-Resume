#pragma once
#include <genesis.h>

typedef struct Section 
{
    // the actual sections to be loaded
    struct Section* links[8][8];

    // x y pixel coordinates of where the links should be
    u16 linkCoords[8][8][2];

    // what the arrow is pointing to
    u16 selected[2];

    void (*updateFunc) (struct Section* this);
} Section;

void followSectionLink(Section* sourceSection, u8 linkIndex);
void setupMainSection(Section* s);