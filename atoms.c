#include "atoms.h"
#include "type.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

static void boolean_writer(void *, Printf);
static void number_writer(void *, Printf);
static void identifier_writer(void *, Printf);
static void string_writer(void *, Printf);
static long last(char *);

void declare_atoms(void) {
    boolean_type = declare(free, boolean_writer);
    number_type = declare(free, number_writer);
    identifier_type = declare(free, identifier_writer);
    string_type = declare(free, string_writer);
}

Object *boolean(bool true_or_false) {
    bool *cell = (bool *)malloc(sizeof(bool));
    *cell = true_or_false;
    return wrap(boolean_type, cell);
}

int is_boolean(Object *boolean) {
    return is_a(boolean_type, boolean);
}

static void boolean_writer(void *boolean, Printf printer) {
    printer("%s", *(bool *)boolean == true ? "true" : "false");
}

Object *number(long number) {
    long *cell = (long *)malloc(sizeof(long));
    *cell = number;
    return wrap(number_type, cell);
}

int is_number(Object *object) {
    return is_a(number_type, object);
}

static void number_writer(void *number, Printf printer) {
    printer("%d", *(long *)number);
}

Object *identifier(char *name) {
    return wrap(identifier_type, strdup(name));
}

int is_identifier(Object *object) {
    return is_a(identifier_type, object);
}

static void identifier_writer(void *name, Printf printer) {
    printer("%s", (char *)name);
}

Object *quoted_string(char *quoted) {
    char *string = strdup(quoted + 1);
    string[last(string)] = '\0';
    return wrap(string_type, string);
}

int is_string(Object *object) {
    return is_a(string_type, object);
}

static long last(char *string) {
    return strlen(string) - 1;
}

static void string_writer(void *text, Printf printer) {
    printer("\"%s\"", (char *)text);    
}
