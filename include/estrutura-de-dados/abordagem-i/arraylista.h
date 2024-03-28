
#include <stdbool.h>

#ifndef ARRAYLISTA_H
#define ARRAYLISTA_H
 typedef struct array_lista ArrayLista, Vetor, vetor_t;
 // criação e destruição do tipo de dado.
 ArrayLista* cria_al();
 bool destroi_al(ArrayLista*); 

 // operações que ela realiza.
 bool insere_al(ArrayLista*, void*); 
 void* remove_al(ArrayLista*); 
 void* indexa_al(vetor_t*, size_t); 

 // relacionada ao seu tamanho.
 bool vazia_al(ArrayLista*);
 size_t tamanho_al(ArrayLista*);

#endif 
