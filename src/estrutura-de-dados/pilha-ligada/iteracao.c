/* Parte de iteração do código de pilha-ligada. Novamente, inclusão exatamente aonde foi
 * removido. Acho que isso não quebra a biblioteca, porque há métodos que só funcionam quando
 * as funções de iteração são criadas. 
 */

struct iterador_da_pilha_pl {
   // Instância do pilha-ligada que se itera.
    PilhaLigada instancia;

   // Total de itens da estrutura dada, e a contagem de iterações feitas.
   size_t inicial; size_t contagem;

   // Referência ao próximo nó à iterar.
   Node cursor;
};

const IterOutputPL NULO_PL = { NULL };

IterPL cria_iter_pl(PilhaLigada a) {
   // Total de bytes do tipo de dado.
   const int sz_iter = sizeof(struct iterador_da_pilha_pl);
   IterPL self = malloc (sz_iter);

   if (self != NULL) {
      // Quantos itens há inicialmente nela(para constante verificação).
      self->inicial = comprimento_pl(a);
      self->cursor = a->topo;
      // Começa, obviamente, com zero itens iterados.
      self->contagem = 0;
      // A referência da instância que se está iterando...
      self->instancia = a;
   }
   return self;
}

bool consumido_iter_pl(IterPL iter)
   { return iter->contagem == iter->inicial; }

size_t contagem_iter_set (IterPL iter) {
   // Verificando se o iterador é válido.
   bool iterador_e_valido = {
      iter != NULL
      && !consumido_iter_pl(iter)
   };

   if (iterador_e_valido)
      return (iter->inicial - iter->contagem);

   // se chegar até aqui é erro na certa.
   perror ("não é possível determinar o tamanho de um iterador inválido!");
   abort();
}

IterOutputPL next_pl (IterPL iter) {
// A iteração partirá do topo da pilha até o último nódulo dela.
   if (consumido_iter_pl(iter))
      return NULO_PL;

   generico_t dado_no_nodulo = iter->cursor->dado;
   // Indo para frente(à partir do topo).
   iter->cursor = iter->cursor->seta;
   // Contabilizando iterações realizadas.
   iter->contagem += 1;
  
   return (IterOutputPL){ .item=dado_no_nodulo };
}

IterPL clona_iter_pl(IterPL iter) {
   IterPL novo = cria_iter_pl(iter->instancia);

   if (novo != NULL) {
   // Copiando informações, em seus atuais estados:
      novo->instancia = iter->instancia;
      novo->contagem = iter->contagem;
      novo->inicial = comprimento_pl(iter->instancia);
      novo->cursor = iter->cursor;
   }
   return novo;
}

void destroi_iter_pl(IterPL a) {
   // Não segura mais o container da estrutura.
   a->instancia = NULL;
   // Livra-se também da referência do atual nódulo iterador nela.
   a->cursor =NULL;
   // libera tal instância.
   free(a);
}
