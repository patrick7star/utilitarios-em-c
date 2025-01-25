/* Grandezas mais legíveis:
 * Faz conversões de grandezas referentes a dados utilizados em computação,
 * ou outros campos. 
 */

#ifndef __LEGIVEL_H__
#define __LEGIVEL_H__
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
// Biblioteca do Unix/Linux:
#include <time.h>
#include <sys/time.h>

 // converte o tempo em segundos para uma forma mais legível.
 extern char* tempo_legivel(double seg);

 // converte o tamanho(em bytes) para uma forma mais humamente legível.
 extern char* tamanho_legivel(size_t bytes); 

 // coloca um valor de modo mais legivel.
 // char* valor_legivel(size_t units); 
 static char* valor_legivel_usize (size_t); 
 static char* valor_legivel_isize   (int64_t);
 static char* valor_legivel_f32   (float);

 // Função macro que aborda todos tipos de entradas:
 #define valor_legivel(U) _Generic((U),      \
   int64_t: valor_legivel_isize,             \
   int: valor_legivel_isize,                 \
   size_t: valor_legivel_usize,              \
   uint32_t: valor_legivel_usize,            \
   float: valor_legivel_f32,                 \
   double: valor_legivel_f64                 \
 )(U)

 static char* tempo_legivel_usize(size_t seg);
 static char* tempo_legivel_timespec(struct timespec t);
 static char* tempo_legivel_timeval(struct timeval t);

#define tempo_legivel(T) _Generic((T), \
   struct timespec: tempo_legivel_timespec, \
   struct timeval: tempo_legivel_timeval, \
   double: tempo_legivel_double, \
   size_t: tempo_legivel_usize \
)(T)

#endif //LEGIVEL_H
