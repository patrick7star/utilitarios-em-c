
/* A estrutura de dados fila, porém com mecanismo interno usando uma 
 * deque ligada. As operações de inserção, remoção e "espiação" ainda
 * são em tempo constante, só indexação é linear, porém tal operação
 * é irrelevante numa fila.
 */

#include <stdbool.h>
#include <stdint.h>
#include "dado.h"

#ifndef FILA_LIGADA_H
#define FILA_LIGADA_H
   typedef struct fila_ligada* FilaLigada;
   typedef FilaLigada FL;

   // criação e liberação da 'fila ligada', novamente renomeando...
   extern FilaLigada cria_fl();
   extern void destroi_fl(FL f);

   // verifica se está vázia, e a quantidade de itens.
   extern bool vazia_fl(FilaLigada f);
   extern uint64_t quantidade_fl(FL f);

   /* Operações sobre a lista, estas que aumentam
    * ou diminuem suas quantias. 
    */

   // Operação de inserção e remoção da 'Pilha Ligada'.
   extern bool enqueue_fl(FL f, Dado d);
   extern Dado dequeue_fl(FilaLigada f);

   // Bisbilhotando as pontas da 'fila'.
   extern Dado frente_fl(FilaLigada f);
   extern Dado traseira_fl(FilaLigada f);

   /* Ambas tem que serem reescritas, são muito específicas
    * para o tipo de dado 'deque'. Obviamente é uma função
    * que não funciona em 'multithreading'.
    */
   extern char* fl_to_str(FilaLigada f);
   extern void visualiza_fl(FilaLigada f);

#endif 
