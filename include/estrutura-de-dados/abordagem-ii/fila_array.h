
#include <stdbool.h>
#include <stdint.h>


#ifndef FILA_ARRAY_H
#define FILA_ARRAY_H
 // o tipo de dado desta estrutura trabalha é de cópia.
 typedef uint16_t dado_t, Dado, DADO;
 // todos os nomes possíveis para esta estrutura de dado:
 typedef struct fila_de_array fila_array_t, FILA_ARRAY, FilaArray;

 /* Alocação e desalocação do tipo de dado na memória. Aqui estão as 
  * múltiplas modabilidades de ser fazer isso. 
  */
 FilaArray* cria_capacidade_fa(size_t capacidade); 
 FilaArray* cria_fa(); 
 bool destroi_fa(FilaArray* q); 

 /* Métodos relacionados a quantia de elementos que o tipo de dado tem. */
 size_t tamanho_fa(FilaArray* q); 
 bool vazia_fa(FilaArray* q);

 /* Estes métodos estão relacionados a inserção, acesso e remoção de itens
  * na estrutura. */
 Dado* primeiro_fa(FilaArray* q);
 bool coloca_fa(FilaArray* q, Dado e); 
 Dado* retira_fa(FilaArray* q); 

 // conversão em string e visualização da fila.
 char* queue_to_str_fa(FilaArray* q); 
 void visualiza_fa(FilaArray* q); 

#endif

#ifndef FILA_ARRAY_H
 /* Métodos acimas porém em inglês, já que os métodos nesta língua são mais
  * conhecidos. Isto aqui é só um embrulho para as chamadas acima. Não todas
  * inicialmente, só as mais conhecidas.
  */
 bool enqueue_fa(fila_array_t* queue, dado_t item) 
   { return coloca_fa(queue, item); }

 dado_t* dequeue_fa(fila_array_t* queue) 
   { return retira_fa(queue, item); }

 dado_t* first_fa(fila_array_t* queue)
   { return primeiro_fa(queue); }

#endif
