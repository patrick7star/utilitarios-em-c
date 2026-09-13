// Módulos dos projeto:
#include "pilhaligada_ref.h"
#include "hashtable_ref.h"
#include "dados-testes.h"
#include "primitivos.h"
#include "memoria.h"
// Biblioteca padrão do C:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void iteracao_da_pilha_ligada(void) {
   PilhaLigada stack = cria_pl();
   int k = 0xabc;
   char* data = NULL;

   assert (vazia_pl(stack));
   imprime_pl(stack, debug_string);
   puts("Okay, it is at the beginning, empty as hell.");

   for (k = 0; k < OBJECTS; k++)
   {
      data = (char*)objects[k];
      assert (coloca_pl(stack, (GenT)data));
   }

   IterPL iter = cria_iter_pl(stack);
   IterOutputPL saida;

   puts("\nUsando a iteração da estrutura ...");
   do {
      saida = next_pl(iter);
      data = (char*)saida.item;
      printf("\t\b\b\b\b- %s\n", data);
   } while(!consumido_iter_pl(iter));

   puts(
      "\nImprimindo a pilha novamente para mostrar que nenhum dado"
      " foi perdido..."
   );
   imprime_pl(stack, debug_string);
   destroi_pl(stack);
}

void iteracao_da_hashmap(void)
{
   HashTable obj; 
   Hash hash = hash_string;
   Eq equal = eq_string;
   IterOutputHT out;
   int32_t* list = NULL, n, *value = NULL;
   IterHT iter = NULL;
   char* key = NULL;

   list = (int*)valores_padronizados_iv;
   obj = new_ht(hash, equal);

   for (n = 0; n < VEHICLES; n++)
      add_ht(obj, (GenT)vehicles[n], box_i32(list[n]));

   print_ht(obj, debug_string, debug_i32);
   iter = cria_iter_ht(obj);

   do {
      out = next_ht(iter);
      key = out.key;
      value = out.value;

      printf("\t- '%s': %d\n", key, *value);
      free(value);
   } while (!consumido_iter_ht(iter));

   drop_ht(obj);
   destroi_iter_ht(iter);
}

int main(void) {
   iteracao_da_pilha_ligada();
   iteracao_da_hashmap();
   return 0;
}
