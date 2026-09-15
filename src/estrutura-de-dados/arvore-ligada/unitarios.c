// Biblioteca padrão do C:
#include <assert.h>
#include <locale.h>
// Bibliotecas locais do projeto:
#include "dados-testes.h"
#include "primitivos.h"
#include "teste.h"
#include "aleatorio.h"

typedef const char** CharList;
typedef const int ConstInt, CInt;

static void insercao_seriada_simetrica(Tree, CharList, CInt);
static void insere_nomes_na_arvore_vazia(Tree);
static void impressao_por_preorder_traversal(Tree, Cursor, int*);
static void insercao_e_impressao_de_arvore_simetrica_simples(Tree);
static void insercao_manual_de_arvore(Tree a);
static void tabela_de_conteudo(Tree, Cursor, int depth);
static Cursor captura_primeira_folha_preorder(Tree);

UNIT_TEST algoritmo_de_insercao_balanceada_generico(void)
{
   char* genero = (char*)sexo;
   const int GENERO = SEXO;
   void (*algoritmo)(Tree, GenT, int, int);
   void (*algoritmo2)(Tree, GenT, int, int);
   bool boolarray[] = {false, false, false, true, true, true};
   const int BASZ = sizeof(boolarray) / sizeof(bool);

   algoritmo = despeja_array_na_arvore;
   algoritmo2 = despeja_array_na_arvore_list;
   Tree a = tree_new_i();
   Tree b = tree_new_i();
   Tree c = tree_new_i(); 
   Tree d = tree_new_i();

   algoritmo2(a, legumes, sizeof(char*), LEGUMES);
   tree_imprime_postorder(a, debug_string);

   algoritmo(b, (GenT)valores_padronizados, sizeof(int), VALORES_PADRONIZADOS);
   tree_imprime_postorder(b, debug_u8);

   algoritmo(c, genero, sizeof(char), GENERO);
   tree_imprime_postorder(c, debug_char);

   printf("BoolArray: %d\n", BASZ);
   algoritmo(d, boolarray, sizeof(bool), BASZ);
   tree_imprime_postorder(d, debug_bool);

   tree_destroi(b);
   tree_destroi(a);
   tree_destroi(c);
   tree_destroi(d);
}

UNIT_TEST tamanho_dos_respecitivos_ponteiros(void)
{
   printf("(char*) %ld bytes.\n", sizeof(char*));
   printf("(uint8_t*) %ld bytes.\n", sizeof(uint8_t*));
   printf("(void*) %ld bytes.\n", sizeof(void*));
   printf("(int*) %ld bytes.\n", sizeof(int*));
   printf("(double*) %ld bytes.\n", sizeof(double*));
   printf("(bool*) %ld bytes.\n", sizeof(bool*));
}

UNIT_TEST iterando_uma_array_de_raw_strings(void)
{
   int n = 0xbc; void* address = NULL;
   char* string = NULL;
   char** list = (char**)legumes; 

   address = list;
   printf("Endereço(legumes): %p | %p\n", address, *list);

   for (n = 0; n < LEGUMES; n++)
   {
      address = (void*)(list + n);
      string = *(list + n);

      printf("\t[%p] %s\n", address, string);
   }
}

UNIT_TEST iteracao_manual_da_list_ptrptr(void)
{
   for (int n = 0; n < LEGUMES; n++)
      printf("\t- %s\n", *(legumes + n));
}

UNIT_TEST todos_tipos_de_impressao_da_arvore(void)
{
   Tree a = tree_cria("nada");   
   CharList list = girls_names;
   const int TOTAL = sizeof(girls_names) / sizeof(char*);
   const int N = TOTAL / 4;

   insercao_seriada_simetrica(a, list, N);
   tabela_de_conteudo(a, tree_root(a), 0);
   puts("\nImpressão em PostOrder:");
   tree_imprime_postorder(a, debug_string);
   puts("\nImpressão em PreOrder:");
   tree_imprime_preorder(a, debug_string);
   tree_destroi(a);
}

UNIT_TEST remocao_de_elementos(void)
{
   Tree a = tree_new("Taylor");
   Cursor r = tree_root(a), X;
   char* remocao = NULL;
   int n = 0xabc, length;

   insercao_manual_de_arvore(a);
   length = tree_size(a);
   printf("Quantia: %d\n", length);

   r = tree_root(a);
   tabela_de_conteudo(a, r, 0);
   X = captura_primeira_folha_preorder(a);
   remocao = tree_remove(a, X);
   length = tree_size(a);

   printf("Quantia: %d\n", length);
   printf("O que foi removido: '%s'\n", remocao);
   tabela_de_conteudo(a, tree_root(a), 0);

   for (n = 1; n <= 7; n++)
   {
      X = captura_primeira_folha_preorder(a);
      remocao = tree_remove(a, X);
      printf("Quantia: %zu\n", tree_size(a));
      printf("O que foi removido: '%s'\n", remocao);
      r = tree_raiz(a);
   }
   tabela_de_conteudo(a, tree_root(a), 0);
   tree_drop(a);
}

UNIT_TEST ramificacao_da_arvore_binaria_visualmente(void)
{
   Tree a = tree_new("Taylor");
   Cursor r = tree_root(a);

   insercao_manual_de_arvore(a);
   r = tree_root(a);
   tabela_de_conteudo(a, r, 0);
   tree_drop(a);
}

UNIT_TEST trabalho_no_metodo_de_destruicao(void)
{
   Tree a = tree_cria("Taylor");

   insercao_seriada_simetrica(a, boys_names, BOYS_NAMES);
   tree_imprime_preorder(a, debug_string);
   tree_destroi(a);
}

UNIT_TEST insercao_simetrica_seriada_na_arvore(void)
{
   Tree a = tree_cria_i();
   
   tree_adiciona_raiz(a, "Kate");
   printf("Quantia(antes): %zu\n", tree_quantidade(a));
   insercao_seriada_simetrica(a, girls_names, GIRLS_NAMES);
   tree_imprime_preorder(a, debug_string);
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

static void realiza_travessia_preorder
  (Tree a, Cursor p, Cursor* out)
{
// Realiza uma travessia preorder da árvore até que a primeira folha seja
// encontrada. Atigindo isso, o cursor é salvo, e a recursão para.
   Cursor r, l;

   if (cursor_e_nulo(p)) { return; }
   if (tree_e_folha(a, p)) { *out = p; return; }

   r = tree_direita(a, p);
   l = tree_esquerda(a, p);

   realiza_travessia_preorder(a, r, out);
   realiza_travessia_preorder(a, l, out);
}

static Cursor captura_primeira_folha_preorder(Tree obj)
{
   Cursor root = tree_raiz(obj), out;
   char* data = NULL;

   realiza_travessia_preorder(obj, root, &out);
   data = cursor_elemento(out);
   printf("Obtido: '%s'\n", data);
   return out;
}

static void insercao_seriada_simetrica
  (Tree out,  const char** input, const int N)
{
   Cursor adicoes[N];
   GenT X = NULL, Y = NULL;
   int c = 0x00000001,
       p = 0x00000000,
       r = 0x00000000;

   X = (GenT)input[p];
   adicoes[0] = tree_adiciona_raiz(out, X);
   p++;

   // Adiciona os dois primeiros elementos no único nó da árvore. Uma em 
   // cada lado.
   X = (GenT)input[p + 0];
   Y = (GenT)input[p + 1];
   adicoes[c + 0] = tree_adiciona_direita(out, adicoes[r], Y);
   adicoes[c + 1] = tree_adiciona_esquerda(out, adicoes[r], X);
   c += 2;
   p += 2;
   r++;

   // Repete o último passo acima continuamente até que os pares se esgotem.
   while (p < (N - 2))
   {
      X = (GenT)input[p + 0];
      Y = (GenT)input[p + 1];
      adicoes[c + 0] = tree_adiciona_direita(out, adicoes[r], Y);
      adicoes[c + 1] = tree_adiciona_esquerda(out, adicoes[r], X);
      c += 2;
      p += 2;
      r++;
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
      printf("[%s](raíz)\n", dado); 
   else
      printf("%s|__ %s\n", buffer, dado); 

   tabela_de_conteudo(a, left, depth + 1);
   tabela_de_conteudo(a, right, depth + 1);
}

