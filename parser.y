%{

#include "type.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

Stack *current_values = NULL;

static void destroy_object_stack(void *);
static Object *pour_stack_into_list(Object *, Stack *);

int main(int argc, char **argv) {
    declare_nil();
    declare_atoms();
    declare_pair();
    current_values = create_stack();
    push(current_values, (void *)create_stack());
    yyparse();
    if (! is_empty(current_values)) {
        write_object((Object *)peek((Stack *)peek(current_values)), (Printf)printf);
    }
    printf("\n");
    destroy_stack(current_values, destroy_object_stack);
    free_dictionary();
    return 0;
}

static void destroy_object_stack(void *object_stack) {
    destroy_stack((Stack *)object_stack, (StackDestructor)destroy);    
}

static Object *pour_stack_into_list(Object *list, Stack *stack) {
    while (! is_empty(stack)) {
        list = pair((Object *)pop(stack), list);
    }
    return list;
}

%}

%token NIL
%token <l> NUMBER
%token <s> IDENTIFIER
%token <s> QUOTED_STRING

%union {
    long l;
    char *s;
}

%%

statement: element;

list: list_head elements '.' pair_tail
    | list_head elements list_tail
    | list_head empty_list;

list_head: '(' {
        push(current_values, create_stack());
    };
    
empty_list: ')' {
        destroy_object_stack((Stack *)pop(current_values));
        push((Stack *)peek(current_values), (void *)nil());
    };

pair_tail: element ')' {
        Stack *stack = pop(current_values);
        Object *second = pop(stack);
        Object *first = pop(stack);
        Object *list = pair(first, second);
        list = pour_stack_into_list(list, stack);
        destroy_object_stack(stack);
        push((Stack *)peek(current_values), (void *)list);
    };
    
list_tail: ')' {
        Stack *stack = pop(current_values);
        Object *last = pop(stack);
        Object *list = pair(last, nil());
        list = pour_stack_into_list(list, stack);
        destroy_object_stack(stack);
        push((Stack *)peek(current_values), (void *)list);
    };
    
elements: element | elements element;
    
element: atom
    | list;
    
atom: NUMBER {
        push((Stack *)peek(current_values), (void *)number($1));
    }
    | IDENTIFIER {
        push((Stack *)peek(current_values), (void *)identifier($1));
    }
    | QUOTED_STRING {
        push((Stack *)peek(current_values), (void *)quoted_string($1));
    }
    | NIL {
        push((Stack *)peek(current_values), (void *)nil());
    };
