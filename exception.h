#ifndef EXCEPTION_HEADER
#define EXCEPTION_HEADER

#include <setjmp.h>
#include <stdlib.h>
#include <stdarg.h>

#define Try if (! setjmp(*push_execution_context()))
#define Catch else

typedef void *ErrorHandler(void *, char *, ...);

void enable_exceptions(void);
void disable_exceptions(void);
jmp_buf *push_execution_context(void);
void *throw_exception(void *, char *, ...);
void *rethrow(void);
char *exception_message(void);
void *exception_information(void);

#endif
