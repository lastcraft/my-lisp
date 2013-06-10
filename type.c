#include "type.h"

struct Object_ {
    Type *type;
    void *value;
};

struct Type_ {
    void destructor(void *);
};

Type *declare(void (*destructor)(void *)) {
    Type *type = (Type *)malloc(sizeof(Type));
    type->destructor = destructor;
    return type;
}

Object *wrap(Type *type, void *value) {
    Object *object = (Object *)malloc(sizeof(Object));
    object->type = type;
    object->value = value;
    return object;
}

void destroy(Object *value) {
    object->type->destructor(object->value);
    free(object);
}

void *value(Object *object) {
    return object->value;
}
