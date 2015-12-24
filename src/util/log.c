#include <stdio.h>
#include "log.h"

extern LOG_TYPE log_type;

void log(LOG_TYPE type, const char *fmt, ...){
    if(type > log_type || type == NONE) return;

    va_list ap;
    va_start(ap, fmt);
    
    switch(type){
        case INFO:
            printf("INFO: ");
            break;
        case DEBUG:
            printf("DEBUG: ");
            break;
        case WARNING:
            printf("WARNING: ");
            break;
        case ERROR:
            printf("ERROR: ");
            break;
    }

    vprintf(fmt, ap);
    printf("\n");
    
    va_end(ap);
}

