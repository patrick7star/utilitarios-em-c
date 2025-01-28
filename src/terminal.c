/* Simples funcionalidade de pegar a dimensão do terminal/ ou emultador de 
 * terminal atual, então retorna-lô. Outras coisas serão colocados no
 * futuro, por enquanto é só isso.
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "terminal.h"



int find(char* string, char pattern) 
{
/* Acha a posição da primeira ocorrência do caractére na string. Se não
 * encontrar nada, então o retorno será -1. */
   int t = strlen(string);

   for (int k = 0; k < t; k++) {
      if (string[k] == pattern)
         return k;
   }
   return -1;
}

#ifdef __linux__          
void destroi_dimensao(Dimensao d) 
   { if (d != NULL) free(d); }

static Dimensao cria_tupla() 
   { return (uint8_t*)calloc(2, sizeof(uint8_t)); }

// tupla com as dimensões do terminal.
Dimensao dimensao_terminal(void) {
   // buffer para pegar resultado.
   char resultado[15];
   // rodando comando e, lendo o resultado.
   FILE * comando = popen("tput lines cols", "r");
   Dimensao tupla = cria_tupla();

   #ifdef __debug__
   int lido = fread_unlocked(&resultado, 1, 8, comando);
   printf("Foram lido %d bytes.\n", lido);
   #else
   // Lendo resultado da chamada da execução do comando ...
   int lido = fread_unlocked(&resultado, 1, 8, comando);
   assert(lido > 0);
   #endif
   pclose(comando);

   const size_t T = 10;
   char linhas[T];
   char colunas[T];

   memset(linhas, '\0', 10);
   memset(colunas, '\0', 10);

   char* ptr = strchr(resultado, '\n') + 1;
   size_t t = ptr - resultado;

   memcpy(linhas, resultado, t);
   strcpy(colunas, ptr);

   #ifdef __debug__
   printf("Linha:\"%s\" e coluna:\"%s\".\n", linhas, colunas);
   #endif

   // convertendo e retornando a tupla com os dados.
   tupla[0] = (uint8_t)atoi(linhas);
   tupla[1] = (uint8_t)atoi(colunas);
   return tupla;
}

struct TerminalSize obtem_dimensao(void) {
   const char* CMD   = "tput cols lines";
   const int N       = 8;
   const char SEP    = '\n';
   const int sz      = sizeof(char);

   struct TerminalSize result; 
   char buffer[N], colunas[N], linhas[N];

   FILE* stream = popen (CMD, "r");
   int   lido   = fread (buffer, sz, N, stream);
   // Fechando stream depois de lido todo conteúdo retornado.
   assert (lido > 2);
   assert (pclose(stream) == 0);

   /* Separando determinados trechos da string em outras strings ...*/
   int   position = find(buffer, SEP);
   char* ptr      = buffer + position + 1; 
   int   qtd      = lido - position;

   // Copia para os apropriados buffers.
   memcpy (colunas, buffer, position);
   memcpy (linhas,  ptr,    qtd);

   // Converte para determinados inteiros.
   result.linhas  = atoi(linhas);
   result.colunas = atoi(colunas);

   return result;   
}
#endif

#ifdef _UT_TERMINAL
/* === === === === === === === === === === === === === === === === === ===+
 *                         Testes Unitários
 * === === === === === === === === === === === === === === === === === ===*/
void o_teste_apenas_chama_e_mostra_os_resultados(void) {
   puts("\nComputando dimensão mínima:");
   Dimensao A = dimensao_terminal();
   printf("\ty=%u x=%u\n", A[0], A[1]);
   destroi_dimensao(A);
}

void obtem_dimensao_por_novo_metodo() {
   puts("O método não usa de alocaçã dinâmica.");

   struct TerminalSize result;
   result = obtem_dimensao();

   printf("linhas: %u\tcolunas: %u\n", result.linhas, result.colunas);
}

void main(void) {
   o_teste_apenas_chama_e_mostra_os_resultados();
   obtem_dimensao_por_novo_metodo();
}
#endif
