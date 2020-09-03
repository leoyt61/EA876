
%{
#include <stdlib.h>
void yyerror(char *);

#include "y.tab.h"

%}


%%
[\[] {return COA;}

[\]] {return COF;}

[\"] {return AS;}

[0-9]+ {return N;}

[0-9]+\.[0-9]+ {return ND;}

[a-zA-z]+ {return L;}

[\{] {return CHA;}

[\}] {return CHF;}

[\:] {return PP;}

[\,] {return VI;}

[\.] {return PT;}

[[:space:]]+ ;

. {}
%%

int yywrap() {
  return 1;
}
