#include "atoms.h"
#include "type.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void number_writer(void *, Printf);
void string_writer(void *, Printf);

void create_atoms(void) {
    number_type = declare(free, number_writer);
    string_type = declare(free, string_writer);
}

Object *number(long l) {
    long *cell = (long *)malloc(sizeof(long));
    *cell = l;
    return wrap(number_type, cell);
}

Object *string(char *s) {
    return wrap(number_type, strdup(s));
}

void number_writer(void *number, Printf printer) {
    printer("%d", *(long *)number);
}

void string_writer(void *string, Printf printer) {
    printer("%s", (char *)string);
}
