#include <genesis.h>

// hue rotation:
// increase R/G/B to cap
// decrease previous to 0
// rinse and repeat

bool DEBUG_COND = TRUE;

void KLog_H1(const char *text, int hex)
{
    if (DEBUG_COND)
    {
        char buffer[512];
        sprintf(buffer, "%s%X", text, hex);
        KLog(buffer);
    }
}

u16 colorize(u16 originalColor, u16 dyeColor)
{
    // calculate greyscale value as highest of rgb values, TODO to test this
    u16 greyscaleValue = max((originalColor & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT, max((originalColor & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT, (originalColor & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT)); 
    // KLog_H1("Raw value: 0x", dyeColor);
    // KLog_H1("Red value: 0x", (dyeColor & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT);
    // KLog_H1("Green value: 0x", (dyeColor & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT);
    // KLog_H1("Blue value: 0x", (dyeColor & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT);
    // KLog_H1("Grey value: 0x", greyscaleValue);
    // KLog("");
    u8 dyeRGB[3] = {  // dye rgb
        (dyeColor & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT,
        (dyeColor & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT,
        (dyeColor & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT
    };
    // KLog_H1("Original color: 0x", originalColor);
    // KLog_H1("Dye color: 0x", dyeColor);

    // normally we'd do something like r = dr * greyscaleValue if we were working with regular-ass floating point numbers, we only have 0-7 though
    // to appoxomate this, we employ the algorithm that is:
    // if bit 3 is set, and the value isn't 0b0100, set the r/g/b to dye minus (7 minus greyscale)
    // otherwise rshift the greyscale value 1 and set it
    for (u8 i=0; i<3; i++)
    {
        if (dyeRGB[i] == 0)
        {
            // if (DEBUG_COND)
            //     KLog_U2("i: ", i, "| set to ", 0);
            dyeRGB[i] = 0;
        }
        else if ((dyeRGB[i]-1) & 0b0100)
        {
            dyeRGB[i] = dyeRGB[i] - min(dyeRGB[i], (7 - greyscaleValue));
        }
        else
        {
            // if (DEBUG_COND)
            //     KLog_U2("i: ", i, "| grey >> 1 = ", greyscaleValue >> 1);
            dyeRGB[i] = greyscaleValue >> 1;
        }
    }

    // KLog_H1("R: 0x", dyeRGB[0]);
    // KLog_H1("G: 0x", dyeRGB[1]);
    // KLog_H1("B: 0x", dyeRGB[2]);

    u16 returnColor = (dyeRGB[0] << VDPPALETTE_REDSFT) + (dyeRGB[1] << VDPPALETTE_GREENSFT) + (dyeRGB[2] << VDPPALETTE_BLUESFT);
    // KLog_H1("Return color: 0x", returnColor);
    // if (DEBUG_COND) KLog("");
    return returnColor;
}

void colorizeRange(u16 start, u16 end, u16 dyeColor)
{
    end = min(63, end); // correct end

    // get old colors
    u16 oldColors[64];

    // array for new colors
    u16 newColors[64];

    // count the colors
    u8 colorCount = end - start;

    PAL_getColors(start, oldColors, colorCount);
    u8 sameColorCount = 0;

    for (int i=0; i<colorCount; i++)
    {
        newColors[i] = colorize(oldColors[i], dyeColor);
        if (newColors[i] == oldColors[i])
            sameColorCount++;
    }

    KLog_U1("Number of same colors:", sameColorCount);
    PAL_setColors(start, newColors, colorCount);
}

u8 RGBNext(u8 i)
{
    i++;
    if (i >= 3) i = 0;
    return i;
}

u8 RGBPrev(u8 i)
{
    i--;
    if (i >= 3) 
        i = 2;  
    return i;
}

// for now, this assumes that S and V are both max
u16 rotateHue(u16 colorToRotate)
{
    u8 RGB[3] = {  // dye rgb
        (colorToRotate & VDPPALETTE_REDMASK) >> VDPPALETTE_REDSFT,
        (colorToRotate & VDPPALETTE_GREENMASK) >> VDPPALETTE_GREENSFT,
        (colorToRotate & VDPPALETTE_BLUEMASK) >> VDPPALETTE_BLUESFT
    };

    // R,G, or B will always be max, so find it
    const int MAX_PRIMARY_VALUE = 0b0111;
    u8 maxxedValue = 4; // bogus initial value
    if (RGB[0] == MAX_PRIMARY_VALUE) maxxedValue = 0;
    else if (RGB[1] == MAX_PRIMARY_VALUE) maxxedValue = 1;
    else maxxedValue = 2;

    // handle two maxxed values
    // if next is also max, decrement maxxedValue
    u8 next = RGBNext(maxxedValue);
    u8 prev = RGBPrev(maxxedValue);
    if (RGB[next] == MAX_PRIMARY_VALUE)
    {
        RGB[maxxedValue]--;
    }
    // if previous value is more than 0, decrement it
    // this will also take care of if 0 and 2 are maxxed
    else if (RGB[prev] > 0)
    {
        RGB[prev]--;
    }
    // else increment the next one
    else
    {
        RGB[next]++;
    }

    return (RGB[0] << VDPPALETTE_REDSFT) + (RGB[1] << VDPPALETTE_GREENSFT) + (RGB[2] << VDPPALETTE_BLUESFT);
}

u16 smoothRotateHue(u16 colorToRotate, u16 brightestColor)
{
    // if brightestColor is black, return one rotate to prevent infinite loop
    if (brightestColor == 0)
        return rotateHue(colorToRotate);

    // rotate the color and colorize the brightest color
    u16 newColor = rotateHue(colorToRotate);
    u16 newBrightest = colorize(brightestColor, colorToRotate);

    KLog_H1("Old Brightest: ", brightestColor);
    KLog_H1("New Brightest: ", newBrightest);

    // if the brightest color is the same as before, do it again until we do it right
    if (newBrightest == brightestColor)
    {
        return smoothRotateHue(newColor, brightestColor);
    }
    else return newColor;
}
