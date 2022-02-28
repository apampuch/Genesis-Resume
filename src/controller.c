#pragma once
#include <genesis.h>
#include "controller.h"

const u16 HOLD_DELAY = 60;  // if a directional button is pressed, this is how long to wait]
                            // until the arrow auto-moves

void menuControls(u16 joy, u16 changed, u16 state)
{
    static u16 hold_count = 0;

    if (joy == JOY_1) 
    {
        // reset the hold counter if a directional button changed
        if (changed & BUTTON_DIR)
        {
            hold_count = 0;
        }
    }
}