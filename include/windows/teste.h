#ifndef __TESTE_H__
#define __TESTE_H__
#include <stdbool.h>
#include <stdint.h>
#include "../definicoes.h"

// nome mais curto, estético e significativo para um ponteiro de
// de função.
 typedef void(*Fn)(void);
 // Tipo de função especial para testes-unitários.
 typedef int (*FnTst) (void);

 extern void debug_aqui(void);

/* Endereço de memória da função a executar; e sua definação,... se é para
 * executa-la?! */
struct TesteConfig { const char* nome; Fn rotina; bool ativado; };

#define Unit(FUNCAO, ESTADO) (  \
   (struct TesteConfig) {        \
      .nome    = #FUNCAO,        \
      .rotina  = FUNCAO,         \
      .ativado = ESTADO          \
   }                             \
)

/* Executa variados testes passados. Os parâmetros seguem a seguinte
 * lógica: o total de argumentos totais, aí começa a alternância
 * entre o endereço da funçõa a ser executada, a confirmação se você
 * deseja realmente executa-lô; um bocado de função seguida do valor
 * verdade da sua execução, descotando o primeiro, que é o total de
 * funções e valores-verdades. Exemplo:
 *
 *    (total, fn1, false, fn2, true, fn3, true, fn4, false, ...)
 *
 * Lembre-se que todos pares tem que bater com a seguinte função e
 * valor verdade, além de bate com o total de argumentos declarados
 * inicialmente.
 *   Na versão do Windows irei inovar, e sempre colocar apenas uma função 
 * que executa testes, portanto, sem as antigas versões.
 */
 void executa_testes  (bool ativado, int total, ...);

/* Converte um valor lógico para uma string, em português, dizendo o que
 * ele representa. Nome similires, mas fazem a mesma coisas também estão
 * dados abaixo, por motivos de compatibilidade. A outra função faz
 * exatamente o reverso, pega uma string que representa um valor verdade,
 * então retorna o valor-verdade em sí.
 */
 bool str_to_bool (char* s);


#endif //TESTE_H
