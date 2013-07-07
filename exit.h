#ifndef EXIT_HEADER
#define EXIT_HEADER

#include "type.h"

Type *exit_code_type;

void declare_exit_code(void);
Object *exit_code(int);
int is_exit_code(Object *);

#endif
