/* Compilando um monte de objetos dos arquivos-fontes, então que usar-los
 * para compilação de terceiros, verificando se a compilação de apenas suas
 * bibliotecas estáticas funcionam. Aqui irei testar algumas funções(sim,
 * não todas) de cada um destes objetos gerados.
 */

// Importando biblioteca padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
// Importando utilitários desta biblioteca:
#include "legivel.h"
#include "tempo.h"
#include "barra_de_progresso.h"
#include "aleatorio.h"
#include "ponto.h"
// Bibliotecas do Linux:
// Dados para testes:
#include "dados_testes.h"

void funcoes_do_aleatorio(void) {
   printf("Letras aleatórias:");
   for (int p = 1; p <= 17; p++)
      printf("|%c| ", alfabeto_aleatorio());
   putchar('\n');

   printf("Letras aleatórias ASCII:");
   for (int p = 1; p <= 11; p++)
      printf("'%c' ", alfabeto_aleatorio());
   putchar('\n');

   puts("Números randômicos entre 1 e 100:");
   for (int p = 1; p <= 13; p++)
      printf("\tseleção: %zu\n", inteiro_positivo(1, 100));
   putchar('\n');
}

void funcoes_do_legivel(void) {
   for (int i = 1; i <= VALORES_PADRONIZADOS_I; i++) {
      size_t valor = valores_padronizados_i[i - 1];
      char* valor_fmt = tempo_legivel(valor);

      printf("\t%zu ===> %s\n", valor, valor_fmt);
      free(valor_fmt);
   }
}

static bool e_par(int k) { return k % 2 == 0;}
static void pausa_ms_randomica(int k) {
   if (e_par(k))
      breve_pausa(Miliseg, 250);
   else
      breve_pausa(Miliseg, 370);
}

void funcoes_do_tempo(void) {
#ifdef _POSIX_C_SOURCE
   Cronometro _clock = cria_cronometro();

   for (int k = 1; k <= 37; k++) {
      printf("decorrido: %lf\n", marca(_clock));
      pausa_ms_randomica(k);
   }

   destroi_cronometro(_clock);
#endif
}

void funcoes_do_progresso(void) {
#ifdef _POSIX_C_SOURCE
   const size_t COUNT = 37;
   PS bar = novo_bps(COUNT, 40);

   for (int k = 1; k <= COUNT; k++) { 
      visualiza_bps(bar); 
      atualiza_bps(bar, k); 
      pausa_ms_randomica(k);
   }
   visualiza_bps(bar); 

   destroi_bps(bar);
#endif
}

void funcoes_de_ponto(void) {
   Ponto a = cria_ponto(16, 15); 
   char* ponto_a_fmt = ponto_to_str(a);

   printf("%s\n", ponto_a_fmt);
   free(ponto_a_fmt);


   Ponto b = cria_ponto(1, 100);
   imprime_ponto(b);

   printf("\ndistância entre eles: %u u.c\n", distancia_ponto(a, b));
}

int main(void) {

   /*
   funcoes_do_legivel();
   funcoes_do_tempo();
   funcoes_do_progresso();
   funcoes_do_aleatorio();
   */
   funcoes_de_ponto();

   return EXIT_SUCCESS;
}
