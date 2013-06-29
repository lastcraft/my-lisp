#include "type.h"
#include <stdlib.h>
#include <stdarg.h>

static Type **dictionary = NULL;
long dictionary_used = 0L;
long dictionary_size = 0L;
static add_to_dictionary(Type *);
static void create_dictionary(void);
static void enlarge_dictionary(void);

struct Object_ {
    Type *type;
    long reference_count;
    void *value;
};

struct Type_ {
    void (*destructor)(void *);
    void (*writer)(void *, Printf);
};

Type *declare(void (*destructor)(void *), void (*writer)(void *, Printf)) {
    Type *type = (Type *)malloc(sizeof(Type));
    type->destructor = destructor;
    type->writer = writer;
    add_to_dictionary(type);
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

void *value(Object *object) {
    return object == NULL ? NULL : object->value;
}

void write_object(Object *object, Printf printer) {
    if (object != NULL) {
        object->type->writer(value(object), printer);
    }
}

void free_dictionary(void) {
    long i;
    for (i = 0L; i < dictionary_used; i++) {
        free(dictionary[i]);
    }
    free(dictionary);
    dictionary = NULL;
    dictionary_used = 0L;
    dictionary_size = 0L;
}

static add_to_dictionary(Type *type) {
    if (dictionary == NULL) {
        create_dictionary();
    } else if (dictionary_used == dictionary_size) {
        enlarge_dictionary();
    }
    dictionary[dictionary_used++] = type;
}

static void create_dictionary(void) {
    dictionary = (Type **)malloc(sizeof(Type *) * 10L);
    dictionary_size = 10L;
}

static void enlarge_dictionary(void) {
    dictionary = (Type **)realloc((void *)dictionary, sizeof(Type *) * (dictionary_size + 10L));
    dictionary_size = dictionary_size + 10L;    
}
