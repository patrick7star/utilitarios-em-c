#include <stdbool.h>
#include <stddef.h>

#ifndef _PILHA_LIGADA_REF
#define _PILHA_LIGADA_REF
 // qualquer referência, de qualquer tipo.
 typedef void* generico_t;
 // Todos nomes que eles podem assumir.
 typedef struct pilha_ligada_abstracao 
   *PilhaLigada, PL, pilha_ligada_t, 
   PILHA_LIGADA, *LinkedStack;
 /* Funções importantes para alguns operações sejam aplicadas sobre os 
  * dados que a pilha porta: visualização, desalocação e etc. */
 typedef bool (*Drop)(generico_t);
 typedef char* (*ToString)(generico_t);

 // Funções que criam e destró a instância do tipo de dado:
 PilhaLigada cria_pl (void);
 bool destroi_pl (PilhaLigada); 
 bool destroi_interno_pl (PilhaLigada, Drop); 

 // Operações alteram a pilha:
 bool coloca_pl (PilhaLigada, generico_t); 
 generico_t retira_pl (PilhaLigada); 
 /* Pega a base da segunda pilha(segundo parâmetro), então coloca ela na 
  * primeira(o primeiro parâmetro da função). A primeira pilha fica assim
  * extendida, a segunda vázia. */
 void extende_pl (PilhaLigada, PilhaLigada);

 /* Operações que verificam informações sobre as estruturas: total de itens,
  * valor no topo da pilha e etc. */
 size_t tamanho_pl (PilhaLigada);
 bool vazia_pl (PilhaLigada);
 void* topo_pl (PilhaLigada);
 
 /* Operação que permite visualizar a pilha, assim como formata-la em 
  * uma string. */
 void imprime_pl (PilhaLigada, ToString); 
 char* stack_to_str_pl (PilhaLigada, ToString);

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 *                     Iterador da pilha-ligada
 *                        e seus métodos
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -- */
 typedef struct iterador_da_pilha_pl IterPL, *IteradorRefPL;
 typedef struct saida_da_iteracao_da_pl IterOutputPL;
 // Constante que representa nenhum item iterado.
 const IterOutputPL NULO_SET;

 // Métodos de criação, mudança e atual estado:
 IterPL cria_iter_pl (PilhaLigada); 
 /* Faz a clonagem do iterador dado, inclusive seu estado(consumido ou 
  * não), no mesmo estágio do que foi usado.*/
 IterPL clona_iter_pl(IteradorRefPL);
 // Total de itens ainda a consumir.
 size_t contagem_iter_pl (IteradorRefPL);
 IterOutputPL next_pl (IteradorRefPL); 
 // Verifica se todos itens do 'conjunto' foram consumido via iterador.
 bool consumido_iter_pl (IteradorRefPL);

/* === === === === === === === === === === === === === === === === === ==
 *                      Renomeação de vários 
 *                métodos e funções para os termos
 *                  mais conhecidos em inglês
 *
 * Observação: ordem dos métodos e funções, seguem a mesma acima; tudo isso
 * para facilitação na hora de achar os correspodentes.
 * === === === === === === === === === === === === === === === === === ==*/
 PilhaLigada new_pl (void);
 bool delete_pl (PilhaLigada); 

 bool push_pl(PilhaLigada, generico_t); 
 generico_t pop_pl(PilhaLigada); 

 size_t len_pl (PilhaLigada);
 bool empty_pl (PilhaLigada);
 void* top_pl (PilhaLigada);

 void print_pl (PilhaLigada, ToString); 

#endif
