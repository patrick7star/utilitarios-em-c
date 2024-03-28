/*   Simples implementação genérica da deque-ligada, ou seja uma lista 
 * duplamente encadeada.
 *
 *   Se ficar muito boa se juntará as estruturas de dados tipo(iii) no
 * projeto utilitários, pois é uma array dinâmica que serve incrivelmente
 * para qualquer projeto que precisa armazenar um grande número de objetos,
 * seja eles qualquer que tipo.
 * 
 *   O que importa nestes tipos de estruturas são seu comportamento, várias
 * outras coisas que necessitam de visualização e cópia, são negligênciados
 * nesta versão de estrutura de dados.
 * 
 *   Assim como os tipos de estrutura(II), toda implementação só ficará num
 * único arquivo(este). Este ainda é melhor que o dois, pois como apenas o
 * comportamento é necesssitado, a implementação das estruturas comuns, são
 * bem menores no tipo(III).
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// tipo vázio que pode assumir qualquer endereço.
typedef void* generico_t;

typedef struct item_nodulo {
   // dado genérico que, pode ser qualquer coisa.
   generico_t dado;

   // referência aos próximos itens de sua estrutura.
   struct item_nodulo* esquerda;
   struct item_nodulo* direita;

} nodulo_t;

// indicador de objeto, qualquer seja, está inválido.
#define INVALIDO NULL

static nodulo_t* cria_nodulo(generico_t dt, nodulo_t* l, nodulo_t* r)
{
   size_t tamanho = sizeof(nodulo_t);
   nodulo_t* novo_nodulo = malloc (tamanho);

   if (novo_nodulo != INVALIDO) {
      // captura dado transferido.
      novo_nodulo->dado = dt;
      // novos nódulos conecta com respectivas posições passadas.
      novo_nodulo->esquerda = l;
      novo_nodulo->direita = r;
   }
   return novo_nodulo;
}

static void destroi_nodulo(nodulo_t* obj) {
   if (obj != INVALIDO) {
      /* libera dado nele contido. Cuidado! Se houver outra referência 
       * externa ao tal dado interno do nódulo será perdida bem aqui. */
      free (obj->dado);
      // se livra de referência.
      obj->direita = NULL;
      obj->esquerda = NULL;
      // finalmente libera o nódulo em sí.
      free (obj);
   }
}

/* simples estrutura de lista duplamente ligada. A implementação deste 
 * difere um pouco das principais, por usar sentinelas. */
typedef struct lista_duplamente_encadeada {
   // sentinelas começo e fim.
   nodulo_t* inicio;
   nodulo_t* final;

   // quantidade de itens nela no momento.
   size_t quantidade;
} dequeligada_t;


dequeligada_t* cria_dl(void) {
   // criando primeira instancia...
   dequeligada_t* instancia = malloc(sizeof(dequeligada_t));

   // se foi instanciado corretamente, então completar os campos.
   if (instancia != NULL) {
      /* Criando cada sentinela da lista(fim e começo). Sem nenhum dado, 
       * ou inicial referência nas suas pontas, e mesmo quando houver só 
       * será em uma, a outra permanecerá sempre inutilizada. */ 
      instancia->inicio = cria_nodulo(NULL, NULL, NULL);
      instancia->final = cria_nodulo(NULL, NULL, NULL);
      /* Elas também serão conectadas uma a outra. */
      instancia->inicio->direita = instancia->final;
      instancia->final->esquerda = instancia->inicio;
      // começa sem nenhum item(zero).
      instancia->quantidade = 0;
   }
   return instancia;
}

static void insere_entre(generico_t e, nodulo_t* left, nodulo_t* right) 
{
   // criação do novo nódulo já conecta as pontas.
   nodulo_t* novo = cria_nodulo(e, left, right);

   if (novo != INVALIDO) {
      assert (left != NULL);
      assert (right != NULL);
      // conectando nódulos da ponta ao 'novo meio'.
      left->direita = novo;
      right->esquerda = novo;
   }
}

static generico_t remove_entre(nodulo_t* e) {
   /* obtém referência do nódulo passado, então inválida campo do nódulo,
    * já que está sendo removido(logo o nódulo restante será destruído), e 
    * tal referência será retornada. 
    */
   generico_t dado_removido = e->dado;
   e->dado = NULL;
   // obtendo seus vizinhos de ambos lados...
   nodulo_t* node_right = e->direita;
   nodulo_t* node_left = e->esquerda;
   
   // desconectando cada um, e conectando um ao outro...
   node_right->esquerda = node_left;
   node_left->direita = node_right;

   destroi_nodulo (e);
   // retorna o dado que ele embrulha...
   return dado_removido;
}

#include <stdbool.h>

bool insere_comeco_dl(dequeligada_t* l, generico_t e) {
   // único caso que a operação pode falhar, se a lista não existir.
   if (l == INVALIDO) return false;

   insere_entre (e, l->inicio, l->inicio->direita);
   // contabilizando o novo dado.
   l->quantidade += 1;
   return true;
}

bool insere_final_dl(dequeligada_t* l, generico_t e) {
   // único caso que a operação pode falhar, se a lista não existir.
   if (l == INVALIDO) return false;

   insere_entre (e, l->final->esquerda, l->final);
   // contabilizando o novo dado.
   l->quantidade += 1;
   return true;
}

bool vazia_dl(dequeligada_t* l)
   { return l->quantidade == 0; }

generico_t comeco(dequeligada_t* l) { 
   // não pode obter um item de uma lista vázia.
   if (vazia_dl (l)) return NULL;

   nodulo_t* item = l->inicio->direita; 
   return item->dado;
}

generico_t final(dequeligada_t* l) {
   // não pode obter um item de uma lista vázia.
   if (vazia_dl (l)) return NULL;

   nodulo_t* item = l->final->esquerda; 
   return item->dado;
}

size_t tamanho_dl(dequeligada_t* l) 
   { return l->quantidade; }

generico_t remove_comeco_dl(dequeligada_t* l) {
   // só remove lista com alguma coisa...
   if (vazia_dl(l)) return INVALIDO;
   
   // descontabilizando item removido.
   l->quantidade -= 1;
   // o primeire item depois do sentinela.
   return remove_entre (l->inicio->direita);
}

generico_t remove_final_dl(dequeligada_t* l) {
   // só remove lista com alguma coisa...
   if (vazia_dl(l)) return INVALIDO;
   
   // descontabilizando item removido.
   l->quantidade -= 1;
   // o primeire item depois do sentinela.
   return remove_entre (l->final->esquerda);
}

bool destroi_dl(dequeligada_t* l) {
   /* só destroi se houver uma lista. */
   if (l != INVALIDO) {
      // destrói todos seus nódulos internos...
      while (!vazia_dl (l)) {
         generico_t retorno = remove_comeco_dl (l);
         if (retorno != INVALIDO) free (retorno);
         puts ("dado interno destruído com sucesso.");
         // remove_comeco_dl (l);
         // remove sentinelas...
         destroi_nodulo (l->inicio);
         destroi_nodulo (l->final);
      }
      // destrói lista e confirma desalocação.
      free (l);
      return true;
   }
   return false;
}

#ifdef _UT_DEQUE_LIGADA
// bibliotecas necessárias:
#include <stdio.h>
#include <assert.h>

#include "dados_testes.h"
#include "aleatorio.h"
#include "teste.h"

void criacao_e_destruicao_trivial(void) {
   dequeligada_t* L = cria_dl();
   puts ("perfeitamente criado.");
   destroi_dl (L);
   puts ("perfeitamente destruído.");
}

void aplica_uma_insercao(void) {
   int valor = 55, valor_ii = 44;
   dequeligada_t* L = cria_dl();
   assert (insere_comeco_dl (L, (int*)&valor));
   assert (insere_final_dl (L, &valor_ii));
   printf ("nº. de itens: %lu\n", tamanho_dl(L));
}

void mostra_fim_e_comeco(dequeligada_t* L) {
   printf (
      "começo: '%s' e fim: '%s'\n", 
      (char*)comeco (L), (char*)final (L)
   );
}

void items_das_pontas(void) {
   char** entradas = girls_names;
   dequeligada_t* L = cria_dl();
   assert (insere_comeco_dl (L, entradas[5]));
   mostra_fim_e_comeco (L);
   assert (insere_final_dl (L, entradas[1]));
   mostra_fim_e_comeco (L);
   printf ("nº. de itens: %lu\n", tamanho_dl(L));
}

void items_das_pontas_ii(void) {
   dequeligada_t* L = cria_dl();
   char** entradas = nomes_de_meninas;

   assert (insere_comeco_dl (L, entradas[5]));
   mostra_fim_e_comeco (L);
   assert (insere_final_dl (L, entradas[1]));
   mostra_fim_e_comeco (L);
   assert (insere_final_dl (L, entradas[2]));
   mostra_fim_e_comeco (L);
   printf ("nº. de itens: %lu\n", tamanho_dl(L));

   printf ("\nremovido: %s\n", (char*)remove_comeco_dl (L));
   mostra_fim_e_comeco (L);
   printf ("nº. de itens: %lu\n", tamanho_dl(L));
   printf ("\nremovido: %s\n", (char*)remove_comeco_dl (L));
   mostra_fim_e_comeco (L);
   printf ("nº. de itens: %lu\n", tamanho_dl(L));
}

void simples_insercao_e_remocao(void) {
   char** entradas = frutas;
   dequeligada_t* L = cria_dl();

   for (size_t i = 1; i <= 8; i++) {
      if (logico())
         insere_comeco_dl (L, entradas[i - 1]);
      else
         insere_final_dl (L, entradas[i - 1]);
      printf ("total de itens: %lu\n", tamanho_dl (L));
   }

   char* remocao;
   while (!vazia_dl (L)) {
      if (logico())
         remocao = remove_final_dl (L);
      else
         remocao = remove_comeco_dl (L);
      printf ("removido: '%s'\n", remocao);
   }
}

void visualiza_uint8bits_deque (dequeligada_t* D) {
   size_t t = tamanho_dl (D);
   printf ("lista: [");

   while (t-- > 0) {
      generico_t remocao = remove_comeco_dl (D);
      printf ("%d, ", *((uint8_t*)remocao));
      insere_final_dl (D, remocao);
   }
   puts ("\b\b]");
}

void visualiza_array_uint8bits (uint8_t* array, uint8_t t) {
   printf ("array: [");

   for (uint8_t i = 0; i < t; i++) {
      uint8_t valor = array[i];
      printf ("%d, ", valor);
   }
   puts ("\b\b]");
}

void ordem_de_insercao_respeitada(void) {
   uint8_t* entradas = valores_padronizados;
   dequeligada_t* D = cria_dl();

   visualiza_array_uint8bits (entradas, 10);
   for (size_t i = 1; i <= 10; i++) {
      uint8_t* valor = &entradas[i - 1];
      if (i % 2 != 0)
         insere_comeco_dl (D, valor);
      else
         insere_final_dl (D, valor);
   }
   visualiza_uint8bits_deque (D);
}

int main(int total, char* argumentos[]) {
   executa_testes (
      6, criacao_e_destruicao_trivial, true,
      simples_insercao_e_remocao, true,
      aplica_uma_insercao, true,
      items_das_pontas, true,
      items_das_pontas_ii, true,
      ordem_de_insercao_respeitada, true
   );
   return EXIT_SUCCESS;
}
#endif

