// Cabeçalho das implementações abaixos:
#include "terminal.h"
// Biblioteca padrão do C:
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#ifdef _WIN32
#include <Windows.h>
#endif


#ifdef __linux__          
#define None -1

static int find(const char* string, char pattern) 
{
/* Acha a posição da primeira ocorrência do caractére na string. Se não
 * encontrar nada, então o retorno será -1. */
   int t = (int)strlen(string);

   for (int k = 0; k < t; k++) {
      if (string[k] == pattern)
         return k;
   }
   return None;
}

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

CROSSLIB struct TerminalSize obtem_dimensao(void) {
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

#ifdef _WIN32
CROSSLIB struct TerminalSize obtem_dimensao(void) {
/* Apenas um 'wrapper' que acessar a biblioteca do Windows, então permite
 * acessar a dimensão do 'console' que está executando. */
   CONSOLE_SCREEN_BUFFER_INFO Out;
   HANDLE In;
   uint8_t altura, largura;

   In = GetStdHandle(STD_OUTPUT_HANDLE);
   GetConsoleScreenBufferInfo(In, &Out);
   altura = (uint8_t)Out.dwSize.Y;
   largura = (uint8_t)Out.dwSize.X;

   return (struct TerminalSize){.linhas=altura, .colunas=largura};
}
#endif

#if defined(__unit_tests__) && defined(__linux__)
/* === === === === === === === === === === === === === === === === === ===+
 *                         Testes Unitários
 * === === === === === === === === === === === === === === === === === ===*/
void o_teste_apenas_chama_e_mostra_os_resultados(void) {
   puts("\nComputando dimensão mínima:");
   Dimensao A = dimensao_terminal();
   printf("\ty=%u x=%u\n", A[0], A[1]);
   destroi_dimensao(A);
}

void obtem_dimensao_por_novo_metodo(void) {
   puts("O método não usa de alocaçã dinâmica.");

   struct TerminalSize result;
   result = obtem_dimensao();

   printf("linhas: %u\tcolunas: %u\n", result.linhas, result.colunas);
}

int main(void) {
   o_teste_apenas_chama_e_mostra_os_resultados();
   obtem_dimensao_por_novo_metodo();

   return 0;
}
#elif defined(__unit_tests__) && defined(_WIN32)
#include <locale.h>
#include <synchapi.h>

void tentando_obter_dimensao_via_console_do_wsl(void) {
   HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
   COORD b = GetLargestConsoleWindowSize(a); 
   CONSOLE_SCREEN_BUFFER_INFO c;
   GetConsoleScreenBufferInfo(a, &c);

   printf("Primeiro método.\n\t\b\bx: %d\ty: %d\n", b.X, b.Y);
   printf("Segundo método.\n\t\b\bx: %d\ty: %d\n", c.dwSize.X, c.dwSize.Y);
}

void funcao_em_acao(void) {
   struct TerminalSize Out;
   const int QUANTIA = 25;

   puts("Por favor, fique redimensionando a janela ...");

   for (int n = 1; n <= QUANTIA; n++) {
      Out = obtem_dimensao();
      printf("linhas: %u\t\b\bcolunas: %u\n", Out.linhas, Out.colunas);
      Sleep(700);
   }
}

int main(int total, char* args[], char* envs[])
{
   setlocale(LC_CTYPE, "en_US.UTF-8");

   tentando_obter_dimensao_via_console_do_wsl();
   funcao_em_acao();

   return EXIT_SUCCESS;
}
#endif
