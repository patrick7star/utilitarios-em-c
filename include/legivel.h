/* Grandezas mais legíveis:
 * Faz conversões de grandezas referentes a dados utilizados em computação,
 * ou outros campos. 
 */

#ifndef LEGIVEL_H
#define LEGIVEL_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
// Biblioteca do Unix/Linux:
#ifdef __linux__
#include <time.h>
#include <sys/time.h>
#endif

/* Fazendo a biblioteca multiplataforma. Isso é importante aqui, pois faz
 * tal lib também utilizável no Windows. */
#if defined(_WIN32)
  // Importantes definições para funcionar no Windows.
  #define _USE_MATH_DEFINES
  #ifdef __LIB_DLL__
    #define CROSSLIB __declspec(dllexport)
  #else
    #define CROSSLIB __declspec(dllimport)
  #endif
#elif defined(__linux__)
    #define CROSSLIB 
#endif


 // Converte o tamanho(em bytes) para uma forma mais humamente legível.
 CROSSLIB char* tamanho_legivel(size_t bytes); 

 // Coloca um valor de modo mais legivel.
 CROSSLIB char* valor_legivel_usize (size_t); 
 CROSSLIB char* valor_legivel_isize (int64_t);
 CROSSLIB char* valor_legivel_f32   (float);
 CROSSLIB char* valor_legivel_f64   (double);

 // Função macro que aborda todos tipos de entradas:
 #define valor_legivel(U) _Generic((U),      \
   int64_t:  valor_legivel_isize,            \
   int:      valor_legivel_isize,            \
   size_t:   valor_legivel_usize,            \
   uint32_t: valor_legivel_usize,            \
   float:    valor_legivel_f32,              \
   double:   valor_legivel_f64               \
 )(U)

 // Converte o tempo em segundos para uma forma mais legível.
 CROSSLIB char* tempo_legivel_double   (double seg);
 CROSSLIB char* tempo_legivel_usize    (size_t seg);
#ifdef __linux__
 CROSSLIB char* tempo_legivel_timespec (struct timespec t);
 CROSSLIB char* tempo_legivel_timeval  (struct timeval t);
#endif


#ifdef __linux__
#define tempo_legivel(T) _Generic((T),       \
   struct timespec: tempo_legivel_timespec,  \
   struct timeval: tempo_legivel_timeval,    \
   double:         tempo_legivel_double,     \
   size_t:         tempo_legivel_usize       \
)(T)
#elif defined(_WIN32)
 #define tempo_legivel(T) _Generic((T),       \
    double:         tempo_legivel_double,     \
    size_t:         tempo_legivel_usize       \
 )(T)
#endif

 /* Constante de peso, tempo e tamanho, definidas e utilizadas, ao longo do 
  * programa. Que podem servir prá quem desejar tal valor aproximado. */
 extern const size_t MILHAR; 
 extern const size_t MILHAO;
 extern const size_t BILHAO;
 extern const size_t TRILHAO;
 extern const size_t QUADRILHAO;
 extern const size_t QUINTILHAO;

 extern const double NANOSEG;
 extern const double MICROSEG;
 extern const double MILISEG;
 extern const double MINUTO;
 extern const double HORA;
 extern const double DIA;
 extern const double MES;
 extern const double ANO;
 extern const double DECADA;
 extern const double SECULO;
 extern const double MILENIO;

 extern const size_t KILO;
 extern const size_t MEGA;
 extern const size_t GIGA;
 extern const size_t TERA;
 extern const size_t PETA;

#endif //LEGIVEL_H
