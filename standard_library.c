#include "standard_library.h"
#include "interpreter.h"
#include "binding.h"
#include "type.h"
#include "function.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "exit.h"

static Object *quit(Object *, ErrorHandler, Binding *);
static Object *quote(Object *, ErrorHandler, Binding *);
static Object *set(Object *, ErrorHandler, Binding *);
static Object *setq(Object *, ErrorHandler, Binding *);
static Object *plus(Object *, ErrorHandler, Binding *);

void declare_standard_library(Binding *binding) {
    add(binding, "quit", built_in(quit));
    add(binding, "exit", built_in(quit));
    add(binding, "quote", special_form(built_in(quote)));
    add(binding, "set", built_in(set));
    add(binding, "setq", special_form(built_in(setq)));
    add(binding, "+", built_in(plus));
}

static Object *quit(Object *arguments, ErrorHandler error, Binding *binding) {
    Object *code = exit_code(is_pair(arguments) && is_number(car(arguments)) ? *(int *)value(car(arguments)) : 0);
    destroy(arguments);
    return error("Quitting", code);
}

static Object *quote(Object *arguments, ErrorHandler error, Binding *binding) {
    Object *as_is = clone(car(arguments));
    destroy(arguments);
    return as_is;
}

static Object *set(Object *arguments, ErrorHandler error, Binding *binding) {
    char *identifier = value(car(arguments));
    add(binding, identifier, clone(car(cdr(arguments))));
    destroy(arguments);
    return nil();
}

static Object *setq(Object *arguments, ErrorHandler error, Binding *binding) {
    char *identifier = value(car(arguments));
    add(binding, identifier, eval(clone(car(cdr(arguments))), error, binding));
    destroy(arguments);
    return nil();
}

static Object *plus(Object *arguments, ErrorHandler error, Binding *binding) {
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
