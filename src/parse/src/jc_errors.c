

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "jc_errors.h"

static mapError g_tLastError = MAP_SUCCESS;
static char g_szLastErrorDesc[128] = "No error";

void mapSetLastError(mapError tError,const char *szDesc,...)
{
    va_list tValist;

    if(szDesc != NULL)
    {
        va_start(tValist,szDesc);
        vsprintf(g_szLastErrorDesc,szDesc,tValist);
        va_end(tValist);
    }
    else
    {
        g_szLastErrorDesc[0] = '\0';
    }

    g_tLastError = tError;
}
mapError mapGetLastError(void)
{
    return g_tLastError;
}
char *mapGetLastErrorDesc(void)
{
    return g_szLastErrorDesc;
}
