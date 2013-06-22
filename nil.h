#ifndef NIL_HEADER
#define NIL_HEADER

#include "type.h"

Type *nil_type;

void declare_nil(void);
Object *nil(void);
int is_nil(Object *);

#endif
