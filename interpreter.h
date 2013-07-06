#ifndef INTERPRETER_HEADER
#define INTERPRETER_HEADER

#include "type.h"
#include "exception.h"

void create_interpreter(void);
void free_interpreter(void);
Object *eval(Object *, ErrorHandler);
Object *eval_function(Object *, Object *, ErrorHandler);
Object *apply(Object *, Object *, ErrorHandler);

#endif
