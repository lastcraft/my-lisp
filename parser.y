%{

#include "type.h"
#include "atoms.h"
#include <stdlib.h>
#include <stdio.h>

Object *current_object = NULL;

int main(int argc, char **argv) {
    create_atoms();
    yyparse();
    write_object(current_object, (Printf)printf);
    printf("\n");
    destroy(current_object);
    free_dictionary();
    return 0;
}

%}

%token <l> NUMBER
%token <s> SYMBOL
%union {
    long l;
    char *s;
    Object *object;
}

%%

statement: element;

list: '(' elements ')';

elements: element
    | element elements;
    
element: atom
    | list;
    
atom: NUMBER { current_object = number($1); }
    | SYMBOL { current_object = string($1); };
