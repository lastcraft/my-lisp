#ifndef BINDING_HEADER
#define BINDING_HEADER

#include "type.h"

typedef struct Binding_ Binding;

Binding *create_binding(Binding *);
Binding *free_binding(Binding *);
void add(Binding *, char *, Object *);
Object *find(Binding *, char *);

#endif
