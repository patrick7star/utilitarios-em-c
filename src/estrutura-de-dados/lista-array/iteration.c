
// Constante para comparações.
const IterOutputAL NULO_AL = { .item = NULL };

struct iterador_da_lista_ligada_al {
   ArrayLista instancia;

   size_t inicial;
   size_t contagem;
};

IterAL cria_iter_al(ArrayLista a) {
   IterAL iter;

   iter.inicial = tamanho_al(a);
   iter.contagem = 0;
   iter.instancia = a;

   return iter;
}

static bool iterador_esta_invalido(IteradorRefAL iter) {
   if (iter == NULL) {
      perror("Não foi passado um iterador válido.");
      return false;
   } else if (iter->instancia == NULL) {
      perror("O iterador não tem uma instância.");
      return false;
   } else if (iter->instancia->array == NULL) {
      perror("Erro na estrutura da lista.");
      return false;
   } else {
      size_t t = tamanho_al(iter->instancia);
      size_t T = iter->inicial;
      return  t != T ;
   }
}

size_t contagem_iter_al (IteradorRefAL iter) {
   if (iterador_esta_invalido(iter)) {
      const char* msg_erro = {
         "não é possível determinar o tamanho "
         "de um iterador inválido!"
      };
      // se chegar até aqui é erro na certa.
      perror (msg_erro); abort();
   }
   return (iter->inicial - iter->contagem);
}

IterOutputAL next_al (IteradorRefAL iter) {
   bool nao_e_possivel_iterar = {
      consumido_iter_al(iter) ||
      iterador_esta_invalido(iter)
   };

   if (nao_e_possivel_iterar)
      return NULO_AL;

   size_t I = iter->inicial;
   size_t c = iter->contagem;
   Vetor e = iter->instancia;
   generico_t dado = indexa_al(e, I - c - 1);

   iter->contagem += 1;
   return (IterOutputAL){ .item=dado };
}

bool consumido_iter_al(IteradorRefAL iter)
   { return iter->contagem == iter->inicial; }

IterAL clona_iter_al(IteradorRefAL iter) {
   IterAL novo;

   novo.instancia = iter->instancia;
   novo.contagem = iter->contagem;
   novo.inicial = tamanho_al(iter->instancia);

   return novo;
}
