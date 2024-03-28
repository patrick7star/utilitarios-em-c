
#include <stdint.h>
#include <stdbool.h>
#include "../ponto.h"

#ifndef DADO_H
#define DADO_H
   struct dado { ArrayPonto dado; uint16_t qtd; };

   // para dados genéricos.
   typedef struct dado* Dado;
   typedef Dado* ArrayDado;

   // criação e liberação do 'Dado'.
   extern Dado cria_dado(ArrayPonto, uint16_t);
   extern void destroi_dado(Dado d); 
   extern Dado clona_dado(Dado d);
   extern Dado cria_dado_branco();

   /* cria uma array, inicialmente 'null'
    * do tipo de dado acima. */
   extern Dado* cria_array_dado(uint64_t n); 
   extern void destroi_array_dado(Dado* a, uint64_t q);

   // verifica se dois dados são iguais.
   extern bool dados_iguais(Dado a, Dado b);

   // pode imprimir o dado e transforma-lô numa string.
   extern void visualiza_dado(Dado d); 
   extern char* dado_para_string(Dado d);

   // contabilização de instâncias deste tipo de dado.
   extern uint64_t instancias_dado();

#endif // DADO_H
