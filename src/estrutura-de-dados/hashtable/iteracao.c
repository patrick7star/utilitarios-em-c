struct Iteracao_da_Hashtable_Ref {
   // contador de itens iterados.
   size_t contagem;
   // posição atual na array.
   size_t indice;

   // referência ao atual item da 'tabela' referenciado.
   nodulo_t* cursor;

   // garantidor de que a 'tabela' não foi alterada.
   size_t inicialmente;
   HashTable instancia;

};

// O valor da iteração, quando não é mais possível consumir, será este.
const IterOutputHT NULO_HT = {NULL, NULL};

IterHT cria_iter_ht (HashTable m) {
   const int sz_iter = sizeof(struct Iteracao_da_Hashtable_Ref);
   nodulo_t* no = m->locais[0];
   IterHT self = malloc (sz_iter);

   if (self != NULL) {
      self->contagem = 0;
      self->indice = 0;
      self->cursor = no;
      self->inicialmente = tamanho_ht (m);
      // referência ao próprio mapa, dicio, table,... como quiser chamar.
      self->instancia = m;
   }
   return self;
}

static bool iterador_valido (IterHT iter) {
/* Para realizar qualquer uma das operações abaixo, é necessário
 * que a instância seja válida, ou seja, ainda existe, ou tem o mesmo
 * tamanho que na criação da instância. */
   size_t T = tamanho_ht(iter->instancia);
   bool referencia_existe = (iter->instancia != NULL);
   return (iter->inicialmente == T && referencia_existe);
}

size_t contagem_iter_ht (IterHT iter) {
/* O restante de iterações é calculado na seguinte forma: total de itens 
 * na "lista" menos os já iterados. */
   if (iterador_valido (iter))
      return iter->inicialmente - iter->contagem;

   // se chegar até aqui é erro na certa.
   perror ("não é possível determinar o tamanho de um iterador inválido!");
   abort();
}

IterOutputHT next_ht (IterHT iter) {
/* O algoritmo que pega o próximo item da iteração. Seu modo de funcionar
 * é o seguinte; vai iterando o cada posição na array de listas-ligadas,
 * se não houver nada nela(sem lista), ele pula para o próximo índice 
 * nela, se houver um nó, seguir a lista ligada. Cada iteração com um
 * item válido(nó) é contabilizado. */
   if (!iterador_valido(iter))
      return NULO_HT;
   else if (contagem_iter_ht(iter) == 0)
      return NULO_HT;

   nodulo_t* atual = iter->cursor;
   if (atual != INVALIDA) {
      // colhendo dados necessários ...
      generico_t vl = atual->valor;
      generico_t ch = atual->chave;

      // movendo pela lista ...
      iter->cursor = iter->cursor->seta;
      // contabiliza iteração.
      iter->contagem++;

      // retorna item "cholido".
      return (IterOutputHT){.key = ch, .value=vl };
   } else  {
      iter->indice += 1;
      /* primeiro 'nódulo' da lista abaixo. */
      iter->cursor = iter->instancia->locais[iter->indice];
      // chama a função recursivamente novamente...
      return next_ht (iter);
   }
}

bool consumido_iter_ht(IterHT iter) 
// Diz se o iterador se esgotou(contagem atingiu valor inicial).
   { return iter->contagem == iter->inicialmente; }

IterHT clona_iter_ht(IterHT iter) {
/* Clona o iterador passado, à partir do estágio que está. A alteração 
 * deste novo clone, ou do original, não alteram a iteração de cada, más 
 * sim, a mudança da estrutura original, que não permite ambos realizar 
 * mais iterações. */ 
   IterHT novo = cria_iter_ht(iter->instancia);

   // Copiando informações:
   if (novo != NULL) {
      novo->instancia = iter->instancia;
      novo->contagem = iter->contagem;
      novo->inicialmente = tamanho_ht(iter->instancia);
      // Camos internos para iteração(até mais importante que os acimas):
      novo->cursor = iter->cursor;
      novo->indice = iter->indice;
   }
   return novo;
}

void destroi_iter_ht(IterHT iter) {
   iter->cursor = NULL;
   iter->instancia = NULL;
   free(iter);
}
