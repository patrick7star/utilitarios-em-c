// Bibliotec padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
// Da própria biblioteca:
#include "definicoes.h"
#include "macros.h"
#include "conjunto_ref.h"
#include "memoria.h"
#include "arvoreligada.h"
#include "dados-testes.h"
#include "legivel.h"
// Auxiliares:
// API do Linux:
#include <unistd.h>
#include <sys/time.h>


void adicao_de_forma_iteravativa(Tree output, const size_t TOTAL);
static bool libera_int(GenT X)
   { free(X); return true; }

struct timeval diferenca(struct timeval a, struct timeval b)
{  
   return (struct timeval){
      .tv_usec = (b.tv_usec -  a.tv_usec),
      .tv_sec = (b.tv_sec - a.tv_sec)
      };
}

int main(int qtd, char* args[], char* envs[])
{
   struct timeval inicio, fim, decorrido;
   Tree obj = tree_cria_i();
   size_t quantia; 
   char* tempostr = NULL, *quantiastr = NULL;
   const size_t M = 5e5L;

   // Faz a medição de um bloco de inserções.
   gettimeofday(&inicio, NULL);
   adicao_de_forma_iteravativa(obj, M);
   gettimeofday(&fim, NULL);
   
   decorrido = diferenca(inicio, fim);
   quantia = tree_quantidade(obj);
   tempostr = tempo_legivel_timeval(decorrido);
   quantiastr = valor_legivel(quantia);

   printf("\nTotal de itens inseridos: %s\n", quantiastr);
   printf("A execução da inserção levou %s.\n", tempostr);
   tree_destroi(obj);
   free(tempostr);
   free(quantiastr);

   return EXIT_SUCCESS;
}


static int16_t* novo_numero(void)
{
   static short CONTAGEM = -100;

   return box_i16(++CONTAGEM);
}

void adicao_de_forma_iteravativa(Tree output, const size_t TOTAL)
{
   // Tamanho líquido igual a 10.
   const int N = 12;
   Cursor adicoes[N + 3];
   int16_t* X = NULL,* Y = NULL;
   Tree out = output;
   int c = 0x00000001, r = 0x00000000;
   size_t quantia;

   X = novo_numero();
   adicoes[0] = tree_adiciona_raiz(out, X);

   // Adiciona os dois primeiros elementos no único nó da árvore. Uma em 
   // cada lado.
   X = novo_numero();
   Y = novo_numero();
   adicoes[c + 0] = tree_adiciona_direita(out, adicoes[r], Y);
   adicoes[c + 1] = tree_adiciona_esquerda(out, adicoes[r], X);
   c += 2;
   r++;

   // Repete o último passo acima continuamente até que os pares se esgotem.
   do {
      if (c >= N) {
         c = 1; r = 0;
         adicoes[0] = adicoes[N - 3];
         adicoes[1] = adicoes[N - 2];
         adicoes[2] = adicoes[N - 1];
      }

      X = novo_numero();
      Y = novo_numero();
      adicoes[c + 0] = tree_adiciona_direita(out, adicoes[r], Y);
      adicoes[c + 1] = tree_adiciona_esquerda(out, adicoes[r], X);
      c += 2;
      r++;

      quantia = tree_quantidade(output);
      
   } while (quantia < TOTAL);
}
