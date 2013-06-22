#include "pair.h"
#include "type.h"
#include "nil.h"
#include <stdlib.h>
#include <stdarg.h>

typedef struct Pair_ {
    Object *car;
    Object *cdr;
} Pair;

static void pair_writer(void *, Printf);
static void destroy_pair(void *);

void declare_pair(void) {
    pair_type = declare(destroy_pair, pair_writer);
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
    if (! is_nil(((Pair *)pair)->cdr)) {
        printer(" . ");
        write_object(((Pair *)pair)->cdr, printer);
    }
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
