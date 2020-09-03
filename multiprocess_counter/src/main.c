#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <math.h>


int main() {

  unsigned long int buffer;
  int i = 0;
  pid_t pid[3];
  int p_filhos = 3;

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;
  unsigned long int *vetor;
  int *vetor2;
  vetor = (unsigned long int*) mmap(NULL, sizeof(unsigned long int)*100, protection, visibility, 0, 0); //memoria compartilhada (vetor) com os numeros da entrada
  vetor2 = (int*) mmap(NULL, sizeof(int)*10, protection, visibility, 0, 0); //memoria compartilhada (vetor) para salvar a quantidade de numeros primos encontrados
  (*(vetor2)) = 0; //comecando com zero numeros primos

  /*
   * bloco para ler a entrada e separar os numeros
   */
  while (1){
    scanf("%lu", &buffer);
    (*(vetor+i)) = buffer;
    i++;
    if (getchar() == 10) break; // se o caractere digitado for quebra de linha, para a leitura
  }

  /*
   * quantidade de processos filhos sera de no maximo 3
   */
  if (i < p_filhos + 1) p_filhos = i - 1;

  /*
   * Percorre todos os processos filhos
   */
  for (int j = 0; j < p_filhos; j++){
    pid[j] = fork(); //cria um processo filho
    if (pid[j] == 0){
      unsigned long int cont = 0;
      int index = j; // processo filho j comecara lendo o numero que esta na posicao j do vetor
      while(1){

        /*
         * caso o processo tente ler um numero que nao existe no vetor, o processo filho eh interrompido
         */
        if (index > i){
          exit(0);
        }
        else{

          /*
           *  caso o contador atinja o numero lido, quer dizer que nao encontrou um divisor, ou seja, encontrou um primo
           */
          if (cont <= (*(vetor + index))){
            cont++;

            /*
             * caso o numero lido seja 0 ou 1 ou o numero seja divisivel pelo contador e o contador seja diferente do numero lido,
             * este numero nao eh primo e o processo comecara a ler o proximo numero. Se existem N processos filhos, o prox a ser lido
             * sera o N-esimo a partir do atual
             */
            if ((*(vetor + index)) == 0 || (*(vetor + index)) == 1 || ((*(vetor + index)) % cont == 0 && cont > 1 && (*(vetor + index)) != cont)){
              cont = 0;
              index += p_filhos;
            }

            /*
             *  caso o contador atinja o numero lido, quer dizer que nao encontrou um divisor, ou seja, encontrou um primo
             *  e comecara a ler o proximo numero. Se existem N processos filhos, o prox a ser lido
             *  sera o N-esimo a partir do atual, por exemplo
             */
            else if (cont == (*(vetor + index))){
              cont = 0;
              index += p_filhos;
              (*(vetor2))++;
            }
          }
        }
      }
    }
  }

  /*
   * espera os 3 processos terminarem
   */
  for (int k = 0; k < p_filhos; k++){
    waitpid(pid[k], NULL, 0);
  }
  printf("%d\n", (*(vetor2))); // imprime a quantidade de numeros primos encontrados
  return 0;
}
