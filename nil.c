#include "nil.h"
#include "type.h"
#include <stdlib.h>
#include <stdarg.h>

static void nil_writer(void *, Printf);
static void do_nothing(void *);

void declare_nil(void) {
    nil_type = declare("Nil", do_nothing, nil_writer);
}

Object *nil(void) {
    return wrap(nil_type, NULL);
}

int is_nil(Object *object) {
    return is_a(nil_type, object);
}

static void do_nothing(void *memory) { }

static void nil_writer(void *nil, Printf printer) {
    printer("()");
}
