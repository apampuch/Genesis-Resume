#include "cavalier_debug.h"

void KLog_H1(const char *text, int hex)
{
    char buffer[512];
    sprintf(buffer, "%s%X", text, hex);
    KLog(buffer);
}

void KLog_H2(const char *text, int hex, const char *text2, int hex2)
{
    char buffer[512];
    sprintf(buffer, "%s%X%s%X", text, hex, text2, hex2);
    KLog(buffer);
}