%{
#include <stdlib.h>
void yyerror(char *);

#include "y.tab.h"

%}
/* Algumas expressoes regulares, para os numeros e operacoes */
%%
[\t]+

[0-9]+ { yylval = atoi(yytext); return N;}

"+" { return PLUS; }
"*" { return TIMES; }
"/" { return DIV; }
"^" { return POW; }
"(" { return LB; }
")" { return RB; }
. {}
[\n] { return EOL; }
%%

int yywrap() {
  return 1;
}
