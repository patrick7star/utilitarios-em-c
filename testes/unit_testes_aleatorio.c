
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

uint64_t conta_valor(uint64_t valor, uint64_t* a, uint64_t n) {
   uint64_t contagem = 0;
   for (uint64_t i = 1; i <=  n; i++) {
      if (valor == a[i - 1])
         contagem++;
   }
   return contagem;
}

#include <math.h>

void visualiza_array(uint64_t* a, uint64_t n) {
   for (uint64_t i = 1; i <= n; i++) {
      if (i % 19 == 0)
         puts("\b\b");
      else 
         printf("%lu, ", a[i - 1]);
   }
   puts("\narray visualizada.");
}

void geracao_de_arrays_de_inteiros_aleatorios() {
   uint64_t t = 500;
   uint64_t* array = array_inteiro_aleatoria(t, 26, 52);
   uint64_t a = conta_valor(29, array, t); 
   uint64_t b = conta_valor(43, array, t); 
   float pa =  (float)a / (float)t;
   float pb = (float)b / (float)t;

   visualiza_array(array, t);
   printf("'%d':%lu e '%d':%lu\n", 29, a, 43, b);
   printf("percentuais %0.2f%% e %0.2f%%\n",  pa * 100, pb * 100);

   // devem ter, aproximadamente, um mesmo percentual.
   assert ( fabs(pa - pb) < 0.05 );
   free(array);
}

void distribuicao_de_valores_logicos() {
   uint16_t t = 6800;
   uint16_t c = 0;

   for (uint16_t k = 1; k <= t; k++) {
      if (logico()) c += 1;
   }
   float p = (float)c / (float)t;
   printf(
      "verdadeiros: %0.2f%%\nfalso: %0.2f%%\n", 
      p * 100.0, (1.0 - p) * 100
   );
   assert (fabs(p - 0.50) < 0.01);
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
   executa_teste_interruptor(
      "função 'inteiro positivo' com amostra gigante",
      amostra_gigante_inteiros_positivos, false
   );
   executa_testes(
      2, geracao_de_arrays_de_inteiros_aleatorios, true,
      distribuicao_de_valores_logicos, true
   );
}
