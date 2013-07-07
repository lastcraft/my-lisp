#include "standard_library.h"
#include "dictionary.h"
#include "type.h"
#include "function.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"

static Object *set(Object *, ErrorHandler, Dictionary *);
static Object *plus(Object *, ErrorHandler, Dictionary *);

void declare_standard_library(Dictionary *dictionary) {
    add(dictionary, "set", built_in(set));
    add(dictionary, "+", built_in(plus));
}

static Object *set(Object *arguments, ErrorHandler error, Dictionary *dictionary) {
    char *identifier = value(car(arguments));
    add(dictionary, identifier, clone(cdr(arguments)));
    destroy(arguments);
    return nil();
}

static Object *plus(Object *arguments, ErrorHandler error, Dictionary *dictionary) {
    long total = 0L;
    Object *tail;
    while (! is_nil(arguments)) {
        tail = clone(cdr(arguments));
        if (! is_number(car(arguments))) {
            Object *bad_number = clone(car(arguments));
            destroy(arguments);
            destroy(tail);
            error("Not a number", (void *)bad_number);
        }
        total = total + *(long *)value(car(arguments));
        destroy(arguments);
        arguments = tail;
    }
    return number(total);
}
