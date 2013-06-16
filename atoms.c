#include "atoms.h"
#include "type.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef struct Pair_ {
    Object *car;
    Object *cdr;
} Pair;

static void nil_writer(void *, Printf);
static void number_writer(void *, Printf);
static void identifier_writer(void *, Printf);
static void string_writer(void *, Printf);
static void pair_writer(void *, Printf);
static long last(char *);
static void do_nothing(void *);
static void destroy_pair(void *);

void create_atoms(void) {
    nil_type = declare(do_nothing, nil_writer);
    number_type = declare(free, number_writer);
    identifier_type = declare(free, identifier_writer);
    string_type = declare(free, string_writer);
    pair_type = declare(destroy_pair, pair_writer);
}

Object *nil(void) {
    return wrap(nil_type, NULL);
}

static void do_nothing(void *memory) { }

static void nil_writer(void *nil, Printf printer) {
    printer("nil");
}

Object *number(long number) {
    long *cell = (long *)malloc(sizeof(long));
    *cell = number;
    return wrap(number_type, cell);
}

static void number_writer(void *number, Printf printer) {
    printer("%d", *(long *)number);
}

Object *identifier(char *name) {
    return wrap(identifier_type, strdup(name));
}

static void identifier_writer(void *name, Printf printer) {
    printer("%s", (char *)name);
}

Object *quoted_string(char *quoted) {
    char *string = strdup(quoted + 1);
    string[last(string)] = '\0';
    return wrap(string_type, string);
}

static long last(char *string) {
    return strlen(string) - 1;
}

static void string_writer(void *text, Printf printer) {
    printer("\"%s\"", (char *)text);    
}

Object *pair(Object *car, Object *cdr) {
    Pair *pair = (Pair *)malloc(sizeof(Pair));
    pair->car = car;
    pair->cdr = cdr;
    return wrap(pair_type, (void *)pair);
}

static void destroy_pair(void *pair) {
    destroy(((Pair *)pair)->car);
    destroy(((Pair *)pair)->cdr);
    free(pair);
}

static void pair_writer(void *pair, Printf printer) {
    printer("(");
    write_object(((Pair *)pair)->car, printer);
    printer(" . ");
    write_object(((Pair *)pair)->cdr, printer);
    printer(")");
}

Object *car(Object *object) {
    Pair *pair = (Pair *)value(object);
    return pair->car;
}

Object *cdr(Object *object) {
    Pair *pair = (Pair *)value(object);
    return pair->cdr;    
}
