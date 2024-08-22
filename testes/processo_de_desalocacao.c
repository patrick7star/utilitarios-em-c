#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "filacircular_ref.h"
#include "pilhaligada_ref.h"
#include "aleatorio.h"
#include "legivel.h"

char* string_aleatoria(void) {
   char buffer[50], *string = calloc(50, sizeof(char));
   int total = inteiro_positivo(20, 45);

   for (int i = 1; i <= 50; i++) {
      if ((i - 1) < total)
         buffer[i - 1] = ascii_char_aleatorio();
      else
         buffer[i - 1] = '\0';
   }

   memcpy(string, buffer, 50);
   return string;
}

void preenche_com_strings_aleatorias(FilaCircular q, size_t n) {
   size_t k = 1;

   do {
      insere_fc(q, string_aleatoria());
      k++;
   } while (k < n);
}

void amostra_de_strings_aleatorias(void) {
   for (int i = 1; i <= 15; i++) {
      char* s = string_aleatoria();
      puts(s);
      free(s);
   }
}

bool free_str(generico_t a)
   { free(a); return true; }

void libera_metade(FilaCircular q) {
   int t = comprimento_fc(q) / 2;

   for (int i = 1; i < t; i++)
      free(retira_fc(q));
}

void desalocacao_da_fila_circular(void) {
   FilaCircular queue = cria_fc();

   preenche_com_strings_aleatorias(queue, 60);
   printf(
      "Total de itens(%s): %zu\n", 
      tamanho_legivel(tamanho_fc(queue, 30)), 
      comprimento_fc(queue)
   );

   puts("Liberar metade da memória?");
   getchar();
   libera_metade(queue);
   printf(
      "Total de itens(%s): %zu\n", 
      tamanho_legivel(tamanho_fc(queue, 30)), 
      comprimento_fc(queue)
   );

   puts("Destruir a fila por completo?");
   getchar();
   free(queue);

   puts("Finalizar o programa?");
   getchar();
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

   preenche_pilha_ligada_com_strings_randomicas(stack, 60);
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
   desalocacao_da_pilha_array();
   return EXIT_SUCCESS;
}
