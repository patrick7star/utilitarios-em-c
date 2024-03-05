
#include <stdbool.h>

#ifndef LEGIVEL_H
#define LEGIVEL_H

 // converte o tempo em segundos para uma forma mais legível.
 extern char* tempo_legivel(double segundos);

 // converte o tamanho(em bytes) para uma forma mais humamente legível.
 extern char* tamanho_legivel(size_t); 

 // coloca um valor de modo mais legivel.
 extern char* valor_legivel(size_t); 

#endif //LEGIVEL_H
