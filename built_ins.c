#include "built_ins.h"
#include "dictionary.h"
#include "type.h"
#include "function.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"

static Object *plus(Object *);

void declare_built_ins(Dictionary *dictionary) {
    add(dictionary, "+", built_in(plus));
}

static Object *plus(Object *arguments) {
    long total = 0L;
    Object *tail;
    while (! is_nil(arguments)) {
        tail = clone(cdr(arguments));
        total = total + *(long *)value(car(arguments));
        destroy(arguments);
        arguments = tail;
    }
    return number(total);
}
