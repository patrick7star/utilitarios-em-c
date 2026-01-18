// Biblioteca padrão em C(libs muito utilizadas.):
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
// Seus módulos:
#include "teste.h"

uint64_t entradas[] = {
   382, 12832, 3842394, 7712340981,
   111931512, 50123812341, 100030231892377
};

double segundos[] = {
   51.3232, 12832.15, 8328.0,
   38832.312, 0.001, 0.038,
   0.000851, 0.000000701, 190.5321
};

void legibilidade_do_tempo(void) {
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

void legibilidade_de_tamanhos(void) {
   for(size_t p = 1; p <= 7; p++) {
		uint64_t valor = entradas[p - 1];
		char* traducao = tamanho_legivel(valor);
		printf ("%20lu ==> %s\n", valor, traducao);
		free (traducao);
	}
}

void o_grosso_de_grande_valores(void) {
   for(int p = 0; p < 7; p++)
      printf(
         "%16lu ===> %s\n",
         entradas[p],
         valor_legivel(entradas[p])
      );

   float inputs_a[] = {3000531.14159, 12345.6789, 1.71001e4, 3.14159e9};
   const int na = sizeof(inputs_a) / sizeof(float);

   puts("\nAgora valores decimais(float/double):");
   for (int k = 0; k < na; k++)
      printf("%21f ===> %s\n", inputs_a[k], valor_legivel(inputs_a[k]));
}

void valor_legivel_de_todos_limites(void)
{
   char* formatacoes[] = {
      // Inteiros positivos sem sinal:
      valor_legivel(UCHAR_MAX),
      valor_legivel(USHRT_MAX),
      valor_legivel(UINT_MAX),
      valor_legivel(ULONG_MAX),
      valor_legivel(SIZE_MAX),
      // Constantes referentes a inteiros com sinal.
      valor_legivel (CHAR_MAX),
      valor_legivel (SHRT_MAX),
      valor_legivel (INT_MAX),
      valor_legivel (LONG_MAX),
      valor_legivel (CHAR_MIN),
      valor_legivel (SHRT_MIN),
      valor_legivel (INT_MIN),
      valor_legivel (LONG_MIN),
      valor_legivel (DBL_MAX),
      valor_legivel (DBL_MIN),
      valor_legivel (FLT_MAX),
      valor_legivel (FLT_MIN)
   };
   const int n = sizeof(formatacoes) / sizeof(char*);

   puts("Inteiros sem sinal: ");
   printf("%21u   8-bits  ==> %s\n", UCHAR_MAX, formatacoes[0]);
   printf("%21u  16-bits  ==> %s\n", USHRT_MAX, formatacoes[1]);
   printf("%21u  32-bits  ==> %s\n", UINT_MAX, formatacoes[2]);
   printf("%21lu  64-bits  ==> %s\n", ULONG_MAX, formatacoes[3]);
   printf("%21zu  (size_t) ==> %s\n", SIZE_MAX, formatacoes[4]);
   puts("\nInteiros com sinal: ");
   printf("%21d   8-bits ==> %s\n", CHAR_MAX, formatacoes[5]);
   printf("%21d   8-bits ==> %s\n", CHAR_MIN, formatacoes[9]);
   printf("%21d  16-bits ==> %s\n", SHRT_MAX, formatacoes[6]);
   printf("%21d  16-bits ==> %s\n", SHRT_MIN, formatacoes[10]);
   printf("%21d  32-bits ==> %s\n", INT_MAX, formatacoes[7]);
   printf("%21d  32-bits ==> %s\n", INT_MIN, formatacoes[11]);
   printf("%21ld  64-bits ==> %s\n",LONG_MAX, formatacoes[8]);
   printf("%21ld  64-bits ==> %s\n",LONG_MIN, formatacoes[12]);
   puts("\nAmbos tipos decimais: ");
   printf("%21lf  64-bits ==> %s\n",DBL_MAX, formatacoes[13]);
   printf("%21.6lf  64-bits ==> %s\n",DBL_MIN, formatacoes[14]);
   printf("%21.6lf  32-bits ==> %s\n",FLT_MAX, formatacoes[15]);
   printf("%21.6lf  32-bits ==> %s\n",FLT_MIN, formatacoes[16]);

   for (int i = 0; i < n; i++)
      free(formatacoes[i]);
}

void tipos_de_tempo_diferentes_a_converter(void) {
   struct timeval a;
   struct timespec b = {5, 803000115};
   char* str_a, *str_b;

   gettimeofday(&a, NULL);
   str_a = tempo_legivel(a);
   str_b = tempo_legivel(b);
   printf("struct timeval ===> %s\nstruct timespec ==> %s\n", str_a, str_b);
   free(str_b);
   free(str_a);
}

void eliminando_digitos_insignificantes(void)
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

int main(void) {
   executa_testes_a(
      true, 6,
         legibilidade_do_tempo, true,
         legibilidade_de_tamanhos, true,
		   o_grosso_de_grande_valores, true,
         valor_legivel_de_todos_limites, true,
         tipos_de_tempo_diferentes_a_converter, true,
         eliminando_digitos_insignificantes, true
	);
   return EXIT_SUCCESS;
}
