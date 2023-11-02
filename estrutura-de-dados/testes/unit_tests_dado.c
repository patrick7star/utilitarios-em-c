
// própria biblioteca.
#include "../dado.h"
#include "../../teste.h"

// biblioteca do C:
#include <assert.h>
#include <unistd.h>

/* Espaço para a criação de testes unitários.*/
void criacao_basica_do_dado() {
   Dado d = cria_dado('M');
   Dado d1 = cria_dado('a');
   Dado d2 = cria_dado('J');
   visualiza_dado(d);
   visualiza_dado(d1);
   visualiza_dado(d2);
   assert(d->dado == 'M');
   assert(d2->dado == 'J');
   destroi_dado(d);
   destroi_dado(d1);
   destroi_dado(d2);
}

void criacao_de_array_do_dado() {
   Dado* a = cria_array_dado(15);
   for(int j = 0; j < 15; j++)
      { assert(a[j]->dado == '#');  }
   destroi_array_dado(a, 15);
}

#include <stdio.h>

void conversao_para_string() {
   Dado x = cria_dado('k');
   Dado X = cria_dado('Z');
   printf("1ª string: '%s'\n2ª string: '%s'\n",
      dado_para_string(x), 
      dado_para_string(X)
   );
   destroi_dado(x);
   destroi_dado(X);
}

void contabilizacao_de_instancias() {
   Dado d1 = cria_dado('m');
   printf("total: %lu dados\n", instancias_dado());
   assert(instancias_dado() == 1);

   Dado d2 = cria_dado('P');
   Dado d3 = cria_dado('.');
   Dado d4 = cria_dado('z');
   printf("total: %lu dados\n", instancias_dado());
   assert(instancias_dado() == 4);

   puts("destruindo o primeiro ...");
   destroi_dado(d1);
   printf("total: %lu dados\n", instancias_dado());
   assert(instancias_dado() == 3);

   puts("criando uma array disso ...");
   ArrayDado array = cria_array_dado(19);
   printf("total: %lu dados\n", instancias_dado());
   assert(instancias_dado() == (19 + 3));
}


// execução dos testes:
void main() {
   executa_teste(
      "uma simples instanciação do 'Dado'",
      criacao_basica_do_dado
   );
   executa_teste(
      "cria de uma array do 'Dado'",
      criacao_de_array_do_dado
   );
   executa_teste(
      "conversão para do 'dado' para string",
      conversao_para_string
   );
   executa_teste(
      "contador-de-referência do Dado",
      contabilizacao_de_instancias
   );
}
