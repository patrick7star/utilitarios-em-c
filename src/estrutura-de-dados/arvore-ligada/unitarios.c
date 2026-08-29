// Biblioteca padrão do C:
#include <assert.h>
#include <locale.h>
// Bibliotecas locais do projeto:
#include "dados-testes.h"
#include "primitivos.h"
#include "teste.h"

typedef const char** CharList;
typedef const int CInt;

static void preenche_folha(Tree, Cursor*, GenT, GenT);
static void insercao_seriada_simetrica(Tree, Cursor*, CharList, int, CInt);
static void insere_nomes_na_arvore_vazia(Tree);
static void impressao_por_preorder_traversal(Tree, Cursor, int*);
static void insercao_e_impressao_de_arvore_simetrica_simples(Tree);
static void insercao_manual_de_arvore(Tree a);
static void tabela_de_conteudo(Tree, Cursor, int depth);

UNIT_TEST ramificacao_da_arvore_binaria_visualmente(void)
{
   Tree a = tree_cria("Taylor");
   Cursor r = tree_raiz(a);

   insercao_manual_de_arvore(a);
   r = tree_raiz(a);
   tabela_de_conteudo(a, r, 0);
   tree_destroi(a);
}

UNIT_TEST trabalho_no_metodo_de_destruicao(void)
{
   Tree a = tree_cria("Taylor");
   Cursor r = tree_raiz(a);
   // Stack para empilhar folhas iteradas.
   Cursor S[2] = { r, r };

   insercao_seriada_simetrica(a, S, boys_names, 0, BOYS_NAMES);
   tree_impressao_preorder(a, debug_string);
   tree_destroi(a);
}

UNIT_TEST insercao_simetrica_seriada_na_arvore(void)
{
   Tree a = tree_cria_i();
   Cursor r = tree_adiciona_raiz(a, "Kate");
   // Stack para empilhar folhas iteradas.
   Cursor S[2] = { r, r };

   printf("Quantia(antes): %zu\n", tree_quantidade(a));
   insercao_seriada_simetrica(a, S, girls_names, 0, GIRLS_NAMES);
   tree_impressao_preorder(a, debug_string);
   printf("Quantia(depois): %zu\n", tree_quantidade(a));
   tree_destroi(a);
}

UNIT_TEST preorder_traversal_algoritmo(void)
{
   Tree a = tree_cria("nada");   
   int cursor = 0;

   insere_nomes_na_arvore_vazia(a);
   impressao_por_preorder_traversal
     (a, tree_raiz(a), &cursor);
   printf("cursor: %d\n", cursor);
   tree_destroi(a);
}

UNIT_TEST percorrimento_manual_de_uma_arvore_simetrica_pequena(void) {
   Tree a = tree_cria_i();

   insercao_e_impressao_de_arvore_simetrica_simples(a);
   tree_destroi(a);
}

UNIT_TEST instancia_insere_depois_conta(void)
{
   puts("Série de testas na árvore binária!");
   Tree arvore = tree_cria_i();
   size_t quantia = tree_quantidade(arvore);

   printf("Número de folhas(antes): %zu\n", quantia);
   insere_nomes_na_arvore_vazia(arvore);
   quantia = tree_quantidade(arvore);
   printf("Número de folhas(depois): %zu\n", quantia);
   tree_destroi(arvore);
}

static void preenche_folha(Tree a, Cursor* S, GenT x, GenT y)
{
   S[0] = tree_adiciona_direita(a, S[0], x); 
   S[1] = tree_adiciona_esquerda(a, S[1], y); 
}

static void insercao_seriada_simetrica
  (Tree a, Cursor* S, const char** samples, int q, const int TOTAL)
{
   bool nao_atingiu_o_total = tree_quantidade(a) < TOTAL;
   bool nao_causa_overflow = (q + 1) < TOTAL;
   GenT X, Y;

   if (nao_atingiu_o_total && nao_causa_overflow)
   {
      X = (GenT)samples[q + 0];
      Y = (GenT)samples[q + 1];

      preenche_folha(a, S, X, Y); 
      insercao_seriada_simetrica
         (a, S, samples, q + 1, TOTAL);
   }
}

static void debug_cursor_char(Cursor e)
   { printf("[%p] '%s'\n", e.nodulo, (char*)cursor_elemento(e)); }

static void impressao_por_preorder_traversal(Tree a, Cursor p, int* q)
{
   if (cursor_e_nulo(p)) return;

   (*q) += 1;
   debug_cursor_char(p);
   impressao_por_preorder_traversal(a, tree_esquerda(a, p), q);
   impressao_por_preorder_traversal(a, tree_direita(a, p), q);
}

static void insercao_e_impressao_de_arvore_simetrica_simples(Tree a)
{
   Cursor p = tree_adiciona_raiz(a, "chocolate");
   Cursor q = tree_adiciona_esquerda(a, p, "sorvete");
   Cursor r = tree_adiciona_direita(a, p, "guloseimas");

   tree_adiciona_direita(a, q, "cobertura");
   tree_adiciona_esquerda(a, q, "granulado");

   tree_adiciona_direita(a, r, "gelatinosas");
   tree_adiciona_esquerda(a, r, "iorgute");

   printf("Quantidade: %zu\n", tree_quantidade(a));

   char* dados[] = {
      a->raiz->dado,
      a->raiz->direito->dado,
      a->raiz->direito->esquerdo->dado,
      a->raiz->direito->direito->dado,
      a->raiz->esquerdo->dado,
      a->raiz->esquerdo->esquerdo->dado,
      a->raiz->esquerdo->direito->dado,
   };

   printf(
      "Raíz(%s)\n\t|__ %s\n\t\t|__ %s\n\t\t|__ %s\n\t|__ %s"
      "\n\t\t|__ %s\n\t\t|__ %s\n",
      dados[0], dados[1], dados[2], dados[3], dados[4], dados[5], dados[6]
   );
}

static void insercao_manual_de_arvore(Tree a)
{
   Cursor p = tree_adiciona_raiz(a, "chocolate");
   Cursor q = tree_adiciona_esquerda(a, p, "sorvete");
   Cursor r = tree_adiciona_direita(a, p, "guloseimas");
   Cursor s, t;

   s = tree_adiciona_direita(a, q, "cobertura");
   tree_adiciona_esquerda(a, q, "granulado");

   t = tree_adiciona_direita(a, r, "gelatinas");
   tree_adiciona_esquerda(a, r, "iorgute");

   tree_adiciona_direita(a, s, "morango");
   tree_adiciona_esquerda(a, s, "creme");

   p = tree_adiciona_direita(a, t, "jujubas");
   q = tree_adiciona_esquerda(a, t, "cereja");

   tree_adiciona_direita(a, p, "chiclete");
   tree_adiciona_esquerda(a, p, "hortelã");

   tree_adiciona_direita(a, q, "uva");
   tree_adiciona_esquerda(a, q, "napolitano");
}

static void insere_nomes_na_arvore_vazia(Tree obj)
{
   char* nome = (char*)girls_names[0], letra;
   Cursor p = tree_adiciona_raiz(obj, nome);

   for (int i = 1; i < GIRLS_NAMES; i++)
   {
      nome = (char*)girls_names[i];
      letra = nome[0];

      if (letra % 2 == 1)
         p = tree_adiciona_direita(obj, p, nome);
      else
         p = tree_adiciona_esquerda(obj, p, nome);
   }
}

static void cria_recuo(char* In, int n)
{
   char* Out = In, BRANCO = ' ';
   int j = 0;

   memset(Out, 0x00, 100);
   for (j = 0; j < 2 * n; j++)
      Out[j] = BRANCO;
}

static void tabela_de_conteudo(Tree a, Cursor p, int depth)
{
   char buffer[100];
   char* dado = cursor_elemento(p);
   Cursor left, right;

   if (cursor_e_nulo(p)) return;

   left = tree_esquerda(a, p);
   right = tree_direita(a, p);
   cria_recuo(buffer, depth);

   if (depth == 0)
      printf("%s(raíz)\n", dado); 
   else
      printf("%s|__ %s\n", buffer, dado); 

   tabela_de_conteudo(a, left, depth + 1);
   tabela_de_conteudo(a, right, depth + 1);
}

