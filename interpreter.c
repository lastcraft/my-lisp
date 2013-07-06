#include "interpreter.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "dictionary.h"
#include <stdio.h>

static Dictionary *dictionary;

void create_interpreter(void) {
    declare_nil();
    declare_atoms();
    declare_pair();
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
    if (is_identifier(identifier)) {
        return apply((char *)value(identifier), arguments);
    } else {
        destroy(arguments);
        return error("Identifier expected", (void *)identifier);
    }
}

Object *apply(char *symbol, Object *arguments) {
    printf("Applying %s to ", symbol);
    write_object(arguments, (Printf)printf);
    printf("\n");
    return nil();
}
