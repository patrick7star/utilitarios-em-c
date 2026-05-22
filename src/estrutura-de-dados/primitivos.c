#include "primitivos.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

size_t hash_string(GenT s, size_t cp)
{
   size_t t = strlen(s);
   char* string;
   size_t x = 0, y = 0, z = SIZE_MAX;

   if (t >= 3)
   {
      string = s;
      x = (size_t)string[t / 2];
      y = (size_t)string[t - 1];
      z = (size_t)string[0];
   }

   return (t*x*y*z + (t + y + z+ x)) % cp;
}

bool eq_string(GenT a, GenT b)
{
   char* str_a = a;
   char* str_b = b;

   return strcmp(str_a, str_b) == 0;
}

size_t hash_u64 (GenT key, size_t cp)
   { return *((uint64_t*)key) % cp; }

bool eq_u64(GenT a, GenT b)
   { return *((uint64_t*)a) == *((uint64_t*)b); }

size_t hash_i64 (GenT key, size_t cp)
   { return *((int64_t*)key) % cp; }

bool eq_i64(GenT a, GenT b)
   { return *((int64_t*)a) == *((int64_t*)b); }

size_t hash_u32 (GenT key, size_t cp)
   { return *((uint32_t*)key) % cp; }

bool eq_u32(GenT a, GenT b)
   { return *((uint32_t*)a) == *((uint32_t*)b); }

size_t hash_i32 (GenT key, size_t cp)
   {  return *((int32_t*)key) % cp;  }

bool eq_i32(GenT a, GenT b)
   { return *((int*)a) == *((int*)b); }

size_t hash_u16(GenT a, size_t cp)
   { return *((uint16_t*)a) % cp; }

bool eq_u16(GenT a, GenT b)
   { return *((uint16_t*)a) == *((uint16_t*)b); }

size_t hash_i16(GenT a, size_t cp)
   { return *((int16_t*)a) % cp; }

bool eq_i16(GenT a, GenT b)
   { return *((int16_t*)a) == *((int16_t*)b); }

size_t hash_u8 (GenT key, size_t cp)
   { return *((uint8_t*)key) % cp; }

bool eq_u8(GenT a, GenT b)
   { return *((uint8_t*)a) == *((uint8_t*)b); }

size_t hash_i8(GenT a, size_t cp)
   { return *((int8_t*)a) % cp; }

bool eq_i8(GenT a, GenT b)
   { return *((int8_t*)a) == *((int8_t*)b); }

size_t hash_bool(GenT x, size_t cp)
   { return 0 ? *((bool*)x): 1; }

bool eq_bool(GenT a, GenT b)
   { return *((bool*)a) == *((bool*)b); }

size_t hash_char(GenT x, size_t cp)
   { return hash_i8(x, cp); }

bool eq_char(GenT a, GenT b)
   { return eq_i8(a, b); }

size_t hash_byte(GenT x, size_t cp)
   { return hash_u8(x, cp); }

bool eq_byte(GenT a, GenT b)
   { return eq_u8(a, b); }

bool eq_sizet(GenT a, GenT b)
   { return eq_u64(a, b); }

size_t hash_sizet (GenT key, size_t cp)
   { return hash_u64(key, cp); }

// Apelido para destacar o primitivo.
typedef size_t SizeT;

float hash_float(GenT key, SizeT cp)
// Como tipos de dados tem o mesmo tamanho, e nunca haverá uma interseção entre
// entre eles na abstração, podemos usar o mesmo tipo de hash.
   { return hash_u32(key, cp); }

float hash_double(GenT key, SizeT cp)
// Como tipos de dados tem o mesmo tamanho, e nunca haverá uma interseção entre
// entre eles na abstração, podemos usar o mesmo tipo de hash.
   { return hash_u64(key, cp); }

/* Parte direcionada as versões de debug e display dos dados primitivos.
 * Também ajuda na impressão destas estruturas, na verdade, elas são ensenciais
 * para tal, sem isso, sequer funcionam.
 */
#define DIGITOS_BUFFER 30
#define size sizeof(char)

char* debug_char(GenT x) {
   /* A expressão diz por sí: um caractére, mais o espaço nulo com mais duas
    * àspas simples. */
   char* fmt = malloc(1 + 1 + 2 * size);
   char* vl = (char*)x;

   // Caractére passado, então o caractére nulo que marca o fim da string.
   *(fmt + 0) = '\'';
   *(fmt + 1) = *vl;
   *(fmt + 2) = '\'';
   *(fmt + 3) = 0x00;

   return fmt;
}

char* debug_u8(GenT x) {
   char* resultado_fmt = malloc(4 * size);
   uint8_t* valor = (uint8_t*)x;

   sprintf(resultado_fmt, "%3u", *valor);
   return resultado_fmt;
}

char* debug_i8(GenT x) {
   // Três dígitos, mais o sinal, mais caractére nulo.
   const int DIGITOS = 3 + 1 + 1;
   char* resultado_fmt = malloc(DIGITOS * size);
   int8_t* valor = (int8_t*)x;

   sprintf(resultado_fmt, "%3i", *valor);
   return resultado_fmt;
}

char* debug_string(GenT x) {
   char* fonte = x;
   /* O '+1' e '+2' significam espaço para o caractére nulo e duas àspas,
    * respectivamente. */
   int t = strlen(fonte) + 1 + 2;
   char* saida = malloc(t * size);

   sprintf(saida, "\"%s\"", fonte);
   return saida;
}

char* debug_u16(GenT x) {
   /* O sete não foi uma escolha árbitraria, ele assume a quantia de caractéres
    * máximo(de 65000 aproximando, não sei o exato) mais o sinal, e ainda mais
    * o caractére nulo. */
   const int DIGITOS = 5 + 1 + 1;
   char* fmt = malloc(DIGITOS * size);
   uint16_t* ptr = x;
   uint16_t valor = *ptr;

   sprintf(fmt, "%u", valor);
   return fmt;
}

char* debug_i16(GenT x) {
   char* fmt = malloc(7 * size);
   int16_t* ptr = x;
   int16_t valor = *ptr;

   sprintf(fmt, "%d", valor);
   return fmt;
}

char* debug_u32(GenT x) {
   // Contando a quantia de digitos do INT_MAX, sinal e caractére nulo.
   const int DIGITOS = 9 + 1 + 1;
   char* fmt = malloc(DIGITOS * size);
   uint16_t* ptr = x;
   uint16_t valor = *ptr;

   sprintf(fmt, "%u", valor);
   return fmt;
}

char* debug_i32(GenT x) {
   const int DIGITOS = 9 + 1 + 1;
   char* fmt = malloc(DIGITOS * size);
   int32_t* ptr = x;
   int32_t valor = *ptr;

   sprintf(fmt, "%d", valor);
   return fmt;
}

char* debug_u64(GenT x) {
   /* Como não tenho ideia a quantia de dígitos, então apenas pega um valor
    * razoável para capturar qualquer "amplitude" que possa vir. */
   const int TAMANHO = DIGITOS_BUFFER * size;
   char* fmt = malloc(TAMANHO);
   uint64_t* ptr = x;
   uint64_t v = *ptr;

   sprintf(fmt, "%lu", v);
   return fmt;
}

char* debug_i64(GenT x) {
   /* Como não tenho ideia a quantia de dígitos, então apenas pega um valor
    * razoável para capturar qualquer "amplitude" que possa vir. */
   const int TAMANHO = DIGITOS_BUFFER * size;
   char* fmt = malloc(TAMANHO);
   int64_t* ptr = x;
   int64_t v = *ptr;

   sprintf(fmt, "%ld", v);
   return fmt;
}

char* debug_f32(GenT x) {
   const int TAMANHO = DIGITOS_BUFFER * size;
   char* fmt = malloc(TAMANHO);
   float* ptr = x;
   float v = *ptr;

   sprintf(fmt, "%f", v);
   return fmt;
}

char* debug_f64(GenT x) {
   const int TAMANHO = DIGITOS_BUFFER * size;
   char* fmt = malloc(TAMANHO);
   double* ptr = x;
   double v = *ptr;

   sprintf(fmt, "%f", v);
   return fmt;
}

char* debug_void(GenT x) {
   const int TAMANHO = DIGITOS_BUFFER * size;
   char* fmt = malloc(TAMANHO);

   sprintf(fmt, "%p", x);
   return fmt;
}
/* Nomes mais apropriados do C. Será apenas um "embrulho" das chamadas acima.
 * Logo, deixarei a implementação a mais compacta o possível. Nem todas terão,
 * a medida que uso com frequência, ou terceiros usam tal biblioteca, irei
 * colocando mais, por enquanto, só isto está ótimo!
 */
char* debug_int(GenT x) { return debug_i32(x); }
char* debug_long(GenT x) { return debug_i64(x); }
char* debug_short(GenT x) { return debug_i16(x); }
char* debug_double(GenT x) { return debug_f64(x); }
char* debug_float(GenT x) { return debug_f32(x); }

