#ifndef TYPE_HEADER
#define TYPE_HEADER

#include <stdarg.h>

typedef struct Object_ Object;
typedef struct Type_ Type;
typedef int (*Printf)(char *, ...);

Type *declare(void (*destructor)(void *), void (*writer)(void *, Printf));
Object *wrap(Type *, void *);
void destroy(Object *);
void *value(Object *);
void write_object(Object *, Printf);

#endif