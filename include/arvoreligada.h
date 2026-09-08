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
 void tree_impreme_preorder(Tree, ToString);

 // Métodos referentes aos acessos do cursor.
 bool cursor_e_nulo    (Cursor);
 GenT cursor_elemento  (Cursor);
 bool cursor_eq        (Cursor, Cursor);

#endif
