/**/

#ifndef TESTE_HEADER_FILE
#define TESTE_HEADER_FILE
#include <stdbool.h>
#include <stdint.h>

/* Fazendo a biblioteca multiplataforma. Isso é importante aqui, pois faz
 * tal lib também utilizável no Windows. */
#if defined(_WIN32)
  // Importantes definições para funcionar no Windows.
  #define _USE_MATH_DEFINES

  #ifdef __LIB_DLL__
    #define CROSSLIB __declspec(dllexport)
  #else
    #define CROSSLIB __declspec(dllimport)
  #endif

#elif defined(__linux__)
    #define CROSSLIB
#endif

 /* Nome mais curto, estético e significativo para um ponteiro de
  * de função. */
 typedef void(*Fn)(void);
 // Tipo de função especial para testes-unitários.
 typedef int (*FnTst) (void);

/* string vázia dizendo que não quer que o teste tem um nome própria, ou
 * uma mensagem explicando-o. */
 #define SEM_MENSAGEM ""
 #define SEM_NOME SEM_MENSAGEM

 extern CROSSLIB void debug_aqui(void);

/* Endereço de memória da função a executar; e sua definação,... se é para
 * executa-la?! */
struct TesteConfig { const char* nome; Fn rotina; bool ativado; };

#define Unit(FUNCAO, ESTADO) (  \
   (struct TesteConfig){         \
      .nome = #FUNCAO,           \
      .rotina = FUNCAO,          \
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
 *
 *   Cada nova geração ganha uma letra mais avançada no alfabeto, isso até
 * este tipo de teste for realmente substituído por algo mais avançado.

 *   Na versão do Windows irei inovar, e sempre colocar apenas uma função
 * que executa testes, portanto, sem as antigas versões. */
 #if defined(_WIN32)
 CROSSLIB void executa_testes  (bool ativado, int total, ...);
 #elif defined(__linux__)
 void executa_tst       (char*, Fn, bool);
 void executa_testes    (int total, ...);
 void executa_testes_a  (bool ativado, int total, ...);
 void executa_testes_b  (bool ativado, int total, ...);
 #endif

/* Converte um valor lógico para uma string, em português, dizendo o que
 * ele representa. Nome similires, mas fazem a mesma coisas também estão
 * dados abaixo, por motivos de compatibilidade. A outra função faz
 * exatamente o reverso, pega uma string que representa um valor verdade,
 * então retorna o valor-verdade em sí.
 */
 extern CROSSLIB bool str_to_bool(char* s);

 /* O compilador nativo do Windows(MSVC) é muito chato com regras do ISO C.
  * Por isso este macro está sendo colocado aqui. Gosto do nome, mas ainda
  * não quero fazer multiplo plataforma. Ele pega o tipo de todo teste, um
  * 'void', maís o qualificador 'static' exigido no 'clang-cl' prá toda
  * função e variável que não é global. Como é prática, todos os nomes que
  * podem ser pensados, poderão ser usados.
  *   Usar isso nas plataformas Unix é totalmente opcional. Acho que é algo
  * que deixa o código bem mais legível.
  */
 #define TESTE          static void
 #define UNIT_TEST      TESTE
 #define TESTE_UNITARIO TESTE
 #define UNITARIO       TESTE

#endif //TESTE_H

