

/* computar a dimensão do terminal e seus 
 * derivados. Também inputs personalizados.
 */
#include <stdint.h>
#include <stdbool.h>
#include "terminal.h"
#include "ponto.h"

#ifndef TELA_H
#define TELA_H
   // nome curto do tipo de dado.
   typedef struct tela* TELA;
   typedef TELA Tela;

   enum sentido { Horizontal, Vertical };
   typedef enum sentido SENTIDO;

   // alocação de liberação da estrutura do tipo Tela.
   extern TELA cria_tela(); 
   extern void destroi_tela(TELA t); 

   // visualização dela.
   extern void visualiza_tela_debug(TELA t);

   /* Faz um risco de um ponto ao outro, seja vertical, horizontal ou
    * diagonal. Se algum ponto estiver fora da dimensão da tela, então
    * nada é feito.
    */
   extern void risca_tela(TELA t, Ponto A, Ponto B); 
   extern void circunscreve_retangulo(Tela, Ponto, Ponto);
   extern void escreve_string(Tela, Ponto, char*, enum sentido);
   extern bool desfaz_alteracao(Tela t); 
   extern Ponto centro_tela(Tela t);
   extern bool marca_ponto(Tela t, Ponto a, unsigned char simbolo); 
   ArrayPonto espiral(Ponto p, uint8_t T);

   /* a contagem de alterações feitas numa Tela; e também o número 
    * ativo delas. */
   extern uint16_t alteracoes_tela(Tela t);
   extern uint16_t instancias_tela(void);

#endif  // TELA(ADT)

