/* Criei recente o algoritmo de 'strip' para strings em C, entretanto, usei um algoritmo
 * bem complexo ao invés de um simples com pilhas. Aqui, com a implementação de pilhas
 * do C, usarei de tal algoritmo. 
 */
// Importando biblioteca padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
// Importando utilitários desta biblioteca:
// #include "aleatorio.h"
#include "pilhaligada_ref.h"
#include "primitivos.h"
// Bibliotecas do Linux:
// Dados para testes:
#include "dados-testes.h"

LStack empilha_uma_string(char* input)
{
   int n; const int N = strlen(input);
   LStack output = new_pl();

   for (n = 0; n < N; n++)   
      push_pl(output, &input[n]);
   return output;
}

bool o_topo_da_pilha_pertence(LStack input, char* pattern)
{
   char* topo = top_pl(input);
   const int LEN = strlen(pattern);
   char letra;

   for (auto k = 0; k < LEN; k++)
   {
      letra = pattern[k];

      if (*topo == letra)
         return true;
   }
   return false; 
}

void aplica_algoritmo_strip(LStack input, char* pattern)
{
// Realiza a aparação, o resultado na ponta desejada, dependende inteiramente se a pilha
// está invertida ou não.
   while (o_topo_da_pilha_pertence(input, pattern))
   {
      char* X = pop_pl(input);
      printf("Removido: %c\n", *X);
   }
}

LStack pilha_invertida(LStack input) {
// Pega uma pilha, inverte seus itens, então retorna a pilha com a inversa.
   LStack output = new_pl();

   while (!empty_pl(input))
      push_pl(output, pop_pl(input));

   destroy_pl(input);
   return output;
}

void transforma_pilha_numa_string(LStack input, char* output)
{
// Consume toda pilha colocando os itens no buffer da string como foram inicialmente 
// empilhados.
   char* letra = NULL;
   int cursor = 0;

   while (!empty_pl(input))
   {
      letra = pop_pl(input);
      output[cursor] = *letra;
      cursor++;
   }
   output[cursor] = '\0';
   destroy_pl(input);
}

void exemplo_feito_com_strip_suffix(void)
{
   char* exemplo = (char*)fruits[FRUITS / 2];
   char buffer[strlen(exemplo)];
   LStack pilha = empilha_uma_string(exemplo);

   printf("\nExemplo: '%s'\n", exemplo);
   print_pl(pilha, debug_char);
   aplica_algoritmo_strip(pilha, "ay");
   print_pl(pilha, debug_char);
   transforma_pilha_numa_string(pilha, buffer);
   printf("Exemplo: '%s'\n", buffer);
}

void exemplo_feito_com_strip_prefix(void)
{
   char* exemplo = (char*)fruits[FRUITS - 5];
   char buffer[strlen(exemplo)];
   LStack pilha = pilha_invertida(empilha_uma_string(exemplo));

   printf("\nExemplo: '%s'\n", exemplo);
   print_pl(pilha, debug_char);
   aplica_algoritmo_strip(pilha, "mra");
   print_pl(pilha, debug_char);
   transforma_pilha_numa_string(pilha, buffer);
   printf("Exemplo: '%s'\n", buffer);
}

int main(int quantia, char* args[], char* env[]) 
{
   exemplo_feito_com_strip_suffix();
   exemplo_feito_com_strip_prefix();
   return EXIT_SUCCESS;
}
