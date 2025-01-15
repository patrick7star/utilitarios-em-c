#ifndef __PONTO_H__
#define __PONTO_H__
#define DLL_EXPORT __declspec(dllexport)
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

 typedef struct coordenada_bidimensional { uint16_t y; uint16_t x; }
   Ponto, PONTO, ponto_t;
 typedef Ponto* ArrayPonto;
 typedef array_de_pontos { Ponto* array; size_t length; } ArrayPonto;

 DLL_EXPORT Ponto cria_ponto (uint16_t y, uint16_t x);
 DLL_EXPORT ArrayPonto cria_array_ponto (size_t qtd, ...);
 DLL_EXPORT bool eq_ponto (Ponto, Ponto); 
 DLL_EXPORT bool ne_ponto (Ponto, Ponto);
 DLL_EXPORT void imprime_ponto (Ponto);
 DLL_EXPORT void imprime_ponto_debug (Ponto);
 DLL_EXPORT char* ponto_to_str (Ponto);
 DLL_EXPORT uint16_t distancia_ponto(Ponto, Ponto); 
 DLL_EXPORT ArrayPonto retangulo_vertices(Ponto p, Ponto q); 
 DLL_EXPORT void imprime_array_ponto (ArrayPonto, size_t); 

#endif  // PONTO(ADT)

