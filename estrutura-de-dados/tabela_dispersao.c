

/* Porquê eu copiar uma versão nova do deque-ligada para o próprio 
 * "caixote" da tabela de dispersão é, pois o arquivo de 'dado' é 
 * diferenciado do original usado pela estrutura. */
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
#include <stdio.h>

HashTable cria_ht() {
   HashTable tabela = malloc(HT_SIZE);

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

/* cálculo hash baseado no comprimento da string dada. Está havendo 
 * uma terceirização para esta função, porque dependendo do dado 
 * tal cálculo muda, então em caso de mudança, ao invés de ficar
 * varrendo no código para mudar, apenas muda aqui, e já funciona
 * para todas funções que utilizam este algoritimo. */
static uint32_t funcao_hash(CHAVE ch)
   { return strlen(ch); }

bool insere_ht(HASH_TABLE d, CHAVE ch, VALOR v) {
   uint32_t indice = funcao_hash(ch);

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

   // índice que ela está na array.
   uint32_t indice = funcao_hash(ch);
   
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

   // índice o qual ela está na 'array de deque'.
   uint32_t indice = funcao_hash(ch);
   
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

/* ao invés de vasculhar, achar, e atualizar, usando trechos do
 * algoritimo acima, apenas deleta, e insere nova entrada. Claro,
 * isso é basicamente o dobro de operações, porém ainda é uma 
 * operação linear. Futuramente, talvez seja necessário fazer como
 * os algoritimos acima. */
bool atualiza_ht(HASH_TABLE d, CHAVE ch, VALOR vl) {
   deleta_ht(d, ch);
   return insere_ht(d, ch, vl);
}

/* nunca dá acesso a instância principal, pois alterações podem
 * causar mudanças inesperadas no programa. */
uint8_t total_de_instancias_ht() 
   { return instancias_ht; }

// função de iteração da tabela, criação, desalocação e seus métodos.
#include "tabeladispersao/iteracao.c"

void visualizacao_debug_ht(HashTable m) {
   IterHT iterador_a_consumir = iter_ht(m);
   Dado item = next_ht(iterador_a_consumir);

   if (tamanho_ht(m) <= 5)
      printf("HashTable: {\n");
   else
      printf("HashTable: {");
   while (item != NULL) { 
      // visualizando-o...
      if (tamanho_ht(m) <= 5)
         printf("\t%s,\n", dado_para_string(item)); 
      else
         printf("%s, ", dado_para_string(item)); 
      // próximo item do iterador.
      item = next_ht(iterador_a_consumir);
   }
   printf("\b\b}\n");

   // destruindo iterador todo consumido.
   destroi_iter_ht(iterador_a_consumir);
}

char* ht_to_str(HashTable m) {
   /* realizando uma iteração antes, descobrindo o comprimento da
    * formatação string de cada 'dado', então acumulando o comprimento
    * de cada dado específico. O que pode ser computador inicialmente,
    * são os delitadores(dois), e espaços e separadores(dois somado) 
    * multiplicados pela quantia de itens, dando mais dois espaços
    * reservas para evitar qualquer overflow.
    */
   uint64_t tamanho_str = 2 * tamanho_ht(m) + (2 + 3);
   IterHT I = iter_ht(m);

   for (Dado e = next_ht(I); e != NULL; e = next_ht(I)) {
      char* dado_str = dado_para_string(e);
      tamanho_str += strlen(dado_str);
   }
   destroi_iter_ht(I);

   char*  resultado_fmt = calloc(tamanho_str, sizeof(char));
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
 * e adiciona-os na nova 'tabela' a ser retornada.*/
HashTable clona_ht(HashTable m) {
   // nova 'tabela de dispersão' zerada.
   HashTable copia = cria_ht();
   puts("tabela clone criada.");
   destroi_array_dl(copia->locais, copia->capacidade);
   // visualizacao_array_dl(m->locais, m->capacidade);
   copia->locais = clona_array_dl(m->locais, m->capacidade);
   /* copiando também numero de listas da array, e o tanto de
    * elementos, para consistência da clonagem. */
   copia->capacidade = m->capacidade;
   copia->total = m->total;
   return copia;
}
 
