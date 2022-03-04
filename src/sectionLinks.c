#include "sections.h"

// this is a stack which keeps track of what link you're on
Section* sectionStack[8];
u16 sectionStackIndex = 0;

Section* currentSection()
{
    return sectionStack[sectionStackIndex - 1];
}

void followSectionLink(Section* destSection)
{
    sectionStack[sectionStackIndex++] = destSection;
    destSection->loadFunc(destSection);
}

void previousLink()
{
    if (sectionStackIndex > 0)
    {
        --sectionStackIndex;
        // load section
        sectionStack[sectionStackIndex]->loadFunc(sectionStack[sectionStackIndex]);
    }
}
