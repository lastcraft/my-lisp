#include "interpreter.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "exit.h"
#include "function.h"
#include "dictionary.h"
#include "standard_library.h"

static Dictionary *dictionary;
static Object *eval_call(Object *, Object *, ErrorHandler, Dictionary *);
static Object *eval_identifier(Object *, ErrorHandler, Dictionary *);
static Object *execute(Callable, Object *, ErrorHandler, Dictionary *);
static Object *eval_arguments(Object *, ErrorHandler, Dictionary *);
static Object *eval_arguments_onto(Object *, Object *, ErrorHandler, Dictionary *);

void create_interpreter(void) {
    declare_nil();
    declare_atoms();
    declare_pair();
    declare_functions();
    declare_exit_code();
    dictionary = create_dictionary();
    declare_standard_library(dictionary);
}

void free_interpreter(void) {
    free_dictionary(dictionary);
    free_declarations();
}

Dictionary *top_level(void) {
    return dictionary;
}

Object *eval(Object *object, ErrorHandler error, Dictionary *dictionary) {
    if (is_pair(object)) {
        Object *result = eval_call(clone(car(object)), clone(cdr(object)), error, dictionary);
        destroy(object);
        return result;
    } else if (is_identifier(object)) {
        return eval_identifier(object, error, dictionary);
    } else {
        return object;
    }
}

Object *apply(Object *function, Object *arguments, ErrorHandler error, Dictionary *dictionary) {
    if (is_built_in(function)) {
        return execute((Callable)value(function), arguments, error, dictionary);
    }
    return nil();
}

static Object *eval_arguments(Object *arguments, ErrorHandler error, Dictionary *dictionary) {
    return eval_arguments_onto(nil(), arguments, error, dictionary);
}

static Object *eval_arguments_onto(Object *target, Object *source, ErrorHandler error, Dictionary *dictionary) {
    if (is_nil(source)) {
        return target;
    }
    Object *evaluated = eval(clone(car(source)), error, dictionary);
    Object *remaining = clone(cdr(source));
    destroy(source);
    return eval_arguments_onto(pair(evaluated, target), remaining, error, dictionary);
}

static Object *eval_call(Object *identifier, Object *arguments, ErrorHandler error, Dictionary *dictionary) {
    if (! is_identifier(identifier)) {
        destroy(arguments);
        return error("Identifier expected", (void *)identifier);
    }
    Object *function;
    if (! (function = find(dictionary, (char *)value(identifier)))) {
        destroy(arguments);
        return error("Unknown identifier", (void *)identifier);
    }
    if (! is_function(function)) {
        destroy(arguments);
        return error("Identifier does not refer to a function", (void *)identifier);
    }
    destroy(identifier);
    return apply(function, arguments, error, dictionary);
}

static Object *eval_identifier(Object *identifier, ErrorHandler error, Dictionary *dictionary) {
    Object *found;
    if (! (found = find(dictionary, (char *)value(identifier)))) {
        return error("Unkown identifier", identifier);
    } else {
        return clone(found);
    }
}

static Object *execute(Callable built_in, Object *arguments, ErrorHandler error, Dictionary *dictionary) {
    return (*built_in)(arguments, error, dictionary);
}
