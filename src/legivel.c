// Cabeçalho com definições da funções:
#include "legivel.h"
// Biblioteca padrão do C:
#include <stdio.h>
#include <string.h>
#include <iso646.h>
#include <math.h>

// Pesos que ajudam na conversão:
const size_t MILHAR     = 1e3;
const size_t MILHAO     = 1e6;
const size_t BILHAO     = 1e9;
const size_t TRILHAO    = (size_t)1e12;
const size_t QUADRILHAO = (size_t)1e15;
const size_t QUINTILHAO = (size_t)1e18;
// para comparação ...
const double NANOSEG    = 1e-9;
const double MICROSEG   = 1e-6;
const double MILISEG    = 1e-3;
const double MINUTO     = 60.0;
const double HORA       = 60 * MINUTO; 
const double DIA        = 24 * HORA;
const double MES        = 30 * DIA;
const double ANO        = 365 * DIA;
const double DECADA     = 10 * ANO;
const double SECULO     = 100 * ANO;
const double MILENIO    = 1000 * ANO;


static char* nova_str(int n)
// Aloca string do tamanho que você desejar.
   /* NOTA: no Windows, n é convertido implicitamente prá um uint64_t. Por
    * motivos de compatibilidade, colocarei nesta fonte, já que GCC não
    * reclama de tal conversão implicita na função de alocação de memória. */
   { return (char*)malloc((uint64_t)n * sizeof(char)); }

static void elimina_digitos_insignificantes
  (char* output, double valor, const char* peso)
{
/* Faz trucagem de digitos fracionais insignificantes para a leitura. */
   // Partes inteiras e decimais do valor, separadas pra análise.
   // double inteiro = truncf(valor);
   double fracao = fabs(truncf(valor) - valor);

   if (fracao >= 0.1 && fracao < 0.95)
      sprintf(output, "%0.1lf %s", valor, peso);
   else if (fracao >= 0.95 && fracao < 0.99999)
      // Tal range de decimais podem produzir dígito zero inútil
      sprintf(output, "%0.0lf %s", round(valor), peso);
   else
      sprintf(output, "%0.0lf %s", valor, peso);
}

/* === === === === === === === === === === === === === === === === === ==
 *                Legibilidade do Tamanho
 * === === === === === === === === === === === === === === === === === ==*/
CROSSLIB char* tamanho_legivel(size_t bytes) { 
   char* resultado_str = nova_str(30);
   char peso_str[5];
   float valor = 0.0;
   // Múltiplos de tamanho(equivalente em bytes).
   const size_t KILO = pow(2, 10); 
   const size_t MEGA = pow(2, 20);
   const size_t GIGA = pow(2, 30);
   const size_t TERA = pow(2, 40);
   const size_t PETA = pow(2, 50);

   if (bytes >= KILO && bytes < MEGA) {
      memcpy(&peso_str, "KiB", 3);
      valor = (float)bytes / (float)KILO;
   } else if (bytes >= MEGA && bytes < GIGA) {
      memcpy(&peso_str, "MiB", 3);
      valor = (float)bytes / (float)MEGA;
   } else if (bytes >= GIGA && bytes < TERA) {
      memcpy(&peso_str, "GiB", 3);
      valor = (float)bytes / (float)GIGA;

   } else if (bytes >= TERA && bytes < PETA) {
      memcpy(&peso_str, "TiB", 3);
      valor = (float)bytes / (float)TERA;
   } else {
      memcpy(&peso_str, "B's", 3);
      valor = (float)bytes;
   }

   // fracao = abs(round(valor) - valor);
   elimina_digitos_insignificantes(resultado_str, valor, peso_str);
   return resultado_str;
}

/* === === === === === === === === === === === === === === === === === ==
 *                Legibilidade do Valor Absoluto
 * === === === === === === === === === === === === === === === === === ==*/
CROSSLIB char* valor_legivel_isize(int64_t unidades) 
{
   char* formatacao = nova_str(15);
   // Apenas a magnitude do valor.
   int64_t m = unidades > 0 ? unidades: (-1) * unidades;
   double potencia, decimal;
   const char* peso;

   // Proposições trabalhadas:
   bool faixa_dos_milhares    = (m >= MILHAR) and (m < MILHAO);
   bool faixa_dos_milhoes     = (m >= MILHAO) and (m < BILHAO);
   bool faixa_dos_bilhoes     = (m >= BILHAO) and (m < TRILHAO);
   bool faixa_dos_trilhoes    = (m >= TRILHAO) and (m < QUADRILHAO);
   bool faixa_dos_quadrilhoes = (m >= QUADRILHAO) and (m < QUINTILHAO);
   bool faixa_dos_quitilhoes  = (m >= QUINTILHAO);

   if (faixa_dos_milhares) {
      peso = "mil";
      potencia = MILHAR;

   } else if (faixa_dos_milhoes) {
      peso = "mi";
      potencia = MILHAO;

   } else if (faixa_dos_bilhoes) {
      peso = "bi";
      potencia = BILHAO;

   } else if (faixa_dos_trilhoes) {
      potencia = TRILHAO;
      peso = "tri";

   } else if (faixa_dos_quadrilhoes) {
      potencia = QUADRILHAO;
      peso = "qd";

   } else if (faixa_dos_quitilhoes) {
      potencia = QUINTILHAO;
      peso = "qn";

   } else 
      { peso = ""; potencia = 1; }

   decimal = (double)unidades /(double)potencia;
   elimina_digitos_insignificantes(formatacao, decimal, peso);
   return formatacao;
}

CROSSLIB char* valor_legivel_usize(size_t unidades) {
   char* peso;
   double potencia, decimal;
   char* resultado_str = nova_str(15);
   uint64_t u = unidades;

   if (u >= 1000 && u < pow(10, 6)) {
      peso = "mil";
      potencia = 1000;
   } else if (u >= pow(10, 6) && u < pow(10, 9)) {
      peso = "mi";
      potencia = pow(10, 6);
   } else if (u >= pow(10, 9) && u < pow(10, 12)) {
      peso = "bi";
      potencia = pow(10, 9);
   } else if (u >= pow(10, 12) && u < pow(10, 15)) {
      potencia = pow(10, 12);
      peso = "tri";
   } else if (u >= pow(10, 15) && u < pow(10, 18)) {
      potencia = pow(10, 15);
      peso = "qd";
   } else if (u >= pow(10, 18) && u < pow(10, 21)) {
      potencia = pow(10, 18);
      peso = "qn";
   } else 
      { peso = ""; potencia = 1; }

   decimal = (double)unidades /(double)potencia;
   elimina_digitos_insignificantes(resultado_str, decimal, peso);
   return resultado_str;
}

CROSSLIB char* valor_legivel_f32 (float decimal) 
   { return valor_legivel_isize((int64_t)decimal); }

CROSSLIB char* valor_legivel_f64 (double decimal)
   { return valor_legivel_isize((int64_t)decimal); }

/* === === === === === === === === === === === === === === === === === ==
 *                   Legibilidade do Tempo
 * === === === === === === === === === === === === === === === === === ==*/
CROSSLIB char* tempo_legivel_double(double segundos) 
{
   // variável com uma letra para agilizar codificação.
   double s = segundos;
   // char* resultado = calloc(30, sizeof(char));
   char* resultado = nova_str(30);
   const char* peso;
   double decimal;
   // Proposições avaliadas abaixo:
   bool na_faixa_dos_meses = (s >= MES && s < ANO);
   bool na_faixa_dos_anos = (s >= ANO && s < DECADA);
   bool na_faixa_das_decadas = (s >= DECADA && s < SECULO);
   bool na_faixa_dos_seculos = (s >= SECULO && s < MILENIO);

   if (s >= 1.0) {
      if (s < MINUTO)
         { decimal = s; peso = "seg"; }
        //  sprintf(resultado, "%0.1lf seg", s);

      else if (s >= MINUTO && s < HORA)
         { decimal = s / MINUTO; peso = "min"; }
        //  sprintf(resultado, "%0.1lf min", s / MINUTO);

      else if (s >= HORA && s < DIA)
         { decimal = s / HORA; peso = "\bh"; }
         // sprintf(resultado, "%0.1lfh", s / HORA);

      else if (s >= DIA && s < MES)
         // sprintf(resultado, "%0.1lf dias", s / DIA);
         { decimal = s / DIA; peso = "dias"; }

      else if (na_faixa_dos_meses)
         // sprintf(resultado, "%0.1lf meses", s / MES);
         { decimal = s / MES; peso = "meses"; }
      else if (na_faixa_dos_anos)
         // sprintf(resultado, "%0.1lf anos", s / ANO);
         { decimal = s / ANO; peso = "anos"; }
      else if (na_faixa_das_decadas)
         // sprintf(resultado, "%0.1lf déc", s / DECADA);
         { decimal = s / DECADA; peso = "déc"; }
      else if (na_faixa_dos_seculos)
         // sprintf(resultado, "%0.1lf séc", s / SECULO);
         { decimal = s / SECULO; peso = "séc"; }
      else
         // sprintf(resultado, "%0.1lf milênios", s / MILENIO);
         { decimal = s / MILENIO; peso = "milênios"; }
   } else {
      if (s >= MILISEG)
         // sprintf(resultado, "%0.0lf ms", s * 1000.0);
         { decimal = s * 1.0e3; peso = "ms"; }
      else if (s >= MICROSEG)
         // sprintf(resultado, "%0.0lf \u03bcs", s * powf(10, 6));
         { decimal = s * 1.0e6; peso = "\u03BCs"; }
      else if (s >= NANOSEG)
         // sprintf(resultado, "%0.0lf ns", s * powf(10, 9));
         { decimal = s * 1.0e9; peso = "ns"; }
      else 
			// qualquer quantia abaixo disso, será considerado zero!
         { decimal = 0.0; peso = "seg"; }
			// strcpy (resultado, "0seg");
   }

   elimina_digitos_insignificantes(resultado, decimal, peso);
   return resultado;
}

CROSSLIB char* tempo_legivel_usize(size_t seg) 
   { return tempo_legivel_double((double)seg); }

#ifdef __linux__
double timespec_to_seg(struct timespec a)
   { return (double)a.tv_sec + (double)a.tv_nsec / 1.0e9; }

char* tempo_legivel_timespec(struct timespec t)
   { return tempo_legivel_double(timespec_to_seg(t)); }

double timeval_to_seg(struct timeval e)
   { return (double)e.tv_sec + (double)e.tv_usec / 1.0e6; }

char* tempo_legivel_timeval(struct timeval t)
   { return tempo_legivel_double(timeval_to_seg(t)); }
#endif

/* === === === === === === === === === === === === === === === === === ===+
 * .......................................................................&
 * ........................Testes Unitários...............................&
 * .......................................................................&
 * === === === === === === === === === === === === === === === === === = */
#if defined(__unit_tests__) && defined(__linux__)
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
#elif defined(_WIN32) && defined(__unit_tests__)
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <locale.h>
// #define _CRT_NONSTDC_NO_DEPRECATE
#define TESTE void static

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

int main(int total, char* args[], char* vars[]) 
{
   setlocale(LC_CTYPE, "en_US.UTF-8");

	legibilidade_do_tempo();
	legibilidade_de_tamanhos();
	o_grosso_de_grande_valores();
	valor_legivel_de_todos_limites();
	eliminando_digitos_insignificantes();

   return EXIT_SUCCESS;
}
#endif
