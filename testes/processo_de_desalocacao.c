#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include "progresso.h"
#include "filacircular_ref.h"
#include "pilhaligada_ref.h"
#include "legivel.h"
#include "teste.h"
#include "aleatorio.h"

static size_t SEED_AIA = 0;
const int QTD_MINIMA = 0;

/* === === === === === === === === === === === === === === === === === ===+
 *                         Funções de Desalocação
 *                      e de Alocação em Massa
 * === === === === === === === === === === === === === === === === === ===*/

// Apenas para alocar memória, o conteúdo em sí não importa.
static int* aloca_inteiro_arbitrario(void) {
   const int sz = sizeof(int);
   int* X = malloc(sz);
   size_t seed = SEED_AIA;

   if (seed % 2 == 0)
      *X = INT_MIN;
   else if (seed % 3 == 0)
      *X = INT_MAX / 2;
   else if (seed % 5 == 0)
      *X = 1 + INT_MAX / 2;
   else if (seed % 7 == 0)
      *X =  INT_MAX / 2 - 1;
   else
      *X = INT_MAX;

   SEED_AIA++;
   return X;
}

static void corta_na_metade_fila(FilaCircular Q, Drop del) {
   size_t t = comprimento_fc(Q) / 2;
   
   while (comprimento_fc(Q) > t) 
      del(retira_fc(Q));
   malloc_trim(QTD_MINIMA);
}

static size_t preenche_ela_com_muita_memoria(FilaCircular q) {
   const size_t MEGABYTE = pow(2, 20);
   const size_t sz = sizeof(int);
   const size_t N = 15 * MEGABYTE / sz;
   PG bar = cria_bp(Temporal, N, 63);

   for (size_t i = 1; i <= N; i++) 
   {
      int* X = aloca_inteiro_arbitrario();
      insere_fc(q, X);

      // Referente a barra de progresso.
      atualiza_bp(&bar, i);
      visualiza_bp(&bar);
   }
   return N;
}

static int STRING_MEDIA;
static int QTD_STR = 1;

char* string_aleatoria(void) {
   int sz = sizeof(char);
   char buffer[50], *string = malloc(50 * sz);
   size_t total = inteiro_positivo(20, 45);

   for (int i = 1; i <= 50; i++) {
      if ((i - 1) < total)
         // buffer[i - 1] = ascii_char_aleatorio();
         buffer[i - 1] = alfabeto_aleatorio();
      else
         buffer[i - 1] = '\0';
   }

   // Computa média aritmética do comprimento das strings geradas.
   STRING_MEDIA += total / ++QTD_STR;

   memcpy(string, buffer, 50);
   return string;
}

void fill_up_with_random_str(FilaCircular q, size_t n) {
   PG bar = cria_bp(Temporal, n, 63);

   for (size_t k = 1; k <= n; k++) {
      char* X = string_aleatoria(); 
      insere_fc(q, X);

      // Referente a barra de progresso.
      atualiza_bp(&bar, k);
      visualiza_bp(&bar);
   }
}

void fill_stack_up_with_random_str(PilhaLigada q, size_t n) {
   PG bar = cria_bp(Temporal, n, 63);

   for (size_t k = 1; k <= n; k++) {
      char* X = string_aleatoria(); 
      coloca_pl(q, X);

      // Referente a barra de progresso.
      atualiza_bp(&bar, k);
      visualiza_bp(&bar);
   }
}

bool free_str(generico_t a)
   { free((char*)a); return true; }

bool free_int32(generico_t a)
   { free((int*)a); return true; }

void libera_porcentual_da_fila(FilaCircular q, Drop del, float p) {
   if (p < 0.0 || p > 1.0) {
      printf("Valor %0.2f é inválido; apenas entre 0 e 1!", p);
      abort();
   }

   size_t C = comprimento_fc(q);
   size_t t = C * p;

   for (size_t i = 1; i < t; i++)
      del(retira_fc(q));
   malloc_trim(QTD_MINIMA);
}

void libera_metade(FilaCircular q, Drop del) 
   { libera_porcentual_da_fila(q, free_int32, 0.5); }

/* === === === === === === === === === === === === === === === === === ===+
 *                      Testes de cada 
 *                            Estrutura em Si
 * === === === === === === === === === === === === === === === === === ===*/

void libera_gigantesca_memoria_quando_ordenado(void) {
   FilaCircular queue = cria_fc();
   const int sz = sizeof(int);

   puts("Inserir 15 MiB nela?");
   getchar();
   preenche_ela_com_muita_memoria(queue);


   printf(
      "Cortar a 'fila' na metade?(%s)",
      tamanho_legivel(tamanho_fc(queue, sz))
   );
   getchar();
   corta_na_metade_fila(queue, free_int32);

   puts("Destrói a lista?"); getchar();
   destroi_inner_fc(queue, free_int32);

   puts("Abandonar programa?"); getchar();
}

void desalocacao_da_fila_circular(void) {
   FilaCircular queue = cria_fc();

   puts("Preencher a fila?"); getchar();
   fill_up_with_random_str(queue, 101667);
   printf(
      "Total de itens(%s): %zu\n", 
      tamanho_legivel(tamanho_fc(queue, STRING_MEDIA)), 
      comprimento_fc(queue)
   );

   printf("Média aritmética das strings: %d\n", STRING_MEDIA);
   puts("Liberar metade da memória?"); getchar();
   libera_porcentual_da_fila(queue, free_str, 0.47);

   printf(
      "Total de itens(%s): %zu\n", 
      tamanho_legivel(tamanho_fc(queue, STRING_MEDIA)), 
      comprimento_fc(queue)
   );
   puts("Destruir a fila por completo?"); getchar();
   destroi_inner_fc(queue, free_str);

   puts("Finalizar o programa?"); getchar();
}

void preenche_pilha_ligada_com_strings_randomicas(PilhaLigada stack, 
 int n)
{
   size_t k = 1;

   do {
      coloca_pl(stack, string_aleatoria());
      k++;
   } while (k < n);
}

void libera_metade_da_pilha(PilhaLigada s) {
   int t = comprimento_pl(s) / 2;

   for (int i = 1; i < t; i++)
      free(retira_pl(s));
}

void desalocacao_da_pilha_array(void) {
   PilhaLigada stack = cria_pl();

   fill_stack_up_with_random_str(stack, 60);
   printf(
      "Total de itens(%s): %zu\n", 
      tamanho_legivel(tamanho_pl(stack, 30)), 
      comprimento_pl(stack)
   );

   puts("Liberar metade da memória?");
   getchar();
   libera_metade_da_pilha(stack);
   printf(
      "Total de itens(%s): %zu\n", 
      tamanho_legivel(tamanho_pl(stack, 30)), 
      comprimento_pl(stack)
   );

   puts("Destruir a fila por completo?");
   getchar();
   free(stack);

   puts("Finalizar o programa?");
   getchar();
   destroi_interno_pl(stack, free_str);
}

int main(void) {
   // desalocacao_da_fila_circular();
   // libera_gigantesca_memoria_quando_ordenado();

   executa_testes (
      1, desalocacao_da_pilha_array
   );
   return EXIT_SUCCESS;
}
