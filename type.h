#ifndef TYPE_HEADER
#define TYPE_HEADER

typedef Object_ Object;
typedef Type_ Type;

Type *declare(void (*)(void *));
Object *wrap(Type *, void *);
void destroy(Object *);
void *value(Object *);

#endif