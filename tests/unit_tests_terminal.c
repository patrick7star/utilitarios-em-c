

// biblioteca padrão:
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

// próprio biblioteca:
#include "../terminal.h"
#include "../teste.h"

void dimensao_alternante() {
   for(int k = 1; k <= 13; k++) {
      Dimensao d = dimensao_terminal();
      printf("dimensão do terminal:\t%ix%i\n", d[0], d[1]);
      sleep(1);
      destroi_dimensao(d);
   }
   assert(true);
}

void string_array_seus_modos() {
   puts("instancia da 'String de Array' criada.");
   StringArray sA = cria_string_array(40, 60, Visivel);
   visualiza_string_array(sA, 40, 60);
   puts("instancia da 'String de Array' criada(sem fundo).");
   StringArray sA1 = cria_string_array(20, 37, Invisivel);
   visualiza_string_array(sA1, 20, 37);
   destroi_string_array(sA1, 20);
   destroi_string_array(sA, 40);
   puts("instancias 'String de Array' destruidas.");
}

#include <string.h>

void string_array_criada_manualmente() {
   StringArray strings = cria_string_array_manual(
      6, "hello, my name is Bob.",
      "shut the fucking door!",
      "i love so much strawberries!",
      "this is what i think it is",
      "look at there!",
      "so many bananas here."
   );
   puts("criadas com sucesso.");

   for (int p = 0; p < 6; p++) {
      assert(strlen(strings[p]) == 28);
      puts(strings[p]);
   }
   puts("todas impressas.");

   destroi_string_array(strings, 6);
   puts("instancias 'String de Array' destruidas.");
}

// execução dos testes:
void main() {
   executa_teste_interruptor(
      "alternancia entre dimensões(manual)",
      dimensao_alternante, false
   );
   executa_teste(
      "modos da 'String de Array'",
      string_array_seus_modos
   );
   executa_teste(
      "criação manual, porém ainda dinâmica, de uma StrArray",
      string_array_criada_manualmente
   );
}
