#include "pilhaligada_ref.h"
#include "hashtable_ref.h"

#include "dados_testes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char* to_string(generico_t x) {
   char* data = (char*)x;
   size_t size = strlen(data) * sizeof(char);
   char* fmt = malloc (size);

   strcpy(fmt, data);
   return fmt;
}

void iteracao_da_pilha_ligada(void) {
   PilhaLigada stack = cria_pl();

   assert (vazia_pl(stack));
   imprime_pl(stack, to_string);
   puts("Okay, it is at the beginning, empty as hell.");

   for (int k = 0; k < OBJETOS; k++) {
      const char* s = objetos[k];
      generico_t dt = (generico_t)s;

      assert (coloca_pl(stack, dt));
      imprime_pl(stack, to_string);
   }

   generico_t* array_de_dados = pilha_to_array_pl(stack);
   size_t Q = comprimento_pl(stack);

   puts("\nIterando todos itens via array(e seu reverso)...");
   for (int i = 1; i <= Q; i++) {
      char* dt_a = array_de_dados[i - 1];
      char* dt_b = array_de_dados[Q - i];
      printf("\t\b\b\b\b- %s\t\t - %s\n", dt_a, dt_b);
   }

   puts(
      "\nImprimindo a pilha novamente para mostrar que nenhum dado"
      " foi perdido..."
   );
   free(array_de_dados);
   imprime_pl(stack, to_string);
   destroi_pl(stack);
}

size_t hash_u16 (generico_t dt, size_t cp) {
// Este não leva em conta o endereço virtual de memória do argumento.
   uint16_t* ptr = dt;
   uint16_t chave = *ptr;
   return  chave * (chave - chave / 2) % cp;
}

bool eq_u16 (generico_t a, generico_t b) 
   { return *((uint16_t*)a) == *((uint16_t*)b); }

char* tostring_u16(generico_t e) {
   char* u16_fmt = malloc(6 * sizeof(char));
   sprintf(u16_fmt, "%u", *((uint16_t*)e));
   return u16_fmt;
}

void array_e_iteracao_de_uma_hashtable(void) {
   HashTable D = cria_ht(hash_u16, eq_u16);
   int T = BOYS_NAMES;

   for (int p = 1; p <= T; p++) {
		int t = p - 1;
      uint16_t* k = (uint16_t*)&valores_padronizados_i[t];
      char* v = (char*)boys_names[t];
      assert(insere_ht(D, k, v));
   }
   generico_t* keys_iter = chaves_ht(D);
   generico_t* values_iter = valores_ht(D);

   printf(
      "\nTotal de itens adicionados: %zu\n\t(%llu e %llu)\n", 
      tamanho_ht(D), BOYS_NAMES, VALORES_PADRONIZADOS_I
   );
   imprime_ht(D, tostring_u16, to_string);
   puts("Todos valores do dicionário:");

   for (int p = 1; p <= tamanho_ht(D); p++) 
   {
      uint16_t* key = keys_iter[p - 1];
      char* value = values_iter[p - 1];

      printf("\t\b\b\b- %u: '%s'\n", *key, value);
   }
   destroi_ht(D);
}


int main(void) {
   iteracao_da_pilha_ligada();
   array_e_iteracao_de_uma_hashtable();
   return 0;
}
