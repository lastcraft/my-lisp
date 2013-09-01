#include "vstring.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

char *vstring(char *format, ...) {
    va_list values;
    va_start(values, format);
    int length = vsnprintf(NULL, 0, format, values);
    va_end(values);
    char *string = (char *)malloc(sizeof(char) * length);
    va_start(values, format);
    vsnprintf(string, length, format, values);
    va_end(values);
    return string;
}
