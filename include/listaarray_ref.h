
#ifndef ARRAYLISTA_H
#define ARRAYLISTA_H
#include <stdbool.h>
#include <stdarg.h>
#include "definicoes.h"

 // todos os apelidos desta estrutura fornecidos:
 typedef struct array_lista *ArrayLista, *Vetor, vetor_t;
 
 // criação e destruição do tipo de dado.
 ArrayLista cria_al();
 ArrayLista cria_com_capacidade_al (size_t); 
 ArrayLista cria_de_al (int, ...);
 bool destroi_al (ArrayLista); 
 void destroi_todas_al (int, ...); 

 /* Operações que obtém dados, e outras que mudam a estrutura interna da
  * lista. */
 bool insere_al (ArrayLista, generico_t); 
 generico_t remove_al (ArrayLista); 
 generico_t remove_indice_al (ArrayLista, size_t);
 generico_t indexa_al (ArrayLista, size_t); 

 bool vazia_al (ArrayLista);
 size_t tamanho_al (ArrayLista);
 /* Retorna o tamanho espaço vácuo, ou seja, a capacidade de itens que 
  * podem ser adicionados sem que a amortização seja acionada. */
 size_t vacuo_al (ArrayLista);

 /* Operação que converte pequenas estruturas em strings. */
 char* to_string_al(ArrayLista L, ToString fn);

/* === === === === === === === === === === === === === === === === === ==
 *                     Iterador da pilha-ligada
 *                        e seus métodos
 * === === === === === === === === === === === === === === === === === ==*/
 typedef struct iterador_da_lista_ligada_al 
   IterAL, *IteradorRefAL;
 /* Saída que o iterador produz. É apenas um único campo na estrutura, 
  * nomeado 'item', que entrega a referência que a estrutura retém. */
 typedef struct saida_da_iteracao_da_al { generico_t item; } 
   IterOutputAL;

 // Métodos de criação, mudança e atual estado:
 IterAL cria_iter_al (ArrayLista); 
 /* Faz a clonagem do iterador dado, inclusive seu estado(consumido ou 
  * não), no mesmo estágio do que foi usado.*/
 IterAL clona_iter_al (IteradorRefAL);
 // Total de itens ainda a consumir.
 size_t contagem_iter_al (IteradorRefAL);
 IterOutputAL next_al (IteradorRefAL); 
 // Verifica se todos itens da estrutura foram consumido via iterador.
 bool consumido_iter_al (IteradorRefAL);

#endif 
