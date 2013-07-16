#include "exception.h"
#include "stack.h"
#include <setjmp.h>
#include <stdlib.h>

static Stack *exception_stack = NULL;
static jmp_buf *current_frame = NULL;
static jmp_buf *pop_execution_context(void);
static char *last_exception_message = NULL;
static char *last_exception_information = NULL;

void enable_exceptions(void) {
    exception_stack = create_stack();
}

void disable_exceptions(void) {
    if (current_frame != NULL) {
        free(current_frame);
        current_frame = NULL;
    }
    destroy_stack(exception_stack, free);
    exception_stack = NULL;
}

jmp_buf *push_execution_context(void) {
    jmp_buf *frame = (jmp_buf *)malloc(sizeof(jmp_buf));
    push(exception_stack, frame);
    return frame;
}

void *throw_exception(char *message, void *information) {
    last_exception_message = message;
    last_exception_information = information;
    longjmp(*pop_execution_context(), 1);
    return NULL;
}

void *rethrow(void) {
    longjmp(*pop_execution_context(), 1);
    return NULL;
}

char *exception_message(void) {
    return last_exception_message;
}

void *exception_information(void) {
    return last_exception_information;
}

static jmp_buf *pop_execution_context(void) {
    if (current_frame != NULL) {
        free(current_frame);
    }
    current_frame = pop(exception_stack);
    return current_frame;
}
