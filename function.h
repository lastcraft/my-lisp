#ifndef FUNCTION_HEADER
#define FUNCTION_HEADER

#include "type.h"

typedef Object *(*Callable)(Object *);
Type *built_in_type;
Type *lambda_type;

void declare_functions(void);
Object *built_in(Callable code);
Object *lambda(Object *, Object *);
int is_function(Object *);

#endif
