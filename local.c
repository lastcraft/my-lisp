#include "local.h"
#include "type.h"
#include "stack.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct StackFrame_ {
    Object **vector;
    long used;
    long size;
} StackFrame;

static StackFrame *create_frame(void);
static void destroy_frame(StackFrame *);
static void add_to_frame(StackFrame *, Object *);
static bool found(StackFrame *frame, Object *object);
static void ensure_space(StackFrame *frame);

static Stack *object_stack = NULL;

void create_local(void) {
    if (object_stack == NULL) {
        object_stack = create_stack();
    }
    push(object_stack, (void *)create_frame());
}

void destroy_local(void) {
    destroy_frame(pop(object_stack));
    if (is_empty(object_stack)) {
        destroy_stack(object_stack, free);
        object_stack = NULL;
    }
}

Object *local(Object *object) {
    add_to_frame((StackFrame *)peek(object_stack), object);
    return object;
}

static StackFrame *create_frame(void) {
    StackFrame *frame = (StackFrame *)malloc(sizeof(StackFrame));
    frame->vector = (Object **)malloc(sizeof(Object *) * 16);
    frame->used = 0L;
    frame->size = 16L;
    return frame;
}

static void destroy_frame(StackFrame *frame) {
    free(frame->vector);
    free(frame);
}

static void add_to_frame(StackFrame *frame, Object *object) {
    if (! found(frame, object)) {
        ensure_space(frame);
        frame->vector[frame->used++] = object;
    }
}

static bool found(StackFrame *frame, Object *object) {
    for (long i = 0; i < frame->size; i++) {
        if (object == frame->vector[i]) {
            return true;
        }
    }
    return false;
}

static void ensure_space(StackFrame *frame) {
    if (frame->size == frame->used) {
        frame->size *= 2;
        frame->vector = (Object **)realloc(frame->vector, sizeof(Object *) * frame->size);
    }
}
