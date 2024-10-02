/* Criando uma simples fila-ligada com o tipo também embutido aqui. Se 
 * desejar trocar o tipo dela futuramente, apenas altere as linhas iniciais.
 */

#ifndef FILA_LIGADA_H
#define FILA_LIGADA_H
#include "definicoes.h"

 // Todos tipos de nomes da estrutura.
 typedef struct fila_ligada_ref fila_ligada_t, *FilaLigada, FL;

 // Aloca e desaloca a 'fila ligada'.
 FilaLigada cria_fl (void);
 bool destroi_fl (FilaLigada f); 

 /* Obtem informações sobre a estrutura: quantidade de itens, estado dela,
  * o item das pontas da fila. 
  */
 size_t tamanho_fl (FilaLigada f);
 bool vazia_fl (FilaLigada f);
 generico_t final_fl (FilaLigada f); 
 generico_t comeco_fl (FilaLigada f);

 /* Altera o estado da 'fila', aumentando seu tamanho, ou diminuindo. */
 bool insere_fl(FilaLigada f, generico_t d); 
 generico_t remove_fl(FilaLigada f);

#endif
