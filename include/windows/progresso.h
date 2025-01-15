/* Declaração das funções e dados abstratos deste módulo. */

/* Implementação da famosa barra de progresso em C. Não será a implementação
 * completa. */


#ifndef __PROGRESSO_H__
#define __PROGRESSO_H__
#define EXPORT __declspec (dllexport)
#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>
#include <time.h>

 typedef enum todas_barras_de_progressos { Simples, Temporal } TipoDeProgresso;

 struct _progresso_simples {
   /* valores atual e o que têm que ser atingido para ser finalizado. */
   size_t atual;
   size_t total;

   // quantos blocos de comprimento têm a 'barra'.
   uint8_t comprimento;

   /* marca a atual porcentagem do 'progresso' para computar a variação. */
   float marco;
 }; 

 struct _progresso_tempo {
   /* valores atual e o que têm que ser atingido para ser finalizado. */
   size_t atual; size_t total;

   // informa se o progresso está esgotado.
   bool esgotado;

   // quantos blocos de comprimento têm a 'barra'.
   uint8_t comprimento;

   /* marcador da variação de tempo.*/
   time_t inicio;
 }; 

 struct _barra_de_progresso_generica {
   // Identificação do tipo de progresso.
   TipoDeProgresso classe;

   // Usará apenas a memória do selecionado.
   union _progresso_escolhido {
      struct _progresso_simples simples; 
      struct _progresso_tempo temporal; 
   } progresso;
 }; 
 
 typedef enum todas_barras_de_progressos TipoDeProgresso;
 typedef struct _progresso_simples ProgressoSimples, PS, *RefPS;
 typedef struct _progresso_tempo ProgressoTemporal, PT, *RefPT;
 typedef struct _barra_de_progresso_generica PG, *RefPG;

 EXPORT PS cria_bps(size_t, uint8_t);
 EXPORT PS cria_padrao_bps (size_t);
 EXPORT bool esgotado_bps (RefPS);
 EXPORT void visualiza_bps(RefPS);
 EXPORT void atualiza_bps(RefPS, size_t);

 EXPORT PT cria_bpt (size_t, uint8_t);
 EXPORT PT cria_padrao_bpt (size_t);
 EXPORT void visualiza_bpt (RefPT);
 EXPORT void atualiza_bpt (RefPT, size_t);
 EXPORT void visualiza_e_atualiza_bpt (RefPT, size_t);
 EXPORT bool esgotado_bpt (RefPT);

 EXPORT PG cria_bp(TipoDeProgresso, size_t, uint8_t);
 EXPORT bool esgotado_bp(RefPG a);
 EXPORT bool atualiza_bp(RefPG a, size_t v);
 EXPORT void visualiza_bp(RefPG a);
 EXPORT bool atualiza_e_visualiza_bp(RefPG b, size_t v);

 EXPORT PG new_bp (TipoDeProgresso, size_t, uint8_t);
 EXPORT bool finished_bp (RefPG);
 EXPORT bool update_bp (RefPG, size_t);
 EXPORT void print_bp (RefPG);
 EXPORT bool update_e_print_bp (RefPG, size_t);

#endif
