#include "type.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "stack.h"
#include "exception.h"
#include <stdio.h>

extern void create_reader(void);
extern void free_reader(void);
extern Object *read(void);

static void print(Object *value);
static void print_error(char *, Object *);
static Object *eval(Object *, Throw);
static Object *apply(char *, Object *);

int main(int argc, char **argv) {
    declare_nil();
    declare_atoms();
    declare_pair();
    create_reader();
    Try {
        print(eval(read(), throw_exception));
    } Catch {
        print_error(exception_message(), (Object *)exception_information());
    }
    free_reader();
    free_declarations();
    return 0;
}

static void print(Object *value) {
    if (value != NULL) {
        write_object(value, (Printf)printf);
        printf("\n");
        destroy(value);
    }
}

static void print_error(char *message, Object *object) {
    printf("%s for object ", message);
    write_object(object, (Printf)printf);
    printf("\n");
    destroy(object);
}

static Object *eval(Object *object, Throw throw_exception) {
    if (is_pair(object)) {
        if (is_identifier(car(object))) {
            Object *result = apply((char *)value(car(object)), cdr(object));
            destroy(object);
            return result;
        } else {
            Object *wrong_object = clone(car(object));
            destroy(object);
            return throw_exception("Identifier expected", (void *)wrong_object);
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
