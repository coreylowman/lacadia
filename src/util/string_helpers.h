#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#include <stdlib.h>

char *string_copy(const char *string);
char *string_copy_n(const char *string, size_t n);

int string_equals(const char *string1,const char *string2);
int string_equals_n(const char *string1,const char *string2, size_t n);

int string_length(const char *string);

#endif