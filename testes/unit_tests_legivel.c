
// biblioteca padrão em C(libs muito utilizadas.):
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

// seus módulos:
#include "../teste.h"
#include "../legivel.c"

uint64_t entradas[] = {
   382, 12832, 3842394, 7712340981,
   111931512, 50123812341, 
   100030231892377
};

double segundos[] = {
   51.3232, 12832.15, 8328.0,
   38832.312, 0.001, 0.038,
   0.000851, 0.000000701, 190.5321
};

// criando hipóteses:
void legibilidade_do_tempo() {
   for(int p = 0; p < 9; p++)
      printf(
         "%16lf ===> %s\n", 
         segundos[p], 
         tempo_legivel(segundos[p])
      );
   // de avaliação manual?
}

void legibilidade_de_tamanhos() {
   for(int p = 0; p < 7; p++)
      printf(
         "%16lu ===> %s\n", 
         entradas[p], 
         tamanho_legivel(entradas[p])
      );
}

void o_grosso_de_grande_valores() {
   for(int p = 0; p < 7; p++)
      printf(
         "%16lu ===> %s\n", 
         entradas[p], 
         valor_legivel(entradas[p])
      );
}


// execução de todas hipóteses dadas:
int main(int argc, char** argv) {
   executa_teste(
      "função de legibilidade humana do tempo",
      legibilidade_do_tempo
   );

   executa_teste(
      "função de legibilidade humana do tamanho",
      legibilidade_de_tamanhos
   );

   executa_testes(1, o_grosso_de_grande_valores, true);

   return EXIT_SUCCESS;
}
