/* Implementação da estrutura 'fila ligada circular' em C. Novamente esta
 * é a abordagem genérica de tal. Se pode usar qualquer referência, porém
 * alguns métodos que exigem a cópia impossíveis aqui. 
 */

#include <stdlib.h>
#include <stdbool.h>

#ifndef FILA_CIRCULAR_H
#define FILA_CIRCULAR_H
 // Diz exatamente qualquer referência pode ser armazenada.
 typedef void* generico_t;
 // todos apelidos da estrutura dada.
 typedef struct lista_circular filacircular_t, FC, *FilaCircular;

 // operações de desalocação e alocação do novo tipo de dado:
 FilaCircular cria_fc(void); 

 // operações que verificam ou, obtém dados dela:
 extern size_t tamanho_fc (FilaCircular);
 bool vazia_fc (FilaCircular);
 /* Primeiro item da 'fila', porque ainda é uma. Entretanto, por tal ser
  * circular, a 'calda' dela referência o 'primeiro' item. */
 generico_t primeiro_fc (FilaCircular);

 // operações que mudam o interno da estrutura:
 void insere_fc (FilaCircular, generico_t);
 generico_t retira_fc (FilaCircular); 
 /* Rotaciona a 'fila circular' no sentido anti-horário. */
 void rotaciona_fc (FilaCircular); 

#endif
