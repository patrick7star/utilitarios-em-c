
#include <stdbool.h>
#include <stdint.h>

#ifndef LEGIVEL_H
#define LEGIVEL_H

   // converte o tempo em segundos para uma forma mais legível.
   extern char* tempo_legivel(double segundos);

   // converte o tamanho(em bytes) para uma forma mais humamente legível.
   extern char* tamnanho_legivel(uint64_t bytes); 

   // coloca um valor de modo mais legivel.
   extern char* valor_legivel(uint64_t); 

#endif //LEGIVEL_H
