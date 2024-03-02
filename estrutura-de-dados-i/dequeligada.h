
#include <stdbool.h>
#include <stddef.h>

#ifndef DEQUE_LIGADA_H
#define DEQUE_LIGADA_H

 // nomes que estrutura ganha:
 typedef struct lista_duplamente_encadeada dequeligada_t, *DequeLigada, DL;
 typedef void* generico_t;

 // criação e desalocação da estrutura.
 dequeligada_t* cria_dl(void); 
 bool destroi_dl(dequeligada_t* l); 

 /* Operações que mudam a estrutura de dados. */
 bool insere_comeco_dl(dequeligada_t* l, generico_t e); 
 bool insere_final_dl(dequeligada_t* l, generico_t e); 
 generico_t remove_comeco_dl(dequeligada_t* l); 
 generico_t remove_final_dl(dequeligada_t* l); 

 // operações de verificação ou acesso aos dados interno da lista.
 bool vazia_dl(dequeligada_t* l);
 generico_t comeco(dequeligada_t* l); 
 generico_t final(dequeligada_t* l); 
 size_t tamanho_dl(dequeligada_t* l);

#endif
