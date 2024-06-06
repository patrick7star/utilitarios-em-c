
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// qualquer referência, de qualquer tipo.
typedef void* generico_t;

typedef struct nodulo {
   // referência de qualquer tipo ao dado.
   generico_t dado;

   // referência ao próximo nódulo.
   struct nodulo* seta;

} nodulo_t;

nodulo_t* cria_nodulo(generico_t dado) {
   nodulo_t* instancia = malloc (sizeof(nodulo_t));
   
   if (instancia != NULL) {
      instancia->dado = dado;
      instancia->seta = NULL;
   }
   return instancia;
}

bool destroi_nodulo(nodulo_t* x) {
   if (x == NULL) return false;
   /* O dado, que é uma referência, não pode ser desalocado aqui, pois não
    * sabe-se como ele é formado; pode ser que seja até algo estático. Aqui
    * apenas desconectamos tal referência. */
   x->dado = NULL;
   x->seta = NULL;
   // liberando ...
   free(x);
   x = NULL;
   return true;
}

typedef struct {
   // quantificando quantos elementos ela realmente tem.
   size_t quantidade;

   // o ínicio da lista ligada.
   nodulo_t* topo;

} PilhaLigada;

// o tipo void* é uma referência genérica que recebe qualquer outro tipo.
typedef void* generico_t;

PilhaLigada* cria_pl (void) {
   PilhaLigada* instancia = malloc(sizeof(PilhaLigada));

   if (instancia != NULL) {
      instancia->quantidade = 0;
      instancia->topo = NULL;
   }
   return instancia;
}

bool coloca_pl(PilhaLigada* s, void* dado) {
   // apenas aceita valores válidos.
   if (s == NULL || dado == NULL) return false;

   // inserindo no começo da lista(que é o topo da lista).
   nodulo_t* novo_item = cria_nodulo(dado);
   novo_item->seta = s->topo;
   s->topo = novo_item;
   // contabilizando inserção ...
   s->quantidade += 1;

   // confirma inserção.
   return true;
}

size_t tamanho_pl(PilhaLigada* s) 
   { return s->quantidade; }

bool vazia_pl(PilhaLigada* s) 
   { return tamanho_pl(s) == 0; }

void visualiza_pl(PilhaLigada* s) {
   if (vazia_pl(s)) { puts("pilha-ligada: []"); return; }

   nodulo_t* cursor = s->topo;

   printf("pilha-ligada: [");
   while (cursor != NULL) {
      printf("%d, ", *(int*)cursor->dado);
      cursor = cursor->seta;
   }
   puts("\b\b]");
}

generico_t retira_pl(PilhaLigada* s) {
   // abandona toda operação, não é possível continuar.
   if (vazia_pl(s)) { return NULL; }

   nodulo_t* item_removido = s->topo;
   generico_t dado_extraido = item_removido->dado;
   // achando novo item no topo...
   s->topo = s->topo->seta;
   destroi_nodulo (item_removido);
   // descontabilizando o item removido.
   s->quantidade -= 1;
   
   return dado_extraido;
}

void* topo_pl(PilhaLigada* s) {
   // abandona toda operação, não é possível continuar.
   if (vazia_pl(s)) { return NULL; }

   // pega o dado no primeiro item da lista.
   return s->topo->dado;
}

void visualiza_pilha_string(PilhaLigada* s) {
   if (vazia_pl(s)) { puts("pilha-ligada: []"); return; }

   nodulo_t* cursor = s->topo;

   printf("pilha-ligada: [");
   while (cursor != NULL) {
      printf("'%s', ", (char*)cursor->dado);
      cursor = cursor->seta;
   }
   puts("\b\b]");
}

bool destroi_pl (PilhaLigada* s) {
   /* se a pilha passada for uma referência inválida(nula), confirmar não
    * desalocação de tal estrutura da memória. */
   if (s == NULL) return false;

   #ifdef _DESTROI_PL
   size_t contagem = 0;
   #endif 

   while (!vazia_pl (s)) {
      retira_pl (s);

      #ifdef _DESTROI_PL
      contagem++;
      #endif 
   }

   #ifdef _DESTROI_PL
   printf ("foram removidos %lu itens.\n", contagem);
   #endif

   free (s);
   // tudo ocorreu conforme, apenas confirma desalocação.
   return true;
}
#ifdef _DESTROI_PL
#include <assert.h>
#include <unistd.h>
#endif

bool destroi_data_too_pl (PilhaLigada* s) {
   /* Mesmo fim que o método de desalocação acima, porém aqui também libera
    * o dado passado junto, ou seja, provavelmente tal pilha contém um 
    * monte de dado na heap, sendo tal no máximo uma array de algum tipo,
    * digo de fácil desalocação. */

   /* se a pilha passada for uma referência inválida(nula), confirmar não
    * desalocação de tal estrutura da memória. */
   if (s == NULL) return false;

   #ifdef _DESTROI_PL
   size_t parada = tamanho_pl (s) / 2;
   size_t cortes = 0;
   #endif

   while (!vazia_pl (s)) {
      generico_t dado = retira_pl(s);
      free (dado);
      dado = NULL;

      #ifdef _DESTROI_PL
      assert (dado == NULL);
      size_t quantia = tamanho_pl (s);
      bool chegou = quantia < parada;
      if (chegou) {
         printf (
            "%03luº. memória cortada à metade!\n"
            "qtd. de itens=%8lu\n", 
            ++cortes, quantia
         );
         parada = quantia / 2;
         sleep (1);
      }
      #endif
   }
   free (s);
   // tudo ocorreu conforme, apenas confirma desalocação.
   return true;
}

#ifdef _UT_PILHA_LIGADA
#include <locale.h>
#include <assert.h>
#include <tgmath.h>
#include <limits.h>
#include <unistd.h>
#include "barra_de_progresso.h"

void pilha_com_i32s (void) {
   PilhaLigada* stack = cria_pl();
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

   assert (tamanho_pl(stack) == 1);
   assert (destroi_pl (stack));
}

void pilha_de_strings (void) {
   PilhaLigada* stack = cria_pl();

   coloca_pl(stack, "wealth and taste");
   visualiza_pilha_string(stack);
   coloca_pl(stack, "sympathy");
   visualiza_pilha_string(stack);
   coloca_pl(stack, "Anastasia screams in vain");
   visualiza_pilha_string(stack);
   coloca_pl(stack, "He said so tshirts");
   visualiza_pilha_string(stack);

   assert (tamanho_pl(stack) == 4);
   assert (destroi_pl (stack));
}

size_t computa_tamanho_da_pilha (PilhaLigada* s) {
   size_t T = sizeof (SIZE_MAX);
   nodulo_t* atual = s->topo;

   do {
      T += sizeof (nodulo_t); 
      atual = atual->seta;
   } while (atual != NULL);
   return T;
}

size_t unidades_por_megabytes (uint16_t nMB, size_t size) 
   { return nMB * pow (2, 20) / size; }

void verificando_vazamento_de_memoria (void) {
   // inteiros de 4 bytes(int) de 50MiB deles.
   size_t total_de_unidades = unidades_por_megabytes(50, sizeof (int));
   PT barra = novo_bpt (total_de_unidades, 40);
   PilhaLigada* stack = cria_pl();
   int* X;

   // adiciona na array valores randômicos de 0 até 30.
   srand ((size_t)&cria_pl);
   puts ("carregando os 50 MiB ...");

   for (size_t i = 1; i <= total_de_unidades; i++) {
      X = calloc (1, sizeof(int));
      *X = rand() % (30 + 1); 

      coloca_pl (stack, X);
      visualiza_e_atualiza_bpt (barra, i);
   }
   puts ("destruído, mas ainda na memória?");
   // uma liberação logo em seguida.
   destroi_data_too_pl (stack);
   puts ("destruição completa, a memória foi liberada?");
   sleep (10);
   puts ("programa finalizado.");
}

void estruturas_tamanhos (void) {
   nodulo_t* no = cria_nodulo ("nada");
   size_t void_ptr_bytes = sizeof (void*);
   size_t pl_bytes = sizeof (PilhaLigada);
   PilhaLigada* stack = cria_pl();
   size_t instancia_bytes = sizeof stack;

   printf ("um ponteiro 'void*' ocupa %lu bytes.\n", void_ptr_bytes);
   printf ("o 'nódulo' ocupa %lu bytes.\n", sizeof (nodulo_t));
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
   assert (instancia_bytes == sizeof (stack));
   printf ("instância total é %lu bytes.\n", computa_tamanho_da_pilha(stack));
   destroi_pl (stack);
}

#include <string.h>

void verificando_vazamento_de_memoria_i (void) {
   PilhaLigada* stack = cria_pl();
   size_t Q = unidades_por_megabytes (300, sizeof (nodulo_t));
   PT barra = novo_bpt (Q, 40);
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
      visualiza_e_atualiza_bpt (barra, i);
   }
   puts ("fazendo todos items na pilha 'dangling points' ...");
   sleep (15);
   free (mesmo_dado);
   puts ("quanto fica a memória?");
   sleep (10);
   puts ("removendo item por item...");
   destroi_pl (stack);
}

int main(int total, char* args[], char* vars[]) {
   // permitindo impressões de caractéres 'latin1'.
   setlocale(LC_CTYPE, "pt_BR.UTF-8");

   // pilha_com_i32s();
   // pilha_de_strings();
   // desativada pois consome bastante CPU e memória.
   verificando_vazamento_de_memoria();
   // estruturas_tamanhos();
   // novo_metodo_de_destruir();
   // verificando_vazamento_de_memoria_i();
   // fim do programa.
   return EXIT_SUCCESS;
}
#endif
