#include "type.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "stack.h"
#include <stdio.h>
#include <setjmp.h>

extern void create_context(void);
extern void free_context(void);
extern Object *read(void);

static void *error(char *);
static void print(Object *value);
static Object *eval(Object *, void *(*)(char *));
static Object *apply(char *, Object *);

static jmp_buf context;
static char *last_error = NULL;

int main(int argc, char **argv) {
    declare_nil();
    declare_atoms();
    declare_pair();
    create_context();
    if (! setjmp(context)) {
        print(eval(read(), error));
    } else {
        printf("Error! %s\n", last_error);
    }
    free_context();
    free_declarations();
    return 0;
}

static void *error(char *message) {
    last_error = message;
    longjmp(context, 1);
    return NULL;
}

static void print(Object *value) {
    if (value != NULL) {
        write_object(value, (Printf)printf);
        printf("\n");
        destroy(value);
    }
}

static Object *eval(Object *object, void *(*error)(char *)) {
    if (is_pair(object)) {
        if (is_identifier(car(object))) {
            Object *result = apply((char *)value(car(object)), cdr(object));
            destroy(object);
            return result;
        } else {
            return error("Identifier expected");
        }
    } else {
        return object;
    }
}

static Object *apply(char *symbol, Object *values) {
    printf("Applying %s to ", symbol);
    write_object(values, (Printf)printf);
    printf("\n");
    return nil();
}
