#include "matrix.h"
// Bibliotec padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Capacidade máxima das arrays declaradas abaixo:
#define MAX MATRIZ_DIMENSAO_QUADRADA

static char STRING_BUFFER[MAX * MAX];
static char MATRIZ_BUFFER[MAX][MAX];
// Funções auxiliares:
// Cursores da matriz.
static int LINHA = 0, COLUNA = 0;
const int BRANCO = '.';


void string_to_matriz(void)
{
/* Pega buffer da string e matriciar ela baseado na quantia de linhas
 * que ela tem. É necessário que ela tenha tal formatação, ou seja, com
 * quebra-de-linhas, pois caso contrário um erro de indexação pode 
 * acontecer. Ele também aproveita os loops para dimensionar o tamanho
 * da matriz, definindo o total de linhas e a coluna máxima. */
   char* input = get_string_buffer();
   const int LEN = strlen(input);
   int n, coluna = 0, linha = 0;
   char caractere = '\0';
   MatrizRef matriz = get_matrix_buffer();

   for (n = 0; n < LEN; n++, coluna++)
   {
      caractere = input[n];

      if (caractere != '\n')
         matriz[linha][coluna] = caractere; 
      else { 
         // Obtendo máxima coluna possível prá da matriz.
         if (coluna > COLUNA)
            COLUNA = coluna;
         coluna = 0; linha++; 
      }
   }
   // Copia o total de linhas contadas.
   LINHA = linha;
}

void matriz_to_string(void)
{
/* Transforma buffer da matriz global numa string. O retorno sairá na 
 * string global. */
   MatrizRef matriz = get_matrix_buffer();
   const int TOTAL = string_buffer_size();
   char* output = get_string_buffer();
   int n, m, q = 0;
   const char VAZIO = ' ';

   memset(output, 0x00, TOTAL);
   printf("COLUNA: %d\tLINHA: %d\n", COLUNA, LINHA);

   for (n = 0; n < LINHA; n++)
   {
      for (m = 0; m < COLUNA; m++, q++)
      {
         if (matriz[n][m] == BRANCO)
            output[q] = VAZIO;
         else
            output[q] = matriz[n][m];
      }
      output[q++] = '\n';
   }
}

void set_string_buffer(char* input)
{
   int length = strlen(input);
   const int MAXIMO = string_buffer_size();
   char* output = get_string_buffer();

   memset(output, '\0', MAXIMO);
   memcpy(output, input, length);
}

void get_dimensao_matrix(int* input)
   { input[0] = LINHA; input[1] = COLUNA; }

void set_dimensao_matriz(int linha, int coluna)
   { LINHA = linha; COLUNA = coluna; }

MatrizRef get_matrix_buffer(void)
   { return MATRIZ_BUFFER; }

int get_dimensao_linha(void)
   { return LINHA; }

int get_dimensao_coluna(void)
   { return COLUNA; }

void matriz_print_debug(void)
{
   int lin, col;

   for (lin = 0; lin < MAX; lin++)
   {
      for (col = 0; col < MAX; col++)
         printf("%c", MATRIZ_BUFFER[lin][col]);
      putchar('\n');
   }
}

// Implementação das funções públicas do módulo.
char* get_string_buffer(void)
   { return STRING_BUFFER; }

int string_buffer_size(void)
   { return MAX * MAX; }

char recuo_usado(void)
   { return BRANCO; }

void clear_matriz_buffer(void)
{
/* Cuida essencialmente do revestimento da árvore, assim a visualização
 * fica mais bonita, e parecida com uma árvore. */
   int n = 0xdef, m = 0xabc;
   const int MAXIMO = MAX;

   for (n = 0; n < MAXIMO; n++)
      for (m = 0; m < MAXIMO; m++)
         MATRIZ_BUFFER[n][m] = BRANCO;
   // Redefini dimensão para algo sem escrita(nula).
   set_dimensao_matriz(0, 0);
}

void clear_string_buffer(void) 
   { memset(STRING_BUFFER, 0x00, MAX * MAX); }
