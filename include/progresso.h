/* Declaração das funções e dados abstratos deste módulo. */

/* Implementação da famosa barra de progresso em C. Não será a implementação
 * completa. */

#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>
#include <time.h>

#ifndef PROGRESSO_H
#define PROGRESSO_H
 // Criando uma interface comum para as barras de progressos.
 typedef enum todas_barras_de_progressos { Simples, Temporal } TipoDeProgresso;

 /* Definição das estruturas de várias barras-de-progresso: */
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
 
 /* Apelido mais legível de todos nomes que a estruturas acima assumem,
  * e claro, suas referências(ponteiros). */
 typedef enum todas_barras_de_progressos TipoDeProgresso;
 typedef struct _progresso_simples ProgressoSimples, PS, *RefPS;
 typedef struct _progresso_tempo ProgressoTemporal, PT, *RefPT;
 typedef struct _barra_de_progresso_generica PG, *RefPG;

 // Barra de Progresso Simples:
 ProgressoSimples cria_bps(size_t, uint8_t);
 // Cria a barra, porém promove um comprimento padrão a ela.
 ProgressoSimples cria_padrao_bps (size_t);
 bool esgotado_bps (RefPS);
 void visualiza_bps(RefPS);
 void atualiza_bps(RefPS, size_t);

 // Barra de Progresso Temporal:
 ProgressoTemporal cria_bpt (size_t, uint8_t);
 ProgressoTemporal cria_padrao_bpt (size_t);
 void visualiza_bpt (RefPT);
 void atualiza_bpt (RefPT, size_t);
 void visualiza_e_atualiza_bpt (RefPT, size_t);
 bool esgotado_bpt (RefPT);

 // Barra de Progresso Genérica:
 PG cria_bp(TipoDeProgresso, size_t, uint8_t);
 bool esgotado_bp(RefPG a);
 bool atualiza_bp(RefPG a, size_t v);
 void visualiza_bp(RefPG a);
 // O retorno é referente a visualização.
 bool atualiza_e_visualiza_bp(RefPG b, size_t v);

 /* Versão em inglês da 'barra genérica' e seus métodos também. */
 PG new_bp (TipoDeProgresso, size_t, uint8_t);
 bool finished_bp (RefPG);
 bool update_bp (RefPG, size_t);
 void print_bp (RefPG);
 bool update_e_print_bp (RefPG, size_t);

#endif
