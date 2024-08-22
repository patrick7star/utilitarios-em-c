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
 size_t comprimento_pl(PilhaLigada);
 size_t tamanho_pl (PilhaLigada, size_t);
 bool vazia_pl (PilhaLigada);
 void* topo_pl (PilhaLigada);
 
 /* Operação que permite visualizar a pilha, assim como formata-la em 
  * uma string. */
 void imprime_pl (PilhaLigada, ToString); 
 char* stack_to_str_pl (PilhaLigada, ToString);

 /* Cria uma array, com a mesma quantidade de itens na pilha, com todos
  * itens empilhados do topo à base, que na array fica respectivamente
  * da esquerda à direita. */
 generico_t* pilha_to_array_pl (PilhaLigada); 

/* === === === === === === === === === === === === === === === === === ==
 *                      Iteradores e seus
 *                         Métodos
 * === === === === === === === === === === === === === === === === === ==*/
 typedef struct iterador_da_pilha_pl *IterPL, *IteradorPL;
 /* O dado retornado quanod iterador 'IterPL'.*/
 typedef struct saida_da_iteracao_da_pl { generico_t item; } IterOutputPL;

 /* Retorna isso quando o iterador está consumido por inteiro. */
 extern const IterOutputPL NULO_PL;

 // Criação e desalocação do iterador:
 IteradorPL cria_iter_pl(PilhaLigada);
 IterPL clona_iter_pl(IteradorPL);

 // Acesso a informação ou muda sua estrutura:
 bool consumido_iter_pl (IteradorPL); 
 size_t contagem_iter_set (IteradorPL);
 IterOutputPL next_pl (IteradorPL);

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

 generico_t* stack_to_array_pl (PilhaLigada); 

#endif
