/* Declaração das funções e dados abstratos deste módulo. */

/* Implementação da famosa barra de progresso em C. Não será a implementação
 * completa. */

#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>

#ifndef PROGRESSO_H
#define PROGRESSO_H
// Criando uma interface comum para as barras de progressos.
enum todas_barras_de_progressos { Simples, Temporal };
typedef enum todas_barras_de_progressos TipoDeProgresso;

// Nomes dos pointeiros de cada tipo, seus nomes completos e abreviações:
typedef struct progresso_simples ProgressoSimples, PS, *RefPS;
typedef struct progresso_tempo *RefPT, ProgressoTemporal, PT;
typedef struct barra_de_progresso_generica PG, *RefPG;

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

#endif