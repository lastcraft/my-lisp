#include "atoms.h"
#include "type.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void nil_writer(void *, Printf);
void number_writer(void *, Printf);
void identifier_writer(void *, Printf);
void do_nothing(void *);

void create_atoms(void) {
    nil_type = declare(do_nothing, nil_writer);
    number_type = declare(free, number_writer);
    identifier_type = declare(free, identifier_writer);
}

Object *nil(void) {
    return wrap(nil_type, NULL);
}

Object *number(long l) {
    long *cell = (long *)malloc(sizeof(long));
    *cell = l;
    return wrap(number_type, cell);
}

Object *identifier(char *s) {
    return wrap(identifier_type, strdup(s));
}

void nil_writer(void *nil, Printf printer) {
    printer("nil");
}

void number_writer(void *number, Printf printer) {
    printer("%d", *(long *)number);
}

void identifier_writer(void *string, Printf printer) {
    printer("%s", (char *)string);
}

void do_nothing(void *memory) { }
