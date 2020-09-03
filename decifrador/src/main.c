/* Tarefa de decifrar uma cifra
 *
 * Dica: veja em referencia/cifrador.c o codigo-fonte do programa
 * que gerou as cifras que voce precisa decifrar!
 */

#include <stdio.h>

#define TAM_BUFFER 60
char buffer_entrada[TAM_BUFFER];
char buffer_saida[TAM_BUFFER];
const char key[] = "SENHASECRETA"; // chave utilizada para a cifra

/*
 * Vigenère cipher
 * 
 * supondo que A = 0, B = 1, ..., Z = 25
 * _K: valor numerico da chave
 * _C: valor numerico do caractere cifrado
 * a: caractere cifrado
 * b: caractere da chave
 */
char decifra_soma_modulo_alfabeto(char a, char b) {
  char a0, b0, c0;
  if (a>='A' && a<='Z') { // apenas caracteres entre A e Z sao cifrados e decifrados
    a0 = a-'A'; // a0 recebe o valor numerico de 'a' ao ser subtraido pelo valor da tabela ascii de A
    b0 = b-'A'; // b0 recebe o valor numerico de 'b' ao ser subtraido pelo valor da tabela ascii de A
    c0 = 'A'+(a0-b0+26)%('Z'-'A'+1); // caractere decifrado sera  o modulo da divisao de (_C - _K + 26) por 26 e somado ao valor de 'A' na tabela ascii para retornar o caractere decifrado de acordo com a tabela ascii
    return c0;
  } else return a;
}

int main() {
  /* Leitura da entrada */
  fgets(buffer_entrada, TAM_BUFFER, stdin);

  int i = 0;
  int j = 0;

  /*
   * percorre todos os caracteres da entrada
   */
  for (i=0; (i<TAM_BUFFER) && (buffer_entrada[i]!='\0') && (buffer_entrada[i]!='\n'); i++) {

    buffer_saida[i] = decifra_soma_modulo_alfabeto(buffer_entrada[i], key[j]); // decifra letra por letra
    j++;
    if (key[j]=='\0') j=0; // a chave sera percorrida ciclicamente
  }
  buffer_saida[i]='\0'; // insere uma quebra de linha ao final da frase decifrada

  /* Escrever saida na tela */
  printf("%s\n", buffer_saida);

  return 0;
}
