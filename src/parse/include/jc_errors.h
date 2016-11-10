


#ifndef __JC_ERRORS_H
#define __JC_ERRORS_H

#ifdef __cplusplus
extern "C"
{
#endif

// #if defined(JCDEBUG)
//     #define JC_DEBUG_PRINTF(fmt,args...) ((void)0)
// #else
//     #define JC_DEBUG_PRINTF(fmt,args...) printf("%s:%d:",fmt,__FUNCTION__,__LINE__,##args);
// #endif

#define JC_DEBUG_PRINTF(fmt,args...) printf("%s:%d:",fmt,__FUNCTION__,__LINE__,##args);

typedef enum mapError
{
    MAP_SUCCESS = 0,
    MAP_ERROR_INVALID_PARAMETER,
    MAP_ERROR_NO_SUCH_FILE,
    MAP_ERROR_BAD_FILE_FORMAT,
}mapError;

extern void mapSetLastError(mapError,const char *szDesc,...);
extern mapError mapGetLastError(void);
extern char *mapGetLastErrorDesc(void);

#ifdef __cplusplus
}
#endif

#endif
