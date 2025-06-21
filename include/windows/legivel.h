/* Grandezas mais legíveis:
 * Faz conversões de grandezas referentes a dados utilizados em computação,
 * ou outros campos. 
 */

#ifndef __LEGIVEL_H__
#define __LEGIVEL_H__
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

 // Converte o tamanho(em bytes) para uma forma mais humamente legível.
 extern char* tamanho_legivel(size_t bytes); 

 // Coloca um valor de modo mais legivel.
 char* valor_legivel_usize (size_t); 
 char* valor_legivel_isize (int64_t);
 char* valor_legivel_f32   (float);
 char* valor_legivel_f64   (double);

 // Função macro que aborda todos tipos de entradas:
 #define valor_legivel(U) _Generic((U),      \
   int64_t:  valor_legivel_isize,            \
   int:      valor_legivel_isize,            \
   int16_t:	 valor_legivel_isize,				\
   uint64_t: valor_legivel_usize,				\
   uint32_t: valor_legivel_usize,				\
   uint16_t: valor_legivel_usize,				\
   float:    valor_legivel_f32,              \
   double:   valor_legivel_f64               \
 )(U)

 // Converte o tempo em segundos para uma forma mais legível.
 char* tempo_legivel_double   (double seg);
 char* tempo_legivel_usize    (size_t seg);

#define tempo_legivel(T) _Generic((T),       \
   double:         tempo_legivel_double,     \
   size_t:         tempo_legivel_usize       \
)(T)

#endif //LEGIVEL_H
