#ifndef FUNCTION_HEADER
#define FUNCTION_HEADER

#include "type.h"
#include "exception.h"
#include "binding.h"

typedef Object *(*Callable)(Object *, ErrorHandler, Binding *);
typedef struct BuiltIn_ BuiltIn;
typedef struct Lambda_ Lambda;
Type *built_in_type;
Type *lambda_type;

void declare_functions(void);
Object *built_in(Callable);
Object *lambda(Object *, Object *);
Callable code(BuiltIn *built_in);
Object *parameters(Object *);
Object *body(Object *);
Object *special_form(Object *);
int is_function(Object *);
int is_special_form(Object *);
int is_built_in(Object *);
int is_lambda(Object *);

#endif
