#include "cavalier_debug.h"
#include "color.h"
#include "controller.h"
#include "sections.h"
#include "vdp_manage.h"
#include <resources.h>

void setupSkillsSection(Section* s);
void setupExperienceSection(Section* s);
void setupEducationSection(Section* s);
void setupContactSection(Section* s);

bool firstLoad = TRUE;
#define SCROLL_END 60

// vals that should be static but can't be due to soft reset bugs
s8 colorTicker;
s8 colorChange;
u8 tileCounter;
u8 masterTimer;
u16 horizOffset;

#define HORIZ_VAL_LEN 32
#define HORIZ_VAL_HALF_LEN 16
s16 verticalScrollValue = 0;
s16 horizScrollValues[HORIZ_VAL_LEN] = 
{   
    0-8, 1-8, 2-8, 3-8, 3-8, 3-8, 2-8, 1-8, 0-8, -1-8, -2-8, -3-8, -3-8, -3-8, -2-8, -1-8,
    0-8, 1-8, 2-8, 3-8, 3-8, 3-8, 2-8, 1-8, 0-8, -1-8, -2-8, -3-8, -3-8, -3-8, -2-8, -1-8,
};
// u16 horizScrollStart = 0; // start value for iteration of horizontal scroll values

u16 andrewsResumeTilesLoc;
u16 backgroundLoc;

void drawAndrewsResumeLetter(u16 index, u16 x, u16 y)
{
    // calculate index offset
    index = 16 * index + andrewsResumeTilesLoc;
    VDP_fillTileMapRectInc(VDP_BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, index), x, y, 4, 4);
}

void updateMainSection(Section* s)
{
    const u8 TICK_RATE = 6;

    const s8 TICKER_MAX = 6;
    const s8 TICKER_MIN = -4;


    // raise title
    if (verticalScrollValue < SCROLL_END)
    {
        verticalScrollValue++;
        VDP_setVerticalScroll(BG_A, verticalScrollValue);
    }

    // fade in menu items
    else if (tileCounter < 16)
    {
        #define FONT_PIX_ROWS 744

        // get the font tiles
        u32* tiles = BubbleFont.tiles;

        u32 tilesInRAM[FONT_PIX_ROWS];
        // for each pixel in options tileset
        for (int i=0; i<FONT_PIX_ROWS; i++)
        {
            u32 pixelRow = 0;
            // each u32 is made up of 8 nybbles for a row of 8 pixels
            for (int sft=0; sft<32; sft += 4)
            {
                u32 mask = 0xF << sft;
                u32 pixel = tiles[i] & mask;
                // debug if not 0

                // if tileCounter >= pixel, write actual pixel palette value to vdp
                if (tileCounter >= pixel >> sft)
                {
                    pixelRow += pixel;
                }
                // otherwise write pixel as 0, which is a nop here                    
            }

            // set the row of pixels
            tilesInRAM[i] = pixelRow;
        }
        
        VDP_loadTileData(tilesInRAM, TILE_FONTINDEX, BubbleFont.numTile, DMA);
        // VDP_loadFontData(&tilesInRAM, BubbleFont.numTile, DMA);
        tileCounter++;
    }
    // enable selection
    else
    {
        // reveal cursor and allow control
        SPR_setVisibility(cursor, VISIBLE);
        JOY_setEventHandler(menuControls);

        if (++masterTimer == TICK_RATE)
        {
            masterTimer = 0;
            colorTicker += colorChange;

            // invert when we reach an end
            if (colorTicker == TICKER_MAX || colorTicker == TICKER_MIN)
                colorChange = -colorChange;

            s8 colorMod = clamp(colorTicker, 0, 4);

            // darken and lighten
            const u8 greens[8] = {0, 1, 2, 3, 4, 5, 6, 7};
            for (u8 i = 0; i < 8; i++)
            {
                u8 colorIndex = i+1;
                u16 colorToSet = greens[i] - min(colorMod, greens[i]);
                colorToSet = colorToSet << VDPPALETTE_GREENSFT;
                PAL_setColor(colorIndex, colorToSet);
            }

        // "sine" wave background
        u16 horizLineCounter = 0;
        horizOffset = (horizOffset + 1) % HORIZ_VAL_HALF_LEN;
        KLog_U1("Offset: ", horizOffset);
        while (horizLineCounter < 320)
        {
            VDP_setHorizontalScrollLine(BG_B, horizLineCounter, &horizScrollValues[horizOffset], HORIZ_VAL_HALF_LEN, DMA);
            horizLineCounter += HORIZ_VAL_HALF_LEN;
        }
        }
    }
}

void loadMainSection(Section* s)
{
    resetVDPStack();

    // load text
    andrewsResumeTilesLoc = loadFreeVDPSpace(&AndrewsResume);
    PAL_setPalette(PAL2, AndResPal.data);

    // load background
    backgroundLoc = loadFreeVDPSpace(&PCB);
    VDP_fillTileMapRectInc(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, backgroundLoc), 0, 0, 42, 28);

    // write text
    // line 1 and 2; x and y
    u16 x = 4; u16 y = 10;
    const int INCREMENT_VALUE = 4;

    // andrew's resume
    drawAndrewsResumeLetter(0, x, y);       x += INCREMENT_VALUE;   // a
    drawAndrewsResumeLetter(1, x, y);       x += INCREMENT_VALUE;   // n    
    drawAndrewsResumeLetter(2, x, y);       x += INCREMENT_VALUE;   // d
    drawAndrewsResumeLetter(3, x, y);       x += INCREMENT_VALUE;   // r
    drawAndrewsResumeLetter(4, x, y);       x += INCREMENT_VALUE;   // e
    drawAndrewsResumeLetter(5, x, y);       x += INCREMENT_VALUE;   // w
    VDP_fillTileMapRectInc(VDP_BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, 9*16 + andrewsResumeTilesLoc), x, y, 2, 2); // apostraphe
    x += INCREMENT_VALUE / 2;
    drawAndrewsResumeLetter(6, x, y);                               // s
    x = 7;
    y += 4;
    
    drawAndrewsResumeLetter(3, x, y);       x += INCREMENT_VALUE;   // r
    drawAndrewsResumeLetter(4, x, y);       x += INCREMENT_VALUE;   // e
    drawAndrewsResumeLetter(6, x, y);       x += INCREMENT_VALUE;   // s
    drawAndrewsResumeLetter(7, x, y);       x += INCREMENT_VALUE;   // u
    drawAndrewsResumeLetter(8, x, y);       x += INCREMENT_VALUE;   // m
    drawAndrewsResumeLetter(4, x, y);                               // e

    // setup background color
    colorizeRange(1,9, RGB24_TO_VDPCOLOR(0x00FF00));

    // setup scrolling modes
    VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_PLANE);

    // place text
    VDP_drawText("Skills", LINK_DRAWTEXT_ARGS(s->linkCoords[0][0], SCROLL_END));
    VDP_drawText("Experience", LINK_DRAWTEXT_ARGS(s->linkCoords[1][0], SCROLL_END));
    VDP_drawText("Education", LINK_DRAWTEXT_ARGS(s->linkCoords[0][1], SCROLL_END));
    VDP_drawText("Contact", LINK_DRAWTEXT_ARGS(s->linkCoords[1][1], SCROLL_END));

    // set scroll if not first load, otherwise set first load to false
    if (firstLoad)
    {
        KLog("yeet");
        firstLoad = FALSE;
        SPR_setVisibility(cursor, HIDDEN);
    }
    else
    {
        VDP_setVerticalScroll(BG_A, SCROLL_END);
        SPR_setVisibility(cursor, VISIBLE);
    }
}

void setupMainSection(Section* s)
{
    // first time bullshit to fix soft reset bugs
    firstLoad = TRUE;
    colorTicker = 0;
    colorChange = 1;
    tileCounter = 0;
    masterTimer = 0;
    horizOffset = 0;

    // DEBUG STRING
    sprintf(s->DBG_STR, "%s", "Main Section");

    // setup scrolling
    verticalScrollValue = -96;
    VDP_setVerticalScroll(BG_A, verticalScrollValue);
    s->updateFunc = &updateMainSection;
    s->loadFunc = &loadMainSection;

    // setup links
    s->links[0][0] = malloc(sizeof(Section));
    s->links[1][0] = malloc(sizeof(Section));
    s->links[0][1] = malloc(sizeof(Section));
    s->links[1][1] = malloc(sizeof(Section));

    setupSkillsSection(s->links[0][0]);
    setupExperienceSection(s->links[1][0]);
    setupEducationSection(s->links[0][1]);
    setupContactSection(s->links[1][1]);

    // coords
    s->linkCoords[0][0][0] = 12;
    s->linkCoords[0][0][1] = 16 * 8;

    s->linkCoords[1][0][0] = 204;
    s->linkCoords[1][0][1] = 16 * 8;

    s->linkCoords[0][1][0] = 12;
    s->linkCoords[0][1][1] = 20 * 8;

    s->linkCoords[1][1][0] = 204;
    s->linkCoords[1][1][1] = 20 * 8;

    setupSkillsSection(s->links[0][0]);
    setupExperienceSection(s->links[1][0]);
    setupEducationSection(s->links[0][1]);
    setupContactSection(s->links[1][1]);

    s->selected[0] = 0;
    s->selected[1] = 0;

    s->maxes[0] = 1;
    s->maxes[1] = 1;
}
