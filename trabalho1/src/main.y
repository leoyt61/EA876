%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void yyerror(char *c);
int yylex(void);
int add, mul, divi, expo;
char* buffer = NULL;
size_t bufferSize = 0;
FILE* myStream;
char* errorBuffer = NULL;
size_t errorBufferSize = 0;
FILE* myErr;
%}

%token N
%token AP FP
%token DIV TIMES PLUS
%token POW
%token EOL

%left PLUS
%left TIMES
%left DIV
%left POW 
%left LB RB

%%
S:
    S EXP EOL {
      /*
       * incrementa sp e salva o valor final no registrador A
       */
      fprintf(myStream, "INC SP\nMOV A, [SP]\nHLT\n");

      /*
       * caso haja uma sub rotina de adicao
       * as sub rotinas seguem o algoritmo shift reduce
       */
      if (add == 1){
        /*
         * salva o endereco de retorno (em SP+1) em D
         * salva o numero a direita (em SP+2) da operacao em B
         * salva o numero a esquerda (em SP+3) da operacao em A
         * salva a adicao em A e salva em SP+2
         * SP agora esta em SP+2
         * pula de volta para o endereco em D
         */
        add = 0;
        fprintf(myStream, "\n\n\naddition:\n\tINC SP\n\tMOV D, [SP]\n\tINC SP\n\tMOV B, [SP]\n\tINC SP\n\tMOV A, [SP]\n\tADD A, B\n\tMOV [SP], A\n\tDEC SP\n\tJMP D\n");
      }

      /*
       * caso haja uma sub rotina de multiplicacao
       */
      if (mul == 1){
        /*
         * salva o endereco de retorno (em SP+1) em D
         * salva o numero a direita (em SP+2) da operacao em B
         * salva o numero a esquerda (em SP+3) da operacao em A
         * salva a multiplicacaco em A e salva em SP+2
         * SP agora esta em SP+2
         * pula de volta para o endereco em D
         */
        mul = 0;
        fprintf(myStream, "\n\n\nmultiplication::\n\tINC SP\n\tMOV D, [SP]\n\tINC SP\n\tMOV B, [SP]\n\tINC SP\n\tMOV A, [SP]\n\tMUL B\n\tMOV [SP], A\n\tDEC SP\n\tJMP D\n");
      }

      /*
       * caso haja uma sub rotina de divisao
       */
      if (divi == 1){
        /*
         * salva o endereco de retorno (em SP+1) em D
         * salva o numero a direita (em SP+2) da operacao em B
         * salva o numero a esquerda (em SP+3) da operacao em A
         * salva a divisao em A e salva em SP+2
         * SP agora esta em SP+2
         * pula de volta para o endereco em D
         */
        divi = 0;
        fprintf(myStream, "\n\n\ndivision:\n\tINC SP\n\tMOV D, [SP]\n\tINC SP\n\tMOV B, [SP]\n\tINC SP\n\tMOV A, [SP]\n\tDIV B\n\tMOV [SP], A\n\tDEC SP\n\tJMP D\n");
      }

      /*
       * caso haja uma sub rotina de potenciacao
       */
      if (expo == 1){
        /*
         * salva o endereco de retorno (em SP+1) em D
         * salva o numero a direita (em SP+2) da operacao em B
         * salva o numero a esquerda (em SP+3) da operacao em A e em C para deixar em um loop e ir multiplicando B vezes o numero em C e colocando em A (operacao de potenciacao)
         * salva a potenciacao em A e salva em SP+2
         * SP agora esta em SP+2
         * pula de volta para o endereco em D
         */
        expo = 0;
        fprintf(myStream, "\n\n\nexponencial:\n\tINC SP\n\tMOV D, [SP]\n\tINC SP\n\tMOV B, [SP]\n\tINC SP\n\tMOV A, [SP]\n\tMOV C, A\n\tCMP B, 0\n\tJE pot_zero\n\tSUB B, 1\n\tloop:\n\t\tSUB B, 1\n\t\tMUL C\n\t\tCMP B, 0\n\t\tJNE loop\n\t\tMOV [SP], A\n\t\tDEC SP\n\t\tJMP D\n\tpot_zero:\n\t\tMOV A, 1\n\t\tMOV [SP], A\n\t\tDEC SP\n\t\tJMP D\n");
      }
      return 0;
    }
  |
  ;

EXP:
    N {
        /*
         * empilha em SP o numero quando der match e SP eh decrementado
         */
        fprintf(myStream, "PUSH %d\n", $1);
    }
  | EXP DIV EXP {

      /*
       * caso o denominador (numero a direita da expressao) seja zero, nao sera possivel realizar a operacao e imprimira uma mensagem de erro 
       */
      if ($3 == 0) {
        yyerror("Não pode dividir por zero");
        exit(1); 
      }
      else {

        /*
         * chamara a sub rotina de divisao e levantara a flag de que havera uma subrotina de divisao
         * o numero a direita da operacao estara em SP+2 e o numero a esquerda estara em SP+3
         * em SP+1 esta o endereco de retorno 
         */
        divi = 1;
        fprintf(myStream, "CALL division\n");
      }
    }
  | EXP TIMES EXP {
      /*
       * chamara a sub rotina de multiplicacao e levantara a flag de que havera uma subrotina de multiplicacao
       * o numero a direita da operacao estara em SP+2 e o numero a esquerda estara em SP+3
       * em SP+1 esta o endereco de retorno 
       */ 
      mul = 1;
      fprintf(myStream, "CALL multiplication\n");
    }
  | LB EXP RB { $$ = $2;}
  | EXP PLUS EXP  {
      /*
       * chamara a sub rotina de adicao e levantara a flag de que havera uma subrotina de adicao
       * o numero a direita da operacao estara em SP+2 e o numero a esquerda estara em SP+3
       * em SP+1 esta o endereco de retorno 
       */
      add = 1;
      fprintf(myStream, "CALL addition\n");
    }
  | EXP POW EXP {
      /*
       * chamara a sub rotina de potenciacao e levantara a flag de que havera uma subrotina de potenciacao
       * o numero a direita da operacao estara em SP+2 e o numero a esquerda estara em SP+3
       * em SP+1 esta o endereco de retorno 
       */
      expo = 1;
      fprintf(myStream, "CALL exponencial\n");
    }
  ;
%%

void yyerror(char *s) {
  myErr = open_memstream(&errorBuffer, &errorBufferSize);
  fprintf(myErr, "%s\n", s);
  fclose(myErr);
  printf("%s", errorBuffer);
  free(errorBuffer);
}

int main() {
  myStream = open_memstream(&buffer, &bufferSize);
  yyparse();
  fclose(myStream);
  printf("%s", buffer);
  free(buffer);
  return 0;
}
