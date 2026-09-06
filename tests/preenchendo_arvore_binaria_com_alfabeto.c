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
// API do Linux:
#include <fcntl.h>
#include <unistd.h>


static Set computa_alfabeto(void);
static void despeja_conjunto_na_arvore(Set, Tree);
static void visualiza_arvore(Tree a);
static Set conjunto_variado_de_strings(void);

int main(int qtd, char* args[], char* envs[])
{
   // Set input = computa_alfabeto();
   Set input = conjunto_variado_de_strings();
   Tree output = tree_cria_i();
   char* formatacao = NULL;

   despeja_conjunto_na_arvore(input, output);
   printf("Árvore tem %zu elementos.\n", tree_quantidade(output));
   visualiza_arvore(output);
   drop_set(input);
   tree_destroi(output);
   formatacao = desenha_arvore();
   printf("\n\nTrasnformação ... resultado:\n'''\n%s\n'''\n", formatacao);

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
   // impressao_de_galho_char(a, p, cursor_elemento(p));
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

void adicao_recursiva_de_mais_duas_folhas
  (Set input, Tree out, Cursor* array, int cursor)
{
   const int N = length_set(input);
   GenT X = NULL, Y = NULL;
   size_t k = cursor;
   auto rotina_recursiva = adicao_recursiva_de_mais_duas_folhas;
   Cursor root = array[k];

   if (length_set(input) < 2)
      return;

   X = deleta_set(input);
   Y = deleta_set(input);
   array[k + 1] = tree_adiciona_direita(out, root, Y);
   array[k + 2] = tree_adiciona_esquerda(out, root, X);

   rotina_recursiva(input, out, array, k + 1);
}

void despeja_conjunto_na_arvore_recursivo(Set input, Tree output)
{
   const int N = length_set(input);
   Cursor adicoes[N];
   Tree out = output;
   GenT X = deleta_set(input);

   adicoes[0] = tree_adiciona_raiz(out, X);
   adicao_recursiva_de_mais_duas_folhas(input, out, adicoes, 0);
} 

void despeja_conjunto_na_arvore_manual(Set input, Tree output)
{
   const int N = length_set(input);
   Cursor adicoes[N];
   GenT X = NULL, Y = NULL;
   Tree out = output;
   int quantia = 0xdeadbeef,
       cursor = 0x00000000,
       raiz = 0x00000000;

   X = deleta_set(input);
   adicoes[0] = tree_adiciona_raiz(out, X);
   cursor++;

   X = deleta_set(input);
   Y = deleta_set(input);
   adicoes[1] = tree_adiciona_direita(out, adicoes[0], Y);
   adicoes[2] = tree_adiciona_esquerda(out, adicoes[0], X);
   cursor += 2;

   X = deleta_set(input);
   Y = deleta_set(input);
   adicoes[3] = tree_adiciona_direita(out, adicoes[1], Y);
   adicoes[4] = tree_adiciona_esquerda(out, adicoes[1], X);
   cursor += 2;

   X = deleta_set(input);
   Y = deleta_set(input);
   adicoes[5] = tree_adiciona_direita(out, adicoes[2], Y);
   adicoes[6] = tree_adiciona_esquerda(out, adicoes[2], X);
   cursor += 2;

   X = deleta_set(input);
   Y = deleta_set(input);
   adicoes[7] = tree_adiciona_direita(out, adicoes[3], Y);
   adicoes[8] = tree_adiciona_esquerda(out, adicoes[3], X);
   cursor += 2;

   X = deleta_set(input);
   Y = deleta_set(input);
   adicoes[9] = tree_adiciona_direita(out, adicoes[4], Y);
   adicoes[10] = tree_adiciona_esquerda(out, adicoes[4], X);
   cursor += 2;

   X = deleta_set(input);
   Y = deleta_set(input);
   adicoes[11] = tree_adiciona_direita(out, adicoes[5], Y);
   adicoes[12] = tree_adiciona_esquerda(out, adicoes[5], X);
   cursor += 2;
   
   raiz = 6;
   while (length_set(input) > 2)
   {
      X = deleta_set(input);
      Y = deleta_set(input);
      adicoes[cursor + 1] = tree_adiciona_direita(out, adicoes[raiz], Y);
      adicoes[cursor + 2] = tree_adiciona_esquerda(out, adicoes[raiz], X);
      cursor += 2;
      raiz++;
   }
}

void despeja_conjunto_na_arvore_iterativa(Set input, Tree output)
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
   
   despeja_conjunto_na_arvore_iterativa(a, b); 
   captura_visualizacao_da_arvore(b, visualiza_arvore);
}
static Set conjunto_variado_de_strings(void)
{
   Set output = new_set(hash_string, eq_string);
   int p = 0xabc;

   for (p = 0; p < GIRLS_NAMES; p++)
      add_set(output, (char*)girls_names[p]);
   for (p = 0; p < BOYS_NAMES; p++)
      add_set(output, (char*)boys_names[p]);
   for (p = 0; p < OBJETOS; p++)
      add_set(output, (char*)objetos[p]);
   for (p = 0; p < VEICULOS; p++)
      add_set(output, (char*)veiculos[p]);
   for (p = 0; p < FRUTAS; p++)
      add_set(output, (char*)frutas[p]);
   for (p = 0; p < FRUITS; p++)
      add_set(output, (char*)fruits[p]);
   for (p = 0; p < NOMES_DE_MENINOS; p++)
      add_set(output, (char*)nomes_de_meninos[p]);
   for (p = 0; p < NOMES_DE_MENINAS; p++)
      add_set(output, (char*)nomes_de_meninas[p]);
   return output;
}
