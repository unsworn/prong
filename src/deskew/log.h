#ifndef log_H_
#define log_H_

#include <stdio.h>

void log_trace(int lvl, const char* format, ...);

#define TRACE(msg, ...) log_trace(2, "TRACE: " msg "\n", __VA_ARGS__)
#define INFO(msg, ...)  log_trace(1, "INFO:  " msg "\n", __VA_ARGS__)
#define ERROR(msg, ...) log_trace(0, "ERROR: " msg "\n", __VA_ARGS__)

#endif
