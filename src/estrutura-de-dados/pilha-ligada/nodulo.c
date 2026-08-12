/*   A parte da criação do nódulo que é um dos núclos da lista-ligada que cria a estrutura
 * de dados pilha aqui, está sendo transportada para cá, pois há muitas linhas, e métodos
 * próprios, além de encabeçar o começo do código, portanto dificulta um pouco a localização
 * das partes importantes, com este entulho todo no começo, que nunca é realmente alterado.
 */
typedef struct nodulo {
   // referência de qualquer tipo ao dado.
   generico_t dado;

   // referência ao próximo nódulo.
   struct nodulo* seta;

} *Node;

// Tamanho já computado aqui, e devidamente nomeado.
#define NODULO_SZ sizeof (struct nodulo)

Node cria_nodulo(generico_t dado) {
   Node instancia = malloc (NODULO_SZ);
  
   if (instancia != NULL) {
      instancia->dado = dado;
      instancia->seta = NULL;
   }
   return instancia;
}

bool destroi_nodulo(Node x) {
   if (x == NULL) return false;
   /* O dado, que é uma referência, não pode ser desalocado aqui, pois não
    * sabe-se como ele é formado; pode ser que seja até algo estático. Aqui
    * apenas desconectamos tal referência. */
   x->dado = NULL;
   x->seta = NULL;
   // liberando ...
   free(x);
   x = NULL;
   return true;
}

bool destroi_nodulo_e_dado(Node x, Drop descontrutor) {
   if (x == NULL) return false;
   /* O dado, que é uma referência, não pode ser desalocado aqui, pois não
    * sabe-se como ele é formado; pode ser que seja até algo estático. Aqui
    * apenas desconectamos tal referência. */
   x->dado = NULL;
   x->seta = NULL;
   // liberando ...
   free(x);
   x = NULL;
   return true;
}
