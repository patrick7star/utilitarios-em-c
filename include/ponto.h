

/* computar a dimensão do terminal e seus 
 * derivados. Também inputs personalizados.
 */
#include <stdint.h>
#include <stdbool.h>
#include "terminal.h"

#ifndef TEXTO_PONTO_H
# define TEXTO_PONTO_H

 /* Representação das coordenadas 'y' e 'x', nesta ordem. */
 typedef Dimensao Ponto, PONTO, ponto_t;
 // uma array do tipo Ponto, seu alocador e destruídor. 
 typedef Ponto* ARRAY_PONTO;
 typedef ARRAY_PONTO ArrayPonto;

 /* cria e destrói o 'Ponto'. */
 extern Ponto cria_ponto(uint8_t y, uint8_t x);
 extern void destroi_ponto(Ponto p);
 extern Ponto clona_ponto(Ponto);

  // verifica se o ponto A é menor ou igual ao ponto B.
 extern bool eq_ponto(Ponto, Ponto); 

 /* todas tipos de visualização do 'Ponto'. */
 extern void visualiza_ponto(Ponto p);
 extern void visualiza_ponto_debug(Ponto p);
 // formatação string de tal.
 extern char* ponto_to_str(Ponto p);

 // a menor distância entre os dois pontos.
 extern uint8_t distancia_ponto(Ponto, Ponto); 


 extern ARRAY_PONTO cria_array_de_pontos(uint16_t); 
 extern void destroi_array_de_pontos(ARRAY_PONTO, uint16_t); 
 extern ARRAY_PONTO cria_manualmente_ap(uint8_t, ...);

   /* retorna todos pontos que formam um retângulo, dado dois pontos.
    * Tais pontos são organizados do canto superior esquerdo com
    * enumeração no sentido anti-horário à partir deste ponto até
    * o inferior esquerdo. */
 extern ArrayPonto retangulo_vertices(Ponto p, Ponto q); 

#endif  // PONTO(ADT)

