#include "type.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "stack.h"
#include <stdio.h>
#include <setjmp.h>

#define try if (! setjmp(context))
#define catch else

extern void create_context(void);
extern void free_context(void);
extern Object *read(void);

static void *throw(void *);
static void print(Object *value);
static Object *eval(Object *, void *(*)(void *));
static Object *apply(char *, Object *);

static jmp_buf context;
static char *last_exception = NULL;

int main(int argc, char **argv) {
    declare_nil();
    declare_atoms();
    declare_pair();
    create_context();
    try {
        print(eval(read(), throw));
    } catch {
        printf("Error! %s\n", (char *)last_exception);
    }
    free_context();
    free_declarations();
    return 0;
}

static void *throw(void *exception) {
    last_exception = exception;
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

static Object *eval(Object *object, void *(*error)(void *)) {
    if (is_pair(object)) {
        if (is_identifier(car(object))) {
            Object *result = apply((char *)value(car(object)), cdr(object));
            destroy(object);
            return result;
        } else {
            return error((void *)"Identifier expected");
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
