
#ifndef __IMPRESSAO_H__
#define __IMPRESSAO_H__
#include "listaarray_ref.h"
#include <stdint.h>

 /* Dado uma lista contento itens que podem ser impressos pela função 'f',
  * então será arranjados na tela de modo a otimizar o espaço. */
 void listar (vetor_t* lista, ToString f); 

 /* Impressão de arrays dos tipos mais básicos da linguagem, dado o seu
  * comprimento. */
 void imprime_array_int    (int* array, int length);
 void imprime_array_bool   (bool* array, int length);
 void imprime_array_float  (float* array, int length);
 void imprime_array_double (double* array, int length);

 // Apenas referentes a strings de qualquer tipo:
 void imprime_array_char         (char*    array,   int length);
 void imprime_array_str          (char*    array[], int total);
 void imprime_array_char_unicode (wchar_t* array,   int length);
 void imprime_array_str_unicode  (wchar_t* array[], int total);

 /* Todas variações de inteiros que existem: */
 void imprime_array_i8  (int8_t*    array, int length);
 void imprime_array_u8  (uint8_t*   array, int length);
 void imprime_array_u16 (uint16_t*  array, int length);
 void imprime_array_i16 (int16_t*   array, int length);
 void imprime_array_u32 (uint32_t*  array, int length);
 void imprime_array_u64 (uint64_t*  array, int length);
 void imprime_array_i64 (int64_t*   array, int length);

 /* Mesmo que todas acimas, porém aceita qualquer tipo(include elas). */
 #define imprime_array(A, N) _Generic((A), \
   double*: imprime_array_double, \
   float*:  imprime_array_float, \
   int*:    imprime_array_int, \
   char*:   imprime_array_char, \
   bool*:   imprime_array_bool, \
   char**:   imprime_array_str, \
   \
   int8_t*:   imprime_array_i8, \
   uint8_t*:  imprime_array_u8, \
   int16_t*:  imprime_array_i16, \
   uint16_t*: imprime_array_u16, \
   uint32_t*: imprime_array_u32, \
   int64_t*:  imprime_array_i64, \
   uint64_t*: imprime_array_u64 \
 )(A, N)
 // Mesmo macro acima, porém com um nome diferente, em inglês.
#define print_array(A, N) imprime_array(A, N)

#endif
