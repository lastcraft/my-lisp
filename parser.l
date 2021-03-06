%{

#include <stdbool.h>
#include "y.tab.h"

%}

%%

\(\) {
    return NIL;
}

true {
    yylval.b = true;
    return BOOLEAN;
}

false {
    yylval.b = false;
    return BOOLEAN;
}

[0-9]+ {
    yylval.l = atoi(yytext);
    return NUMBER;
}

\"[^"]*\" {
    yylval.s = yytext;
    return QUOTED_STRING;
}

[a-zA-Z_<>!?*/=+-][a-zA-Z0-9_<>!?=+*/-]* {
    yylval.s = yytext;
    return IDENTIFIER;
}

[\t ];

\n {
    return 0;
}

\( { return '('; }
\) { return ')'; }
\. { return '.'; }

. ;

%%
