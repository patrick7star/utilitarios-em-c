

// biblioteca padrão em C(libs muito utilizadas.):
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

// seus módulos:
#include "../teste.h"
#include "../tela.h"
#include "../ponto.h"

/* série de testes unitários */
void cria_uma_instancia_simples() {
   TELA t = cria_tela();
   visualiza_tela_debug(t);
   destroi_tela(t);
}

void simples_riscos_na_tela() {
   TELA t = cria_tela();
   assert(alteracoes_tela(t) == 0);

   // um risco vertical.
   Ponto p = cria_ponto(5, 12);
   Ponto q = cria_ponto(19, 12);
   risca_tela(t, p, q);
   destroi_ponto(p);
   destroi_ponto(q);

   // um risco horizontal.
   p = cria_ponto(4, 16);
   q = cria_ponto(4, 32);
   risca_tela(t, p, q);
   destroi_ponto(p);
   destroi_ponto(q);

   assert(alteracoes_tela(t) == 2);
   visualiza_tela_debug(t);
   destroi_tela(t);
}

#include <time.h>

void riscos_gerados_aleatorio() {
   srand(time(NULL));
   Dimensao D = dimensao_terminal();
   TELA t = cria_tela();

   for (int q = 0; q < 12; q++) {
      // coordenadas aleatórias.
      uint8_t y = rand() % D[0];
      uint8_t x = rand() % D[1];
      Ponto p = cria_ponto(y, x);

      y = rand() % D[0];
      x = rand() % D[1];
      Ponto r = cria_ponto(y, x);

      risca_tela(t, p, r);
   }

   visualiza_tela_debug(t);
   destroi_tela(t);
   destroi_dimensao(D);
}

void fazendo_varios_retangulos() {
   Tela t = cria_tela();

   Ponto a = cria_ponto(5, 8); 
   Ponto b = cria_ponto(9, 28);
   circunscreve_retangulo(t, a, b);
   destroi_ponto(a); destroi_ponto(b);

   a = cria_ponto(11, 38); 
   b = cria_ponto(20, 15);
   circunscreve_retangulo(t, a, b);
   destroi_ponto(a); destroi_ponto(b);

   a = cria_ponto(11, 48); 
   b = cria_ponto(26, 75);
   circunscreve_retangulo(t, a, b);
   destroi_ponto(a); destroi_ponto(b);

   a = cria_ponto(11, 48); 
   b = cria_ponto(26, 75);
   circunscreve_retangulo(t, a, b);
   destroi_ponto(a); destroi_ponto(b);

   a = cria_ponto(2, 41); 
   b = cria_ponto(17, 84);
   circunscreve_retangulo(t, a, b);
   destroi_ponto(a); destroi_ponto(b);

   visualiza_tela_debug(t);
   destroi_tela(t);
}

void escrevendo_strings_na_tela() {
   Tela t = cria_tela();

   Ponto a = cria_ponto(11, 48); 
   escreve_string(t, a, "pumpinkin", Horizontal);
   destroi_ponto(a);

   a = cria_ponto(2, 50); 
   escreve_string(t, a, "strawberry", Horizontal);
   destroi_ponto(a);

   a = cria_ponto(6, 31); 
   escreve_string(t, a, "watermelon", Horizontal);
   destroi_ponto(a);

   a = cria_ponto(2, 20); 
   escreve_string(t, a, "banana", Vertical);
   destroi_ponto(a);

   a = cria_ponto(5, 78); 
   escreve_string(t, a, "pear", Vertical);
   destroi_ponto(a);

   a = cria_ponto(15, 17); 
   escreve_string(t, a, "grape", Vertical);
   destroi_ponto(a);

   a = cria_ponto(19, 68); 
   escreve_string(t, a, "orange", Vertical);
   destroi_ponto(a);

   a = cria_ponto(16, 74); 
   escreve_string(t, a, "lemon", Vertical);
   destroi_ponto(a);

   a = cria_ponto(24, 12); 
   escreve_string(t, a, "those fruits are so much tastable", Horizontal);
   destroi_ponto(a);

   visualiza_tela_debug(t);
   destroi_tela(t);
}

void main(int argc, char** argv) {
   executa_teste_interruptor(
      "cria uma simples instâncias",
      cria_uma_instancia_simples,
      false
   );

   executa_teste_interruptor(
      "variados riscos na Tela",
      simples_riscos_na_tela,
      false
   );

   executa_teste_interruptor(
      "risco gerados aleatoriamente",
      riscos_gerados_aleatorio,
      false
   );

   executa_testes(
      5, riscos_gerados_aleatorio, true,
      simples_riscos_na_tela, false,
      cria_uma_instancia_simples, false,
      fazendo_varios_retangulos, true,
      escrevendo_strings_na_tela, true
   );
}


