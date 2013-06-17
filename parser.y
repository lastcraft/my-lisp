%{

#include "type.h"
#include "atoms.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

Stack *current_values = NULL;
Object *temporary_object = NULL;

int main(int argc, char **argv) {
    create_atoms();
    current_values = create_stack();
    push(current_values, (void *)nil());
    yyparse();
    write_object(peek(current_values), (Printf)printf);
    printf("\n");
    if (temporary_object != NULL) {
        destroy(temporary_object);
    }
    destroy_stack(current_values, (StackDestructor)destroy);
    free_dictionary();
    return 0;
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

pair: pair_head '.' pair_tail;

pair_head: '(' element {
        temporary_object = (Object *)pop(current_values);
        push(current_values, (void *)nil());
    };
    
pair_tail: element ')' {
        Object *current_object = (Object *)pop(current_values);
        push(current_values, pair(temporary_object, current_object));
        temporary_object = NULL;
    };
    
element: atom
    | pair;
    
atom: NUMBER {
        destroy((Object *)pop(current_values));
        push(current_values, (void *)number($1));
    }
    | IDENTIFIER {
        destroy((Object *)pop(current_values));
        push(current_values, (void *)identifier($1));
    }
    | QUOTED_STRING {
        destroy((Object *)pop(current_values));
        push(current_values, (void *)quoted_string($1));
    }
    | NIL {
        destroy((Object *)pop(current_values));
        push(current_values, (void *)nil());
    };
