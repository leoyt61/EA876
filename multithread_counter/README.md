# Contando números primos usando multithread

Esta tarefa consiste em receber um texto na entrada e contar quantos números
primos existem nessa entrada. Porém, trata-se de
um exercício de programação, e não somente de uma solução com algoritmo.

**O objetivo desta tarefa é fazer o sistema operar como multi-thread,
usando a instrução `pthread_create()`.**

As soluções deverão ser estruturadas de forma que:

1. cada número deve ser analisado (quanto a ser primo ou não) por
uma thread independente.
1. As threads deverão (ao menos potencialmente) poder
executar em paralelo.
1. O programa deverá assumir um **número máximo de threads paralelas** igual a
`4`.
1. Todas as threads devem encerrar *antes* do fim do programa principal

**AVISO: os números serão inteiros, sem sinal, entre 0 e 2^(64)-1**

Soluções que não contemplem esses três requisitos não serão aceitas. Eles serão
verificados manualmente, caso a caso.

## Exemplos

Entrada | Saida
------- | -----
`0\n` | `0\n`
`1\n` | `0\n`
`2 1\n` | `1\n` (operando em duas threads)
`2 1 0\n` | `1\n` (operando em três threads)
`3 5 2 7 9 1 0\n | `4\n` (operando em quatro threads)

## Instruções adicionais

1. Vá diretamente ao conjunto de testes para verificar como tratar casos
   limítrofes ou mal-definidos.
1. Lembre-se de seguir as [instruções de submissão](docs/instrucoes.md).
1. Se desejar, proponha novos casos de teste.
