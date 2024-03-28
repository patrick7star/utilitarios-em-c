
#include "dado.h"

// nome curto para o tipo de 'dado' abaixo.
typedef struct node* No;

/* Tipo de objeto que, armazena o objeto na memória
 * e aponta para mais dois objetos do próprio tipo
 * de "maneira bidirecional". */
struct node {
   // dado que ele contém. Lembrando que pode haver nenhum também.
   Dado dado;

   // duas direções do modelo.
   No proximo;
   No anterior;

   /* se é um tipo especial de 'nó', onde o dado que ele contém
    * é inexistente, e se houver algo lá, é irrelevante para a 
    * aplicação. */
   bool e_sentinela;
};
#define SIZE_NODE sizeof(struct node)

#include <stdlib.h>
#include <stdio.h>

/* recebe tipo de 'dado' como argmento, absorve
 * tal 'dado', e referência outros 'nós'(nada
 * por enquanto). */
No cria_no(Dado d) {
   No no = malloc(sizeof(struct node));
   if (no != NULL) {
      no->dado = d;
      no->proximo = NULL;
      no->anterior = NULL;
      no->e_sentinela = false;
      return no;
   }
   return NULL;
}
No cria_sentinela(void) {
   No no = malloc(SIZE_NODE);
   if (no != NULL) {
      no->dado = cria_dado_branco();
      no->proximo = NULL;
      no->anterior = NULL;
      no->e_sentinela = true;
      return no;
   }
   return NULL;
}
// destrói o tipo de 'dado' alocado acima.
void destroi_no(No no) {
   if ( no != NULL ) {
      destroi_dado(no->dado);
      no->proximo = NULL;
      no->anterior = NULL;
      free(no);
      no = NULL;
   }
}

#include <stdint.h>
/* Basico na sua modelagem: o total de 
 * itens líquido(inserções menos remoções),
 * a referência ao começo da 'lista', e 
 * uma referência ao seu fim. */
struct deque_ligada {
   // contador de itens.
   uint64_t qtd;

   /* sentinelas, não armazenam qualquer dados, apenas
    * ajudam na adição e remoção. */
   No comeco;
   No traseira;
};
// já dando um nome novo para ela.
typedef struct deque_ligada* DequeLigada;
// tamanho deste tipo de estrutura.
#define DL_SIZE sizeof(struct deque_ligada)

/* Aloca uma instancia ao objeto, inicia 
 * o 'total líquido de itens' em zero,
 * e as referências as suas pontas para
 * nada. */
DequeLigada cria_dl() {
   size_t tamanho = sizeof(DL_SIZE);
   DequeLigada l = (DequeLigada) malloc(tamanho);

   // criando ambos sentinelas.
   l->comeco = cria_sentinela();
   l->traseira = cria_sentinela();

   // conecta os sentinelas ...
   l->comeco->proximo = l->traseira;
   l->traseira->anterior = l->comeco;

   l->qtd = 0;
   return l;
}

/* retorna o contador líquido de itens. */
uint64_t tamanho_dl(DequeLigada l) 
   { return l->qtd; }

#include <stdbool.h>

// se a contabilização está em zero, ela está vázia.
bool vazia_dl(DequeLigada l) 
   { return l->qtd == 0; }
/* verifica se a 'lista' passada é válida, ou seja
 * se ela existe. */
bool deque_valida(DequeLigada d)
   { return d != NULL; }

No insere_entre(No esquerdo, No direito, Dado d) {
   if (esquerdo == NULL || direito == NULL || d == NULL) 
      { perror("um dos 'nós'/ou o 'dado' está inválido!"); }

   No novo_no = cria_no(d);
   if (novo_no == NULL)
      { perror("falha ao criar um novo 'nó'."); }
      
   /* o novo 'nó' aponta tanto para o 'nó esquerdo', quanto para
    * o 'nó direito'. */
   novo_no->anterior = esquerdo;
   novo_no->proximo = direito;
   /* pontas apontam para no novo 'nó', cada um do seu referencial. */
   esquerdo->proximo = novo_no;
   direito->anterior = novo_no;

   // retorna uma referẽncia para novo 'nó' criado, e devidamente
   // referênciados pelos seus "inquilínos".
   return novo_no;
}

Dado remove_entre(No no_central) {
   // verifica se no_central é válido.
   if (no_central == NULL)
      { perror("o 'nó' não é válido."); }

   // identificam as pontas para onde aponta.
   No left = no_central->anterior;
   No right = no_central->proximo;

   // aponta um ao outro, ignorando o 'nó' central.
   left->proximo = right;
   right->anterior = left;

   // cópia do valor do 'nó', para então destruir o 'nó'.
   Dado dado_interno = clona_dado(no_central->dado);

   // retorna o 'dado' do 'nó', a pouco, destruido.
   return dado_interno;
}

/* Encapsula o 'dado' em um 'nó', faz com este
 * 'nó' ao primeiro elemento da 'lista', então
 * faz este 'nó' a nova referência ao primeiro
 * elemento, como foi desenhado o ADT. */
bool insere_comeco_dl(DequeLigada l, Dado d) {
   // falha ao alocar recurso, implica falha na inserção.
   if (!deque_valida(l))
      { return false; }

   // pontas do 'nó' central a ser inserido.
   No left = l->comeco;
   No right = l->comeco->proximo;
   
   // tal modo exclui a necessidade de casos especiais.
   // No resultado = insere_entre(left, right, d);
   insere_entre(left, right, d);

   // a contabilização do item adicionado.
   l->qtd++;
   // confirma a inserção.
   return true;
}

/* Mesmo procedimento acima, porém na outra ponta.*/
bool insere_traseira_dl(DequeLigada l, Dado d) {
   if (!deque_valida(l))
      { return false; }

   No right = l->traseira;
   No left = right->anterior;
   // No resultado = insere_entre(left, right, d);
   insere_entre(left, right, d);

   l->qtd++;
   return true;
}

/* Impressão detalhada da 'lista', onde ambas pontas
 * que são operadas nela estão bem referênciadas,
 * para uma diferenciação dos demais. A lista também
 * tem um nome inicial. Basicamente o que acontece
 * aqui é atrevessar a lista, via referência, e 
 * imprimir tais valores de forma organizada. */
void imprime_dl(DequeLigada l) {
   No atual = l->comeco->proximo;
   printf("Deque Ligada: [");

   /* imprime 'atual' item referênciado, e aponta 
    * em seguido para o que ele aponta. Entretanto,
    * só imprime ele se for válido. */
   while (!atual->e_sentinela) {
      printf("%s, ", dado_para_string(atual->dado));
      atual = atual->proximo;
   }

   // não precisa de recuo para uma lista vázia.
   if (vazia_dl(l))
      puts("]");
   else
      puts("\b\b]");
}

/* O processo é pegar a referência da ponta
 * que está sendo operada, e fazer seu 'nó'
 * referênciar o 'próximo' dele, enquanto 
 * mantém com um ponteiro auxiliar a referência
 * deste para futuro retorno. */
Dado remove_comeco_dl(DequeLigada l) {
   if (vazia_dl(l)) 
      { perror("não é possível remover uma 'lista' vázia."); }
   else if (!deque_valida(l))
      { perror("a 'lista' está inválida."); }

   // descontando 'item' a ser removido...
   l->qtd--;

   /* retira o 'nó' que está entre o 'sentinela esquerdo', e
    * o elemento depois do primeiro.
    */
   return remove_entre(l->comeco->proximo);
}

// mesmo que o acima, porém na ponta direita.
Dado remove_traseira_dl(DequeLigada l) {
   if (vazia_dl(l)) 
      { perror("não é possível remover uma 'lista' vázia."); }
   else if (!deque_valida(l))
      { perror("a 'lista' está inválida."); }

   l->qtd--;
   return remove_entre(l->traseira->anterior);
}

/* libera os 'dados' alocados do objeto, removendo
 * em alguma ponta continuamente até o total
 * chegue à zero. O objeto obtido é simplesmente
 * destruído também. */
void destroi_dl(DequeLigada l) {
   if (l != NULL) {
      while (!vazia_dl(l)) 
         { remove_comeco_dl(l); }

      // remove também os 'nós' sentinelas.
      free(l);
      l = NULL;
   }
}

/* obtem referência do primeiro item da 'lista',
 * o objeto da ponta esquerda. Se houver algum
 * é claro, pois se não, um 'null' será passado. */
Dado frente_dl(DequeLigada l) {
   // a lista tem que ter algo.
   if (vazia_dl(l)) 
      { perror("não é possível remover uma 'lista' vázia."); }
   else if (!deque_valida(l))
      { perror("a 'lista' está inválida."); }

   // primeiro elemento, depois do sentinela:
   No primeiro = l->comeco->proximo;

   // entrega a referência do dado, nada de cópia.
   return primeiro->dado;
}

/* o mesmo que acima, porém na ponta esquerda dela. */
Dado traseira_dl(DequeLigada l) { 
   if (vazia_dl(l)) 
      { perror("não é possível remover uma 'lista' vázia."); }
   else if (!deque_valida(l))
      { perror("a 'lista' está inválida."); }

   // aqui é o penúltimo elemento, antes do 'sentinela traseiro'.
   No ultimo = l->traseira->anterior;
   return ultimo->dado;
}

/* Faz uma busca de modo 'sentinela' na 'lista', por um item. Uma 
 * operação que é realizada em tempo linear portanto O(n). */
bool contem_dl(DequeLigada d, Dado e) {
   // se estiver vázia, não tem qualquer travesia a aconter.
   if (vazia_dl(d))
      return false;

   // item depois do 'sentinela esquerdo':
   No atual = d->comeco->proximo;

   /* percorre até encontrar o sentinela da outra ponta da lista. */
   while (!atual->e_sentinela ) {
      // compara os dados internos de ambos 'nós'.
      if(dados_iguais(e, atual->dado))
         return true;

      // vai ao próximo.
      atual = atual->proximo;
   }

   // se chegou até aqui, é pois, não encontrou nada.
   return false;
}

// remove um 'dado', se este estiver contido nela.
bool remove_dl(DequeLigada d, Dado e) {
   // não pode remove lista vázias.
   if (vazia_dl(d))
      return false;

   // começa do primeiro 'item'.
   No no_atual = d->comeco->proximo;

   while (!no_atual->e_sentinela ) {
      /* se achar tal dado em algum 'nó', remove ele assim como
       * a remoção interna das pontas. */
      if(dados_iguais(e, no_atual->dado)) {
         // desconta item removido.
         d->qtd--;
         remove_entre(no_atual);
         // confirma a remoção.
         return true;
      }

      // vai ao próximo.
      no_atual = no_atual->proximo;
   }

   // nada foi encontrado, portanto negar remoções.
   return false;
}

#define SIZE_ARRAY_DL sizeof(DequeLigada)
typedef DequeLigada* ARRAY_DL;

// cria uma array de 'n' tamanho com deques-ligadas.
ARRAY_DL cria_array_dl(uint32_t n) {
   ARRAY_DL array = (ARRAY_DL)calloc(n, SIZE_ARRAY_DL);

   for(uint32_t i = 0; i < n; i++)
      array[i] = cria_dl();

   return array;
}

// destrói uma array-de-deque-ligadas de 'n' tamanho.
void destroi_array_dl(ARRAY_DL array, uint32_t n) {
   for(uint32_t i = 0; i < n; i++)
      destroi_dl(array[i]);
   free(array);
}

/* o mesmo que a impressão acima, porem como debug, ou seja, algumas
 * features podem ser acionadas ou desativas, de acordo como o desejado. 
 */
void destroi_dl_debug(DequeLigada d, bool com_info) {
   uint64_t t = d->qtd;

   while (!vazia_dl(d)) {
      remove_comeco_dl(d);
      if (t < 25 && com_info) 
        { imprime_dl(d); }
   }

   free(d);
   if (com_info)
      printf("todos %lu itens foram destruídos\n", t);
}

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

uint64_t iter_tamanho_dl(IterDequeLigada iter)
   { return iter->total - iter->indice; }

void destroi_iter_dl(IterDL iter) {
   // acando linque de referência para estrutura de dado.
   iter->referencia = NULL;
   // assim também como para o 'nó'.
   iter->atual = NULL;
   // então libera a estrutura.
   free(iter);
}

typedef DequeLigada DL;
/* Clona uma determinada deque. O processo disso é simplesmente
 * remover todos dados como uma 'fila', clonando cada dado, e 
 * inserindo este clone na nova 'deque'.
 */
DequeLigada clona_dl(DL l){
   DL nova = cria_dl();
   uint64_t total = tamanho_dl(l);

   while (total > 0) {
      // remove o primeiro, dos restantes, na frente da deque.
      Dado remocao = remove_comeco_dl(l);
      // faz uma cópia deste tipo de dado.
      Dado copia = clona_dado(remocao);
      /* colocam ambos na traseira de ambas listas, o removido de 
       * onde ele foi retirado, já a cópia na nova "lista clone".
       */
      insere_traseira_dl(l, remocao);
      insere_traseira_dl(nova, copia);
      // isso será feito o total de vezes da quantia de itens na
      // lista a ser clonada.
      total--;
   }

   return nova;
}

ARRAY_DL clona_array_dl(ARRAY_DL array, uint32_t n) {
   ARRAY_DL nova = calloc(n, SIZE_ARRAY_DL);

   for(uint32_t p = 0; p < n; p++) {
      DL atual = array[p];
      DL copia = clona_dl(atual);
      nova[p] = copia;
   }

   return nova;
}

#include <string.h>

char* dl_to_str(DequeLigada l) {
   // computando o tamanho da string para alocar.
   IterDL i = cria_iter_dl(l);
   uint8_t maior_str = 0;
   char* resultado_fmt;

   for (Dado e = next_dl(i); e != NULL; e = next_dl(i)) {
      char* dado_str = dado_para_string(e);
      size_t t = strlen(dado_str);

      // obtendo maior string.
      if (strlen(dado_str) > maior_str)
         maior_str = t;

      // não é preciso mais esta string.
      free(dado_str);
   }
   destroi_iter_dl(i);

   /* tamanho da string é a maior string de 'dado', mais dois caractéres
    * sendo o separador e um espaço, multiplicado pela quantia total
    * de elementos da lista, somado mais quatro caractéres, sendo dois
    * para os colchetes, e outros dois para o recuo de caractéres ou 
    * um buffer. */
   size_t x = sizeof(char); // tamanho de um caractére.
   uint64_t t = (maior_str + 2 * x) * tamanho_dl(l) + 4 * x;
   bool houve_iteracao = false;
   resultado_fmt = (char*)calloc(t, x);

   // gerando iterador novamente...
   i = cria_iter_dl(l);
   // cholchete limitador dos itens.
   resultado_fmt[0] = '[';
   for (Dado e = next_dl(i); e != NULL; e = next_dl(i)) {
      char* dadostr = dado_para_string(e);
      // separador e espaço do outro item, e o 'dado' em string em sí.
      strcat(resultado_fmt, dadostr);
      strcat(resultado_fmt, ", ");
      // removendo string alocada dinâmicamente depois de copiada.
      free(dadostr);
      // informa que este bloco interior foi executado no mínimo uma vez.
      houve_iteracao = true;
   }
   // destruindo iterador consumido novamente...
   destroi_iter_dl(i);

   // delimite os itens pelo outro lado.
   if (houve_iteracao)
      // neste caso elimina espaço e vírgula restante.
      strcat(resultado_fmt, "\b\b]");
   else
      strcat(resultado_fmt, "]");

   // retorna toda string formada.
   return resultado_fmt;
}

void visualizacao_array_dl(ARRAY_DL array, uint64_t n) {
   puts("Array de Deque Ligada: [");
   for(uint64_t p = 0; p < n; p++) {
      char* dlstr = dl_to_str(array[p]);
      printf("\b\b\b\t%s\n", dlstr);
   }
   puts("]");
}

