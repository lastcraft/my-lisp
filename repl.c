#include "type.h"
#include "nil.h"
#include "pair.h"
#include "exception.h"
#include "exit.h"
#include "interpreter.h"
#include <stdio.h>

extern void create_reader(void);
extern void free_reader(void);
extern Object *read(void);

static int exit_status = 0;
static int request_exit = 0;
static void will_exit(int);
static int exiting(void);
static void print(Object *value);
static void print_error(char *, Object *);

int main(int argc, char **argv) {
    enable_exceptions();
    create_interpreter();
    create_reader();
    do {
        Try {
            print(eval(read(), throw_exception, top_level()));
        } Catch {
            if (is_exit_code((Object *)exception_information())) {
                will_exit(*(int *)value((Object *)exception_information()));
            } else {
                print_error(exception_message(), (Object *)exception_information());
            }
        }
    } while (! exiting());
    free_reader();
    free_interpreter();
    disable_exceptions();
    return exit_status;
}

static void will_exit(int exit_code) {
    request_exit = 1;
    exit_status = exit_code;
}

static int exiting(void) {
    return request_exit;
}

static void print(Object *value) {
    if (value != NULL) {
        write_object(value, (Printf)printf);
        printf("\n");
        destroy(value);
    }
}

static void print_error(char *message, Object *object) {
    printf("%s: ", message);
    write_object(object, (Printf)printf);
    printf("\n");
    destroy(object);
}
