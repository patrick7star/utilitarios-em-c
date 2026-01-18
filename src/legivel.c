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

#ifdef _WIN32
CROSSLIB size_t filetime_to_usize (FILETIME v)
{
   ULARGE_INTEGER output = {
      .u = {
         v.dwLowDateTime,
         v.dwHighDateTime
      }
   };
   return output.QuadPart;
}

CROSSLIB char* tempo_legivel_filetime (FILETIME x)
{
   size_t nanos = filetime_to_usize(x);
   // Converte os intervalos de 100ns para segundos.
   size_t segs = nanos / 1e6L;

   return tempo_legivel_usize(segs);
}
#endif

/* === === === === === === === === === === === === === === === === === ===+
 * .......................................................................&
 * ........................Testes Unitários...............................&
 * .......................................................................&
 * === === === === === === === === === === === === === === === === === = */
#if defined(__unit_tests__) && defined(__linux__)
 #include "legivel/testes-unix.c"
#elif defined(_WIN32) && defined(__unit_tests__)
 #include "legivel/testes-win.c"
#endif
