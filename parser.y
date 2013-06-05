%token NUMBER SYMBOL
%union {
    long l;
    char *s;
}
%%

list: '(' elements ')';
elements: atom | list | atom elements | list elements
atom: NUMBER | SYMBOL;
