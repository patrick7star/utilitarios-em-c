

/* criando uma simples fila-ligada com o tipo também embutido aqui. Se desejar 
 * trocar o tipo dela futuramente, apenas altere as linhas iniciais. 
 */

#include <stdbool.h>
#include <stddef.h>

#ifndef FILA_LIGADA_H
#define FILA_LIGADA_H

 // tal ponteiro pode assumir qualquer valor.
 typedef void* generico_t;
 typedef struct fila_ligada fila_ligada_t, *FilaLigada, FL;

 // Aloca e desaloca a 'fila ligada'.
 fila_ligada_t* cria_fl ();
 bool destroi_fl (fila_ligada_t* f); 

 /* Obtem informações sobre a estrutura: quantidade de itens, estado dela,
  * o item das pontas da fila. 
  */
 size_t tamanho_fl (fila_ligada_t* f) 
 bool vazia_fl(fila_ligada_t* f) 
 generico_t final_fl (fila_ligada_t* f); 
 generico_t comeco_fl (fila_ligada_t* f);

 /* Altera o estado da 'fila', aumentando seu tamanho, ou diminuindo. */
 bool insere_fl(fila_ligada_t* f, generico_t d); 
 generico_t remove_fl(fila_ligada_t* f);


#endif
