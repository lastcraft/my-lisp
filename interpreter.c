#include "interpreter.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "exit.h"
#include "function.h"
#include "binding.h"
#include "standard_library.h"

static Binding *binding;
static Object *eval_call(Object *, Object *, ErrorHandler, Binding *);
static Object *eval_identifier(Object *, ErrorHandler, Binding *);
static Object *eval_arguments(Object *, ErrorHandler, Binding *);
static Object *eval_arguments_onto(Object *, Object *, ErrorHandler, Binding *);
static Object *execute(Callable, Object *, ErrorHandler, Binding *);

void create_interpreter(void) {
    declare_nil();
    declare_atoms();
    declare_pair();
    declare_functions();
    declare_exit_code();
    binding = create_binding(NULL);
    declare_standard_library(binding);
}

void free_interpreter(void) {
    free_binding(binding);
    free_declarations();
}

Binding *top_level(void) {
    return binding;
}

Object *eval(Object *object, ErrorHandler error, Binding *binding) {
    if (is_pair(object)) {
        Object *result = eval_call(clone(car(object)), clone(cdr(object)), error, binding);
        destroy(object);
        return result;
    } else if (is_identifier(object)) {
        return eval_identifier(object, error, binding);
    } else {
        return object;
    }
}

Object *apply(Object *function, Object *arguments, ErrorHandler error, Binding *binding) {
    if (is_built_in(function)) {
        if (! is_special_form(function)) {
            arguments = eval_arguments(arguments, error, binding);
        }
        return execute(code((BuiltIn *)value(function)), arguments, error, binding);
    }
    return nil();
}

static Object *eval_arguments(Object *arguments, ErrorHandler error, Binding *binding) {
    return reverse(eval_arguments_onto(nil(), arguments, error, binding));
}

static Object *eval_arguments_onto(Object *target, Object *source, ErrorHandler error, Binding *binding) {
    if (is_nil(source)) {
        return target;
    }
    Object *evaluated = eval(clone(car(source)), error, binding);
    Object *remaining = clone(cdr(source));
    destroy(source);
    return eval_arguments_onto(pair(evaluated, target), remaining, error, binding);
}

static Object *eval_call(Object *identifier, Object *arguments, ErrorHandler error, Binding *binding) {
    if (! is_identifier(identifier)) {
        destroy(arguments);
        return error("Identifier expected", (void *)identifier);
    }
    Object *function;
    if (! (function = find(binding, (char *)value(identifier)))) {
        destroy(arguments);
        return error("Unknown identifier", (void *)identifier);
    }
    if (! is_function(function)) {
        destroy(arguments);
        return error("Identifier does not refer to a function", (void *)identifier);
    }
    destroy(identifier);
    return apply(function, arguments, error, binding);
}

static Object *eval_identifier(Object *identifier, ErrorHandler error, Binding *binding) {
    Object *found;
    if (! (found = find(binding, (char *)value(identifier)))) {
        return error("Unkown identifier", identifier);
    } else {
        return clone(found);
    }
}

static Object *execute(Callable code, Object *arguments, ErrorHandler error, Binding *binding) {
    return (*code)(arguments, error, binding);
}
