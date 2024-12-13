
#ifndef __IMPRESSAO_H__
#define __IMPRESSAO_H__
#include "listaarray_ref.h"

 /* Dado uma lista contento itens que podem ser impressos pela função 'f',
  * então será arranjados na tela de modo a otimizar o espaço. */
 void listar (vetor_t* lista, ToString f); 

 /* Impressão de arrays dos tipos mais básicos da linguagem, dado o seu
  * comprimento. */
 void imprime_array_int    (int* array, int length);
 void imprime_array_bool   (bool* array, int length);
 void imprime_array_char   (char* array, int length);
 void imprime_array_str    (char* array[], int total);
 void imprime_array_float  (float* array, int length);
 void imprime_array_double (double* array, int length);

 /* Mesmo que todas acimas, porém aceita qualquer tipo(include elas). */
 #define imprime_array(A, N) _Generic((A), \
   double*: imprime_array_double, \
   float*:  imprime_array_float, \
   int*:    imprime_array_int, \
   char*:   imprime_array_char, \
   bool*:   imprime_array_bool, \
   char**:   imprime_array_str \
 )(A, N)

#endif
