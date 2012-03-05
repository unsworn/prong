#include <stdarg.h>

#include "log.h"

#if DEBUG
int gLogVerbose = 2;
#else
int gLogVerbose = 0;
#endif

void
log_trace(int lvl, const char* format, ...)
{

    if (lvl > gLogVerbose)
        return ;
    
    va_list stdl;
    va_start(stdl, format);
    vfprintf(stderr, format, stdl);
    va_end(stdl);    
}

void
log_verbose(int v)
{
    gLogVerbose = v;
}
