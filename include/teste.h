

#ifndef TESTE_H
#define TESTE_H
#include <stdbool.h>
#include <stdint.h>
#include "terminal.h"
   
// nome mais curto, estético e significativo para um ponteiro de 
// de função.
 typedef void(*Fn)(void);
 // Tipo de função especial para testes-unitários.
 typedef int (*FnTst) (void);

/* string vázia dizendo que não quer que o teste tem um nome própria, ou 
 * uma mensagem explicando-o. */
 #define SEM_MENSAGEM ""
 #define SEM_NOME SEM_MENSAGEM

 extern void debug_aqui(void);
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
 *
 *   Cada nova geração ganha uma letra mais avançada no alfabeto, isso até
 * este tipo de teste for realmente substituído por algo mais avançado. */
void executa_tst(char*, Fn, bool); 
void executa_testes(uint8_t total, ...);
void executa_testes_a(bool ativado, int total, ...);

/* Converte um valor lógico para uma string, em português, dizendo o que 
 * ele representa. Nome similires, mas fazem a mesma coisas também estão 
 * dados abaixo, por motivos de compatibilidade. A outra função faz 
 * exatamente o reverso, pega uma string que representa um valor verdade, 
 * então retorna o valor-verdade em sí. 
 */
 extern bool str_to_bool(char* s);

 // macros importantes para melhorar a visualização:
 #define bool_to_str(VL) ((bool)VL) ? "verdadeiro": "falso"
 #define null_to_str(PTR) ( !((bool)PTR) ? "none": "válido")
 /* É frequente o uso da instrução 'while (true)' para criar loops sem 
  * parada(loops infinitos). Aqui, usando de 'macros', vamos criar tal 
  * tipo de declaração.
  *   Também pega todos nomes variádos que tal trecho pode ter. Na verdade
  * são mais uma versão aproximada do primeiro. */
 #define loop_infinito while (true)
 #define LOOP_INFINITO loop_infinito 
 #define LOOP loop_infinito
 #define loop loop_infinito
 #define __LOOP__ loop_infinito
 #define __loop__ loop_infinito

/* -- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
 *
 *                         Ainda em fase
 *                            de testes
 *
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---*/
/* converte um inteiro, qualquer que seja seu tamanho, na sua forma
 * binária. */
 extern char* binario_str(size_t);

#endif //TESTE_H
