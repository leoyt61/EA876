#include <imageprocessing.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>

imagem img, img2;
pthread_mutex_t trava;
int i = 0;
int j = -1;
int n = 7; //grau de blur

void *worker(void *arg){
  while(1) {
    pthread_mutex_lock(&trava); // trava a thread ate setar o `i` e `j` da imagem que a thread vai ler
    j++;
    if (j >= img.height) {
      if (i >= img.width - 1) {
        pthread_mutex_unlock(&trava); // destrava a thread pois ja atingiu fora da matriz da imagem
        break;
      }
      j = 0;
      i++;
    }
    //encontra o `i` inicial e final e o `j` inicial e final que a thread vai ler para calcular a media
    int start_i = i - n;
    int start_j = j - n;
    int end_i = i + n;
    int end_j = j + n;

    //contar quantos pixels ele passa
    int cont = 0;

    //soma dos valores de r, g, b
    float sum1 = 0;
    float sum2 = 0;
    float sum3 = 0;

    //tratando se o `i` e `j` estiverem fora da imagem
    if (i - n < 0) start_i = 0;
    if (j - n < 0) start_j = 0;
    if (i + n > img.width) end_i = img.width;
    if (j + n > img.height) end_j = img.height;

    int _i = i; // `i` central
    int _j = j; // `j` central
    pthread_mutex_unlock(&trava); // destrava a thread

    //fazendo o somatorio dos pixels
    for (int sti = start_i; sti < end_i; sti++) {
      for (int stj = start_j; stj < end_j; stj++) {
        cont++;
        int idx = sti + (stj*img.width);
        sum1+=img.r[idx];
        sum2+=img.g[idx];
        sum3+=img.b[idx];
      }
    }

    //calculando a media de r, g, b no quadrado
    float res1 = sum1/cont;
    float res2 = sum2/cont;
    float res3 = sum3/cont;

    //tratando overflow dos pixels
    if (res1 < 0) res1 = 0;
    else if (res1 > 255) res1 = 255;
    if (res2 < 0) res2 = 0;
    else if (res2 > 255) res2 = 255;
    if (res3 < 0) res3 = 0;
    else if (res3 > 255) res3 = 255;

    //colocando as medias na nova imagem
    img2.r[_i + (_j*img.width)] = res1;
    img2.g[_i + (_j*img.width)] = res2;
    img2.b[_i + (_j*img.width)] = res3;
  }
  return NULL;
}

int main() {
  struct timeval rt0, rt1, drt;
  char buffer[1000] = "data/";
  char buffer3[1000];
  scanf("%s", buffer3); //ler o nome da imagem a ser lida
  strcat(buffer, buffer3); //concatenando o nome do arquivo com o diretorio de entrada
  char buffer2[1000] = "out/thread-";
  strcat(buffer2, buffer3); //concatenando o nome do arquivo com o diretorio de saida
  img = abrir_imagem(buffer);
  gettimeofday(&rt0, NULL); // pegando o tempo logo apos abrir a imagem

  //setando a nova imagem
  img2.width = img.width;
  img2.height = img.height;
  img2.r = malloc(sizeof(float)*img2.width*img2.height);
  img2.g = malloc(sizeof(float)*img2.width*img2.height);
  img2.b = malloc(sizeof(float)*img2.width*img2.height);

  //setando as threads
  pthread_t *workers;
  workers = malloc(sizeof(pthread_t)*(img.width*img.height));
  void *args;

  /*
   * Criando as threads
   */
  for (int i = 0; i < 6; i++) {
    pthread_create(&(workers[i]), NULL, worker, args);
  }

  /*
   * Esperando as threads terminarem
   */
  for (int i = 0; i < 6; i++) {
    pthread_join(workers[i], NULL);
  }
  gettimeofday(&rt1, NULL); //pegar o tempo logo apos terminar o filtro
  salvar_imagem(buffer2, &img2);
  liberar_imagem(&img2);

  timersub(&rt1, &rt0, &drt);
  printf("%ld.%06ld ", drt.tv_sec, drt.tv_usec); //imprimir o tempo de execucao do filtro
  return 0;
}
