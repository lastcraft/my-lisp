#include "interpreter.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "exit.h"
#include "function.h"
#include "binding.h"
#include "standard_library.h"

static Binding *top_level_binding;
static Object *eval_call(Object *, Object *, ErrorHandler, Binding *);
static Object *eval_identifier(Object *, ErrorHandler, Binding *);
static Object *eval_arguments(Object *, ErrorHandler, Binding *);
static Object *eval_arguments_onto(Object *, Object *, ErrorHandler, Binding *);
static Object *execute(Callable, Object *, ErrorHandler, Binding *);
static Object *eval_lambda(Object *block, Object *parameters, Object *arguments, ErrorHandler error, Binding *binding);
static void bind_parameters(Object *, Object *, ErrorHandler, Binding *);

void create_interpreter(void) {
    declare_nil();
    declare_atoms();
    declare_pair();
    declare_functions();
    declare_exit_code();
    top_level_binding = create_binding(NULL);
    declare_standard_library(top_level_binding);
}

void free_interpreter(void) {
    free_binding(top_level_binding);
    free_declarations();
}

Binding *top_level(void) {
    return top_level_binding;
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
        Callable built_in_code = code((BuiltIn *)value(function));
        destroy(function);
        return execute(built_in_code, arguments, error, binding);
    } else if (is_lambda(function)) {
        arguments = eval_arguments(arguments, error, binding);
        Object *lambda_body = body(function);
        Object *lambda_parameters = parameters(function);
        destroy(function);
        return eval_lambda(lambda_body, lambda_parameters, arguments, error, binding);
    } else {
        destroy(arguments);
        return error("Not a known function type", function);
    }
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
    return apply(clone(function), arguments, error, binding);
}

static Object *eval_identifier(Object *identifier, ErrorHandler error, Binding *binding) {
    Object *found;
    if (! (found = find(binding, (char *)value(identifier)))) {
        return identifier;
    } else {
        destroy(identifier);
        return clone(found);
    }
}

static Object *execute(Callable code, Object *arguments, ErrorHandler error, Binding *binding) {
    return (*code)(arguments, error, binding);
}

static Object *eval_lambda(Object *body, Object *parameters, Object *arguments, ErrorHandler error, Binding *binding) {
    bind_parameters(parameters, arguments, error, binding);
    return eval(body, error, binding);
}

static void bind_parameters(Object *parameters, Object *arguments, ErrorHandler error, Binding *binding) {
    if (is_nil(parameters)) {
        return;
    }
    if (is_nil(arguments)) {
        destroy(arguments);
        error("Too few arguments for", clone(car(parameters)));
    }
    if (! is_identifier(car(parameters))) {
        destroy(arguments);
        error("Bad function parameter", clone(car(parameters)));
    }
    Object *argument = clone(car(arguments));
    add(binding, (char *)value(car(parameters)), argument);
    Object *remaining_arguments = clone(cdr(arguments));
    destroy(arguments);
    return bind_parameters(cdr(parameters), remaining_arguments, error, binding);
}
