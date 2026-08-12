/*   Todos testes unitários do módulo pilha-ligada-ref. 
 *   Tem que ser incluido exatamente aonde os trechos foram recortados para serem colocados 
 * aqui, não é garantido que a inclusão em outra parte no módulo principal funcione 
 * corretamente.
 */

// Biblioteca padrão do C:
#include <locale.h>
#include <assert.h>
#include <tgmath.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
// Própria biblioteca:
#include "progresso.h"
#include "teste.h"
#include "dados_testes.h"
#include "macros.h"
#include "primitivos.h"

// Funções auxiliares nos testes abaixos:
size_t computa_tamanho_da_pilha (PilhaLigada s);
size_t unidades_por_megabytes (uint16_t nMB, size_t size);
bool free_str(generico_t b) { free(b); return true; }
bool deleta_u16(generico_t dt) { free(dt); return true; }
char* conststr_to_str(generico_t dt); 
generico_t clona_constchar(generico_t a);

TESTE pilha_com_i32s (void); 
TESTE pilha_de_strings (void);
TESTE verificando_vazamento_de_memoria (void); 
TESTE estruturas_tamanhos (void); 
TESTE verificando_vazamento_de_memoria_i (void); 
TESTE amostra_simples_de_todos_seus_metodos(void); 
TESTE processo_de_inversao_da_lista_ligada(void); 
TESTE metodo_de_clonagem_da_pilha(void);
TESTE transforma_a_pilha_numa_array(void);

int main(int total, char* args[], char* vars[]) 
{
   // permitindo impressões de caractéres 'latin1'.
   setlocale(LC_CTYPE, "pt_BR.UTF-8");

   // Verificando algumas coisas da linguagem:
   executa_testes_b(false, 1, Unit(estruturas_tamanhos, true));

   executa_testes_b(
      true, 6,
         Unit(pilha_com_i32s, true),
         Unit(pilha_de_strings, true),
         Unit(amostra_simples_de_todos_seus_metodos, true),
         Unit(processo_de_inversao_da_lista_ligada, true),
         Unit(metodo_de_clonagem_da_pilha, true),
         Unit(transforma_a_pilha_numa_array, true)
   );

   // Desativada pois consome bastante CPU e memória:
   executa_testes_b(
      false, 2,
         Unit(verificando_vazamento_de_memoria, true),
         Unit(verificando_vazamento_de_memoria_i, false)
   );
   // fim do programa.
   return EXIT_SUCCESS;
}

TESTE transforma_a_pilha_numa_array(void)
{
   LStack pilha = new_pl();
   char In[] = "aeiou";
   int In_a[] = {7777777, 666666, 55555, 4444, 333, 22, 1};
   int sz = sizeof(char), N;
   char* Output = NULL;
   int* Out = NULL, k;
   const int Na = sizeof(In_a) / sizeof(int);

   push_pl(pilha, &In[0]);
   push_pl(pilha, &In[1]);
   push_pl(pilha, &In[2]);
   push_pl(pilha, &In[3]);
   push_pl(pilha, &In[4]);

   N = length_pl(pilha);
   print_pl(pilha, debug_char);
   printf("\tTopo: '%c'\n\n", *((char*)top_pl(pilha)));
   Output = into_array_pl(pilha, sz);

   for (k = 0; k < N; k++)
      printf("\t%c ---- %c\n", In[k], Output[k]);

   pilha = new_pl();

   for (k = 0; k < Na; k++)
      push_pl(pilha, &In_a[k]);

   print_pl(pilha, debug_int);
   Out = into_array_pl(pilha, sizeof(int));

   for (k = 0; k < Na; k++)
      printf("\t%-9d ---- %9d\n", In_a[k], Out[k]);

   printf("Valor do meio(original): %d\n", In_a[Na / 2]);
   printf("Valor do meio(copia): %d\n", Out[Na / 2]);
   In_a[Na / 2] += 5;
   puts("Após modificação na array original ...");
   printf("Valor do meio(original): %d\n", In_a[Na / 2]);
   printf("Valor do meio(copia): %d\n", Out[Na / 2]);
   puts("Mostra que realmente é uma cópia.");
}

TESTE pilha_com_i32s (void) {
   PilhaLigada stack = cria_pl();
   int array[] = {38, 1, -15};
   int* ptr;

   coloca_pl(stack, &array[1]);
   coloca_pl(stack, &array[0]);
   assert (coloca_pl(stack, &array[2]));

   visualiza_pl(stack);

   ptr = retira_pl(stack);
   printf("último valor: %d\n", *ptr);
   ptr = retira_pl(stack);
   printf("último valor: %d\n", *ptr);

   visualiza_pl(stack);

   int* qual_o_topo = topo_pl(stack);
   printf("topo da pilha: %d\n", *qual_o_topo);

   assert (comprimento_pl(stack) == 1);
   assert (destroi_pl (stack));
}

TESTE pilha_de_strings (void) {
   PilhaLigada stack = cria_pl();

   coloca_pl(stack, "wealth and taste");
   visualiza_pilha_string(stack);
   coloca_pl(stack, "sympathy");
   visualiza_pilha_string(stack);
   coloca_pl(stack, "Anastasia screams in vain");
   visualiza_pilha_string(stack);
   coloca_pl(stack, "He said so tshirts");
   visualiza_pilha_string(stack);

   assert (comprimento_pl(stack) == 4);
   assert (destroi_pl (stack));
}

size_t computa_tamanho_da_pilha (PilhaLigada s) {
   size_t T = sizeof (SIZE_MAX);
   Node atual = s->topo;

   do {
      // T += sizeof (nodulo_t);
      T += NODULO_SZ;
      atual = atual->seta;
   } while (atual != NULL);
   return T;
}

size_t unidades_por_megabytes (uint16_t nMB, size_t size)
   { return nMB * pow (2, 20) / size; }


TESTE verificando_vazamento_de_memoria (void) {
   // inteiros de 4 bytes(int) de 50MiB deles.
   size_t total_de_unidades = unidades_por_megabytes(50, sizeof (int));
   PG barra = cria_bp (Temporal, total_de_unidades, 40);
   PilhaLigada stack = cria_pl();
   int* X;

   // adiciona na array valores randômicos de 0 até 30.
   srand ((size_t)&cria_pl);
   puts ("carregando os 50 MiB ...");

   for (size_t i = 1; i <= total_de_unidades; i++) {
      X = calloc (1, sizeof(int));
      *X = rand() % (30 + 1);

      coloca_pl (stack, X);
      atualiza_e_visualiza_bp (&barra, i);
   }
   puts ("destruído, mas ainda na memória?");
   // uma liberação logo em seguida.
   destroi_interno_pl (stack, deleta_u16);
   puts ("destruição completa, a memória foi liberada?");
   sleep (10);
   puts ("programa finalizado.");
}

TESTE estruturas_tamanhos (void) {
   Node no = cria_nodulo ("nada");
   size_t void_ptr_bytes = sizeof (void*);
   size_t pl_bytes = sizeof (pilha_ligada_t);
   PilhaLigada stack = cria_pl();
   size_t instancia_bytes = sizeof *stack;

   printf(
      "Método sizeof estrutura(%lu) e sua instâncias(%lu)\n",
      pl_bytes, instancia_bytes
   );
   printf ("um ponteiro 'void*' ocupa %lu bytes.\n", void_ptr_bytes);
   // printf ("o 'nódulo' ocupa %lu bytes.\n", sizeof (nodulo_t));
   printf ("o 'nódulo' ocupa %lu bytes.\n", NODULO_SZ);
   printf ("instância de 'nódulo' ocupa %lu bytes.\n", sizeof no);
   printf ("uma 'pilha ligada' ocupa %lu bytes.\n", instancia_bytes);
   assert (coloca_pl (stack, "nada"));
   assert (coloca_pl (stack, "dado"));
   assert (coloca_pl (stack, "casa"));
   printf (
      "instância vázia de 'pilha ligada'... %lu bytes.\n"
      "... mesma instância, com alguns elementos ... %lu bytes\n",
      instancia_bytes, sizeof (stack)
   );
   assert (pl_bytes == instancia_bytes);
   printf (
      "instância total é %lu bytes.\n",
      computa_tamanho_da_pilha(stack)
   );
   destroi_pl (stack);
}

TESTE verificando_vazamento_de_memoria_i (void) {
   PilhaLigada stack = cria_pl();
   // size_t Q = unidades_por_megabytes (300, sizeof (nodulo_t));
   size_t Q = unidades_por_megabytes (300, NODULO_SZ);
   PT barra = cria_bpt (Q, 40);
   // char* mesmo_dado = calloc (50, sizeof (char));
   // strcpy (mesmo_dado,"uma string muito grande, bem grande mesmo.");
   // um bocado de zeros:
   int* mesmo_dado = calloc (Q, sizeof (int));
   puts ("array de 150MB:");
   for (size_t k = 1; k <= Q; k++)
      mesmo_dado[k - 1] = 8;

   // adiciona na array valores randômicos de 0 até 30.
   srand ((size_t)&cria_pl);

   for (size_t i = 1; i <= Q; i++) {
      // referênciando o mesmo dado milhões de vezes.
      coloca_pl (stack, mesmo_dado);
      visualiza_e_atualiza_bpt (&barra, i);
   }
   puts ("fazendo todos items na pilha 'dangling points' ...");
   sleep (15);
   free (mesmo_dado);
   puts ("quanto fica a memória?");
   sleep (10);
   puts ("removendo item por item...");
   destroi_pl (stack);
}

char* conststr_to_str(generico_t dt) {
   char* s = dt;
   char* saida = malloc((strlen(s) + 1) * sizeof(char));
   strcpy(saida, dt);
   return saida;
}

TESTE amostra_simples_de_todos_seus_metodos(void) {
   PilhaLigada stack = cria_pl();

   printf("Está vázia? %s\n", bool_to_str(vazia_pl(stack)));
   printf("Total de itens: %lu\n", comprimento_pl(stack));
   for (size_t k = 0; k < FRUTAS; k++) {
      char* e = (char*)frutas[k];
      printf("\tAdicionando agora '%s'...\n", e);
      assert (coloca_pl(stack, e));
   }
   printf("Total de itens: %lu\n", comprimento_pl(stack));
   printf("Está vázia? %s\n", bool_to_str(vazia_pl(stack)));
   char* item_no_topo = topo_pl(stack);
   printf("No topo: '%s'\n", item_no_topo);

   char* pilha_str = stack_to_str_pl(stack, conststr_to_str);
   printf("\nSua formatação string:\n\t%s\n", pilha_str);
   free(pilha_str);
   puts("Visualização dela:");
   imprime_pl(stack, conststr_to_str);

   puts("\nTestando método extender, e também métodos renomeados em inglês.");
   PilhaLigada stack_i = cria_pl();
   for (size_t k = 0; k < OBJETOS; k++) {
      char* e = (char*)objetos[k];
      printf("\tAdicionando agora '%s'...\n", e);
      assert (push_pl(stack_i, e));
   }
   printf("Total de itens: %lu\n", len_pl(stack));
   printf("Total de itens: %lu\n", len_pl(stack_i));
   print_pl(stack_i, conststr_to_str);
   extende_pl(stack, stack_i);
   assert (empty_pl(stack_i));
   delete_pl(stack_i);
   print_pl(stack, conststr_to_str);

   destroi_pl(stack);
}

TESTE processo_de_inversao_da_lista_ligada(void) {
   PilhaLigada stack = cria_pl();

   for (size_t k = 0; k < FRUTAS; k++) {
      char* e = (char*)frutas[k];
      assert (coloca_pl(stack, e));
   }

   print_pl(stack, conststr_to_str);

   inverte_pl(stack);
   print_pl(stack, conststr_to_str);

   printf("\nRemovendo na fila-ligada invertida...");
   do {
      generico_t a = pop_pl(stack);
      char* b = (char*)a;
      printf("\t\b\b\b- '%s'\n", b);

   } while(!empty_pl(stack));

   puts("\nTeste de inversão com um e dois itens apenas:");
   assert(coloca_pl(stack, "nada"));
   assert(coloca_pl(stack, "nenhuma"));
   print_pl(stack, conststr_to_str);
   inverte_pl(stack);
   print_pl(stack, conststr_to_str);
}

generico_t clona_constchar(generico_t a)
{
   if (a == NULL) {
      perror("Valor inválido");
      abort();
   }

   char* obj = (char*)a;
   int t = strlen(obj);
   int sz = (t + 1) * sizeof(char);
   char* clone = malloc(sz);

   memcpy(clone, obj, sz);
   return clone;
}

TESTE metodo_de_clonagem_da_pilha(void) {
   PilhaLigada stack = cria_pl();
   const int N = OBJETOS / 2;

   for (size_t k = 0; k < N; k++) {
      char* e = (char*)objetos[k];
      assert (coloca_pl(stack, e));
   }

   print_pl(stack, conststr_to_str);

   PilhaLigada S = clona_pl(stack, clona_constchar);
   print_pl(S, conststr_to_str);
   destroi_interno_pl(S, free_str);
}
