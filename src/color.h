#pragma once
#include <genesis.h>

u16 colorize(u16 originalColor, u16 dyeColor);
void colorizeRange(u16 start, u16 end, u16 dyeColor);
u16 smoothRotateHue(u16 colorToRotate, u16 brightestColor);
u16 rotateHue(u16 colorToRotate);