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
    u16 maxes[2]; // max x and y values

    void (*updateFunc) (struct Section* self);
    void (*loadFunc) (struct Section* self);
    char DBG_STR[32];
    void* extraData[8]; // extra data we need to pass in, keep track of this manually and cast
} Section;

Section* currentSection();
void followSectionLink(Section* destSection);
void previousLink();
void setupMainSection(Section* s);
