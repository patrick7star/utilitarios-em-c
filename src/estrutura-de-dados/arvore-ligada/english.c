/* Submódulo que cuida da implementação de todos métodos da árvore, mas em
 * Inglês, já que tais operações e funções são mais conhecidas deste modo.
 */

Tree tree_new_i(void)
   {  return tree_cria_i(); }

Tree tree_new(GenT data)
   { return tree_cria(data); }

bool tree_drop(Tree self)
   { return tree_destroi(self); }

Cursor tree_root(Tree self)
   { return tree_raiz(self); }

Cursor tree_parent(Tree self, Cursor p)
   { return tree_pai(self, p); }

Cursor tree_right(Tree self, Cursor p)
   { return tree_direita(self, p); }

Cursor tree_left(Tree self, Cursor p)
   { return tree_esquerda(self, p); }

int tree_num_children(Tree self, Cursor p)
   { return tree_numero_de_criancas(self, p); }

Cursor tree_add_root(Tree self, GenT data)
   { return tree_adiciona_raiz(self, data); }

Cursor tree_add_left(Tree self, Cursor p, GenT e)
   { return tree_adiciona_esquerda(self, p, e); }

Cursor tree_add_right(Tree self, Cursor p, GenT e)
   { return tree_adiciona_direita(self, p, e); }

size_t tree_size(Tree self)
   { return tree_quantidade(self); }

size_t tree_length(Tree self)
   { return tree_size(self); }

size_t tree_total(Tree self)
   { return tree_size(self); }

bool tree_empty(Tree self)
   { return tree_vazia(self); }

bool tree_is_leaf(Tree self, Cursor p)
   { return tree_e_folha(self, p); }

void tree_print_preorder(Tree a, ToString fmt)
   { return tree_imprime_preorder(a, fmt); }

