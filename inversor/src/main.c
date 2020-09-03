/* Data de submissao:
 * Nome:
 * RA:
 */

#include <stdio.h>
#define TAM_PILHA 100
const int tam_buffer=100;

/*
 * Struct para a Pilha
 */
typedef struct {
  char dados[TAM_PILHA];
  int n;
} pilha, *Pilha;


/*
 * Definir o tamanho inicial da pilha
 */
void pilha_init(Pilha p) {
  p->n = 0;
}

/*
 * Adicionar um elemento à pilha (Empilhar)
 */
void push(Pilha p, char c) {
  if (p->n < 10) {
    p->dados[p->n]=c;
    (p->n)++;
  }
}

/*
 * "Remover" o último elemento da pilha (Desempilhar)
 */
char pop(Pilha p) {
  char c;
  if ((p->n)>0) {
    (p->n)--;
    c = p->dados[p->n];
  }
  return c;
}

/*
 * Imprimir a pilha
 */
void imprimir_pilha(Pilha p){
  while (p->n > 0)
    printf("%c", pop(p));
}

int main() {
  char buffer[tam_buffer];
  fgets(buffer, tam_buffer, stdin); //entrada
  pilha p;
  int pivot = -1;
  pilha_init(&p);

  do {
    pivot++;

    /*
     * Empilha se o caractere for diferente de espaço ou quebra de linha
     */
    if (buffer[pivot] != ' ' && buffer[pivot] != '\n'){
      push(&p, buffer[pivot]);
    }

    /*
     * Imprime o que tem na pilha ao contrário
     */
    else{
      imprimir_pilha(&p);
    }

    if (buffer[pivot] == ' ')
      printf(" ");
  } while (buffer[pivot] != '\n');
  printf("\n");
  return 0;
}
