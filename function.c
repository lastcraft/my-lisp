#include "function.h"
#include <stdlib.h>

typedef struct Lambda_ {
    Object *arguments;
    Object *body;
} Lambda;

static void built_in_writer(void *, Printf);
static void lambda_writer(void *, Printf);
static void do_nothing(void *);

void declare_functions(void) {
    built_in_type = declare(do_nothing, built_in_writer);
    lambda_type = declare(free, lambda_writer);
}

Object *built_in(Callable code) {
    return wrap(built_in_type, (void *)code);
}

Object *lambda(Object *arguments, Object *body) {
    Lambda *lambda = (Lambda *)malloc(sizeof(Lambda));
    lambda->arguments = arguments;
    lambda->body = body;
    return wrap(lambda_type, (void *)lambda);
}

int is_function(Object *object) {
    return is_a(built_in_type, object) || is_a(lambda_type, object);
}

static void built_in_writer(void *built_in, Printf printer) {
    printer("<Built-in: %p>", (Callable)value((Object *)built_in));
}

static void lambda_writer(void *lambda, Printf printer) {
    printer("<Lambda: ");
    write_object(((Lambda *)lambda)->arguments, printer);
    printer(" ");
    write_object(((Lambda *)lambda)->body, printer);
    printer(">");
}

static void do_nothing(void *memory) { }
