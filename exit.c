#include "exit.h"
#include "type.h"
#include <stdlib.h>

static void exit_code_writer(void *, Printf);

void declare_exit_code(void) {
    declare(free, exit_code_writer);
}

Object *exit_code(int code) {
    int *cell = (int *)malloc(sizeof(int));
    *cell = code;
    return wrap(exit_code_type, cell);
}

int is_exit_code(Object *object) {
    return is_a(exit_code_type, object);
}

static void exit_code_writer(void *code, Printf printer) {
    printer("%d", *(int *)code);
}
