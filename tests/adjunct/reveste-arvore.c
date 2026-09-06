#include "reveste-arvore.h"
// Bibliotec padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Capacidade máxima das arrays declaradas abaixo:
#define MAX 150
#define CRUZ_BUFFER_SIZE 6
// Apelido do matriz bidimensional com determinada dimensão:
typedef char (*MatrizRef)[MAX];

static char RESULTADO[MAX * MAX];
static char MATRIZ_BUFFER[MAX][MAX];
// Cursores da matriz.
static int LINHA = 0, COLUNA = 0;
const int BRANCO = '.';

// Funções auxiliares(privadas) usadas em algumas funções públicas abaixo.
static void matriz_debug(void);
static void aplica_remendo(void);
static void matriz_to_string(void);
static void string_to_matriz(void);

// Implementação das funções públicas do módulo.
char* string_buffer(void)
   { return RESULTADO; }

int string_buffer_size(void)
   { return MAX * MAX; }

char* desenha_arvore(void)
{
   printf("Função '%s' iniciada.\n", __FUNCTION__);

   string_to_matriz();
   printf("Dimensão da matriz: %d x %d\n", LINHA, COLUNA);
   aplica_remendo();
   matriz_to_string();

   return string_buffer();
}

char recuo_usado(void)
   { return BRANCO; }

static void zera_matriz(void)
{
/* Cuida essencialmente do revestimento da árvore, assim a visualização
 * fica mais bonita, e parecida com uma árvore. */
   int n = 0xdef, m = 0xabc;
   const int MAXIMO = MAX;

   for (n = 0; n < MAXIMO; n++)
      for (m = 0; m < MAXIMO; m++)
         MATRIZ_BUFFER[n][m] = BRANCO;
}

static void string_to_matriz(void)
{
/* Pega buffer da string e matriciar ela baseado na quantia de linhas
 * que ela tem. É necessário que ela tenha tal formatação, ou seja, com
 * quebra-de-linhas, pois caso contrário um erro de indexação pode 
 * acontecer. Ele também aproveita os loops para dimensionar o tamanho
 * da matriz, definindo o total de linhas e a coluna máxima. */
   char* input = string_buffer();
   const int LEN = strlen(input);
   int n, coluna = 0, linha = 0;
   char caractere = '\0';
   MatrizRef matriz = MATRIZ_BUFFER;

   zera_matriz();

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

static void matriz_to_string(void)
{
/* Transforma buffer da matriz global numa string. O retorno sairá na 
 * string global. */
   char (*matriz)[MAX] = MATRIZ_BUFFER;
   const int TOTAL = MAX * MAX;
   char* output = string_buffer();
   int n, m, q = 0;

   memset(output, 0x00, TOTAL);

   for (n = 0; n < LINHA; n++)
   {
      for (m = 0; m < COLUNA; m++, q++)
      {
         if (matriz[n][m] == BRANCO)
            output[q] = ' ';
         else
            output[q] = matriz[n][m];
      }
      output[q++] = '\n';
   }
}

static bool match_formato(char cruz[])
{
/* Formato que a cruz tem que ser compátivel. As partes com não pontos
 * são as partes relevantes prá correspondência:
 *          .........
 *          ....o....
 *          ...o|__..
 *          ....o....
 *          .........
 */
   const char CENTRO             = cruz[0];
   const char LATERAL_DIREITO1   = cruz[1];
   const char LATERAL_DIREITO2   = cruz[2];
   const char LATERAL_ESQUERDO   = cruz[3];
   const char SUPERIOR           = cruz[4];
   const char SUPERIOR_DIREITO   = cruz[5];

   return   CENTRO == '|' && LATERAL_DIREITO1 == '_'                 && 
            LATERAL_DIREITO2 == '_' &&  LATERAL_ESQUERDO == BRANCO   && 
            SUPERIOR == BRANCO  && SUPERIOR_DIREITO == BRANCO;
}

static bool match_disponbilidade(char cruz[])
{
// Dá positivo quando acertar trecho que não está mais vázio.
   const char CENTRO             = cruz[0];
   const char LATERAL_DIREITO1   = cruz[1];
   const char LATERAL_DIREITO2   = cruz[2];

   return CENTRO == BRANCO && LATERAL_DIREITO2 == BRANCO  &&
          LATERAL_DIREITO2; 
}

static void extrai_cruz(MatrizRef input, int lin, int col, char* output)
{
// A composição da cruz na array se dará à partir do centro, então um
// giro seletivo no sentido horário.
   MatrizRef image = input;

   // Centro:
   output[0] = image[lin][col];
   // Lado Direito(1):
   output[1] = image[lin][col + 1];
   // Lado Direito(2):
   output[2] = image[lin][col + 2];
   // Lado Esquerdo:
   output[3] = image[lin][col - 1];
   // Superior:
   output[4] = image[lin - 1][col];
   // Superior Direito:
   output[5] = image[lin - 1][col + 1];
}

static void aplica_remendo(void)
{
/* O algoritmo pega a matriz com a àrvore inscrita nela, e completa os 
 * galhos não terminados.*/
   char cruz[CRUZ_BUFFER_SIZE] = {0x00}, pixel;
   int linha, lin, col;
   MatrizRef image = MATRIZ_BUFFER;
   bool linha_verificada;
   const char VERTICAL = '|';
   const int MARGEM = 5;

   for (lin = LINHA + MARGEM; lin > 1; lin--)
   {
      // Marcador evita mais colunas a esquerdas serem verificadas, pois 
      // não haverá mais casos, já que a ramificação é mais uma por linha.
      linha_verificada = false;

      for (col = (COLUNA + MARGEM); col > 1; col--)
      {
         extrai_cruz(image, lin, col, cruz);

         if (match_formato(cruz) && !linha_verificada)
         {
            // Já considera a linha acima, pois a mesma linha dará 'match'.
            linha = lin - 1; 

            do {
               extrai_cruz(image, linha, col, cruz);

               if (match_disponbilidade(cruz))
                  image[linha][col] = VERTICAL;
               else
                  break;
               linha--;
            } while (linha > 1);
            linha_verificada = true;
         }
      }
   }
}

static void matriz_debug(void)
{
   int lin, col;

   for (lin = 0; lin < MAX; lin++)
   {
      for (col = 0; col < MAX; col++)
         printf("%c", MATRIZ_BUFFER[lin][col]);
      putchar('\n');
   }
}

#ifdef __unit_tests__
int main(void)
{
   char** amostras = {

   };
}
#endif
