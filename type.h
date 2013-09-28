#ifndef TYPE_HEADER
#define TYPE_HEADER

#include <stdarg.h>

typedef struct Object_ Object;
typedef struct Type_ Type;
typedef int (*Printf)(char *, ...);
typedef void (*Free)(void *);

Type *declare(char *name, void (*destructor)(void *), void (*writer)(void *, Printf));
Object *wrap(Type *, void *);
void destroy(Object *);
Object *clone(Object *);
int is_a(Type *, Object *);
char *type_name(Object *);
void *value(Object *);
void write_object(Object *, Printf);
void free_declarations(void);

#endif