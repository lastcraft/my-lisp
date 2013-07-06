#ifndef INTERPRETER_HEADER
#define INTERPRETER_HEADER

#include "type.h"
#include "exception.h"

Object *eval(Object *, ErrorHandler);
Object *eval_function(Object *, Object *, ErrorHandler);
Object *apply(char *, Object *);

#endif
