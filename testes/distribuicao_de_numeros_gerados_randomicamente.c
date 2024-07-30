#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <inttypes.h>
#include "aleatorio.h"
#include "barra_de_progresso.h"
#include "hashtable_ref.h"

typedef struct {
   // Quantia de dígitos:
   size_t um;
   size_t dois;
   size_t tres;

   // Total de itens processados.
   size_t total;

   // Tabela com a frequência dos números sorteados:
   HashTable tabela;

} DistribuicaoInfo;

static size_t bv_gerados = 0;
static size_t bc_gerados = 0;

size_t* box_value(size_t x) {
   size_t* dado, size = sizeof(size_t);
   dado = malloc(size);
   *dado = x;
   bv_gerados++;
   return dado;
}

uint16_t* box_chave(uint16_t y) {
   uint16_t* dado;
   size_t size = sizeof(uint16_t);
   dado = malloc(size);
   *dado = y;
   bc_gerados++;
   return dado;
}

char* stringfy_sizet(generico_t dt) {
   size_t vl = *((size_t*)dt);
   char* fmt = malloc(15 * sizeof(char));

   sprintf(fmt, "%zu", vl);
   return fmt;
}

char* stringfy_u16(generico_t dt) {
   uint16_t vl = *((uint16_t*)dt);
   char* fmt = malloc(15 * sizeof(char));

   sprintf(fmt, "%u", vl);
   return fmt;
}

static size_t localizacao(generico_t dt, size_t c) {
   size_t n = *((uint16_t*)dt);
   return n % c; 
}

static bool igualdade(generico_t a, generico_t b) {
   uint16_t x = *((uint16_t*)a);
   uint16_t y = *((uint16_t*)b);

   return x == y;
}

DistribuicaoInfo inicializa(void) {
   return (DistribuicaoInfo){ 0, 0, 0, 0, cria_ht(localizacao, igualdade)};
}

void free_distribuicao(DistribuicaoInfo* a) {
   destroi_ht(a->tabela); 
   a->tabela = NULL; 
   a->um = 0; 
   a->dois = 0;
   a->tres = 0;
}

static size_t* ZERO = NULL;
static bool ZERO_ALOCADO = false;

void insere_na_tabela(HashTable m, uint16_t N) {
   if (!ZERO_ALOCADO) {
      assert (ZERO == NULL);
      ZERO_ALOCADO = true;
      ZERO = box_value(0);
      assert (ZERO != NULL);
   }

   // Atualizando frequência de tal número.
   uint16_t* key = box_chave(N);
   // Insere zero reserva na tabela.
   bool insercao_funcionou = insere_ht(m, key, ZERO);

   if (insercao_funcionou) {
      /* Atualiza para um novo zero. Método,... ousado, para não fazer
       * milhões de chamadas de sistemas prá alocar zeros de modo adoidado.
       */
      size_t* vl = box_value(0);
      assert(atualiza_ht(m, key, vl));
   } else {
      size_t* ptr_vl = (size_t*)obtem_ht(m, key);
      assert (ptr_vl != NULL);
      *ptr_vl += 1;
      free(key);
   }
}

void processa(DistribuicaoInfo* R, size_t N) {
   insere_na_tabela(R->tabela, (uint16_t)N);

   if (N < 10)
      R->um += 1;
   else if (N >= 10 && N <= 99)
      R->dois += 1;
   else
      R->tres++;
   R->total += 1;
}

void visualiza(DistribuicaoInfo* R) {
   float T = (float) R->total;
   float p1 = (float)R->um / T;
   float p2 = R->dois/ T;
   float p3 = R->tres/ T;

   printf("\nAnálize final da distribuição de %zu:\n", R->total);
   printf("\b\b\t1 - 9:%25.2f%%\n", p1 * 100.0);
   printf("\b\b\t10 - 99:%23.2f%%\n", p2 * 100.0);
   printf("\b\b\t100 - 1000:%20.2f%%\n", p3 * 100.0);
}

float media_aritmetica_da_frequencia_de_sorteio(HashTable m) {
   float soma = 0.0;
   generico_t* vA = valores_ht(m);
   size_t t = tamanho_ht(m);

   for (size_t i = 1; i <= t; i++) {
      size_t x = *((size_t*)vA[i - 1]);
      soma += (float)x;
   }
   free(vA);

   return soma / (float)t;
}

int main(void) {
   const size_t TOTAL[] = { 16201092, 50832 };
   ProgressoSimples bar = novo_bps(TOTAL[0], 50);
   DistribuicaoInfo report = inicializa();

   puts("Dez números gerados aleatoriamente:");
   for (int i = 1; i <= 10; i++) 
      printf("\t%02dº ===> %2zu\n", i, inteiro_positivo(1, 19));

   for (size_t i = 1; i <= TOTAL[0]; i++) {
      processa(&report, inteiro_positivo(0, 1000));
      atualiza_bps(bar, i);
      visualiza_bps(bar);
   }

   size_t Q = tamanho_ht(report.tabela);
   printf("Total de chaves: %zu\n", Q);
   assert(Q == 1001);
   printf(
      "boxes-de-chaves:%zu\tboxes-de-values:%zu\n",
      bc_gerados, bv_gerados
   );

   visualiza(&report);
   printf(
      "Média de frequência: %0.1f sorteios\n",
      media_aritmetica_da_frequencia_de_sorteio(report.tabela)
   );
   destroi_bps(bar);
   free_distribuicao(&report);

   puts("\nA distribuição se tirarem os números de três dígitos:");
   bar = novo_bps(TOTAL[1], 70);
   report = inicializa();

   for (size_t i = 1; i <= TOTAL[1]; i++) {
      processa(&report, inteiro_positivo(0, 99));
      atualiza_bps(bar, i);
      visualiza_bps(bar);
   }

   visualiza(&report);
   imprime_ht(report.tabela, stringfy_u16, stringfy_sizet);
   destroi_bps(bar);
   printf(
      "Média de frequência: %0.1f sorteios\n",
      media_aritmetica_da_frequencia_de_sorteio(report.tabela)
   );

   return 0;
}
