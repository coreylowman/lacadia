#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

typedef enum {
    NONE = 0,
    INFO = 1,
    DEBUG = 2,
    WARNING = 3,
    ERROR = 4,
} LOG_TYPE;

void log(LOG_TYPE type, const char *fmt, ...)

#endif
