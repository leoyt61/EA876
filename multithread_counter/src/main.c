#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>

pthread_mutex_t trava;
int myIndex = -1;
int qtd = 0; // quantidade de numeros primos
int tamanho = 0; // quantidade de threads que havera

void* worker(void *arg) {
  unsigned long int* vetor = (unsigned long int*) arg;
  while(1) {
    pthread_mutex_lock(&trava); // trava a thread ate ela setar qual posicao do vetor ira ler
    int i = myIndex + 1;
    myIndex++;
    pthread_mutex_unlock(&trava); // destrava a thread
    if (i >= tamanho) break; // se a thread tentar ler uma posicao que nao tem numero no vetor, ela sera encerrada
    if (vetor[i] == 0 || vetor[i] == 1) continue; // se o numero lido for 0 ou 1, o numero nao eh primo e a thread partira para a prox posicao
    unsigned long int cont = 2;

    /*
     * se o numero lido for divisivel por qualquer numero que nao seja 1 ou ele mesmo, sera um numero primo
     */
    while (1) {
      if (vetor[i]%cont == 0) break;
      cont++;
    }
    if (vetor[i] == cont) qtd++;
  }
  return NULL;
}

int main() {
  unsigned long int buffer;
  int i = 0;
  pthread_t workers[4];
  int t_filhos = 3;
  void *args;
  unsigned long int *vetor;
  vetor = malloc(sizeof(unsigned long int)*1000);

  /*
   * lendo as entradas e salvando no vetor alocado
   */
  while (1){
    scanf("%lu", &buffer);
    vetor[i] = buffer;
    i++;
    if (getchar() == 10) break;
  }

  tamanho = i;
  if (i < t_filhos + 1) t_filhos = i - 1; // numero maximo de threads sera 4

  args = (void*) vetor;

  /* 
   * disparando as threads
   */
  for (int i=0; i<t_filhos; i++) {
    pthread_create(&(workers[i]), NULL, worker, args);
  }

  /* 
   * esperando as threads terminarem
   */
  for (int i=0; i<t_filhos; i++) {
    pthread_join(workers[i], NULL);
  }

  printf("%d\n", qtd); // imprime a quantidade de numeros primos
  
  return 0;
}
