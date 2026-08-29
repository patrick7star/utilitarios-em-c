static Node cria_node(GenT data)
{
   const int size = sizeof(nodulo_t);
   Node self = malloc(size);

   if (self == NULL)
   {
      perror("Falha ao alocar memória!");
      abort();
   }

   (*self).dado = data;
   (*self).esquerdo = NULL;
   (*self).direito = NULL;
   (*self).pai = NULL;

   return self;
}

static bool destroi_node(Node self)
{
   (*self).pai = NULL;
   (*self).esquerdo = NULL;
   (*self).direito = NULL;
   (*self).dado = NULL;
   free(self);
   return true;
}

static Node validacao(Tree self, Cursor p)
{
// Retorna uma cópia do nódulo interno do cursor.
   if (cursor_e_nulo(p))
      return NULL;

   if (self != p.container && p.container) 
      // { perror("'p' não pertence a está árvore."); abort(); }
      INTERROMPE("'p' não pertence a está árvore.");
   if ((*p.nodulo).pai == p.nodulo)
      // { perror("'p' não é mais válido."); abort(); }
      INTERROMPE("'p' não é mais válido.");

   return p.nodulo;
}

