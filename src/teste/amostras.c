
#include "amostras.h"
#include "progresso.h"
#include "dados_testes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>
//#include <tgmath.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define bool_to_str(logico) logico ? "true": "false"
/* === === === === === === === === === === === === === === === === === ===+
 *                   Testes Antigos Sem Retorno 
 * === === === === === === === === === === === === === === === === === ===*/

void stringficacao_de_valores_primitivos() {
   printf("\t%-5d ==> %10s\n", 15, bool_to_str(15));
   printf("\t%-5c ==> %10s\n", 'M', bool_to_str('M'));
   printf("\t%-5d ==> %10s\n", 0, bool_to_str(0));
   printf("\t%-5s ==> %10s\n", "false", bool_to_str(false));
}

void percorrendo_string() {
   char* entrada = "this planet is prettly BLUE!";
   char* atual = entrada;
   while (*atual) {
      printf("|%c| ", atual[0]);
      atual++;
   }
   puts("");
}

void string_minuscula(char* in, char* out)
{
   int t = strlen(in); 

   for (int i = 0; i < t; i++)
      out[i] = tolower(in[i]);
   out[t] = '\0';
}

void transforma_toda_string_em_minuscula() {
   char* entradas[] = {
      "jogo legal!", "Capital", "The United States of America",
      "brazil", "cachorro", "Diego", "tEsTe MaIs DIScarAdO"
   };
   char conversao[100];

   for (size_t i = 0; i < 7; i++)
   {
      string_minuscula(entradas[i], conversao);
      printf("%s ===> %s\n", entradas[i], conversao);
   }   
}

/* === === === === === === === === === === === === === === === === === ===+
 *                  Testes que Retorna Sinal 
 *                    de Saída do Processo
 *
 * Funções que são amostras para os protótipos:
 * === === === === === === === === === === === === === === === === === ===*/

// [okay]
int capitaliza_palavras(void) {
   puts("Capitalizando tais palavras ...");

   for (size_t i = 1; i <= FRUTAS; i++) {
      char* fruta = (char*)frutas[i - 1];
      size_t size = strlen(fruta) + 1;
      char copia[size];
	   
      #ifdef __linux__
      strcpy(copia, fruta);
      #elif defined(_WIN32)	   
	strcpy_s(copia, size, fruta);
      #endif
      copia[0] = (char)toupper(copia[0]);
      printf("\t%s ===> %s\n", fruta, copia);
   } 
   return EXIT_SUCCESS;
}

// [okay]
int string_alternada(void) {
   puts("Colocando caractéres de forma alternada ...");
   for (size_t i = 1; i <= GIRLS_NAMES; i++) {
      char* nome = (char*)girls_names[i - 1];
      size_t size = strlen(nome) + 1;
      char copia[size];
	   
      #ifdef __linux__
      strcpy(copia, nome);
      #elif defined(_WIN32)
      strcpy_s(copia, size, nome);
      #endif

      for (size_t i = 0; i < strlen(copia); i++) {
         if (i % 2 == 0) 
            copia[i] = toupper(copia[i]);
         else
            copia[i] = tolower(copia[i]);
      }
      printf("\t%s ===> %s\n", nome, copia);
   } 
   return EXIT_SUCCESS;
}

// [falha]
int carregando_ate_cem(void) {
	#ifdef __linux__
   PG bar = cria_bp(Simples, 100, 50);
   size_t contagem = 1;
   // 70 milisegundos ...
   struct timespec pausa = { 0, 70000000 };

   do {
      visualiza_bp(&bar);
      atualiza_bp(&bar, contagem++);
      nanosleep(&pausa, NULL);

      if (contagem == 70)
         return EXIT_FAILURE;
   } while (contagem <= 100);

	#elif defined(_WIN32)
   PG bar = cria_bp(Simples, 100, 50);
   size_t contagem = 1;
   // 70 milisegundos ...
	DWORD pausa = 70;

   do {
      visualiza_bp(&bar);
      atualiza_bp(&bar, contagem++);
      // nanosleep(&pausa, NULL);
		Sleep(pausa);

      if (contagem == 70)
         return EXIT_FAILURE;
   } while (contagem <= 100);
	#endif

   return EXIT_SUCCESS;
}

// [falha]
int simples_mensagem_do_fortune(void) {
	#ifdef _WIN32
   FILE* stream = _popen("fortune", "r");
   char buffer[5000];

   fread(buffer, sizeof(char), 5000, stream);
   _pclose(stream);
   puts(buffer);

	#elif defined(__linux__)
   FILE* stream = popen("fortune", "r");
   char buffer[5000];

   fread(buffer, sizeof(char), 5000, stream);
   pclose(stream);
   puts(buffer);
	#endif

   return EXIT_FAILURE;
}

// [falha]
int executa_algo_mas_crash(void) {
   for (int p = 1; p <= 11; p++) {
      puts(veiculos[p - 1]);
      if (p == 7)
         assert (p == 8);
   }
   return EXIT_SUCCESS;
}

bool atingiu_99_porcentos(uint16_t n)
{
   float a = (float)n;
   float N = (float)UINT16_MAX;
   
   return (a / N) > 0.95;
}
// [okay]
int conta_ate_esgotar_valor(void)
{
   uint16_t q = 1;
   uint16_t L1 = UINT16_MAX / 4;
   uint16_t L2 = UINT16_MAX / 2;
   uint16_t L3 = 2 * (UINT16_MAX / 3);
   const uint16_t P = 2315;

   printf("Contando de 1 até %u ...\n", UINT16_MAX);
   fflush(stdout);

   do {
      if (q % P == 0)
         printf("\t\b\b\bAtingiu %7u.\n", q);

      if (q >= L1 && q < L2)
         q += 2;
      else if (q >= L2 && q < L3)
         q += 3;
      else if (q >= L3)
         q += 4;
      else
         q++;
   } while(!atingiu_99_porcentos(q));

   puts("feito.");

   return EXIT_SUCCESS;
}

