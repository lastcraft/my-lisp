#ifndef PAIRS_HEADER
#define PAIRS_HEADER

#include "type.h"

Type *pair_type;

void declare_pair(void);
Object *pair(Object *, Object *);
int is_pair(Object *);
Object *car(Object *);
Object *cdr(Object *);
Object *reverse(Object *);

#endif
