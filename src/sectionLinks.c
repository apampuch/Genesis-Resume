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
    sectionStack[sectionStackIndex] = destSection;
    sectionStackIndex++;
    destSection->loadFunc(destSection);
}

void previousLink()
{
    if (sectionStackIndex > 1)
    {
        --sectionStackIndex;
        KLog(sectionStack[sectionStackIndex - 1]->DBG_STR);
        // load section
        sectionStack[sectionStackIndex - 1]->loadFunc(sectionStack[sectionStackIndex - 1]);
    }
}
