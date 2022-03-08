#include "color.h"
#include "controller.h"

u16 palHistoryStack[32][8];
u16 dyeColorStack[32];
u8 stackPointer = 0; // should point to first blank

u16 DBGdyeColor = RGB24_TO_VDPCOLOR(0xFF0000);

void colorizeDebugCallback(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1) 
    {
        if (changed == BUTTON_RIGHT || changed == BUTTON_LEFT)
        {
            if (state & BUTTON_RIGHT)
            {
                // store in stacks
                // increment pointer
                dyeColorStack[stackPointer] = DBGdyeColor;
                // dyeColor = rotateHue(dyeColor);
                DBGdyeColor = smoothRotateHue(DBGdyeColor, PAL_getColor(8));
                PAL_getColors(1, palHistoryStack[stackPointer++], 8);
                
                // colorize range
                colorizeRange(1,9, DBGdyeColor);
            }
            else if (state & BUTTON_LEFT && stackPointer > 0)
            {
                // decrement history pointer
                // load colors
                PAL_setColors(1, palHistoryStack[--stackPointer], 8);
                // reset dye color
                DBGdyeColor = dyeColorStack[stackPointer];
            }
        }
    }
}
