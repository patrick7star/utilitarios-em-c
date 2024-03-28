
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifndef BP_PROGRESSO_TEMPORAL
#define BP_PROGRESSO_TEMPORAL
 // como o objeto é apelidade, suas versões curtas e longa.
 typedef struct progresso* ProgressoSimples;
 typedef ProgressoSimples PS;

 // criação e destruição do objeto.
 PS novo_bps(size_t total, uint8_t qb);
 void destroi_bps(PS);

 // impressão da barra, quando necessário.
 void visualiza_bps(PS p);

 // atualiza o valor que está o progresso.
 void atualiza_bps(PS, size_t);

 // verifica se o temporizador está terminado.
 bool esgotado_bps(PS);

 /* Barra de progresso temporal, ou seja, a visualização é liberada à cada
  * fração de tempo decorrida. Todos métodos dele incluído abaixo também:
  */
 typedef struct progresso_tempo *ProgressoTemporal, *PT;

 // métodos de alocação e desalocação da estrutura de dados.
 PT novo_bpt (size_t, uint8_t); 
 PT cria_bpt (size_t);
 void destroi_bpt (PT);

 // visualização e atualização do estado atual da 'barra'.
 bool esgotado_bpt (PT);
 void visualiza_bpt (PT);
 void atualiza_bpt (PT, size_t);
 // ambas combinações acima.
 void visualiza_e_atualiza_bpt (PT, size_t);

#endif // BP_PROGRESSO
