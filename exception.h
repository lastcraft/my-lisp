#ifndef EXCEPTION_HEADER
#define EXCEPTION_HEADER

#include <setjmp.h>
#include <stdlib.h>

#define Try if (! setjmp(*execution_context()))
#define Catch else

typedef void *Throw(char *, void *);

jmp_buf *execution_context(void);
void *throw_exception(char *, void *);
char *exception_message(void);
void *exception_information(void);

#endif
