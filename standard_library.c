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
static Object *set_pling(Object *, ErrorHandler, Binding *);
static Object *setq(Object *, ErrorHandler, Binding *);
static Object *setq_pling(Object *, ErrorHandler, Binding *);
static Object *lambda_built_in(Object *, ErrorHandler, Binding *);
static Object *defun(Object *, ErrorHandler, Binding *);
static Object *branch(Object *, ErrorHandler, Binding *);
static Object *plus(Object *, ErrorHandler, Binding *);
static Object *set_value(Object *, Object *, ErrorHandler, Binding *);
static Object *overwrite_value(Object *, Object *, Binding *);
static int is_argument_list(Object *);

void declare_standard_library(Binding *binding) {
    add(binding, "quit", built_in(quit));
    add(binding, "exit", built_in(quit));
    add(binding, "quote", special_form(built_in(quote)));
    add(binding, "set", built_in(set));
    add(binding, "set!", built_in(set_pling));
    add(binding, "setq", special_form(built_in(setq)));
    add(binding, "setq!", special_form(built_in(setq_pling)));
    add(binding, "lambda", special_form(built_in(lambda_built_in)));
    add(binding, "defun", special_form(built_in(defun)));
    add(binding, "if", special_form(built_in(branch)));
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
    Object *symbol = clone(car(arguments));
    Object *rvalue = clone(car(cdr(arguments)));
    destroy(arguments);
    return set_value(symbol, rvalue, error, binding);
}

static Object *set_pling(Object *arguments, ErrorHandler error, Binding *binding) {
    Object *symbol = clone(car(arguments));
    Object *rvalue = clone(car(cdr(arguments)));
    destroy(arguments);
    return overwrite_value(symbol, rvalue, binding);
}

static Object *setq(Object *arguments, ErrorHandler error, Binding *binding) {
    Object *symbol = clone(car(arguments));
    Object *rvalue = eval(clone(car(cdr(arguments))), error, binding);
    destroy(arguments);
    return set_value(symbol, rvalue, error, binding);
}

static Object *setq_pling(Object *arguments, ErrorHandler error, Binding *binding) {
    Object *symbol = clone(car(arguments));
    Object *rvalue = eval(clone(car(cdr(arguments))), error, binding);
    destroy(arguments);
    return overwrite_value(symbol, rvalue, binding);
}

static Object *lambda_built_in(Object *arguments, ErrorHandler error, Binding *binding) {
    Object *parameters = clone(car(arguments));
    if (! is_argument_list(parameters)) {
        destroy(arguments);
        return error("Not an argument list", parameters);
    }
    Object *block = clone(car(cdr(arguments)));
    if (! is_pair(block)) {
        destroy(parameters);
        destroy(arguments);
        return error("Not a code block", block);
    }
    return lambda(parameters, block);
}

static Object *defun(Object *arguments, ErrorHandler error, Binding *binding) {
    Object *symbol = clone(car(arguments));
    if (! is_identifier(symbol)) {
        destroy(arguments);
        return error("Not an identifier", symbol);
    }
    Object *lambda_arguments = clone(cdr(arguments));
    destroy(arguments);
    Object *new_lambda = lambda_built_in(lambda_arguments, error, binding);
    return overwrite_value(symbol, new_lambda, binding);
}

static Object *branch(Object *arguments, ErrorHandler error, Binding *binding) {
    bool condition;
    Try {
        Object *argument_result = eval(clone(car(arguments)), error, binding);
        condition = is_true(argument_result);
    } Catch {
        destroy(arguments);
        return rethrow();
    }
    Object *true_block = clone(car(cdr(arguments)));
    if (is_nil(true_block)) {
        destroy(arguments);
        return error("No block to execute", true_block);
    } else if (condition) {
        destroy(arguments);
        return eval(true_block, error, binding);
    }
    destroy(true_block);
    Object *false_block = clone(car(cdr(cdr(arguments))));
    destroy(arguments);
    if (! is_nil(false_block)) {
        return eval(false_block, error, binding);
    }
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
            return error("Not a number", (void *)bad_number);
        }
        total = total + *(long *)value(car(arguments));
        destroy(arguments);
        arguments = tail;
    }
    return number(total);
}

static Object *set_value(Object *symbol, Object *rvalue, ErrorHandler error, Binding *binding) {
    if (! is_identifier(symbol)) {
        destroy(rvalue);
        return error("Not an identifier", symbol);
    }
    char *identifier = (char *)value(symbol);
    if (NULL != find(binding, identifier)) {
        destroy(rvalue);
        return error("Already declared", symbol);
    }
    add(binding, identifier, rvalue);
    destroy(symbol);
    return nil();
}

static Object *overwrite_value(Object *symbol, Object *rvalue, Binding *binding) {
    overwrite(binding, (char *)value(symbol), rvalue);
    destroy(symbol);
    return nil();
}

static int is_argument_list(Object *list) {
    return 1;
}
