#include "exception.h"
#include <setjmp.h>

static jmp_buf jump_buffer;
static char *last_exception_message = NULL;
static char *last_exception_information = NULL;

jmp_buf *execution_context(void) {
    return &jump_buffer;
}

void *throw_exception(char *message, void *information) {
    last_exception_message = message;
    last_exception_information = information;
    longjmp(*execution_context(), 1);
    return NULL;
}

char *exception_message(void) {
    return last_exception_message;
}

void *exception_information(void) {
    return last_exception_information;
}
