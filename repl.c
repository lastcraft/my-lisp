#include "type.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "stack.h"
#include <stdio.h>

extern void create_context(void);
extern void free_context(void);
extern Object *read(void);

static void print(Object *value);
static Object *eval(Object *);
static Object *apply(char *, Object *);

int main(int argc, char **argv) {
    declare_nil();
    declare_atoms();
    declare_pair();
    create_context();
    print(eval(read()));
    free_context();
    free_declarations();
    return 0;
}

static void print(Object *value) {
    if (value != NULL) {
        write_object(value, (Printf)printf);
        printf("\n");
    }
}

static Object *eval(Object *object) {
    if (is_pair(object)) {
        if (is_identifier(car(object))) {
            Object *result = apply((char *)value(car(object)), cdr(object));
            destroy(object);
            return result;
        } else {
            printf("Identifier expected\n");
            return NULL;
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
