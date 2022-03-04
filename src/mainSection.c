#include "cavalier_debug.h"
#include "color.h"
#include "sections.h"
#include "vdp_manage.h"
#include <resources.h>

void setupSkillsSection(Section* s);
void setupExperienceSection(Section* s);
void setupEducationSection(Section* s);
void setupContactSection(Section* s);

s16 verticalScrollValue = 0;

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
    static u8 masterTimer;

    const s8 TICKER_MAX = 6;
    const s8 TICKER_MIN = -4;
    static s8 colorTicker = 0;
    static s8 colorChange = 1;
    static u8 tileCounter = 0;

    // raise title
    if (verticalScrollValue<60)
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
        if (++masterTimer == TICK_RATE)
        {
            masterTimer = 0;
            colorTicker += colorChange;

            // invert when we reach an end
            if (colorTicker == TICKER_MAX || colorTicker == TICKER_MIN)
                colorChange = -colorChange;

            s8 colorMod = clamp(colorTicker, 0, 5);

            // darken and lighten
            const u8 greens[8] = {0, 1, 2, 3, 4, 5, 6, 7};
            for (u8 i = 0; i < 8; i++)
            {
                u8 colorIndex = i+1;
                u16 colorToSet = greens[i] - min(colorMod, greens[i]);
                colorToSet = colorToSet << VDPPALETTE_GREENSFT;
                PAL_setColor(colorIndex, colorToSet);
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
}

void setupMainSection(Section* s)
{
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
}
