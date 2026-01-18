#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <locale.h>
#include <string.h>
#include "teste.h"

#define SIZE_FMT 30

static char FMT[SIZE_FMT];

static uint64_t entradas[] = {
   382, 12832, 3842394, 7712340981,
   111931512, 50123812341, 100030231892377
};

static double segundos[] = {
   51.3232, 12832.15, 8328.0,
   38832.312, 0.001, 0.038,
   0.000851, 0.000000701, 190.5321
};


TESTE legibilidade_do_tempo(void) {
   double inputs[] = {
      5 * DIA + 30 * HORA, 8000 * MES, 165 * MES + 4e10, 14 * ANO + 30 * MES,
      1500 * DECADA, 14 * DECADA + 80 * ANO, 400 * DIA, 100 * DIA
   };
   const int sz = sizeof(double);
   const int n = sizeof(inputs) / sz;

   for(int p = 0; p < 9; p++)
      printf(
         "%16lf ===> %s\n",
         segundos[p],
         tempo_legivel(segundos[p])
      );

   puts("\nEscalas de tempo extremamentes grandes:");
   for(int p = 0; p < n; p++)
      printf("%26.0lf ===> %s\n", inputs[p], tempo_legivel(inputs[p]));
   // de avaliação manual?
}

TESTE legibilidade_de_tamanhos(void) {
   for(size_t p = 1; p <= 7; p++) {
		uint64_t valor = entradas[p - 1];
		char* traducao = tamanho_legivel(valor);
		printf ("%20llu ==> %s\n", valor, traducao);
		free (traducao);
	}
}

TESTE o_grosso_de_grande_valores(void) {
   float inputs_a[] = {3000531.14159, 12345.6789, 1.71001e4, 3.14159e9};
   const int na = sizeof(inputs_a) / sizeof(float);

   for(int p = 0; p < 7; p++)
      printf("%16llu ===> %s\n", entradas[p], valor_legivel(entradas[p]));

   puts("\nAgora valores decimais(float/double):");
   for (int k = 0; k < na; k++)
      printf("%21f ===> %s\n", inputs_a[k], valor_legivel(inputs_a[k]));
}

TESTE valor_legivel_de_todos_limites(void)
{
   struct ParES {
      // Formatação da decisão dos dígitos.
      const char* const In_a;
      // Representação do inteiro de máquina trabalhado.
      const char* const In_b;
      // Resultado da conversão.
      char* Out;
   };

   char* (*transform_a)(size_t) = valor_legivel_usize;
   char* (*transform_b)(int64_t) = valor_legivel_isize;
   char* (*transform_c)(double) = valor_legivel_f64;
   struct ParES results[] = {
      // Inteiros positivos sem sinal:
      (struct ParES) {"%21u", "8-bits", transform_a(UCHAR_MAX)},
      (struct ParES) {"%21u","16-bits", transform_a(USHRT_MAX)},
      (struct ParES) {"%21u", "32-bits", transform_a(UINT_MAX)},
      (struct ParES) {"%21zu", "size_t", transform_a(SIZE_MAX)},
      // Constantes referentes a inteiros com sinal.
      (struct ParES) {"%21d", "8-bits", transform_b(CHAR_MAX)},
      (struct ParES) {"%21d", "8-bits", transform_b(CHAR_MIN)},
      (struct ParES) {"%21d", "16-bits", transform_b(SHRT_MAX)},
      (struct ParES) {"%21d", "16-bits", transform_b(SHRT_MIN)},
      (struct ParES) {"%21d", "32-bits", transform_b(INT_MAX)},
      (struct ParES) {"%21d", "32-bits", transform_b(INT_MIN)},
      (struct ParES) {"%21ld", "64-bits", transform_b(LONG_MAX)},
      (struct ParES) {"%21ld", "64-bits", transform_b(LONG_MIN)},
      // Valores decimais:
      (struct ParES) {"%21.6lf", "64-bits", transform_c(DBL_MAX)},
      (struct ParES) {"%21.6lf", "64-bits", transform_c(DBL_MIN)},
      (struct ParES) {"%21.6f", "32-bits", transform_c(FLT_MAX)},
       (struct ParES) {"%21.6f", "32-bits", transform_c(FLT_MIN)}

      // Nota: algums não compatíveis com Windows, foram desabilitados, ou seja
      // apenas foram comentados.
   };
   const int N = sizeof(results) / sizeof(struct ParES);

   puts("Inteiros sem sinal: ");
   puts("\nInteiros com sinal: ");
   puts("\nAmbos tipos decimais: ");

   for (int i = 0; i < N; i++)
      free(results[i].Out);
}

TESTE eliminando_digitos_insignificantes(void)
{
   double input[] = {
      3.153, 352.001, 0.1234, 153.7, 12.07, 3.915, 19.94,
      103.95, 1.96
   };
   const int n = sizeof(input) / sizeof(double);
   char buffer[50];
   const char* peso = "[peso]";

   puts("Verificando a eliminação de dígitos insignificantes ...\n");
   for (int k = 0; k < n; k++)
   {
      elimina_digitos_insignificantes(buffer, input[k], peso);
      printf("%15lf ===> %14s\n", input[k], buffer);
   }
}

static const char* debug_filetime(FILETIME obj)
{
   size_t nanos = filetime_to_usize(obj);

   memset(FMT, 0x00, SIZE_FMT);
   sprintf(FMT, "(FILETIME) %zuns", nanos);

   return FMT;
}

UNIT_TEST conversao_do_selo_de_tempo_do_windows(void)
{
   FILETIME idle, kernel, user;
   char* conversoes[3];
   
   if (GetSystemTimes(&idle, &kernel, &user))
      puts("Função chamada com sucesso.");
   else
      puts("A função falhou.");

   conversoes[0] = tempo_legivel(idle);
   conversoes[1] = tempo_legivel(kernel);
   conversoes[2] = tempo_legivel_filetime(user);

   printf("idle: %s | %s\n", conversoes[0], debug_filetime(idle));
   printf("kernel: %s | %s\n", conversoes[1], debug_filetime(kernel));
   printf("user: %s | %s\n", conversoes[2], debug_filetime(user));

   free(conversoes[0]);
   free(conversoes[1]);
   free(conversoes[2]);
}

int main(int total, char* args[], char* vars[])
{
   setlocale(LC_CTYPE, "en_US.UTF-8"); 

   executa_testes(
      true, 6,
         Unit(legibilidade_do_tempo, true),
         Unit(legibilidade_de_tamanhos, true),
         Unit(o_grosso_de_grande_valores, true),
         Unit(valor_legivel_de_todos_limites, false),
         Unit(eliminando_digitos_insignificantes, true),
         Unit(conversao_do_selo_de_tempo_do_windows, true)
   );

   return EXIT_SUCCESS;
}
