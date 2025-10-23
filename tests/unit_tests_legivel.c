
// biblioteca padrão em C(libs muito utilizadas.):
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

// seus módulos:
#include "teste.h"
#include "legivel.h"

uint64_t entradas[] = {
   382, 12832, 3842394, 7712340981,
   111931512, 50123812341, 100030231892377
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
   for(size_t p = 1; p <= 7; p++) {
		uint64_t valor = entradas[p - 1];
		char* traducao = tamanho_legivel(valor);
		printf ("%20lu ==> %s\n", valor, traducao);
		free (traducao);
	}
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
   executa_testes(
      3, legibilidade_do_tempo, true,
      legibilidade_de_tamanhos, true,
		o_grosso_de_grande_valores, true
	);

   return EXIT_SUCCESS;
}
