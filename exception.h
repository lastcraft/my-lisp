#ifndef EXCEPTION_HEADER
#define EXCEPTION_HEADER

#include <setjmp.h>
#include <stdlib.h>

#define Try if (! setjmp(*push_execution_context()))
#define Catch else

typedef void *ErrorHandler(char *, void *);

void enable_exceptions(void);
void disable_exceptions(void);
jmp_buf *push_execution_context(void);
void *throw_exception(char *, void *);
void *rethrow(void);
char *exception_message(void);
void *exception_information(void);

#endif
