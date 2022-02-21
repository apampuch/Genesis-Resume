#pragma once
#include <genesis.h>

typedef struct Section {
    Map background;
    struct Section* links[4];
} Section;

void followSectionLink(Section* sourceSection, u8 linkIndex);
