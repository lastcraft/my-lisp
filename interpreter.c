#include "interpreter.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "exit.h"
#include "function.h"
#include "binding.h"
#include "local.h"
#include "standard_library.h"

static Binding *top_level_binding;
static Object *eval_object(Object *, ErrorHandler, Binding *);
static Object *eval_call(Object *, Object *, ErrorHandler, Binding *);
static Object *eval_identifier(Object *, ErrorHandler, Binding *);
static Object *eval_arguments(Object *, ErrorHandler, Binding *);
static Object *eval_arguments_onto(Object *, Object *, ErrorHandler, Binding *);
static Object *eval_lambda(Object *block, Object *parameters, Object *arguments, ErrorHandler error, Binding *binding);
static Object *apply_built_in(Object *, Object *, ErrorHandler, Binding *);
static Object *apply_lambda(Object *, Object *, ErrorHandler, Binding *);
static Object *execute(Callable, Object *, ErrorHandler, Binding *);
static void bind_parameters(Object *, Object *, ErrorHandler, Binding *);

void create_interpreter(void) {
    declare_nil();
    declare_atoms();
    declare_pair();
    declare_functions();
    declare_exit_code();
    create_local();
    top_level_binding = create_binding(NULL);
    declare_standard_library(top_level_binding);
}

void free_interpreter(void) {
    destroy_local();
    free_binding(top_level_binding);
    free_declarations();
}

Binding *top_level(void) {
    return top_level_binding;
}

Object *eval(Object *object, ErrorHandler error, Binding *binding) {
    Object *result;
    create_local();
    Try {
        result = eval_object(local(clone(object)), error, binding);
    } Catch {
        destroy_local();
        rethrow();
    }
    destroy_local();
    return result;
}

Object *apply(Object *function, Object *arguments, ErrorHandler error, Binding *binding) {
    if (is_built_in(function)) {
        return apply_built_in(function, arguments, error, binding);
    } else if (is_lambda(function)) {
        return apply_lambda(function, arguments, error, binding);
    } else {
        return error("Not a known function type", clone(function));
    }
}

static Object *eval_object(Object *object, ErrorHandler error, Binding *binding) {
    if (is_pair(object)) {
        return eval_call(car(object), cdr(object), error, binding);
    } else if (is_identifier(object)) {
        return eval_identifier(object, error, binding);
    } else {
        return clone(object);
    }
}

static Object *eval_arguments(Object *arguments, ErrorHandler error, Binding *binding) {
    return reverse(eval_arguments_onto(nil(), arguments, error, binding));
}

static Object *eval_arguments_onto(Object *evaluations, Object *arguments, ErrorHandler error, Binding *binding) {
    if (is_nil(arguments)) {
        return evaluations;
    }
    return eval_arguments_onto(pair(eval(car(arguments), error, binding), evaluations),
                               cdr(arguments),
                               error,
                               binding);
}

static Object *eval_call(Object *identifier, Object *arguments, ErrorHandler error, Binding *binding) {
    if (! is_identifier(identifier)) {
        return error("Identifier expected", (void *)clone(identifier));
    }
    Object *function = find(binding, (char *)value(identifier));
    if (function == NULL) {
        return error("Unknown identifier", (void *)clone(identifier));
    }
    if (! is_function(function)) {
        return error("Identifier does not refer to a function", (void *)clone(identifier));
    }
    return apply(local(function), arguments, error, binding);
}

static Object *eval_identifier(Object *identifier, ErrorHandler error, Binding *binding) {
    Object *found = find(binding, (char *)value(identifier));
    return found != NULL ? clone(found) : clone(identifier);
}

static Object *eval_lambda(Object *body, Object *parameters, Object *arguments, ErrorHandler error, Binding *binding) {
    binding = create_binding(binding);
    Object *result = NULL;
    Try {
        bind_parameters(parameters, arguments, error, binding);
        result = eval(clone(body), error, binding);
        free_binding(binding);
    } Catch {
        free_binding(binding);
        return rethrow();
    }
    return result;
}

static Object *apply_built_in(Object *function, Object *arguments, ErrorHandler error, Binding *binding) {
    if (! is_special_form(function)) {
        arguments = local(eval_arguments(arguments, error, binding));
    }
    Callable built_in_code = code((BuiltIn *)value(function));
    return execute(built_in_code, arguments, error, binding);
}

static Object *apply_lambda(Object *function, Object *arguments, ErrorHandler error, Binding *binding) {
    arguments = local(eval_arguments(arguments, error, binding));
    return eval_lambda(body(function), parameters(function), arguments, error, binding);
}

static Object *execute(Callable code, Object *arguments, ErrorHandler error, Binding *binding) {
    return (*code)(arguments, error, binding);
}

static void bind_parameters(Object *parameters, Object *arguments, ErrorHandler error, Binding *binding) {
    if (is_nil(parameters)) {
        return;
    }
    if (is_nil(arguments)) {
        error("Too few arguments for", clone(car(parameters)));
    }
    if (! is_identifier(car(parameters))) {
        error("Bad function parameter", clone(car(parameters)));
    }
    add(binding, (char *)value(car(parameters)), clone(car(arguments)));
    bind_parameters(cdr(parameters), cdr(arguments), error, binding);
}
