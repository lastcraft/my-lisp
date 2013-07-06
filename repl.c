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
static Object *eval(Object *, ErrorHandler);
static Object *eval_function(Object *, Object *, ErrorHandler);
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

static Object *eval(Object *object, ErrorHandler throw_exception) {
    if (is_pair(object)) {
        Object *result = eval_function(clone(car(object)), clone(cdr(object)), throw_exception);
        destroy(object);
        return result;
    } else {
        return object;
    }
}

static Object *eval_function(Object *identifier, Object *arguments, ErrorHandler throw_exception) {
    if (is_identifier(identifier)) {
        return apply((char *)value(identifier), arguments);
    } else {
        destroy(arguments);
        return throw_exception("Identifier expected", (void *)identifier);
    }
}

static Object *apply(char *symbol, Object *arguments) {
    printf("Applying %s to ", symbol);
    write_object(arguments, (Printf)printf);
    printf("\n");
    return nil();
}
