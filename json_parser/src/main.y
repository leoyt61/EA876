

%{
#include <stdio.h>
  #include <stdlib.h>
void yyerror(char *c);
int yylex(void);
%}

%token  AS L N COA COF CHA CHF PP VI PT ND EOL


%%

S:
  JS{printf("VALIDO\n");}
  ;

JS:
  CHA ST PP VA JO
  ;

JO:
  CHF
  | VI ST PP VA JO
  ;

ST:
  AS LN AS
  ;

VA:
  L
  | NU
  | LI
  | JS
  | ST
  ;

LN:
  L 
  | NU
  | LN LN
  ;

NU:
  N
  | ND
  ;

LI:
  COA VA ES
  | COA COF
  ;

ES:
  COF
  | VI VA ES
  ;



%%

void yyerror(char *s) {
  printf("INVALIDO\n");
}

int main() {
  yyparse();
    return 0;

}
