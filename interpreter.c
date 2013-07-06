#include "interpreter.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "function.h"
#include "dictionary.h"

static Dictionary *dictionary;

void create_interpreter(void) {
    declare_nil();
    declare_atoms();
    declare_pair();
    declare_functions();
    dictionary = create_dictionary();
}

void free_interpreter(void) {
    free_dictionary(dictionary);
    free_declarations();
}

Object *eval(Object *object, ErrorHandler error) {
    if (is_pair(object)) {
        Object *result = eval_function(clone(car(object)), clone(cdr(object)), error);
        destroy(object);
        return result;
    } else {
        return object;
    }
}

Object *eval_function(Object *identifier, Object *arguments, ErrorHandler error) {
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
    return apply(function, arguments, error);
}

Object *apply(Object *function, Object *arguments, ErrorHandler error) {
    return nil();
}
