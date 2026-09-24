#include "reveste-arvore.h"
// Outros submódulos:
#include "matrix.h"
// Bibliotec padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Capacidade máxima das arrays declaradas abaixo:
#define CRUZ_BUFFER_SIZE 6

// Funções auxiliares(privadas) usadas em algumas funções públicas abaixo.
static void aplica_remendo(void);

char* desenha_arvore(void)
{
   const int LINHA = get_dimensao_linha();
   const int COLUNA = get_dimensao_linha();

   printf("Função '%s' iniciada.\n", __FUNCTION__);
   printf("Dimensão da matriz: %d x %d\n", LINHA, COLUNA);
   // Antes de começar um novo processo de matriciação, vamos limpar-la.
   clear_matriz_buffer();
   string_to_matriz();
   aplica_remendo();
   clear_string_buffer();
   matriz_to_string();

   return get_string_buffer();
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
   const char BRANCO             = recuo_usado();

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
   const char BRANCO             = recuo_usado();

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
   MatrizRef image = get_matrix_buffer();
   bool linha_verificada;
   const char VERTICAL = '|';
   const int MARGEM = 5;
   const int COLUNA = get_dimensao_coluna();
   const int LINHA = get_dimensao_linha();

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
