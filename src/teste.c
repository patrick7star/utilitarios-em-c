
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>
#ifdef __linux__
#include <tgmath.h>

#elif defined(_WIN32)
#include <math.h>
#include <locale.h>
#endif

#include "tempo.h"
#include "legivel.h"
#include "terminal.h"
/* incluindo aqui por quê? Primeiro, tem macros importantes definidos lá,
 * que é preciso também usar aqui; segundo, o 'header guards' garantem que 
 * não entra-se num loop de inclusão. */
#include "teste.h"


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

/* Cuida da estração de bits de quqlquer tipo de dado primitivo dado. Ainda
 * não está terminado, e não tem qualquer protótipo para qualquer mínimo
 * uso ainda. 
 *    
 *    Funções na ordem encontrada:
 *       - algarismo
 *       - binario_str
 *       - binario_complemento_de_dois_oito_bits
 *       - inverte_array
 *       - binario_complemento_de_dois
 */
#include "teste/bits.c"

/* Nova feature de testes, sendo está pega o nome do teste, e também
 * permite a paralelização, se assim for desejado.
 *
 *    O que ele fornece:
 *       - testes_unitarios
 *       - Unit
 *
 *    Seus testes-unitários:
 *       - novo_suite_de_testes_unitarios
 *       - resultado_de_todos_testes_desativados
 */
#ifdef _POSIX_C_SOURCE
// #include "teste/testador.c"
#endif


#ifdef _UT_TESTE
/* === === === === === === === === === === === === === === === === === ===+
 *
 *                         Testes Unitários
 *   Testando todos estrutura, métodos e funções declarados e implementados
 * acima. Esta parte abaixo pode futuramente ser colocada em outra arquivo
 * e incluído aqui, não faz a menor diferença, e por cima deixa tal arquivo
 * mais limpo. 
 * === === === === === === === === === === === === === === === === === ===*/
static void imprime_debug(uint8_t* array, size_t t) {
   if (t == 0) 
      { printf("[]\n"); return; }

   printf("[");
   for (size_t k = 0; k < t; k++)
      printf("%d, ", array[k]);
   printf("\b\b]\n");
}

void teste_conversao_binaria_antiga_implementacao() {
   for (uint8_t i = 0; i <= 16; i++)
      printf("%d ===> %s\n",i, binario_str(i));
}

void amostra_da_nova_implementacao_de_binario() {
   for (uint8_t n = 1; n < 16; n++) {
      uint8_t* resultado =  binario_complemento_de_dois_oito_bits(n);
      imprime_debug(resultado, 8);
   }
}

void extracao_de_bits_implementacao_geral() {
   for (uint8_t n = 1; n < 16; n++) {
      uint8_t* resultado =  binario_complemento_de_dois_oito_bits(n);
      uint8_t* outro_resultado = binario_complemento_de_dois(n);
      printf("antiga: ");
      imprime_debug(resultado, 8);
      inverte_array(outro_resultado, 8);
      printf(" atual: ");
      imprime_debug(outro_resultado, 8);
      puts("");
   }
}

void verificando_obtendo_de_potencias_de_dois() {
   // mascaras necessárias para operações AND com todos bits.
   uint8_t qtd = 30;
   // complementando máscaras...
   for (size_t p = 1; p <= qtd; p++) {
      size_t n = (size_t)pow(2, p - 1);
      printf("%2zuº ---> %9zu -->%31s\n", p, n, binario_str(n));
   }
}

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
         "%zuº)\t%-13s==>%13s\n", 
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
	setlocale(LC_CTYPE, "en_US.UTF-8");

   executa_testes_a(
      true, 7, teste_conversao_binaria_antiga_implementacao, true,
         amostra_da_nova_implementacao_de_binario, true,
         extracao_de_bits_implementacao_geral, true,
         // iteração para gerar máscaras funciona!
         verificando_obtendo_de_potencias_de_dois, true,
         stringficacao_de_valores_primitivos, true,
         converte_strings_de_valores_logicos, true,
         // consome bastante tempo...
         testes_tal_declaracao_de_loop, false
   );

   // Teste da função interna sem nada com atual módulo:
   executa_testes(
      4, percorrendo_string, true,
         transforma_toda_string_em_minuscula, true,
         primeira_versao_alternativa_de_executa_testes, true,
         teste_interruptor_renomeado, true
   );

   return EXIT_SUCCESS;
}
#endif 

