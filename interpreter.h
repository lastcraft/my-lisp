#ifndef INTERPRETER_HEADER
#define INTERPRETER_HEADER

#include "type.h"
#include "exception.h"
#include "binding.h"

Type *binding_type;

void create_interpreter(void);
void free_interpreter(void);
Binding *top_level(void);
void declare_binding(void);
Object *wrap_binding(Binding *);
Binding *unwrap_binding(Object *);
Object *eval(Object *, ErrorHandler, Binding *);
Object *apply(Object *, Object *, ErrorHandler, Binding *);

#endif
