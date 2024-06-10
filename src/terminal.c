

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "terminal.h"

#ifdef _POSIX_C_SOURCE           // Apenas válido para "Linux".
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

   #ifdef _INSTANCIA
   int lido = fread_unlocked(&resultado, 1, 8, comando);
   printf("Foram lido %d bytes.\n", lido);
   #else
   // Lendo resultado da chamada da execução do comando ...
   fread_unlocked(&resultado, 1, 8, comando);
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

   #ifdef _INSTANCIA
   printf("Linha:\"%s\" e coluna:\"%s\".\n", linhas, colunas);
   #endif

   // convertendo e retornando a tupla com os dados.
   tupla[0] = (uint8_t)atoi(linhas);
   tupla[1] = (uint8_t)atoi(colunas);
   return tupla;
}
#endif

#ifdef _UT_TERMINAL
void o_teste_apenas_chama_e_mostra_os_resultados(void) {
   puts("\nComputando dimensão mínima:");
   Dimensao A = dimensao_terminal();
   printf("\ty=%u x=%u\n", A[0], A[1]);
   destroi_dimensao(A);
}

void main(void) {
   o_teste_apenas_chama_e_mostra_os_resultados();
}
#endif
