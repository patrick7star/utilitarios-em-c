

/* Novamente, mesmo caso que na 'fila', ao 
 * invés de fazer todo o código de listagem,
 * só para o caso da 'pilha', pega uma deque
 * e só reexporto as operações que cabem a
 * estrutura de dados 'Pilha'. No futuro, bem
 * lá no futuro, copio o código dela, e apago
 * o código que não tem relevância para 'pilhas'.
 */

#include "dado.h"

#ifndef PILHA_LIGADA_H
#define PILHA_LIGADA_H
   typedef struct pilha_ligada* PILHA_LIGADA;
   typedef PILHA_LIGADA PL;

   // criação e liberação da 'Fila', novamente renomeando...
   extern PILHA_LIGADA cria_pl();
   extern void destroi_pl(PL p);

   // verifica se há itens na 'Pilha'.
   extern bool vazia_pl(PILHA_LIGADA p);
   // o total de elementos líquido nela(adições menos remoções).
   extern uint64_t quantidade_pl(PL p);

   /* Operações sobre a lista, estas que aumentam
    * ou diminuem suas quantias. 
    */

   // Operação de inserção e remoção da 'Pilha Ligada'.
   extern bool coloca_pl(PL p, Dado d);
   extern Dado retira_pl(PILHA_LIGADA p);

   // Bisbilhota o 'dado' que está no 'topo' dela. 
   extern Dado topo_pl(PILHA_LIGADA p);

   /* Ambas tem que serem reescritas, são muito específicas
    * para o tipo de dado 'deque'. Obviamente é uma função
    * que não funciona em 'multithreading'.
    */
   extern char* pl_to_str(PILHA_LIGADA p);
   extern void visualiza_pl(PILHA_LIGADA p);

   // iterador da estrutura.
   typedef struct iterador*  IterPilhaLigada;
   typedef IterPilhaLigada IterPL;

   // criação e desconstrução do iterador.
   extern IterPilhaLigada cria_iter_pl(PILHA_LIGADA p);
   extern void destroi_iter_pl(IterPL i); 
   // obtém o próximo item(um Dado) do iterador.
   extern Dado next_pl(IterPilhaLigada iterador);
   // contagem de iterações restantes.
   extern uint64_t contagem_iter_pl(IterPL iterador);

#endif // PILHA_LIGADA_H
