#include <imageprocessing.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/time.h>
#include <math.h>


int main() {
  struct timeval rt0, rt1, drt;
  pid_t pid[6];
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  char buffer[1000] = "data/";
  char buffer3[1000];
  scanf("%s", buffer3); //ler o nome da imagem a ser lida
  strcat(buffer, buffer3); //concatenando o nome do arquivo com o diretorio de entrada
  char buffer2[1000] = "out/process-";
  strcat(buffer2, buffer3); //concatenando o nome do arquivo com o diretorio de saida

  imagem *p_img;
  p_img = (imagem*) mmap(NULL, sizeof(imagem), protection, visibility, 0, 0); // memoria compartilhada da imagem original
  (*p_img) = abrir_imagem(buffer);
  gettimeofday(&rt0, NULL); // pegando o tempo logo apos abrir a imagem
  
  //setando a nova imagem
  imagem *p_img2;
  p_img2 = (imagem*) mmap(NULL, sizeof(imagem), protection, visibility, 0, 0); // memoria compartilhada da nova imagem
  (*p_img2).width = (*p_img).width;
  (*p_img2).height = (*p_img).height;
  (*p_img2).r = (float*) mmap(NULL, sizeof(float)*(*p_img2).width*(*p_img2).height, protection, visibility, 0, 0);
  (*p_img2).g = (float*) mmap(NULL, sizeof(float)*(*p_img2).width*(*p_img2).height, protection, visibility, 0, 0);
  (*p_img2).b = (float*) mmap(NULL, sizeof(float)*(*p_img2).width*(*p_img2).height, protection, visibility, 0, 0);

  for (int i = 0; i < 6; i++) {
    pid[i] = fork(); //cria o processo
    if (pid[i] == 0) {
      int indexi = i;
      int indexj = 0;
      int n = 7; //grau de blur
      while(1) {
        if (indexi >= (*p_img).width) {
          indexi = indexi - (*p_img).width;
          indexj++;
        }
        if (indexj >= (*p_img).height) exit(0); //se sair da imagem
        else{

          //encontra o `i` inicial e final e o `j` inicial e final que a thread vai ler para calcular a media
          int start_i = indexi - n;
          int start_j = indexj - n;
          int end_i = indexi + n;
          int end_j = indexj + n;

          //soma dos valores de r, g, b
          float sum1 = 0;
          float sum2 = 0;
          float sum3 = 0;

          //contar quantos pixels ele passa
          int cont = 0; 

          //tratando se o `i` e `j` estiverem fora da imagem
          if (indexi - n < 0) start_i = 0;
          if (indexj - n < 0) start_j = 0;
          if (indexi + n > (*p_img).width) end_i = (*p_img).width;
          if (indexj + n > (*p_img).height) end_j = (*p_img).height;
          
          //fazendo o somatorio dos pixels
          for (int sti = start_i; sti < end_i; sti++) {
            for (int stj = start_j; stj < end_j; stj++) {
              cont++;
              int idx = sti + (stj*(*p_img).width);
              sum1+=(*p_img).r[idx];
              sum2+=(*p_img).g[idx];
              sum3+=(*p_img).b[idx];
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
          (*p_img2).r[indexi + (indexj*(*p_img).width)] = res1;
          (*p_img2).g[indexi + (indexj*(*p_img).width)] = res2;
          (*p_img2).b[indexi + (indexj*(*p_img).width)] = res3;
          indexi+=6; // vai de 6 em 6 pois existem 6 processos
        }
      }
    }
  }

  // espera os processos terminarem
  for (int k = 0; k < 6; k++){
    waitpid(pid[k], NULL, 0);
  }

  gettimeofday(&rt1, NULL); //pegar o tempo logo apos terminar o filtro
  salvar_imagem(buffer2, &(*p_img2));
  timersub(&rt1, &rt0, &drt);
  printf("%ld.%06ld ", drt.tv_sec, drt.tv_usec); //imprimir o tempo de execucao do filtro
  return 0;
}
