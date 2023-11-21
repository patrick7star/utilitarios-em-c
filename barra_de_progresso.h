
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifndef BP_PROGRESSO_TEMPORAL
#define BP_PROGRESSO_TEMPORAL
   // como o objeto é apelidade, suas versões curtas e longa.
   typedef struct progresso_tempo* ProgressoTemporal;
   typedef struct progresso_tempo* PT;
   typedef struct progresso* ProgressoSimples;
   typedef ProgressoSimples PS;

   // criação e destruição do objeto.
   extern PT novo_bpt(uint64_t total, uint8_t qb); 
   extern PS novo_bps(uint64_t total, uint8_t qb);
   extern void destroi_bps(PS);
   extern void destroi_bpt(PT);

   // impressão da barra, quando necessário.
   extern void visualiza_bpt(PT p);
   extern void visualiza_bps(PS p);

   // atualiza o valor que está o progresso.
   extern void atualiza_bpt(PT, uint64_t);
   extern void atualiza_bps(PS, uint64_t);

   // verifica se o temporizador está terminado.
   extern bool esgotado_bps(PS);
   extern bool esgotado_bpt(PT);

#endif // BP_PROGRESSO
