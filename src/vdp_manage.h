#include "genesis.h"

#define LINK_POS_TO_TEXT_POS_X(X) ((X + 20) / 8)
#define LINK_POS_TO_TEXT_POS_Y(Y, SCROLL) ((Y + 4 + SCROLL) / 8)
// the last two arguments of VDP_drawText to convert link position to tile position
#define LINK_DRAWTEXT_ARGS(SEC_LC2, SCRL) LINK_POS_TO_TEXT_POS_X(SEC_LC2[0]), LINK_POS_TO_TEXT_POS_Y(SEC_LC2[1], SCRL)

Sprite* cursor;
u16 getVDPStack();
void resetVDPStack();
u16 loadFreeVDPSpace(const TileSet* t);
