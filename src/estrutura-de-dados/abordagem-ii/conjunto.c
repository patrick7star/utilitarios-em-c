

/* Este projeto é importante, pois é colocado em um somente arquivo, sem 
 * extensão de qualquer outras dependências.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/* Definição simples do tipo de dado, e algums métodos importantes 
 * subjacentes a ele. Se for necessário alterar o tipo de dado, é está 
 * parte somente que é recomendado mudar, tanto seu apelido, como alguns 
 * métodos, depedendo de como o dado está sendo projejado. */
typedef int64_t dado_t;

char* dado_to_str(dado_t x) {
   // converte ele numa string para impressão abaixo.
   char* resultado_fmt = malloc(4 * sizeof(char));
   sprintf(resultado_fmt, "%ld", x);
   return resultado_fmt;
}

bool dado_eq(dado_t x, dado_t y)
   // verfifica se dois dados são iguais.
   { return x == y; }

bool destroi_dado(dado_t a) { return true; }


/* Como a implementação do dicionário, este que é a estrutura ignitora
 * desta, será feito de modo dinâmico, ou seja, ao invés de uma array
 * para adicionar 'colisões', será feita com 'lista-ligadas', logo é 
 * preciso fazer um 'nó' para ligar tais módulos. Aqui está a estrutura
 * para isso, como os métodos que precisaram para guia-la. Claro, não todos
 * métodos de uma lista ligada, apenas os necessários nesta abstração. */
struct nodulo {
   // dado interno.
   dado_t dado;
   // alto referência(ponteiro) para o próximo 'nó'.
   struct nodulo* seta;
};

static struct nodulo* cria_nodulo(dado_t v) {
   struct nodulo* no = malloc(sizeof(struct nodulo));
   // se não for capaz de alocar, apenas interrompe o programa.
   if (no == NULL) {
      puts("[error(1)]não foi possível alocar um novo 'nódulo'!");
      abort();
   }
   no->dado = v;
   no->seta = NULL;
   return no;
}

static bool destroi_nodulo(struct nodulo* n) { 
   n->seta = NULL;
   destroi_dado(n->dado);
   free(n); 
   n = NULL;
   return true;
}

static char* nodulo_to_str(struct nodulo* n) {
   char* ptr_string = malloc(100 * sizeof(char));
   if (n->seta == NULL)
      sprintf(ptr_string, "{(%s) | ---}", dado_to_str(n->dado));
   else
      sprintf(ptr_string, "{(%s) | ###}", dado_to_str(n->dado));
   return ptr_string;
}

typedef struct hashmap {
   // quantidade de elementos e total de arrays com listas-ligadas.
   size_t capacidade;
   size_t quantidade;
   // array com referência de cada começo de lista-ligada.
   struct nodulo** array;
} conjunto_t;

size_t hash(conjunto_t* s, dado_t x) 
   // computa um número único dado o tipo de valor, e o atual valor passado.
   { return x % s->capacidade; }

conjunto_t* cria_com_capacidade_set(size_t capacidade) {
   conjunto_t* conjunto = malloc(sizeof(conjunto_t));

   // se alocou corretamente, construir seu interno.
   if (conjunto != NULL) {
      conjunto->capacidade = capacidade;
      conjunto->quantidade = 0;
      conjunto->array = calloc(capacidade, sizeof(struct nodulo*));
      // inicializando cada vagão com referência a nada.
      for (size_t p = 0; p < capacidade; p++)
	 conjunto->array[p] = NULL;
      #ifdef _DEBUG_CRIA_SET
      printf(
	 "a 'set' with capacity %zu succesufully created.\n",
	 conjunto->capacidade
      );
      #endif
   }
   return conjunto;
}

conjunto_t* cria_set() {
   size_t capacidade;
   #ifdef _DEBUG_CRIA_SET
   capacidade = 12;
   #else
   capacidade = 50;
   #endif
   // criação com valor automático decidido em codificação.
   return cria_com_capacidade_set(capacidade);
}

static size_t destroi_lista_de_nodulos(struct nodulo* inicio) {
   /* destrói todos 'nódulos' de uma lista-ligada, e contabiliza tais 
    * remoções, retornando o total no fim. */
   size_t contagem = 0;
   struct nodulo* atual = inicio;

   while (atual != NULL) {
      struct nodulo* desanexado = atual;
      atual = atual->seta;
      #ifdef _DEBUG_DESTROI_SET
      printf("destruction of %s is done.\n", nodulo_to_str(desanexado));
      #endif
      destroi_nodulo(desanexado);
      contagem++;
   }
   // qunatos foram destruídos.
   return contagem;
}

bool destroi_set(conjunto_t* s) {
   // contagem dos nódulos destruídos.
   size_t contagem = 0;

   for (size_t i = 0; i < s->capacidade; i++) {
      struct nodulo* atual = s->array[i];
      contagem += destroi_lista_de_nodulos(atual);
   }
   if (contagem != s->quantidade)
      return false;
   return true;
}

bool insere_set(conjunto_t* s, dado_t k) {
   size_t indice = hash(s, k);
   struct nodulo* novo = cria_nodulo(k);

   #ifdef _DEBUG_INSERE_SET
   // info sobre a inserção e atual estado.
   printf(
      "insertion of %s performed, there is %zu items now.\n",
      nodulo_to_str(novo), s->quantidade + 1
   );
   #endif

   // cálculo hash não apresentar ligação coma a tabela.
   if (indice >= s->capacidade)
      return false;
   /* em caso de colisão, é necessário guardar o módulo na ponta da lista
    * pois aí vai varrendo, e verificando, se já não há algum valor 
    * semelhante, para então negar a inserção. */
   if (s->array[indice] == NULL)
      s->array[indice] = novo;
   else {
      struct nodulo* cursor = s->array[indice];
      struct nodulo* anterior = NULL;

      while (cursor != NULL) {
	 /* se houver já algum 'nódulo' igual ao que se quer colocar, 
	  * então a inserção pode ser considerada um fracasso. */
	 if (dado_eq(cursor->dado, k)) {
	    #ifdef _DEBUG_INSERE_SET
	    printf("datum '%s' already exists here.\n", dado_to_str(k));
	    #endif
	    // liberando alocado, já que a chamada da função não passa
	    // daqui se o bloco 'if' foi acionado.
	    destroi_nodulo(novo);
	    return false;
	 }
	 anterior = cursor;
	 cursor = cursor->seta;
      }
      anterior->seta = novo;
   }

   s->quantidade += 1;
   // confirmando inserção.
   return true;
}

static void visualiza_lista_de_nodulos(struct nodulo* inicio) {
   /* destrói todos 'nódulos' de uma lista-ligada, e contabiliza tais 
    * remoções, retornando o total no fim. */
   size_t contagem = 0;
   struct nodulo* atual = inicio;

   if (atual == NULL) {
      printf("   ---\n");
      return;
   } else
      printf("   ");
   while (atual != NULL) {
      printf(" %s ", nodulo_to_str(atual));
      atual = atual->seta;
      contagem++;
   }
   printf("\t%zu itens\n", contagem);
}

static void visualizacao_interna_set(conjunto_t* s) {
   puts("Conjunto: [");
   for (size_t k = 0; k < s->capacidade; k++) 
      visualiza_lista_de_nodulos(s->array[k]);
   puts("]");
}

size_t tamanho_set(conjunto_t* s)
   { return s->quantidade;}

bool vazia_set(conjunto_t* s) 
   { return tamanho_set(s) == 0; }

#include <string.h>

char* set_to_str(conjunto_t* s) {
#ifdef _WIN64 // para Windows somente.
   rsize_t total = 1000;
   char* string = calloc(total,  sizeof(char));
   size_t capacidade = s->capacidade;
   struct nodulo* atual;

   // ínicio da string.
   strcat_s(string, total, "conjunto: {");
   // caso ela venha está vázia.
   if (vazia_set(s)) {
      strcat_s(string, total, "}");
      return string;
   }

   for (size_t k = 0; k < capacidade; k++) {
      atual = s->array[k];
      if (atual == NULL)
	 continue;
      else {
	 while (atual != NULL) {
	    char* dadostr = dado_to_str(atual->dado);
	    strcat_s(string, total, dadostr);
	    strcat_s(string, total, ", ");
	    atual = atual->seta;
	 }
      }
   }
   strcat_s(string, total, "\b\b}");
   return string;
#elif _POSIX_SOURCE // para linux e outros.
   char* string = calloc(1000,  sizeof(char));
   size_t capacidade = s->capacidade;
   struct nodulo* atual;

   strcat(string, "conjunto: {");
   if (vazia_set(s)) {
      strcat(string, "}");
      return string;
   }

   for (size_t k = 0; k < capacidade; k++) {
      atual = s->array[k];
      if (atual == NULL)
	 continue;
      else {
	 while (atual != NULL) {
	    char* dadostr = dado_to_str(atual->dado);
	    strcat(string, dadostr);
	    strcat(string, ", ");
	    atual = atual->seta;
	 }
      }
   }
   strcat(string, "\b\b}");
   return string;
#endif
}

// #include <assert.h>
bool deleta_set(conjunto_t* s, dado_t e) {
   // primeiro caso do 'dado' não existir, se o 'conjunto' estiver vázio.
   if (vazia_set(s))
      return false;

   // se apontar índice inválido na array, então tal dado não existe.
   size_t indice = hash(s, e);

   /* se a referência na array for 'nula', então não tem como deletar o
    * item no 'conjunto', pois ele não faz parte dele. */
   if (s->array[indice] == NULL) return false;

   if (dado_eq(s->array[indice]->dado, e)) {
      // se estiver no ínicio da lista-ligada.
      struct nodulo* aux = s->array[indice];
      s->array[indice] = s->array[indice]->seta;
      s->quantidade -= 1;
      destroi_nodulo(aux);
      #ifdef _DEBUG_DELETA_SET
      puts("this node was deleted at first of the list.");
      #endif
      return true;
   } else {
      struct nodulo* anterior = NULL;
      struct nodulo* cursor = s->array[indice];
      bool encontrou_algo = false;

      // iterando buscando por uma combinação...
      while (cursor->seta != NULL) {
	 if (dado_eq(cursor->dado, e)) { 
	    encontrou_algo = true;
	    break; 
	 }
	 anterior = cursor;
	 cursor = cursor->seta;
      }
      if (encontrou_algo) {
	 // remoção porque está em qualquer outro ponto.
	 anterior->seta = cursor->seta;
	 s->quantidade -= 1;
	 destroi_nodulo(cursor);
	 #ifdef _DEBUG_DELETA_SET
	 puts("remove item after iteration, thus it wasn't at first.");
	 #endif
	 return true;
      }
   }
   /* último é percorrer toda array do índice, e comparar com cada item
    * do caminho, se não houver encontrado nada é porque o 'dado' passado
    * não está neste 'conjunto'. */
   return false;
}

bool pertence_set(conjunto_t* s, dado_t k) {
   /* O algoritmo para verificar pertencimento é o mesmo trecho usado no
    * interno das operações básicas acima, computa o índice hash para,
    * então varre toda a lista(se houver uma) em busca do nódulo com o 
    * dado correspondente, daí então, se achar um confirmar pertencimento,
    * pois no pior caso(não havendo um dado), retorna um valor lógico 
    * falso. */
   // primeiro caso do 'dado' não existir, se o 'conjunto' estiver vázio.
   if (vazia_set(s)) return false;

   // se apontar índice inválido na array, então tal dado não existe.
   size_t indice = hash(s, k);
   struct nodulo* cursor = s->array[indice];

   // iterando buscando por uma combinação...
   while (cursor != NULL) {
      if (dado_eq(cursor->dado, k)) { return true; }
      cursor = cursor->seta;
   }
   /* último é percorrer toda array do índice, e comparar com cada item
    * do caminho, se não houver encontrado nada é porque o 'dado' passado
    * não está neste 'conjunto'. */
   return false;
}

#ifdef _UT_CONJUNTO
#include <assert.h>
#include <time.h>

void insercoes_randomicas(conjunto_t* s, size_t qtd) {
   time_t sistema_clock = time(NULL);
   srand(sistema_clock);

   for (size_t n = 1; n <= qtd; n++)
      insere_set(s, rand() % 256);
}

void operacoes_basicas_na_estrutura() {
   conjunto_t* S = cria_set();

   puts("first batch of insections:");
   insere_set(S, 22);
   insere_set(S, 2);
   insere_set(S, 89);
   visualizacao_interna_set(S);
   assert (pertence_set(S, 2));
   assert (!pertence_set(S, 7));
   insere_set(S, 13);
   insere_set(S, 7);
   assert (pertence_set(S, 7));
   visualizacao_interna_set(S);

   puts("second batch of insections:");
   insercoes_randomicas(S, 13);
   visualizacao_interna_set(S);
   
   insere_set(S, 2);
   printf("show it in the end:\n%s\n", set_to_str(S));

   size_t inicialmente = tamanho_set(S);
   assert (pertence_set(S, 89));
   assert(deleta_set(S, 7));
   assert(deleta_set(S, 89));
   assert (!pertence_set(S, 7));
   assert (!pertence_set(S, 89));
   printf("%s\n", set_to_str(S));
   assert (tamanho_set(S) + 2 == inicialmente);
   visualizacao_interna_set(S);

   // tentar remover inexistentes!
   assert(!deleta_set(S, 173));
   assert(!deleta_set(S, 89));

   assert (destroi_set(S));
}

void insercoes_randomicas_i(conjunto_t* s, size_t qtd) {
   time_t sistema_clock = time(NULL);
   srand(sistema_clock);

   for (size_t n = 1; n <= qtd; n++) {
      if (rand() % 2 == 0)
         insere_set(s, rand() % RAND_MAX);
      else 
         insere_set(s, (-1) * rand() % RAND_MAX);
   }
}

void taxa_de_insercao_e_remocao() {
   conjunto_t* S = cria_set();
   // tamanho antes de mais inserções:
   size_t inseridos = tamanho_set(S);
   // variação de inserções:
   size_t variacao = 0;
   double decorrido = 0;
   double antigo_decorrido = 0.0;

   for (size_t i = 1; i <= 9; i++) {
      inseridos = tamanho_set(S);
      // cronometrando...
      time_t inicio = time(NULL);
      insercoes_randomicas_i(S, 41000);
      time_t final = time(NULL);
      antigo_decorrido = decorrido;
      decorrido = difftime(final, inicio);

      printf(
         "%lu/41000 em %3.0fseg [%4.1f%%]\n", 
         tamanho_set(S) - inseridos, decorrido,
         (antigo_decorrido / decorrido * 100)
      );
   }

   destroi_set(S);
}

size_t _tamanho_da_lista_encadeada_aleatoria(conjunto_t* set) {
   // pausa para pegar o seed do timestamp.
   // nanosleep(&(struct timespec){ 0, 325000000 }, NULL);
   srand(time(NULL));
   size_t linha = rand() % set->capacidade;
   struct nodulo* atual = set->array[linha];
   size_t contagem = 0;

   while (atual != NULL) {
      contagem++;
      atual = atual->seta;
   }
   return contagem;
}

size_t _media_de_tamanho_da_lista(conjunto_t* set) { 
   size_t quantidade = 0, total = (set->capacidade / 2);
   for (size_t k = 1; k <= total; k++) 
      quantidade += _tamanho_da_lista_encadeada_aleatoria(set);
   return quantidade / total; 
}

void crescimento_da_lista_encadeada_interna() {
   conjunto_t* S = cria_set();
   size_t q = tamanho_set(S);

   for (size_t i = 1; i <= 19; i++) {
      insercoes_randomicas_i(S, 24000);
      size_t T = _media_de_tamanho_da_lista(S);
      printf(
         "relação 'total de itens' e 'comprimento da lista interna'" 
         "\b\b\b: %7lu/%5lu\n", tamanho_set(S), T
      );
      q = T;
   }

   destroi_set(S);
}

int main(int total, char* args[], char* vars[]) {
   // operacoes_basicas_na_estrutura();
   // testes abaixo consumem imensa quantidade de tempo e CPU.
   // taxa_de_insercao_e_remocao();
   crescimento_da_lista_encadeada_interna();
   return EXIT_SUCCESS;
}
#endif
