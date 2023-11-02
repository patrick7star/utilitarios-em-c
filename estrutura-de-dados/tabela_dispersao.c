

#include "tabeladispersao/dado.h"
#include "tabeladispersao/deque_ligada.h"
#include <inttypes.h>

struct tabela_de_dispersao {
   // "vagões" inicialmente alocados.
   uint32_t capacidade;

   // os "vagões" em si, são na verdade uma array de deques-ligadas.
   ARRAY_DL locais;

   // o total de items inseridos da tabela.
   uint64_t total;
};

// nomes mais compactos para codificação:
typedef struct tabela_de_dispersao* HASH_TABLE;
typedef HASH_TABLE HashTable;
typedef HASH_TABLE HT;
// tamano em bytes da estrutura de dados.
#define HT_SIZE sizeof(struct tabela_de_dispersao)

// contabilização do números de instâncias criadas.
static uint8_t instancias_ht = 0;

#include <stdlib.h>

HASH_TABLE cria_ht() {
   HASH_TABLE tabela = (HASH_TABLE)malloc(HT_SIZE);

   // se tiver sido alocado com sucesso.
   if (tabela != NULL) {
      tabela->locais = cria_array_dl(50);
      tabela->capacidade = 50;
      tabela->total = 0;
      // também contabiliza a instância.
      instancias_ht++;
   }

   // retornando a instância, seja ela válida ou não.
   return tabela;
}

void destroi_ht(HASH_TABLE d) {
   destroi_array_dl(d->locais, d->capacidade);
   free(d);
   // retirando esta 'tabela' da contagem.
   instancias_ht--;
}

#include <stdbool.h>
#include <string.h>

bool vazia_ht(HASH_TABLE d) 
   { return d->total == 0; }

uint64_t tamanho_ht(HASH_TABLE d)
   { return d->total; }

bool insere_ht(HASH_TABLE d, CHAVE ch, VALOR v) {
   /* cálculo hash baseado no comprimento da string dada. */
   uint32_t indice = strlen(ch);

   // para aqui, tal 'hash' não "bate" com a tabela.
   if (indice > d->capacidade)
      return false;

   Dado entrada = cria_dado(ch, v);
   // verifica se já não existe uma chave destá adicionada.
   if (contem_dl(d->locais[indice], entrada)) 
      return false;

   // contabilizando inserção.
   d->total++;

   /* se passou, então simplesmente insera a nova entrada na lista,
    * usando da interface comum entre a operação interna da lista e,
    * esta agora para confirmar a operação. 
    */
   return insere_comeco_dl(d->locais[indice], entrada);
}

bool deleta_ht(HASH_TABLE d, CHAVE ch) {
   // não se pode remover do nada.
   if (vazia_ht(d))
      return false;

   /* cálculo hash baseado no comprimento da string dada. */
   uint32_t indice = strlen(ch);
   
   if (vazia_dl(d->locais[indice]))
      /* se a lista dada pelo hash aponta para uma lista vázia, isso
       * indica que tal entra não existe na tabela. */
      return false;
   else  {
      Dado aux = cria_dado(ch, 3);
      /* caso dê uma lista válida, usa a função de remoção por comparação
       * da própria lista, para verifica se tem e remover. Neste caso,
       * usa o própria retorno do "método" da lista para dizer se a 
       * operação foi bem sucedida, ou seja, se foi removido algo ou não.
       */
      bool remocao_realizada = remove_dl(d->locais[indice], aux);
      if (remocao_realizada)
         d->total--;
      return remocao_realizada;
   }
}

bool contem_ht(HASH_TABLE d, CHAVE ch) {
   // não se pode remover do nada.
   if (vazia_ht(d))
      return false;

   /* cálculo hash baseado no comprimento da string dada. */
   uint32_t indice = strlen(ch);
   
   if (vazia_dl(d->locais[indice]))
      /* se a lista dada pelo hash aponta para uma lista vázia, isso
       * indica que tal entra não existe na tabela. */
      return false;
   else  {
      Dado aux = cria_dado(ch, 3);
      /* caso dê uma lista válida, usa a função de remoção por comparação
       * da própria lista, para verifica se tem e remover. Neste caso,
       * usa o própria retorno do "método" da lista para dizer se a 
       * operação foi bem sucedida, ou seja, se foi removido algo ou não.
       */
      return contem_dl(d->locais[indice], aux);
   }
}

bool atualiza_ht(HASH_TABLE d, CHAVE ch, VALOR vl) {
   deleta_ht(d, ch);
   return insere_ht(d, ch, vl);
}

/* nunca dá acesso a instância principal, pois alterações podem
 * causar mudanças inesperadas no programa. */
uint8_t total_de_instancias_ht() 
   { return instancias_ht; }

/* iterador para a estrutura de dados. Essencial, pois servirá de 
 * base para impressão de tal estrutura, além de outros que precisam
 * verificar todos dados internos.
 */
struct iteracao_tabela_de_dispersao {
   // iterador da atual lista com itens.
   IterDL atual;

   // referência da array de deques.
   ARRAY_DL listas;
   // qtd. de deques na array.
   uint32_t* tamanho;
   // índice na array de listas.
   uint32_t cursor;

   // contagem de itens válidos iterados.
   uint64_t contagem;
   // referência ao total inicial de itens na tabela.
   uint64_t total_de_itens;
   /* referência ao atual tamanho de itens na tabela, se houver alguma
    * variação, este 'iterador' torna-se imediatamente inválido. */
   uint64_t* total_inicial;
};

typedef struct iteracao_tabela_de_dispersao* IterHT;

#define ITER_HT_SIZE sizeof(struct iteracao_tabela_de_dispersao)

#include <stdio.h>
#include <assert.h>

IterHT iter_ht(HASH_TABLE m) {
   if (m == NULL) {
      puts("'tabela de dispersão' inválida!");
      abort();
   }

   IterHT iterador = (IterHT)malloc(ITER_HT_SIZE);

   if (iterador != NULL) {
      // assumindo nenhum problema inicialmente.
      iterador->contagem = 0;
      iterador->listas = m->locais;
      iterador->cursor = 0;
      iterador->tamanho = &m->capacidade;
      iterador->total_de_itens = m->total;
      iterador->total_inicial = &m->total;
      iterador->atual = iter_dl(iterador->listas[0]);
   } else {
      perror("houve problema na alocação para 'IterHT'!");
      abort();
   }

   return iterador;
}

// mexe no cursor do iterador, passando listas que não possuem itens.
bool pula_ate_lista_nao_vazia(IterHT i) {
   // diz que não é possível encontrar mais listas vázias.
   if (i->cursor >= *(i->tamanho))
      return false;

   while (vazia_dl(i->listas[i->cursor])) 
      { i->cursor++; }
   // diz que uma lista não vázia foi achada.
   return true;
}

Dado next_ht(IterHT i) {
   if (i == NULL) {
      puts("iterador 'IterHT' inválido!");
      abort();
   }  else if (i->total_de_itens != *(i->total_inicial)) {
      puts("iterador ficou inválido, pois números de itens na tabela foi modificado!");
      abort();
   } else if ( i->contagem == i->total_de_itens ) {
      // puts("todos itens válidos consumidos.");
      // todos itens válidos consumidos.
      return NULL;
   }

   Dado item = next_dl(i->atual);
   // cria iteração da pŕoxima lista na array de DL.
   if (item == NULL) {
      pula_ate_lista_nao_vazia(i);
      if (pula_ate_lista_nao_vazia(i)) {
         // destroi iterador vázio da última listas.
         destroi_iter_dl(i->atual);
         // se achou alguma, então cria novo iterador.
         i->atual = iter_dl(i->listas[i->cursor]);
         i->cursor++;
         // então faz uma nova iteração.
         item = next_dl(i->atual);
      } else 
         // caso contrário, apenas retorna inválido imediatamente.
         return NULL;
   }

   // contabilizando um item válido consumido, e rotornando-o.
   i->contagem++;
   return item;
}

void destroi_iter_ht(IterHT iterador) {
   // se desfaz de tais referências.
   iterador->listas = NULL;
   iterador->tamanho = NULL;
   iterador->total_inicial = NULL;
   free(iterador);
}

void visualizacao_debug_ht(HashTable m) {
   IterHT iterador_a_consumir = iter_ht(m);
   Dado item = next_ht(iterador_a_consumir);

   printf("HashTable: {");
   while (item != NULL) { 
      // visualizando-o...
      printf("%s, ", dado_para_string(item)); 
      // próximo item do iterador.
      item = next_ht(iterador_a_consumir);
   }
   printf("\b\b}\n");

   // destruindo iterador todo consumido.
   destroi_iter_ht(iterador_a_consumir);
}

char* ht_to_str(HashTable m) {
   /* assumindo uma chave com 20 caractéres, e um valor, com 5 digitos,
    * contando também o sinal,... em média então um valor razoável 
    * seria 29 caractéres por entrada -- claro, contabilizando o 
    * separador, mais espaços, vírgula, sem falar as aspas da chave 
    * string; no fim também contabiliza-se os "parentêses" que envolvem 
    * toda  formatação. */
   uint64_t tamanho_str = (20 + 5 + 1 + 2 + 2 + 1) * tamanho_ht(m) + 2;
   char*  resultado_fmt = (char*)calloc(tamanho_str, sizeof(char));
   IterHT iterador = iter_ht(m);
   Dado item = next_ht(iterador);
   bool foi_iterador = false;

   resultado_fmt[0] = '{';
   while (item != NULL) { 
      char* item_str = dado_para_string(item);
      // printf("item ==> %s\n", item_str);
      // só concatena se for válido.
      // concatenando-o...
      strcat(resultado_fmt, item_str);
      strcat(resultado_fmt, ", ");
      // próximo item do iterador.
      item = next_ht(iterador);
      // diz que houve no mínimo um loop.
      foi_iterador = true;
   }

   if (foi_iterador)
      // "come" espaço e vírgula extra.
      strcat(resultado_fmt, "\b\b}");
   else
      strcat(resultado_fmt, "}");

   // destruindo iterador todo consumido e retornando resultado...
   destroi_iter_ht(iterador);
   return resultado_fmt;
}

/* cria uma nova tabela, iterar os itens da 'tabela' passada, clona-os
 * e adiciona-os na nova 'tabela' a ser retornada.
 */
HashTable clona_ht(HashTable m) {
   // nova 'tabela de dispersão' zerada.
   HashTable M = cria_ht();
   // apagando a 'array de deque' vázia, então anexando a um clone.
   destroi_array_dl(M->locais, M->capacidade);
   M->locais = clona_array_dl(m->locais, m->capacidade);
   M->capacidade = m->capacidade;
   M->total = m->total;

   // verifica se todos itens foram passados.
   assert(tamanho_ht(m) == tamanho_ht(M));
   return M;
}
