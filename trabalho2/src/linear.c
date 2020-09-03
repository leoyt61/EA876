#include <imageprocessing.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

float* blur(imagem _img, int i, int j, int n, float *vetor) {
  
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
  if (i + n > _img.width) end_i = _img.width;
  if (j + n > _img.height) end_j = _img.height;

  //fazendo o somatorio dos pixels
  for (int sti = start_i; sti < end_i; sti++) {
    for (int stj = start_j; stj < end_j; stj++) {
      cont++;
      int idx = sti + (stj*_img.width);
      sum1+=_img.r[idx];
      sum2+=_img.g[idx];
      sum3+=_img.b[idx];
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

  // colocando os valores no vetor
  vetor[0] = res1;
  vetor[1] = res2;
  vetor[2] = res3;
  return vetor;
}

int main() {
  struct timeval rt0, rt1, drt;
  int n = 7;
  imagem img, img2;
  char buffer[1000] = "data/";
  char buffer3[1000];
  scanf("%s", buffer3); //ler o nome da imagem a ser lida
  strcat(buffer, buffer3); //concatenando o nome do arquivo com o diretorio de entrada
  char buffer2[1000] = "out/linear-";
  strcat(buffer2, buffer3); //concatenando o nome do arquivo com o diretorio de saida
  img = abrir_imagem(buffer);
  gettimeofday(&rt0, NULL); // pegando o tempo logo apos abrir a imagem
  
  //setando a nova imagem
  img2.width = img.width;
  img2.height = img.height;
  img2.r = malloc(sizeof(float)*img2.width*img2.height);
  img2.g = malloc(sizeof(float)*img2.width*img2.height);
  img2.b = malloc(sizeof(float)*img2.width*img2.height);
  float *vetor;
  vetor = malloc(sizeof(float)*3);

  //percorre todos os pixels
  for (int i=0; i<(img.width); i++) {
    for (int j=0; j<(img.height); j++) {
      vetor = blur(img, i, j, n, vetor); //calcula a media de r, g e b no quadrado
      img2.r[i + (j*img2.width)] = vetor[0]; //pegando o valor do vetor e colocando em r de acordo com o `i` e `j`
      img2.g[i + (j*img2.width)] = vetor[1]; //pegando o valor do vetor e colocando em g de acordo com o `i` e `j`
      img2.b[i + (j*img2.width)] = vetor[2]; //pegando o valor do vetor e colocando em b de acordo com o `i` e `j`
    }
  }

  gettimeofday(&rt1, NULL); //pegar o tempo logo apos terminar o filtro
  salvar_imagem(buffer2, &img2);
  liberar_imagem(&img2);
  timersub(&rt1, &rt0, &drt);
  printf("%ld.%06ld ", drt.tv_sec, drt.tv_usec); //imprimir o tempo de execucao do filtro
  return 0;
}
