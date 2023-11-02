
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifndef BP_PROGRESSO_TEMPORAL
#define BP_PROGRESSO_TEMPORAL
   /* A impressão da 'barra' se dará passado
    * um determinado tempo. Tudo isso para
    * minizar a impressão descontrolada que
    * toma muito do CPU. */
   struct progresso_tempo {
      /* valores atual e o que têm que ser 
       * atingido para ser finalizado. */
      uint64_t atual;
      uint64_t total;
      // informa se o progresso está esgotado.
      bool esgotado;
      // quantos blocos de comprimento têm a 'barra'.
      uint8_t comprimento;
      /* marcador da variação de tempo.*/
      time_t inicio;
   };
   // chamndo objeto de:
   typedef struct progresso_tempo* ProgressoTemporal;
   // apelido mais curto para futuras instancias.
   typedef struct progresso_tempo* PT;

   extern PT novo_bpt(uint64_t total, uint8_t qb); 

   // impressão da barra, quando necessário.
   extern void visualiza_bpt(PT p);

   // atualiza o valor que está o progresso.
   extern void atualiza_bpt(PT p, uint64_t novo);
#endif // BP_PROGRESSO_TEMPORAL

#ifndef BP_PROGRESSO
#define BP_PROGRESSO
   struct progresso {
      /* valores atual e o que têm que ser 
       * atingido para ser finalizado. */
      uint64_t atual;
      uint64_t total;
      // informa se o progresso está esgotado.
      bool esgotado;
      // quantos blocos de comprimento têm a 'barra'.
      uint8_t comprimento;
      /* marca a atual porcentagem do 'progresso'
       * para computar a variação. */
      float marco;
   };
   // chamndo objeto de:
   typedef struct progresso* ProgressoSimples;
   // um mais curto para facilitar codifição e legibilidade.
   typedef ProgressoSimples PS;

   // criação do progresso da barra.
   extern PS novo_bps(uint64_t total, uint8_t qb);

   // impressão da barra, quando necessário.
   extern void visualiza_bps(PS p);

   // atualiza o valor que está o progresso.
   extern void atualiza_bps(PS p, uint64_t novo);
#endif // BP_PROGRESSO
