#include "type.h"
#include <stdlib.h>
#include <stdarg.h>

static Type **declarations = NULL;
long declarations_used = 0L;
long declarations_size = 0L;
static void add_to_declarations(Type *);
static void create_declarations(void);
static void enlarge_declarations(void);

struct Object_ {
    Type *type;
    long reference_count;
    void *value;
};

struct Type_ {
    char *name;
    void (*destructor)(void *);
    void (*writer)(void *, Printf);
};

Type *declare(char *name, void (*destructor)(void *), void (*writer)(void *, Printf)) {
    Type *type = (Type *)malloc(sizeof(Type));
    type->name = name;
    type->destructor = destructor;
    type->writer = writer;
    add_to_declarations(type);
    return type;
}

Object *wrap(Type *type, void *value) {
    Object *object = (Object *)malloc(sizeof(Object));
    object->type = type;
    object->reference_count = 1L;
    object->value = value;
    return object;
}

void destroy(Object *object) {
    if (object != NULL) {
        if (object->reference_count > 1L) {
            object->reference_count--;
        } else {
            object->type->destructor(object->value);
            free(object);
        }
    }
}

Object *clone(Object *object) {
    object->reference_count++;
    return object;
}

int is_a(Type *type, Object *object) {
    return type == object->type;
}

char *type_name(Object *object) {
    return object->type->name;
}

void *value(Object *object) {
    return object == NULL ? NULL : object->value;
}

void write_object(Object *object, Printf printer) {
    if (object != NULL) {
        object->type->writer(value(object), printer);
    }
}

void free_declarations(void) {
    long i;
    for (i = 0L; i < declarations_used; i++) {
        free(declarations[i]);
    }
    free(declarations);
    declarations = NULL;
    declarations_used = 0L;
    declarations_size = 0L;
}

static void add_to_declarations(Type *type) {
    if (declarations == NULL) {
        create_declarations();
    } else if (declarations_used == declarations_size) {
        enlarge_declarations();
    }
    declarations[declarations_used++] = type;
}

static void create_declarations(void) {
    declarations = (Type **)malloc(sizeof(Type *) * 10L);
    declarations_size = 10L;
}

static void enlarge_declarations(void) {
    declarations = (Type **)realloc((void *)declarations, sizeof(Type *) * (declarations_size + 10L));
    declarations_size = declarations_size + 10L;    
}
