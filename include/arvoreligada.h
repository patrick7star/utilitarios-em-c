/* Implementação de uma árvore binária encadeada.*/
#ifndef _ARVORE_LIGADA_H_
#define _ARVORE_LIGADA_H_
#include <stdbool.h>
#include "definicoes.h"

 typedef struct nodulo_da_arvore_binaria *Nodulo, *Node;
 typedef struct arvore_binaria *ArvoreBinaria, *Tree;
 typedef struct cursor_da_arvore { Node nodulo; Tree container; } Cursor;

 // Modos de criação(alocação) e desalocação da árvore.
 Tree  tree_cria_i    (void);
 Tree  tree_cria      (GenT);
 bool  tree_destroi   (Tree);

 // Altera a estrutura de árvore internamente: 
 Cursor  tree_adiciona_raiz       (Tree, GenT);
 Cursor  tree_adiciona_esquerda   (Tree, Cursor, GenT);
 Cursor  tree_adiciona_direita    (Tree, Cursor, GenT);
 GenT    tree_remove              (Tree, Cursor);

 // Acessa alguns elementos da árvore.
 Cursor  tree_direita   (Tree, Cursor); 
 Cursor  tree_esquerda  (Tree, Cursor); 
 Cursor  tree_pai       (Tree, Cursor); 
 Cursor  tree_raiz      (Tree);

 // Informações sobre à árvore ou alguns dos seus nódulos.
 int     tree_numero_de_criancas (Tree, Cursor);
 size_t  tree_quantidade         (Tree self);
 bool    tree_vazia              (Tree self);
 bool    tree_e_folha            (Tree, Cursor);

 // Impressão e formatação dela.
 void tree_imprime_preorder   (Tree, ToString);
 void tree_imprime_postorder  (Tree, ToString);

 // Métodos referentes aos acessos do cursor.
 bool cursor_e_nulo    (Cursor);
 GenT cursor_elemento  (Cursor);
 bool cursor_eq        (Cursor, Cursor);

 /* Todos métodos acima em Inglês. Isso é uma tradução de interface, ou 
  * melhor, um complemento, já que as demais não somem.
  * 
  * Nota: Os métodos 'length', 'total' e 'size' indicam a mesma coisa por
  *       enquanto, que é o total de itens que a árvore tem no momento. 
  *       Estou indecido com o que fazer. Todos são bons nomes para descrever  *       isso, entretanto, outros são ambiguos para o tipo de estrutura 
  *       trabalhado, assim como uma função totalmente diferente. A escolha
  *       mais adequada por enquanto foi manter elas como a mesma, porém
  *       cuidado, que futuramente isso mudará. Portanto, precaução com o
  *       uso das três arbritrariamente. Escolha apenas uma, então use ela
  *       frequentemente.
  */
 Tree    tree_new_i           (void);
 Tree    tree_new             (GenT);
 bool    tree_drop            (Tree);
 Cursor  tree_root            (Tree);
 Cursor  tree_parent          (Tree, Cursor);
 Cursor  tree_right           (Tree, Cursor);
 Cursor  tree_left            (Tree, Cursor);
 int     tree_num_children    (Tree, Cursor);
 Cursor  tree_add_root        (Tree, GenT);
 Cursor  tree_add_left        (Tree, Cursor, GenT);
 Cursor  tree_add_right       (Tree, Cursor, GenT);
 size_t  tree_size            (Tree);
 size_t  tree_total           (Tree);
 size_t  tree_length          (Tree);
 bool    tree_empty           (Tree);
 bool    tree_is_leaf         (Tree, Cursor);
 void    tree_print_preorder  (Tree, ToString);

#endif
