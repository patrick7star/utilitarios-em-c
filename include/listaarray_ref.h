
#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>

#ifndef ARRAYLISTA_H
#define ARRAYLISTA_H

 /* A referência genérica com um nome mais legível do que se trata. */
 typedef void* generico_t;
 // todos os apelidos desta estrutura fornecidos:
 typedef struct array_lista *ArrayLista, *Vetor, vetor_t;
 typedef char* (*ToString)(generico_t);
 
 // criação e destruição do tipo de dado.
 ArrayLista cria_al();
 ArrayLista cria_com_capacidade_al (size_t); 
 ArrayLista cria_de_al (uint8_t qtd, ...);
 bool destroi_al (ArrayLista); 
 void destroi_todas_al (uint8_t qtd, ...); 

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

#endif 
