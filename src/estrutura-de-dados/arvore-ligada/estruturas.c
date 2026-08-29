
typedef size_t SizeT;
typedef struct nodulo_da_arvore_binaria nodulo_t;
typedef struct arvore_binaria binary_tree_t;

struct arvore_binaria {
   Node raiz;
   SizeT quantia;

};

struct nodulo_da_arvore_binaria {
   GenT dado;
   Node pai;
   Node esquerdo;
   Node direito;

};

const Cursor CURSOR_NULL = { 
   .nodulo     = NULL,
   .container  = NULL
};

#define NodePtrSz sizeof(nodulo_t*)
#define NodeSize sizeof(nodulo_t)
