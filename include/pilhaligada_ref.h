#ifndef _PILHA_LIGADA_REF
#define _PILHA_LIGADA_REF
#include "definicoes.h"

 // Todos nomes que eles podem assumir.
 typedef struct pilha_ligada_abstracao
   *PilhaLigada, PL, *PilhaL, pilha_ligada_t, PILHA_LIGADA, 
   *LinkedStack, LS, *LStack, linked_stack_t, LINKED_STACK;

 // Funções que criam e destró a instância do tipo de dado:
 PilhaL  cria_pl            (void);
 bool    destroi_pl         (PilhaL);
 bool    destroi_interno_pl (PilhaL, Drop);

 /* Todas operações que alteram a pilha internamente, seja a expandido/ou reduzindo, seja
  * alterando suas posições abstratas. 
  *
  * extende_pl -  Pega a base da segunda pilha(segundo parâmetro), então coloca ela na
  *               primeira(o primeiro parâmetro da função). A primeira pilha fica assim
  *               extendida, a segunda vázia. 
  * inverte_pl -  Inverte a pilha, topo vira base, base vira topo.
  */
 bool coloca_pl   (PilhaL, GenT);
 GenT retira_pl   (PilhaLigada);
 void extende_pl  (PilhaL, PilhaL);
 void inverte_pl  (PilhaLigada);

 /* Operações que verificam informações sobre as estruturas: total de itens,
  * valor no topo da pilha e etc. */
 size_t  comprimento_pl  (PilhaLigada);
 size_t  tamanho_pl      (PL*, size_t);
 bool    vazia_pl        (PilhaLigada);
 GenT    topo_pl         (PilhaLigada);

 /* Operação que permite visualizar a pilha, assim como formata-la em
  * uma string. */
 void    imprime_pl      (PL*, ToString);
 char*   stack_to_str_pl (PL*, ToString);

 /* Cria uma array, com a mesma quantidade de itens na pilha, com todos
  * itens empilhados do topo à base, que na array fica respectivamente
  * da esquerda à direita. */
 GenT to_array_pl       (PilhaLigada, int sz);
 GenT into_array_pl     (PilhaL, int size);

/* === === === === === === === === === === === === === === === === === ==
 *                      Iteradores e seus
 *                         Métodos
 * === === === === === === === === === === === === === === === === === ==*/
 typedef struct iterador_da_pilha_pl *IterPL, *IteradorPL;
 /* O dado retornado quanod iterador 'IterPL'.*/
 typedef struct saida_da_iteracao_da_pl { generico_t item; }
   IterOutputPL, IOutPL;
   // NOTA: Evite de usar 'IOutPL', ele tem como objetivo somente alinhar os métodos abaixo
   //       para leitura do usuário da biblioteca, nada mais.

 /* Retorna isso quando o iterador está consumido por inteiro. */
 extern const IterOutputPL NULO_PL;

 // Criação e desalocação do iterador:
 IterPL  cria_iter_pl      (PilhaLigada);
 IterPL  clona_iter_pl     (IteradorPL);
 // Acesso a informação ou muda sua estrutura:
 bool    consumido_iter_pl (IteradorPL);
 size_t  contagem_iter_set (IteradorPL);
 IOutPL  next_pl           (IteradorPL);

/* === === === === === === === === === === === === === === === === === ==
 *                      Renomeação de vários
 *                métodos e funções para os termos
 *                  mais conhecidos em inglês
 *
 * Observação: ordem dos métodos e funções, seguem a mesma acima; tudo isso
 * para facilitação na hora de achar os correspodentes.
 * === === === === === === === === === === === === === === === === === ==*/
 LStack  new_pl      (void);
 bool    destroy_pl  (LinkedStack);
 bool    delete_pl   (LinkedStack);
 bool    push_pl     (LS*, GenT);
 GenT    pop_pl      (LinkedStack);
 size_t  length_pl   (LinkedStack);
 size_t  len_pl      (LStack);
 bool    empty_pl    (LinkedStack);
 GenT    top_pl      (LinkedStack);
 void    print_pl    (LS*, ToString);
 void    extend_pl   (LS*, LS*);
 void    reverse_pl  (LStack);

#endif
