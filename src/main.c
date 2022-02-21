/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>
#include <resources.h>
#include "text.h"
#include "color.h"

int main()
{
    // setup font
    PAL_setPaletteColors(0, &TreePal);
    PAL_setPaletteColors(16, &FontPal);
    VDP_loadFont(&BubbleFont, DMA);
    VDP_setTextPalette(PAL1);

    // print skills
    for (int i=2; i<23+i; i++)
    {
        VDP_drawText(skills[i], 2, i);
    }

    while(1)
    {
        //For versions prior to SGDK 1.60 use VDP_waitVSync instead.
        SYS_doVBlankProcess();
    }
    return (0);
}
