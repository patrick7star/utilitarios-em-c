/* Implementação da estrutura 'fila ligada circular' em C. Novamente esta
 * é a abordagem genérica de tal. Se pode usar qualquer referência, porém
 * alguns métodos que exigem a cópia impossíveis aqui. 
 */

#ifndef FILA_CIRCULAR_H
#define FILA_CIRCULAR_H
#include "definicoes.h"

 // todos apelidos da estrutura dada.
 typedef struct lista_circular fila_circular_t, FC, *FilaCircular;

 // operações de desalocação e alocação do novo tipo de dado:
 FilaCircular cria_fc (void); 
 void destroi_fc (FilaCircular); 
 void destroi_inner_fc (FilaCircular, Drop); 

 // operações que verificam ou, obtém dados dela:
 size_t comprimento_fc (FilaCircular);
 size_t tamanho_fc (FilaCircular, size_t);
 bool vazia_fc (FilaCircular);
 /* Primeiro item da 'fila', porque ainda é uma. Entretanto, por tal ser
  * circular, a 'calda' dela referência o 'primeiro' item. */
 generico_t primeiro_fc (FilaCircular);

 // operações que mudam o interno da estrutura:
 void insere_fc (FilaCircular, generico_t);
 generico_t retira_fc (FilaCircular); 
 /* Rotaciona a 'fila circular' no sentido anti-horário. */
 bool rotaciona_fc (FilaCircular); 

 // Visualização da fila:
 void imprime_fc(FilaCircular, ToString); 

#endif
