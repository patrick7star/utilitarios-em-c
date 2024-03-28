
#include <stdint.h>
#include <stdbool.h>
#include "dado.h"

#ifndef DEQUE_LIGADA_H
#define DEQUE_LIGADA_H
   struct deque_ligada {
      // contador de itens.
      uint64_t qtd;

      // são ponteiros para cada ponta da abstração.
      struct node* comeco;
      struct node* traseira;
   };

   // tamanho em bytes da estrutura.
   #define DL_SIZE sizeof(struct deque_ligada)

   typedef struct deque_ligada* DequeLigada;
   // apelido curto de tal estrutura.
   typedef struct deque_ligada* DL ;

   // alocação e liberação da 'DequeLigada'.
   extern DequeLigada cria_dl(); 
   extern void destroi_dl(DequeLigada l); 
   extern DequeLigada clona_dl(DL l);

   // alocação de liberação de uma array de 'Deques-Ligadas'.
   typedef DequeLigada* ARRAY_DL;
   extern ARRAY_DL cria_array_dl(uint32_t n); 
   extern void destroi_array_dl(ARRAY_DL array, uint32_t n); 
   extern ARRAY_DL clona_array_dl(ARRAY_DL a, uint64_t n); 

   /* Visualizando e obtendo informações sobre
    * 'lista'(Deque), como seu estado: o total 
    * de itens líquidos nela(inserções - remoções); 
    * se tem algum; referências de ambas pontas; e
    * também uma visualização, se possível, dela. */

   // verifica se está vázia e se tem o respectivo dado.
   extern bool vazia_dl(DequeLigada l);
   extern bool contem_dl(DequeLigada l, Dado e);

   // espia as pontas da deque-ligada,e imprime ela.
   extern Dado frente_dl(DequeLigada l); 
   extern Dado traseira_dl(DequeLigada l); 

   // total de elementos na 'Deque'.
   extern uint64_t tamanho_dl(DequeLigada l);

   /* Operações básicas que alteram a 'Deque'. 
    * Inserções e remoções em ambas pontas. */
   extern bool insere_comeco_dl(DequeLigada l, Dado d); 
   extern bool insere_traseira_dl(DequeLigada l, Dado d); 
   extern Dado remove_comeco_dl(DequeLigada l); 
   extern Dado remove_traseira_dl(DequeLigada l); 
   extern bool remove_dl(DequeLigada l, Dado e);

   // visualização da 'deque ligada' e uma array deste tipo.
   extern void imprime_dl(DequeLigada l); 
   extern char* dl_to_str(DequeLigada l); 
   extern void visualizacao_array_dl(ARRAY_DL array, uint64_t n); 

#endif // DEQUE_LIGADA_H

#ifndef ITER_DL_H
#define ITER_DL_H
   /* Aqui cuida apenas do iterador: criação, destruição, "métodos"
    * dele, e suas operações(operações comuns de operadores).
    */
   typedef struct iterador_deque_ligada* IterDequeLigada;
   typedef IterDequeLigada IterDL;

   // construtor e destrutor do tipo de dado.
   extern IterDequeLigada cria_iter_dl(DequeLigada d); 
   extern void destroi_iter_dl(IterDL i); 

   // função mais importante para iteração 'next'.
   extern Dado next_dl(IterDL i); 

   // total de elementos restantes a iterar.
   extern uint64_t iter_tamanho_dl(IterDL i);

   // outros nomes possíveis para alguns métodos:
   extern IterDL iter_dl(DL d)
      { return cria_iter_dl(d); }

#endif // ITER_DL_H
