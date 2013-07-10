#ifndef INTERPRETER_HEADER
#define INTERPRETER_HEADER

#include "type.h"
#include "exception.h"
#include "binding.h"

void create_interpreter(void);
void free_interpreter(void);
Binding *top_level(void);
Object *eval(Object *, ErrorHandler, Binding *);
Object *apply(Object *, Object *, ErrorHandler, Binding *);

#endif
