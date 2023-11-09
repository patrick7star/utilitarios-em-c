

// O algoritimo também será passado para cá, pelo motivo de organização.

// nome curto para o tipo de 'dado' abaixo.
typedef struct node* No;

/* Tipo de objeto que, armazena o objeto na memória
 * e aponta para mais dois objetos do próprio tipo
 * de "maneira bidirecional". */
struct node {
   // dado que ele contém. Lembrando que pode haver nenhum também.
   Dado dado;

   // duas direções do modelo.
   No proximo;
   No anterior;

   /* se é um tipo especial de 'nó', onde o dado que ele contém
    * é inexistente, e se houver algo lá, é irrelevante para a 
    * aplicação. */
   bool e_sentinela;
};
#define SIZE_NODE sizeof(struct node)

#include <stdlib.h>
#include <stdio.h>

/* recebe tipo de 'dado' como argmento, absorve tal 'dado', e referência
 * outros 'nós'(nada por enquanto). */
No cria_no(Dado d) {
   No no = malloc(sizeof(struct node));
   if (no != NULL) {
      no->dado = d;
      no->proximo = NULL;
      no->anterior = NULL;
      no->e_sentinela = false;
      return no;
   }
   return NULL;
}

No cria_sentinela(void) {
   No no = malloc(SIZE_NODE);
   if (no != NULL) {
      no->dado = cria_dado_branco();
      no->proximo = NULL;
      no->anterior = NULL;
      no->e_sentinela = true;
      return no;
   }
   return NULL;
}
// destrói o tipo de 'dado' alocado acima.
void destroi_no(No no) {
   if ( no != NULL ) {
      destroi_dado(no->dado);
      no->proximo = NULL;
      no->anterior = NULL;
      free(no);
      no = NULL;
   }
}
