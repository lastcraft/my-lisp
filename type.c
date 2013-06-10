#include "type.h"
#include <stdlib.h>

static Type **dictionary = NULL;
long dictionary_used = 0L;
long dictionary_size = 0L;
static add_to_dictionary(Type *);
static void create_dictionary(void);
static void enlarge_dictionary(void);
static void free_dictionary(void);

struct Object_ {
    Type *type;
    void *value;
};

struct Type_ {
    void (*destructor)(void *);
};

Type *declare(void (*destructor)(void *)) {
    Type *type = (Type *)malloc(sizeof(Type));
    type->destructor = destructor;
    add_to_dictionary(type);
    return type;
}

Object *wrap(Type *type, void *value) {
    Object *object = (Object *)malloc(sizeof(Object));
    object->type = type;
    object->value = value;
    return object;
}

void destroy(Object *object) {
    object->type->destructor(object->value);
    free(object);
}

void *value(Object *object) {
    return object->value;
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

static void free_dictionary(void) {
    long i;
    for (i = 0L; i < dictionary_used; i++) {
        free(dictionary[i]);
    }
    free(dictionary);
    dictionary = NULL;
    dictionary_used = 0L;
    dictionary_size = 0L;
}
