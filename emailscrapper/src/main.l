
%{
#include <stdio.h>

//Apenas um REGEX gigante abaixo
%}

%%

[a-zA-Z0-9]+([a-zA-Z\.\_\-]+[a-zA-Z0-9])?@[a-zA-Z0-9]+([a-zA-Z0-9\.\_]+[a-zA-Z0-9])?/[[:space:].] { printf("%s\n", yytext);}

[[:space:]]+ ; 
[[:^space:]]+ ;

%%

int main() {
  yylex();
  return 0;
}
