/*   Implementação mais básica de uma arvore binária em C, baseando-se no
 * que já foi feito em Python. Existes algoritmos mais simples para a
 * linguagem, entretanto, este aqui é bem mais intuitivo e organizado.
 */

// Declaração das estruturas, funções e métodos, abaixo:
#include "arvoreligada.h"
// Biblioteca padrões em C:
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
// Módulos do projeto:

#define INTERROMPE(MSG) ({ perror(MSG); abort(); })

/* === === === === === === === === === === === === === === === === === === ==
 *                         Estruturas e Métodos
 *
 * Métodos e definições de partes não importantes, foram realocadas em outros
 * arquivos, então "importadas" prá cá. No caso aqui, 'estruturas.c' ficam
 * as definições, já 'cursor.c' e 'nodulo.c' os métodos referentes aos
 * respectivas definições.
 * === === === === === === === === === === === === === === === === === === */
#include "arvore-ligada/estruturas.c"
#include "arvore-ligada/cursor.c"
#include "arvore-ligada/nodulo.c"

Tree tree_cria_i(void)
{
   const int size = sizeof(binary_tree_t);
   Tree self = malloc(size);

   if (self == NULL)
      { perror("Falha ao alocar memória!"); abort(); }

   return self;
}

Tree tree_cria(GenT data)
{
   Node objeto;
   Tree self = tree_cria_i();

   objeto = cria_node(data);
   (*self).raiz = objeto;
   (*self).quantia++;
   return self;
}

static void preorder_traversal_indexacao
  (Tree a, Cursor p, Node* list, size_t* q)
{
   if (cursor_e_nulo(p) || (*q >= tree_quantidade(a)))
      return;

   list[*q] = validacao(a, p);
   *q += 1;
   preorder_traversal_indexacao(a, tree_esquerda(a, p), list, q);
   preorder_traversal_indexacao(a, tree_direita(a, p), list, q);
}

bool tree_destroi(Tree self)
{
/* Há uma travessia 'preorder', então todos nós são depositados numa lista
 * alocada exatamente para este tamanho. Depois os 'nódulos' são desalocados
 * um por um.
 */
   Tree a = self;
   size_t qtd = tree_quantidade(a), q = 0;
   const size_t QUANTIA = qtd * NodePtrSz;
   Node* todos = malloc(QUANTIA);
   Node atual = NULL;

   if (todos == NULL)
      return false;
   else
   /* Colocando todos 'nódulos' numa lista, assim fica mais fácil a
    * desalocação de cada. */
    preorder_traversal_indexacao(a, tree_raiz(a), todos, &q);

   while (q > 0)
   {
      atual = todos[q - 1];
      q--;
      destroi_node(atual);
   }
   free(self);
   free(todos);
   return true;
}

Cursor tree_raiz(Tree self)
   { return cursor_novo(self, (*self).raiz); }

Cursor tree_pai(Tree self, Cursor p)
   { return cursor_novo(self, validacao(self, p)); }

Cursor tree_direita(Tree self, Cursor p)
{
   Node nodulo = validacao(self, p);

   if (nodulo == NULL)
      return CURSOR_NULL;
   else
      return cursor_novo(self, (*nodulo).direito);
}

Cursor tree_esquerda(Tree self, Cursor p)
{
   Node nodulo = validacao(self, p);
   return cursor_novo(self, nodulo->esquerdo);
}

int tree_numero_de_criancas(Tree self, Cursor p)
{
   Node nodulo = validacao(self, p);

   if ((*nodulo).direito == NULL && (*nodulo).esquerdo == NULL)
      return 0;
   else if ((*nodulo).direito != NULL && (*nodulo).esquerdo == NULL)
      return 1;
   else if ((*nodulo).direito == NULL && (*nodulo).esquerdo != NULL)
      return 1;
   return 2;
}

Cursor tree_adiciona_raiz(Tree self, GenT data)
{
   Node objeto;

   objeto = cria_node(data);
   (*self).raiz = objeto;
   (*self).quantia = 1;

   printf("Valor inserido na raíz com sucesso.\n");
   return cursor_novo(self, objeto);
}

Cursor tree_adiciona_esquerda(Tree self, Cursor p, GenT e)
{
   Node nodulo = validacao(self, p);
   Node novo = cria_node(e);

   if ((*nodulo).esquerdo != NULL)
      { perror("Lado 'esquerdo' já existe!"); abort(); }

   (*self).quantia += 1;
   (*novo).pai = nodulo;
   (*nodulo).esquerdo = novo;

   return cursor_novo(self, novo);
}

Cursor tree_adiciona_direita(Tree self, Cursor p, GenT e)
{
   Node nodulo = validacao(self, p);
   Node novo = cria_node(e);

   if ((*nodulo).direito != NULL)
      { perror("Lado 'direito' já existe!"); abort(); }

   (*self).quantia += 1;
   (*novo).pai = nodulo;
   (*nodulo).direito = novo;

   return cursor_novo(self, novo);
}

GenT tree_remove(Tree self, Cursor p)
{
   Node nodulo = validacao(self, p);
   Node raiz = (*self).raiz;
   Node pai = NULL, crianca = NULL;
   GenT dado = NULL;

   if (tree_numero_de_criancas(self, p) == 2)
      INTERROMPE("'p' tem duas crianças!");

   if ((*nodulo).esquerdo == NULL)
      crianca = (*nodulo).direito;
   else
      crianca = (*nodulo).esquerdo;

   if (crianca != NULL)
      crianca->pai = nodulo->pai;

   if (nodulo == raiz)
      raiz = crianca;
   else {
      pai = nodulo->pai;

      if (nodulo == pai->esquerdo)
         pai->esquerdo = crianca;
      else
         pai->direito = crianca;
   }

   (*self).quantia--;
   nodulo->pai = nodulo;
   dado = nodulo->dado;
   destroi_node(nodulo);

   return dado;
}

size_t tree_quantidade(Tree self)
   { return (*self).quantia; }

bool tree_vazia(Tree self)
   { return tree_quantidade(self) == 0; }

bool tree_e_folha(Tree self, Cursor p)
   { return tree_numero_de_criancas(self, p) == 0; }

/* === === === === === === === === === === === === === === === === === === ==
 *                      Travesia e Formatação
 * === === === === === === === === === === === === === === === === === === */
static void recursao_preorder(Tree a, Cursor p, ToString fmt)
{
   Cursor esquerdo, direito;
   char* string = NULL;

   if (cursor_e_nulo(p)) return;

   esquerdo = tree_esquerda(a, p);
   direito = tree_direita(a, p);
   string = fmt(cursor_elemento(p));

   // Imprime formatação, então libera a string.
   printf("%s, ", string); free(string);
   recursao_preorder(a, esquerdo, fmt);
   recursao_preorder(a, direito, fmt);
}

void tree_imprime_preorder(Tree a, ToString fmt)
{
// Imprime a árvore dado ela e seu formatador, no percorrimento 'preorder'.
   printf("Arvore-Binária(%zu) [", tree_quantidade(a));
   recursao_preorder(a, tree_raiz(a), fmt);
   puts("\b\b]\n");
}

static void recursao_postorder(Tree a, Cursor p, ToString fmt)
{
   Cursor esquerdo, direito;
   char* string = NULL;

   if (cursor_e_nulo(p)) return;

   esquerdo = tree_esquerda(a, p);
   direito = tree_direita(a, p);
   string = fmt(cursor_elemento(p));

   recursao_postorder(a, esquerdo, fmt);
   recursao_postorder(a, direito, fmt);
   // Imprime formatação, então libera a string.
   printf("%s, ", string); free(string);
}

void tree_imprime_postorder(Tree a, ToString fmt)
{
// Imprime a árvore dado ela e seu formatador, no percorrimento 'postorder'.
   printf("Arvore-Binária(%zu) [", tree_quantidade(a));
   recursao_postorder(a, tree_raiz(a), fmt);
   puts("\b\b]\n");
}

/* === === === === === === === === === === === === === === === === === === ==
 *                   Tradução da Interface(complementar)
 * === === === === === === === === === === === === === === === === === === */
 #include "arvore-ligada/english.c"

/* === === === === === === === === === === === === === === === === === === ==
 *                      Algoritmos Relacionados
 * === === === === === === === === === === === === === === === === === === */
void despeja_array_na_arvore(Tree out,  GenT input, int size, const int N)
{
/*   Pega todos itens da array, então despeja-a na árvore binária, de forma 
 * que ela sempre fica balanceada. Os parâmetros aqui são a árvore, que é 
 * onde serão despejados. A array 'input', que é onde cada item está. O 
 * tamanho dos dados nesta array 'size', isso porque o algoritmo aceita
 * valores genéricos, e você precisa do tamanho de cada para iterar de forma
 * certa. Por fim, o comprimento do array, ou seja, o números de itens que
 * há nela.
 *   Obs.: O algoritmo precisa que ela tenha mais de três elementos.
 */
   assert(tree_empty(out));

   Cursor adicoes[N];
   GenT X = NULL, Y = NULL;
   int c = 1, p = 0, r = 0;
   uint8_t* array = input;
   bool UM_VALOR_PAR = (N % 2 == 0);

   X = (GenT)(array + p);
   adicoes[0] = tree_adiciona_raiz(out, X);
   p++;

   // Adiciona os dois primeiros elementos no único nó da árvore. Uma em 
   // cada lado.
   X = (GenT)(array + p + 0);
   Y = (GenT)(array + p + 1);
   adicoes[c + 0] = tree_add_right(out, adicoes[r], Y);
   adicoes[c + 1] = tree_add_left(out, adicoes[r], X);
   c += 2;
   p += 2;
   r++;

   // Repete o último passo acima continuamente até que os pares se esgotem.
   while (((N - 1) - p) >= 2)
   {
      X = (GenT)(array + p + 0);
      Y = (GenT)(array + p + 1);
      adicoes[c + 0] = tree_add_right(out, adicoes[r], Y);
      adicoes[c + 1] = tree_add_left(out, adicoes[r], X);
      c += 2;
      p += 2;
      r++;
   }

   // O algoritmo não funciona bem com valores pares, deixando sempre um
   // restante. Este trecho resolve isso.
   if (UM_VALOR_PAR)
   {
      Y = (GenT)(array + p + 0);
      adicoes[c + 0] = tree_add_right(out, adicoes[r], Y);
      c++; p++; r++;
   }
}

void despeja_array_na_arvore_list
  (Tree out,  GenT input, int size, const int N)
{
   assert(tree_empty(out));

   Cursor adicoes[N];
   uint8_t* X = NULL, * Y = NULL;
   int c = 1, p = 0, r = 0;
   uint8_t** array = input;
   bool UM_VALOR_PAR = (N % 2 == 0);

   X = *(array + p + 0);
   adicoes[0] = tree_adiciona_raiz(out, X);
   p++;

   // Adiciona os dois primeiros elementos no único nó da árvore. Uma em 
   // cada lado.
   X = *(array + p + 0);
   Y = *(array + p + 1);
   adicoes[c + 0] = tree_add_right(out, adicoes[r], Y);
   adicoes[c + 1] = tree_add_left(out, adicoes[r], X);
   c += 2;
   p += 2;
   r++;

   // Repete o último passo acima continuamente até que os pares se esgotem.
   while (((N - 1) - p) >= 2)
   {
      X = *(array + p + 0);
      Y = *(array + p + 1);
      adicoes[c + 0] = tree_add_right(out, adicoes[r], Y);
      adicoes[c + 1] = tree_add_left(out, adicoes[r], X);
      c += 2;
      p += 2;
      r++;
   }

   // O algoritmo não funciona bem com valores pares, deixando sempre um
   // restante. Este trecho resolve isso.
   if (UM_VALOR_PAR)
   {
      Y = *(array + p + 0);
      adicoes[c + 0] = tree_add_right(out, adicoes[r], Y);
      c++; p++; r++;
   }
}
/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
 *                      Testes Unitários
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */
#ifdef __unit_tests__
#include "arvore-ligada/unitarios.c"

int main(int total, char* args[], char* vars[])
{
   executa_testes_b(
      false, 7,
         Unit(instancia_insere_depois_conta, true),
         Unit(percorrimento_manual_de_uma_arvore_simetrica_pequena, true),
         Unit(preorder_traversal_algoritmo, true),
         Unit(insercao_simetrica_seriada_na_arvore, true),
         Unit(trabalho_no_metodo_de_destruicao, true),
         Unit(ramificacao_da_arvore_binaria_visualmente, true),
         Unit(remocao_de_elementos, true)
      );

   // Testes referentes a impressão somente.
   executa_testes_b(
      false, 2,
         Unit(preorder_traversal_algoritmo, false),
         Unit(todos_tipos_de_impressao_da_arvore, true)
      );

   // Série de testes reservadas para os algoritmos da árvore.
   executa_testes_b(
      true, 4,
         Unit(tamanho_dos_respecitivos_ponteiros, true),
         Unit(algoritmo_de_insercao_balanceada_generico, true),
         Unit(iterando_uma_array_de_raw_strings, false),
         Unit(iteracao_manual_da_list_ptrptr, false)
      );
   return EXIT_SUCCESS;
}
#endif
