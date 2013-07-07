#ifndef FUNCTION_HEADER
#define FUNCTION_HEADER

#include "type.h"
#include "exception.h"
#include "dictionary.h"

typedef Object *(*Callable)(Object *, ErrorHandler, Dictionary *);
Type *built_in_type;
Type *lambda_type;

void declare_functions(void);
Object *built_in(Callable);
Object *lambda(Object *, Object *);
int is_function(Object *);
int is_built_in(Object *);
int is_lambda(Object *);

#endif
