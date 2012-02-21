#ifndef log_H_
#define log_H_

#include <stdio.h>
      
#if DEBUG

    #define TRACE(msg, ...) fprintf(stderr, "TRACE: " msg "\n", __VA_ARGS__ )
    
#else                                                           

    #define TRACE(msg, ...)

#endif

#endif