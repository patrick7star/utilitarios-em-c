

#ifndef __FILA_ARRAY_REF_H__
#define __FILA_ARRAY_REF_H__
#include "definicoes.h"

 typedef struct fila_de_array_referencia fila_array_t, *FilaArray, FA;

 // alocação e desalocação de recursos.
 FilaArray  cria_com_capacidade_fa (size_t); 
 FilaArray  cria_fa                (void); 
 bool       destroi_fa             (FilaArray q); 

 // obtém informações sobre a estrutura, inclusive dados internos.
 size_t     tamanho_fa  (FilaArray);
 bool       vazia_fa    (FilaArray);
 generico_t primeiro_fa (FilaArray);
 size_t     capacidade  (FilaArray);
 // Retorna a diferença entre a 'capacidade' e o total de 'itens' nela.
 size_t     vacuo_fa    (FilaArray); 

 // Operações que mudam a estrutura interna da estrutura.
 bool        coloca_fa (FilaArray q, generico_t e); 
 generico_t  retira_fa (FilaArray q); 

 /* Visualização genérica, que exige que se passe um método de transformação
  * de strign para ela, assim ela pode imprimir tal estrutura de dados. */
 void print_fila_fa (FilaArray, ToString); 

#endif
