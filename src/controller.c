#include <genesis.h>
#include "controller.h"
#include "sections.h"
#include "vdp_manage.h"

void menuControls(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1) 
    {
        Section* s = currentSection();
        // reset the hold counter if a directional button changed
        if (state & BUTTON_DOWN && s->selected[1] < s->maxes[1])
        {
            s->selected[1]++;
        }
        
        if (state & BUTTON_UP && s->selected[1] > 0)
        {
            s->selected[1]--;
        }

        if (state & BUTTON_LEFT && s->selected[0] > 0)
        {
            s->selected[0]--;
        }

        if (state & BUTTON_RIGHT && s->selected[0] < s->maxes[0])
        {
            s->selected[0]++;
        }

        // redraw the cursor
        u16* xy = s->linkCoords[s->selected[0]][s->selected[1]];
        // KLog_U2("SX: ", s->selected[0], "SY: ", s->selected[1]);
        // KLog_U2("X: ", xy[0], "Y: ", xy[1]);
        SPR_setPosition(cursor, xy[0], xy[1]);
    
        if (state & BUTTON_A)
        {
            // follow link
            followSectionLink(s->links[s->selected[0]][s->selected[1]]);
        }
        if (state & BUTTON_B)
        {
            // go back if possible
            previousLink();  
        }
    }
}