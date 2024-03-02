

#include <stddef.h>
#include <stdbool.h>

#ifndef FILA_ARRAY_H
 // ponteiro 'void' aceita qualquer referência passada para ela.
 typedef void* generico_t;
 typedef struct fila_de_array fila_array_t;

 // alocação e desalocação de recursos.
 fila_array_t* cria_com_capacidade_fa(size_t capacity); 
 fila_array_t* cria_fa(void); 
 bool destroi_fa(fila_array_t* q); 

 // obtém informações sobre a estrutura, inclusive dados internos.
 size_t tamanho_fa(fila_array_t* q);
 bool vazia_fa(fila_array_t* q);
 generico_t primeiro_fa(fila_array_t* q);

 // operações que mudam a estrutura interna da estrutura.
 bool coloca_fa(fila_array_t* q, void* e); 
 generico_t retira_fa(fila_array_t* q); 

#endif
