#include "function.h"
#include <stdlib.h>

struct BuiltIn_ {
    Callable code;
    int special_form;
};

struct Lambda_ {
    Object *arguments;
    Object *body;
    int special_form;
};

static void built_in_writer(void *, Printf);
static void lambda_writer(void *, Printf);

void declare_functions(void) {
    built_in_type = declare(free, built_in_writer);
    lambda_type = declare(free, lambda_writer);
}

Object *built_in(Callable code) {
    BuiltIn *built_in = (BuiltIn *)malloc(sizeof(BuiltIn));
    built_in->code = code;
    built_in->special_form = 0;
    return wrap(built_in_type, (void *)built_in);
}

Object *lambda(Object *arguments, Object *body) {
    Lambda *lambda = (Lambda *)malloc(sizeof(Lambda));
    lambda->arguments = arguments;
    lambda->body = body;
    lambda->special_form = 0;
    return wrap(lambda_type, (void *)lambda);
}

Callable code(BuiltIn *built_in) {
    return built_in->code;
}

int is_function(Object *object) {
    return is_built_in(object) || is_lambda(object);
}

int is_special_form(Object *function) {
    if (is_built_in(function)) {
        return ((BuiltIn *)value(function))->special_form;
    } else {
        return ((Lambda *)value(function))->special_form;
    }
}

int is_built_in(Object *object) {
    return is_a(built_in_type, object);
}

int is_lambda(Object *object) {
    return is_a(lambda_type, object);
}

static void built_in_writer(void *built_in, Printf printer) {
    printer("<Built-in: %p>", ((BuiltIn *)value((Object *)built_in))->code);
}

static void lambda_writer(void *lambda, Printf printer) {
    printer("<Lambda: ");
    write_object(((Lambda *)lambda)->arguments, printer);
    printer(" ");
    write_object(((Lambda *)lambda)->body, printer);
    printer(">");
}
