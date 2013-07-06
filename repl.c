#include "type.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "stack.h"
#include "exception.h"
#include "interpreter.h"
#include <stdio.h>

extern void create_reader(void);
extern void free_reader(void);
extern Object *read(void);

static void print(Object *value);
static void print_error(char *, Object *);

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
