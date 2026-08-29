bool cursor_eq(Cursor self, Cursor outro)
{
   return (self.nodulo == outro.nodulo) && 
          (self.container == outro.container); 
}

bool cursor_e_nulo(Cursor self)
   { return (self.nodulo == NULL) && (self.container == NULL); }

GenT cursor_elemento(Cursor self)
{
// Retorna 'null' em caso de não haver um elemento.
   if (cursor_e_nulo(self))
      return NULL;

   return (*self.nodulo).dado;
}

static Cursor cursor_cria(Tree container, Node node)
{
// Método construtor do 'Cursor'.
   Cursor self;

   self.container = container;
   self.nodulo = node;
   return self;
}

static Cursor cursor_novo(Tree self, Node obj)
{
   if (obj == NULL)
      return CURSOR_NULL;
   else
      return cursor_cria(self, obj);
}
