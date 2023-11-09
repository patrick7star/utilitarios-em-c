
// trecho de iteração da estrutura está sendo passado para cá.

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
   IterHT iterador = malloc(ITER_HT_SIZE);

   if (m == NULL) {
      puts("'tabela de dispersão' inválida!");
      abort();
   }

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
      free(iterador);
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
