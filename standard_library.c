#include "standard_library.h"
#include "interpreter.h"
#include "local.h"
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
static Object *numerically_equal(Object *, ErrorHandler, Binding *);
static Object *plus(Object *, ErrorHandler, Binding *);
static Object *minus(Object *, ErrorHandler, Binding *);
static Object *nil_p(Object *, ErrorHandler, Binding *);
static Object *type_to_string(Object *, ErrorHandler, Binding *);
static Object *set_value(Object *, Object *, ErrorHandler, Binding *);
static Object *overwrite_value(Object *, Object *, Binding *);
static int is_argument_list(Object *);
static bool compare_numbers(long, Object *, ErrorHandler);

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
    add(binding, "=", built_in(numerically_equal));
    add(binding, "+", built_in(plus));
    add(binding, "-", built_in(minus));
    add(binding, "nil?", built_in(nil_p));
    add(binding, "type->string", built_in(type_to_string));
}

static Object *quit(Object *arguments, ErrorHandler error, Binding *binding) {
    int code = 0;
    if (is_pair(arguments) && is_number(car(arguments))) {
        code = *(int *)value(car(arguments));
    }
    return error("Quitting", exit_code(code));
}

static Object *quote(Object *arguments, ErrorHandler error, Binding *binding) {
    return clone(car(arguments));
}

static Object *set(Object *arguments, ErrorHandler error, Binding *binding) {
    return set_value(car(arguments), cadr(arguments), error, binding);
}

static Object *set_pling(Object *arguments, ErrorHandler error, Binding *binding) {
    return overwrite_value(car(arguments), cadr(arguments), binding);
}

static Object *setq(Object *arguments, ErrorHandler error, Binding *binding) {
    return set_value(car(arguments),
                     local(eval(cadr(arguments), error, binding)),
                     error,
                     binding);
}

static Object *setq_pling(Object *arguments, ErrorHandler error, Binding *binding) {
    return overwrite_value(car(arguments),
                           local(eval(cadr(arguments), error, binding)),
                           binding);
}

static Object *lambda_built_in(Object *arguments, ErrorHandler error, Binding *binding) {
    Object *parameters = car(arguments);
    if (! is_argument_list(parameters)) {
        return error("Not an argument list", clone(parameters));
    }
    Object *block = cadr(arguments);
    if (! is_pair(block)) {
        return error("Not a code block", clone(block));
    }
    return lambda(clone(parameters), clone(block));
}

static Object *defun(Object *arguments, ErrorHandler error, Binding *binding) {
    Object *symbol = car(arguments);
    if (! is_identifier(symbol)) {
        return error("Not an identifier", clone(symbol));
    }
    Object *new_lambda = lambda_built_in(cdr(arguments), error, binding);
    return overwrite_value(symbol, new_lambda, binding);
}

static Object *branch(Object *arguments, ErrorHandler error, Binding *binding) {
    bool condition = is_true(eval(car(arguments), error, binding));
    Object *true_block = cadr(arguments);
    if (is_nil(true_block)) {
        return error("No block to execute", clone(true_block));
    }
    if (condition) {
        return eval(true_block, error, binding);
    }
    Object *false_block = caddr(arguments);
    if (! is_nil(false_block)) {
        return eval(false_block, error, binding);
    }
    return nil();
}

static Object *numerically_equal(Object *arguments, ErrorHandler error, Binding *binding) {
    if (is_nil(arguments)) {
        return error("Nothing to compare", nil());
    }
    Object *initial = car(arguments);
    if (! is_number(initial)) {
        return error("Must be a number", clone(initial));
    }
    long comparison = *(long *)value(initial);
    return boolean(compare_numbers(comparison, cdr(arguments), error));
}

static Object *plus(Object *arguments, ErrorHandler error, Binding *binding) {
    long total = 0L;
    if (is_nil(arguments)) {
        return number(0L);
    }
    if (! is_number(car(arguments))) {
        return error("Not a number", (void *)car(arguments));
    }
    return number(*(long *)value(car(arguments))
                  + *(long *)value(plus(cdr(arguments), error, binding)));
}

static Object *minus(Object *arguments, ErrorHandler error, Binding *binding) {
    if (is_nil(arguments)) {
        return error("Starting value needed for subtraction", nil());
    }
    Object *first = car(arguments);
    if (! is_number(first)) {
        return error("Number expected", clone(first));
    }
    if (is_nil(cdr(arguments))) {
        return clone(first);
    }
    Object *second = cadr(arguments);
    if (! is_number(second)) {
        return error("Number expected", clone(second));
    }
    return number(*(long *)value(first) - *(long *)value(second));
}

static Object *nil_p(Object *arguments, ErrorHandler error, Binding *binding) {
    if (is_nil(arguments)) {
        return error("Argument needed for unary operator nil?", nil());
    }
    return boolean(is_nil(car(arguments)));
}

static Object *type_to_string(Object *arguments, ErrorHandler error, Binding *binding) {
    if (is_nil(arguments)) {
        return error("Argument needed for unary operator nil?", nil());
    }
    return unquoted_string(type_name(car(arguments)));
}

static Object *set_value(Object *symbol, Object *rvalue, ErrorHandler error, Binding *binding) {
    if (! is_identifier(symbol)) {
        return error("Not an identifier", clone(symbol));
    }
    char *identifier = (char *)value(symbol);
    if (NULL != find(binding, identifier)) {
        return error("Already declared", clone(symbol));
    }
    add(binding, identifier, clone(rvalue));
    return nil();
}

static Object *overwrite_value(Object *symbol, Object *rvalue, Binding *binding) {
    overwrite(binding, (char *)value(symbol), clone(rvalue));
    return nil();
}

static int is_argument_list(Object *list) {
    return is_nil(list) || is_pair(list);
}

static bool compare_numbers(long comparison, Object *arguments, ErrorHandler error) {
    if (is_nil(arguments)) {
        return true;
    }
    Object *first = car(arguments);
    if (! is_number(first)) {
        return error("Must be a number", clone(first));
    }
    if (comparison != *(long *)value(first)) {
        return false;
    }
    return compare_numbers(comparison, cdr(arguments), error);
}
