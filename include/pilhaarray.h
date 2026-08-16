/*   Simple implementation of static array. Don't come in my mind what this 
 * wasn't done before, it is a simple implementation than any other done
 * so far. This differ from other made until here, because implementation,
 * like your documentation was done primarily in English. This because i 
 * haver had no done like this, so i tried once to see how it gets.
 *   Although there is methods declares here, neither all of them have an
 * implementation. This header file it is a copy of 'linked stack' and its
 * literal translation, i chose for allow methods i didn't wrote yet. What
 * does matter it is core of collections you can count on. The other ones 
 * will be implemented gradually over time, when to be most in demand.
 */
#ifndef _ARRAY_STACK_
#define _ARRAY_STACK_
#include "definicoes.h"

 // All possible names to structed that is worked here:
 typedef struct abstract_array_stack
   STACK, stack_t, AS, *Stack, *ArrayStack;

 Stack  new_pa            (void);
 bool   destroy_pa        (Stack);
 bool   destroy_with_pa   (Stack, Drop);

 void   push_pa    (Stack, GenT);
 GenT   pop_pa     (Stack);
 void   extend_pa  (AS*, AS*);
 void   reverse_pa (Stack);

 size_t length_pa  (ArrayStack);
 size_t size_pa    (AS*, size_t);
 bool   empty_pa   (ArrayStack);
 GenT   top_pa     (ArrayStack);

 void   print_pa   (AS*, ToString);
 char*  to_string  (AS*, ToString);

 GenT  to_array_pl   (AS*, int sz);
 GenT  into_array_pl (AS*, int size);

/* === === === === === === === === === === === === === === === === === === ==
 *                Iteration Methods and Related Functions
 * === === === === === === === === === === === === === === === === === === */
 typedef struct iteration_of_array_stack *IterPA, *IteratorPA;
 typedef struct output_of_array_stack_iteration { GenT item; }
   IterOutputPA, IOutPA;

 extern const IterOutputPA NULL_PA;

 IterPA  create_iter_pa    (ArrayStack);
 IterPA  clone_iter_pa     (IteratorPA);
 bool    exhausted_iter_pa (IteratorPA);
 size_t  count_iter_pa     (IteratorPA);
 IOutPA  next_pa           (IteratorPA);

/* === === === === === === === === === === === === === === === === === === ==
 *                   Translate of Methods for Portuguese(pt)
 * 
 *   How the base of code was made in English, translate now change little 
 * of bit here. Instead of translate for English as it was done with the 
 * others, now such translate is added with Portuguese methods and functions.
 * === === === === === === === === === === === === === === === === === === */
 // Translate of every match in Portuguese:
 typedef stack_t PILHA, pilha_t, PA;
 typedef Stack *Pilha, *PilhaArray;

 Pilha  nova_pa        (void);
 bool   destroi_pa     (PilhaArray);
 bool   delete_pl      (PilhaArray);
 bool   coloca_pa      (PA*, GenT);
 GenT   retira_pa      (PilhaArray);
 size_t comprimento_pl (PilhaArray);
 bool   vazia_pl       (PilhaArray);
 GenT   topo_pl        (PilhaArray);
 void   imprime_pl     (PA*, ToString);
 void   extende_pl     (PA*, PA*);
 void   reverte_pl     (PilhaArray);

#endif
