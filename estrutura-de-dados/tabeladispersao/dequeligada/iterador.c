

/* Jogarei o trecho de código do iterador para cá, para deixar mais
 * fácil a localização dos componentes do código.
 * Sem falar que, com a intenção da implementação de mais métodos do 
 * tipo, só preciso anexa-lôs aqui, ao invés de ter que achar um meio
 * no antigo arquivo, ou pior, anexa-lô lá, deixando o código de
 * forma desorganizada.
 */

/* iterador que referência todos dados internos nos 'nós' da lista.
 * Quando chega ao fim simplesmente retorna 'null', muito parecido
 * com outras linguagens. */
struct iterador_deque_ligada {
   // quantidade de 'dados' na lista na geração deste.
   uint64_t total;

   /* posição na iteração, tanto a númerica baseado no tamanho dela,
    * quanto o 'nó' atual apontado. */
   uint64_t indice;
   No atual;

   // referência da própria 'deque' para verifica o total de elementos.
   DequeLigada referencia;
};

// Nome mais curto para 'codificação' com também 'importação'.
typedef struct iterador_deque_ligada* IterDequeLigada;
typedef IterDequeLigada IterDL;

#define ITER_SIZE sizeof(struct iterador_deque_ligada)

IterDequeLigada cria_iter_dl(DequeLigada d) {
   IterDequeLigada I = malloc(ITER_SIZE);

   // se tiver alocado corretamente ...
   if (I != NULL) {
      I->total = tamanho_dl(d);
      I->indice = 0;
      I->atual = d->comeco->proximo;
      I->referencia = d;
   }

   // retorna instâncias criada e inicilizada, ou a falhar disso.
   return I;
}

// função de iteração, sempre dá o próximo elemento dele.
Dado next_dl(IterDequeLigada iter) {
   /* se houve alguma mudança interna na lista, não será possível
    * continuar a iteração mesmo que ainda não tenha finalizado. */
   if (iter->total != tamanho_dl(iter->referencia))
      return NULL;

   // se tiver chegado no 'nó sentinela' não é possível mais iterar.
   if (!iter->atual->e_sentinela) {
      // estamos no elemento ...
      iter->indice++;
      Dado dataref = iter->atual->dado;
      // indo para frente.
      iter->atual = iter->atual->proximo;
      // pegando atual dado.
      return dataref;
   }

   // alcançou a outra ponta, ou pode nem ter saído dela.
   return NULL;
}

/* O total consumido de dados se dá por, total de itens na 'deque',
 * menos onde o índice(começado de zero) está no momento. */
uint64_t iter_tamanho_dl(IterDequeLigada iter)
   { return iter->total - iter->indice; }

void destroi_iter_dl(IterDL iter) {
   if ( iter != NULL ) {
      // acando linque de referência para estrutura de dado.
      iter->referencia = NULL;
      // assim também como para o 'nó'.
      iter->atual = NULL;
   }
   // então libera a estrutura.
   free(iter);
   iter = NULL;
}

