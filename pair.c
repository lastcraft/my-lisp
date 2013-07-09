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
static void write_cdr(Object *, Printf);
static void destroy_pair(void *);
static Object *reverse_onto(Object *, Object *);

void declare_pair(void) {
    pair_type = declare(destroy_pair, pair_writer);
}

Object *pair(Object *car, Object *cdr) {
    Pair *pair = (Pair *)malloc(sizeof(Pair));
    pair->car = car;
    pair->cdr = cdr;
    return wrap(pair_type, (void *)pair);
}

int is_pair(Object *object) {
    return is_a(pair_type, object);
}

static void destroy_pair(void *pair) {
    destroy(((Pair *)pair)->car);
    destroy(((Pair *)pair)->cdr);
    free(pair);
}

static void pair_writer(void *pair, Printf printer) {
    printer("(");
    write_object(((Pair *)pair)->car, printer);
    write_cdr(((Pair *)pair)->cdr, printer);
    printer(")");
}

static void write_cdr(Object *object, Printf printer) {
    if (is_nil(object)) {
    } else if (is_pair(object)) {
        printer(" ");
        write_object(car(object), printer);
        write_cdr(cdr(object), printer);
    } else {
        printer(" . ");
        write_object(object, printer);
    }
}

Object *car(Object *object) {
    Pair *pair = (Pair *)value(object);
    return pair->car;
}

Object *cdr(Object *object) {
    Pair *pair = (Pair *)value(object);
    return pair->cdr;    
}

Object *reverse(Object *object) {
    if (! is_pair(object)) {
        return object;
    }
    return reverse_onto(nil(), object);
}

static Object *reverse_onto(Object *target, Object *source) {
    if (is_nil(source)) {
        return target;
    }
    Object *head = clone(car(source));
    Object *tail = clone(cdr(source));
    destroy(source);
    return reverse_onto(pair(head, target), tail);
}