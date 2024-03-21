

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* tipos de plano para, tanto a 'Matriz de Char',
 * como para a 'String de Array'. */
typedef enum plano_de_fundo { Visivel, Invisivel } PlanoFundo;
// apelido melhor para 'char**'.
typedef char** StrArray;

char** cria_string_array(uint8_t linhas, uint8_t colunas, 
  PlanoFundo tipo) 
{
   size_t tamanho = sizeof(char*);
   char ** matriz = calloc(linhas, tamanho);
   const size_t size = sizeof (char);

   for(uint8_t k = 0; k < linhas; k++) {
      matriz[k] = calloc (colunas, size);

      char fundo;
      switch (tipo) {
         case Invisivel:
            fundo = ' ';
            break;
         case Visivel:
            fundo = '.';
            break;
         default:
            perror ("enumerador inválido!");
            abort();
      }
      memset(matriz[k], fundo, colunas);
   }
   return matriz;
}

void visualiza_string_array(StrArray a, uint8_t l, uint8_t c) {
   if (a != NULL) {
      char* barra = malloc(c + 1);
      memset(barra, '+', (c + 2));
      // imprime barra superior.
      printf(".%s.\n", barra);

      for(uint8_t i = 0; i < l; i++) {
         printf("+ ");
         for(uint8_t j = 0; j < c; j++)
            { printf("%c", a[i][j]); }
         puts(" +");
      }

      // imprime barra inferior.
      printf("*%s*\n", barra);
      // liberando as estruturas das 'barra's.
      free(barra);
   } else
      { puts("'string' inválida!"); }
}

void destroi_string_array(StrArray sa, uint8_t n) {
   for(uint8_t k = 0; k < n; k++) 
      { free(sa[k]); } 
   free(sa);
}

static char** string_array(uint8_t linhas, uint8_t colunas) {
   size_t tamanho = sizeof(char*);
   char ** matriz = calloc(linhas, tamanho);
   for(uint8_t k = 0; k < linhas; k++) {
      matriz[k] = calloc(colunas, sizeof(char));
      memset(matriz[k], ' ', colunas);
   }
   return matriz;
}

static char** separa(char * str) {
   size_t t = strlen(str);
   char ** tupla = string_array(2, t);
   uint8_t p, x = 0;
   bool separador_nao_encontrado = true;
   for(p = 0; p < t-1; p++) {
      if (str[p] == '\n')
         { separador_nao_encontrado = false; continue; }
      if (separador_nao_encontrado) 
         { tupla[0][p] = str[p]; x=p;}
      else
         { tupla[1][p-x] = str[p]; }
   }
   return tupla;
}

// tipo de dado que explicita dimensão.
typedef uint8_t* Dimensao;

void destroi_dimensao(Dimensao d) 
   { if (d != NULL) free(d); }

Dimensao cria_tupla() 
   { return (uint8_t*)calloc(2, sizeof(uint8_t)); }

// tupla com as dimensões do terminal.
Dimensao dimensao_terminal() {
   // buffer para pegar resultado.
   char resultado[15];

   // rodando comando e, lendo o resultado.
   FILE * comando = popen("tput lines cols", "r");
   int lido = fread_unlocked(&resultado, 1, 8, comando);
   #ifdef _DIMENSAO_TERMINAL
   printf("foram lido %ld bytes.\n", lido);
   #endif
   pclose(comando);

   // repartindo direitinho cada parte.
   char ** rst = separa(resultado);
   char * colunas = rst[1];
   char * linhas = rst[0];
   //printf("linha:\"%s\" e coluna:\"%s\".\n", linhas, colunas);

   // convertendo e retornando a tupla com os dados.
   Dimensao tupla = cria_tupla();
   tupla[0] = (uint8_t)atoi(linhas);
   tupla[1] = (uint8_t)atoi(colunas);
   return tupla;
}

#include <stdarg.h>

static size_t maximo(size_t* array, uint8_t n) {
   size_t maior = array[0];

   for (uint8_t k = 1; k < n; k++) {
      if (maior < array[k])
         maior = array[k];
   }

   return maior;
}

#include <inttypes.h>

/* pega várias de strings de variados tamanhos, computa a maior,
 * e aloca baseado na maior string -- obviamente -- o total de 
 * colunas da matriz de caractéres. O total de linhas dela é baseado
 * no total de argumentos passado.
 */
StrArray cria_string_array_manual(uint8_t total, ...) {
   // não é uma fila de verdade, porém é indexada como uma(FIFO).
   char** fila = cria_string_array(total, UINT8_MAX - 1, Invisivel);
   // array com todos tamanhos das strings.
   size_t* comprimentos = calloc(total, sizeof(size_t));
   va_list argumentos; 

   va_start(argumentos, total);
   // iterando todos argumentos...
   for (uint8_t i = 1; i <= total; i++) {
      char* s = va_arg(argumentos, char*);
      size_t comprimento = strlen(s);
      uint8_t indice = i - 1;
      comprimentos[indice] = comprimento;
      memcpy(fila[indice], s, comprimento);
   }
   va_end(argumentos);

   // criando uma array mais ajustada, baseada no tamanho.
   uint8_t M = maximo(comprimentos, total);
   char** strings = cria_string_array(total, M, Visivel);

   // copiando as strings para nova matriz de caractéres.
   for (uint8_t i = 1; i <= total; i++) {
      uint8_t indice = i - 1;
      memcpy(strings[indice], fila[indice], M);
   }

   // liberando array auxiliar, então retornando array...
   destroi_string_array(fila, total);
   return strings;
}

