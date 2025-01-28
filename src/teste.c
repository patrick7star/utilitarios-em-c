
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>
#include <tgmath.h>
// para não compilar no Windows, assim mantém compatibilidade
#ifndef _WIN64
#include "tempo.h"
#include "legivel.h"
#endif
#include "terminal.h"
/* incluindo aqui por quê? Primeiro, tem macros importantes definidos lá,
 * que é preciso também usar aqui; segundo, o 'header guards' garantem que 
 * não entra-se num loop de inclusão. */
#include "teste.h"
#include "macros.h"


/*   Primeiro suite de testes criados. Na verdade é um monte de funções, 
 * baseado em uma única funçõa base. Difícilmente pode se descontinuada,
 * porque foi amplamente utilizada, e ainda é enquanto escrevo isso, apesar
 * de está com os dias contados.
 *   Foi passada para um próprio arquivo, por está dispersada pelo código,
 * sem falar que é uma das features com mais linhas, e até a mais importante
 * deste módulo(e de ele ser tão usado). 
 *    
 *    Funções:
 *       - nome_do_teste
 *       - executa_testes 
 *       - executa_testes_a
 *       - executa_teste_interruptor 
 *
 *    Variável:
 *       - testes_contagem
 */
#include "teste/old-tester.c"

// conta quantos usos a função abaixo tem.
static uint8_t contagem_de_usos = 0;
/* ótima ferramenta de 'debug', onde indica a linha e o arquivo onde 
 * possível erro pode está. */
void debug_aqui(void) {
   printf("\no erro está bem ... ... ...aqui!(%d)\n", contagem_de_usos + 1);
   contagem_de_usos++;
}

/* Módulo inclui funções e objetos que são utilizados no escopo principal
 * para que tal fique compatível com a plataforma Windows. 
 *
 *    Funções:
 *       - pausa
 *       - espera_pouco_segundos
 *       - intervalo_de_milisegundos
 */
#include "teste/windows.c"

/* Função e seus protótipos para converter uma string que representa um 
 * valor lógica, seja qualquer a forma que ele está escrito, no respectivo
 * valor lógico que representa. As funções auxiliares a função principal(
 * str_to_bool) também é listada, pois alguns testes fazem uso delas.
 *
 *    Funções:
 *          - string_esta_minuscula
 *          - string_minuscula
 *          - str_to_bool
 */
#include "teste/booleano.c"


#ifdef _UT_TESTE
/* === === === === === === === === === === === === === === === === === ===+
 *
 *                         Testes Unitários
 *   Testando todos estrutura, métodos e funções declarados e implementados
 * acima. Esta parte abaixo pode futuramente ser colocada em outra arquivo
 * e incluído aqui, não faz a menor diferença, e por cima deixa tal arquivo
 * mais limpo. 
 * === === === === === === === === === === === === === === === === === ===*/
void converte_strings_de_valores_logicos() {
   struct par {
      char* entrada;
      bool saida;
   };

   struct par matches[] = {
      {"verdadeiro", true }, { "true", true }, { "falso", false }, 
      { "false", false }, { "FALSO", false }, { "FALse", false }, 
      { "TRUE", true }, { "vErDaDeiRO", true }, { "Falso", false }, 
      { "TrUe", true }, { "True", true }, {"T", true}, {"V", true}, 
      {"f", false}, {"F", false}
   };

   for (size_t p = 0; p < 15; p++) {
      char* e = matches[p].entrada;
      bool s = matches[p].saida;
      printf(
         "%luº)\t%-13s==>%13s\n", 
         (p + 1), e, bool_to_str(s)
      );
      assert (str_to_bool(e) == s);
   }
}

void testes_tal_declaracao_de_loop(void) {
#ifdef _POSIX_C_SOURCE
   Temporizador timer = cria_temporizador(Segundo, 3);
   loop_infinito {
      puts("mensagem fica se repetindo.");
      breve_pausa(Miliseg, 200);

      if (esgotado(timer)) {
         puts("o termino de tal bloco chegou!!");
         break;
      }
   }
   destroi_temporizador(timer);
#else
   puts("testes não existente para sistemas diferente de Linux!");
#endif
}

int main(int qtd, char* argumentos[], char* env_vars[]) {
   executa_testes_a(
      true, 3, 
         // iteração para gerar máscaras funciona!
         stringficacao_de_valores_primitivos, false,
         converte_strings_de_valores_logicos, false,
         // consome bastante tempo...
         testes_tal_declaracao_de_loop, false
   );

   #ifdef __linux__
   // Teste da função interna sem nada com atual módulo:
   executa_testes(
      4, percorrendo_string, true,
         transforma_toda_string_em_minuscula, true,
         primeira_versao_alternativa_de_executa_testes, true,
         teste_interruptor_renomeado, true
   );
   #endif

   return EXIT_SUCCESS;
}
#endif 

