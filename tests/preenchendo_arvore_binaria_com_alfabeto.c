// Bibliotec padrão do C:
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
// Da própria biblioteca:
#include "definicoes.h"
#include "macros.h"
#include "aleatorio.h"
#include "conjunto_ref.h"
#include "memoria.h"
#include "arvoreligada.h"
#include "dados-testes.h"
// Auxiliares:
#include "reveste-arvore.h"
#include "captura-stdout.h"
#include "matrix.h"
// API do Linux:
#include <fcntl.h>
#include <unistd.h>


static Set computa_alfabeto(void);
static void despeja_conjunto_na_arvore(Set, Tree);
static Set conjunto_variado_de_strings(void);
static Set converte_chararra_to_set(const char*);
static Set from_rawstring_to_set(const char*);

void despeja_qualquer_palavra(const char* palavra)
{
   Set input = from_rawstring_to_set(palavra);
   Tree output = tree_cria_i();
   char* formatacao = NULL;

   printf("Palavra: '%s'\n", palavra);
   despeja_conjunto_na_arvore(input, output);
   drop_set(input);
   tree_destroi(output);
   formatacao = desenha_arvore();
   puts(formatacao);
}

void despeja_alfabeto_na_arvore(void)
{
   Set input = computa_alfabeto();
   Tree output = tree_cria_i();
   char* formatacao = NULL;

   despeja_conjunto_na_arvore(input, output);
   drop_set(input);
   tree_destroi(output);
   formatacao = desenha_arvore();
   puts(formatacao);
}

void despeja_strings_na_arvore(void)
{
   Set input = conjunto_variado_de_strings();
   Tree output = tree_cria_i();
   char* formatacao = NULL;

   despeja_conjunto_na_arvore(input, output);
   printf("Árvore tem %zu elementos.\n", tree_quantidade(output));
   drop_set(input);
   tree_destroi(output);
   formatacao = desenha_arvore();
   printf("\n\nTrasnformação ... resultado:\n'''\n%s\n'''\n", formatacao);
}

void arvores_das_palavras(void)
{
   char* inputs[] = {
      (char*)girls_names[GIRLS_NAMES / 2 - 4],
      (char*)objects[OBJECTS / 3],
      (char*)legumes[LEGUMES / 2 - 2]
   };

   for (int n = 0; n < 3; n++)
   {
      despeja_qualquer_palavra(inputs[n]);
      matriz_print_debug();
   }
}

int main(int qtd, char* args[], char* envs[])
{
   arvores_das_palavras();
   // despeja_alfabeto_na_arvore();
   // despeja_strings_na_arvore();

   return EXIT_SUCCESS;
}

static void imprime_recuo(const int N)
{
   for (int m = 0; m < N; m++)
      putchar(recuo_usado());
}

static void impressao_de_galho_string(Tree a, Cursor p, GenT data)
{
   char* dado = data;

   if (cursor_eq(tree_raiz(a), p))
      printf("%s(raíz)\n", dado);
   else
      printf("|__ %s\n", dado);
}

static void impressao_de_galho_char(Tree a, Cursor p, GenT data)
{
   char* dado = data;

   if (cursor_eq(tree_raiz(a), p))
      printf("[raíz]%c\n", *dado);
   else
      printf("|__ %c\n", *dado);
}

static void preorder_recursao(Tree a, Cursor p, int d)
{
   Cursor r, l;

   if (cursor_e_nulo(p))
      return;

   imprime_recuo(3 * d);
   r = tree_direita(a, p);
   l = tree_esquerda(a, p);

   impressao_de_galho_string(a, p, cursor_elemento(p));
   preorder_recursao(a, l, d + 1);
   preorder_recursao(a, r, d + 1);
}

static void visualiza_arvore(Tree a)
   { preorder_recursao(a, tree_raiz(a), 0); }

Set computa_alfabeto(void)
{
   Set output = new_set(hash_char, eq_char);
   const int TOTAL = 2 * 26;
   const int NUM_SIMULACOES = 500;
   char Y, *X = NULL;

   for (int i = 0; i < NUM_SIMULACOES; i++)
   {
      if (length_set(output) == 26)
         break;

      Y = alfabeto_aleatorio();
      X = box_char(Y);
      add_set(output, X);
   }
   return output;
}

void algoritmo_de_despejo(Set input, Tree output)
{
   const int N = length_set(input);
   Cursor adicoes[N];
   GenT X = NULL, Y = NULL;
   Tree out = output;
   int quantia = 0xdeadbeef,
       c = 0x00000001,
       r = 0x00000000;

   X = deleta_set(input);
   adicoes[0] = tree_adiciona_raiz(out, X);

   // Adiciona os dois primeiros elementos no único nó da árvore. Uma em 
   // cada lado.
   X = deleta_set(input);
   Y = deleta_set(input);
   adicoes[c + 0] = tree_adiciona_direita(out, adicoes[r], Y);
   adicoes[c + 1] = tree_adiciona_esquerda(out, adicoes[r], X);
   c += 2;
   r++;

   // Repete o último passo acima continuamente até que os pares se esgotem.
   while (length_set(input) >= 2)
   {
      X = deleta_set(input);
      Y = deleta_set(input);
      adicoes[c + 0] = tree_adiciona_direita(out, adicoes[r], Y);
      adicoes[c + 1] = tree_adiciona_esquerda(out, adicoes[r], X);
      c += 2;
      r++;
   }

   // Verifica se restou alguma unidade de elemento à adicionar. Se sim,
   // apenas adiciona num lado aleatório dos último colocado.
   if (length_set(input) > 0)
   {
      Y = deleta_set(input);

      if (logico())
         adicoes[c] = tree_adiciona_direita(out, adicoes[r], Y);
      else
         adicoes[c] = tree_adiciona_esquerda(out, adicoes[r], Y);
   }
}

void despeja_conjunto_na_arvore(Set a, Tree b)
{ 
   puts("O que será despejado na árvore:");
   print_set(a, debug_string);
   
   algoritmo_de_despejo(a, b); 
   captura_visualizacao_da_arvore(b, visualiza_arvore);
}
static Set conjunto_variado_de_strings(void)
{
   Set output = new_set(hash_string, eq_string);
   int p = 0xabc;

   for (p = 0; p < BOYS_NAMES; p++)
      add_set(output, (char*)boys_names[p]);

   return output;
}

static Set from_rawstring_to_set(const char* input)
{
   Set output = new_set(hash_char, eq_char);
   const int TOTAL = strlen(input);
   char *X = NULL;
   int i = 0;

   for (i = 0; i < TOTAL; i++)
      add_set(output, box_char(input[i]));
   return output;
}
