#include "section.h"

// this is a stack which keeps track of what link you're on
Section* sectionStack[8];
u8 sectionStackIndex = 0;

void followSectionLink(Section* sourceSection, u8 linkIndex)
{
    if (sourceSection->links[linkIndex] != NULL)
    {
        sectionStack[++sectionStackIndex] = sourceSection->links[linkIndex];
        // load section
    }
}

void previousLink()
{
    if (sectionStackIndex > 0)
    {
        sectionStackIndex--;
        // load section
    }
}