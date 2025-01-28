// Cabeçalho com definições da funções:
#include "legivel.h"
// Biblioteca padrão do C:
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iso646.h>

// Pesos que ajudam na conversão:
const size_t MILHAR     = 1e3;
const size_t MILHAO     = 1e6;
const size_t BILHAO     = 1e9;
const size_t TRILHAO    = 1e12;
const size_t QUADRILHAO = 1e15;
const size_t QUINTILHAO = 1e18;
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


// Aloca string do tamanho que você desejar.
static char* nova_str(int n)
   { return (char*)malloc(n * sizeof(char)); }

char* tempo_legivel_double(double segundos) 
{
   // variável com uma letra para agilizar codificação.
   double s = segundos;
   // char* resultado = calloc(30, sizeof(char));
   char* resultado = nova_str(30);
   // Proposições avaliadas abaixo:
   bool na_faixa_dos_meses = (s >= MES && s < ANO);
   bool na_faixa_dos_anos = (s >= ANO && s < DECADA);
   bool na_faixa_das_decadas = (s >= DECADA && s < SECULO);
   bool na_faixa_dos_seculos = (s >= SECULO && s < MILENIO);

   if (s >= 1.0) {
      if (s < MINUTO)
         sprintf(resultado, "%0.1lf seg", s);

      else if (s >= MINUTO && s < HORA)
         sprintf(resultado, "%0.1lf min", s / MINUTO);

      else if (s >= HORA && s < DIA)
         sprintf(resultado, "%0.1lfh", s / HORA);

      else if (s >= DIA && s < MES)
         sprintf(resultado, "%0.1lf dias", s / DIA);

      else if (na_faixa_dos_meses)
         sprintf(resultado, "%0.1lf meses", s / MES);
      else if (na_faixa_dos_anos)
         sprintf(resultado, "%0.1lf anos", s / ANO);
      else if (na_faixa_das_decadas)
         sprintf(resultado, "%0.1lf déc", s / DECADA);
      else if (na_faixa_dos_seculos)
         sprintf(resultado, "%0.1lf séc", s / SECULO);
      else
         sprintf(resultado, "%0.1lf milênios", s / MILENIO);
   } else {
      if (s >= MILISEG)
         sprintf(resultado, "%0.0lf ms", s * 1000.0);
      else if (s >= MICROSEG)
         sprintf(resultado, "%0.0lf \u03bcs", s * powf(10, 6));
      else if (s >= NANOSEG)
         sprintf(resultado, "%0.0lf ns", s * powf(10, 9));
      else {
			// qualquer quantia abaixo disso, será considerado zero!
			strcpy (resultado, "0seg");
		}
   }
   return resultado;
}

char* tamanho_legivel(size_t bytes) { 
   // char* resultado_str = calloc(30, sizeof(char));
   char* resultado_str = nova_str(30);
   char peso_str[5];
   float valor;
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

   sprintf(resultado_str, "%0.1f %s", valor, peso_str);
   return (char*)resultado_str;
}

/* === === === === === === === === === === === === === === === === === ==
 *                Legibilidade do Valor Absoluto
 * === === === === === === === === === === === === === === === === === ==*/
char* valor_legivel_isize(int64_t unidades) 
{
   char* formatacao = malloc(15);
   // Apenas a magnitude do valor.
   int64_t m = unidades > 0 ? unidades: (-1) * unidades;
   char* peso;
   double potencia;

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

   double decimal = (double)unidades /(double)potencia;
   double inteiro = truncf(decimal);
   double fracao = fabs(inteiro - decimal);

   if (fracao < 0.09)
   // Não mostra a parte fracionaria do número se for insignificante.
      sprintf(formatacao, "%0.0lf%s", decimal, peso); 
   else
      sprintf(formatacao, "%0.1lf%s", decimal, peso);

   return formatacao;
}

char* valor_legivel_usize(size_t unidades) {
   char* peso;
   double potencia;
   char* resultado_str = malloc(15);
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

   double novo_valor = (double)unidades /(double)potencia;

   if (novo_valor < 1.0)
      sprintf(resultado_str, "%d%s", (uint16_t)novo_valor, peso);
   else
      sprintf(resultado_str, "%0.1f%s", novo_valor, peso);
   return resultado_str;
}

char* valor_legivel_f32 (float decimal) 
   { return valor_legivel_isize((int64_t)decimal); }

char* valor_legivel_f64 (double decimal)
   { return valor_legivel_isize((int64_t)decimal); }

/* === === === === === === === === === === === === === === === === === ==
 *                   Legibilidade do Tempo
 * === === === === === === === === === === === === === === === === === ==*/
char* tempo_legivel_usize(size_t seg) 
   { return tempo_legivel_double((double)seg); }

double timespec_to_seg(struct timespec a)
   { return (double)a.tv_sec + (double)a.tv_nsec / 1.0e9; }

char* tempo_legivel_timespec(struct timespec t)
   { return tempo_legivel_double(timespec_to_seg(t)); }

double timeval_to_seg(struct timeval e)
   { return (double)e.tv_sec + (double)e.tv_usec / 1.0e6; }

char* tempo_legivel_timeval(struct timeval t)
   { return tempo_legivel_double(timeval_to_seg(t)); }

/* === === === === === === === === === === === === === === === === === ===+
 * .......................................................................&
 * ........................Testes Unitários...............................&
 * .......................................................................&
 * === === === === === === === === === === === === === === === === === = */
#ifdef _UT_LEGIVEL
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
   for(int p = 0; p < 9; p++)
      printf(
         "%16lf ===> %s\n", 
         segundos[p], 
         tempo_legivel(segundos[p])
      );
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

int main(void) {
   executa_testes_a(
      true, 5, 
         legibilidade_do_tempo, true,
         legibilidade_de_tamanhos, true,
		   o_grosso_de_grande_valores, true,
         valor_legivel_de_todos_limites, true,
         tipos_de_tempo_diferentes_a_converter, true
	);
   return EXIT_SUCCESS;
}
#endif
