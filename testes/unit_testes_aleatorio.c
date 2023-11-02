
// biblioteca padrão em C(libs muito utilizadas.):
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
// seus módulos:
#include "../teste.h"
#include "../aleatorio.h"


// criando hipóteses:
void amostra_pequena_inteiros_positivos() {
   uint64_t total = 18;
   for(uint64_t p = 1; p <= total; p++) {
      uint64_t a = 53, b = 179;
      uint64_t x = inteiro_positivo(a, b);
      fflush(stdout);
      if (p % 5 == 0)
         { puts(""); }
      else
         { printf("%lu, ", x); }
      assert(x >= a && x<= b);
   }
}

void poucos_testes_alfabeto_aleatorio() {
   uint64_t total = 18;
   for(uint64_t p = 1; p <= total; p++) {
      unsigned char ch = alfabeto_aleatorio();
      printf("seleção(%lu): %c\n", p + 1, ch);
   }
}

void amostra_gigante_inteiros_positivos() {
   uint64_t total = 65000;
   bool maximo_processado = false;
   bool minimo_selecionado = false;
   for(uint64_t p = 1; p <= total; p++) {
      uint64_t a = 53, b = 179;
      uint64_t x = inteiro_positivo(a, b);
      assert(x >= a && x<= b);
      if (x == b)
         { maximo_processado = true; }
      else 
         { minimo_selecionado = true; }
   }
   puts("sem erros com valores maiores que o pedido.");
   assert(maximo_processado);
   assert(minimo_selecionado);
   puts("todos máximos e mínimos também foram selecionados.");
}

// execução de todas hipóteses dadas:
void main(int argc, char** argv) {
   executa_teste(
      "sorteio de algumas letras do alfabeto",
      poucos_testes_alfabeto_aleatorio
   );
   executa_teste(
      "função 'inteiro positivo' com pequena amostra",
      amostra_pequena_inteiros_positivos
   );
   executa_teste(
      "função 'inteiro positivo' com amostra gigante",
      amostra_gigante_inteiros_positivos
   );
}
