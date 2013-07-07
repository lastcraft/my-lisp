#ifndef INTERPRETER_HEADER
#define INTERPRETER_HEADER

#include "type.h"
#include "exception.h"
#include "dictionary.h"

void create_interpreter(void);
void free_interpreter(void);
Dictionary *top_level(void);
Object *eval(Object *, ErrorHandler, Dictionary *);
Object *apply(Object *, Object *, ErrorHandler, Dictionary *);

#endif
