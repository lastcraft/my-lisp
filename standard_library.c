#include "standard_library.h"
#include "interpreter.h"
#include "dictionary.h"
#include "type.h"
#include "function.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "exit.h"

static Object *quit(Object *, ErrorHandler, Dictionary *);
static Object *quote(Object *, ErrorHandler, Dictionary *);
static Object *set(Object *, ErrorHandler, Dictionary *);
static Object *setq(Object *, ErrorHandler, Dictionary *);
static Object *plus(Object *, ErrorHandler, Dictionary *);

void declare_standard_library(Dictionary *dictionary) {
    add(dictionary, "quit", built_in(quit));
    add(dictionary, "exit", built_in(quit));
    add(dictionary, "quote", special_form(built_in(quote)));
    add(dictionary, "set", built_in(set));
    add(dictionary, "setq", special_form(built_in(setq)));
    add(dictionary, "+", built_in(plus));
}

static Object *quit(Object *arguments, ErrorHandler error, Dictionary *dictionary) {
    Object *code = exit_code(is_pair(arguments) && is_number(car(arguments)) ? *(int *)value(car(arguments)) : 0);
    destroy(arguments);
    return error("Quitting", code);
}

static Object *quote(Object *arguments, ErrorHandler error, Dictionary *dictionary) {
    Object *as_is = clone(car(arguments));
    destroy(arguments);
    return as_is;
}

static Object *set(Object *arguments, ErrorHandler error, Dictionary *dictionary) {
    char *identifier = value(car(arguments));
    add(dictionary, identifier, clone(car(cdr(arguments))));
    destroy(arguments);
    return nil();
}

static Object *setq(Object *arguments, ErrorHandler error, Dictionary *dictionary) {
    char *identifier = value(car(arguments));
    add(dictionary, identifier, eval(clone(car(cdr(arguments))), error, dictionary));
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
