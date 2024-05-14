

#include <stddef.h>
#include <stdbool.h>

#ifndef FILA_ARRAY_I_H
#define FILA_ARRAY_I_H
 // ponteiro 'void' aceita qualquer referência passada para ela.
 typedef void* generico_t;
 typedef struct fila_de_array fila_array_t, *FilaArray, FA;

 // alocação e desalocação de recursos.
 fila_array_t* cria_com_capacidade_fa (size_t); 
 fila_array_t* cria_fa (void); 
 bool destroi_fa (fila_array_t* q); 

 // obtém informações sobre a estrutura, inclusive dados internos.
 size_t tamanho_fa (FilaArray);
 bool vazia_fa (FilaArray);
 generico_t primeiro_fa (FilaArray);
 size_t capacidade (FilaArray);
 // Retorna a diferença entre a 'capacidade' e o total de 'itens' nela.
 size_t vacuo_fa(FilaArray); 

 // operações que mudam a estrutura interna da estrutura.
 bool coloca_fa (fila_array_t* q, generico_t e); 
 generico_t retira_fa (fila_array_t* q); 

 #define __to_string__ char* (__str__)(generico_t)
 /* Visualização genérica, que exige que se passe um método de transformação
  * de strign para ela, assim ela pode imprimir tal estrutura de dados. */
 void print_fila_fa(FilaArray, __to_string__); 

#endif
