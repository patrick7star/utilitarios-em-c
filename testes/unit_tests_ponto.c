


// biblioteca padrão em C(libs muito utilizadas.):
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

// seus módulos:
#include "../teste.h"
#include "../ponto.h"

void criacao_da_array_de_pontos() {
   ArrayPonto array = cria_array_de_pontos(7);

   srand(time(NULL));
   for (int k = 1; k <= 7; k++) {
      uint8_t y = rand() % 10;
      uint8_t x = rand() % 100;
      array[k - 1] = cria_ponto(y, x);
   }

   for (int p = 1; p <= 7; p++)
      printf("%s\t", ponto_to_str(array[p - 1]));
   destroi_array_de_pontos(array, 7);
}

void todos_vertices_do_retangulo() {
   Ponto p = cria_ponto(5, 10);
   Ponto q = cria_ponto(8, 19);

   ArrayPonto vertices = retangulo_vertices(p, q);

   Ponto p1 = cria_ponto(5, 10);
   assert(eq_ponto(vertices[0], p1));
   printf("%s\t", ponto_to_str(vertices[0]));

   Ponto p2 = cria_ponto(5, 19);
   assert(eq_ponto(vertices[1], p2));
   printf("%s\t", ponto_to_str(vertices[1]));

   Ponto p3 = cria_ponto(8, 19);
   assert(eq_ponto(vertices[2], p3));
   printf("%s\t", ponto_to_str(vertices[2]));

   Ponto p4 = cria_ponto(8, 10);
   assert(eq_ponto(vertices[3], p4));
   printf("%s\t", ponto_to_str(vertices[3]));

   destroi_array_de_pontos(vertices, 4);
   destroi_ponto(p); destroi_ponto(q);
   destroi_ponto(p1); destroi_ponto(p2);
   destroi_ponto(p3); destroi_ponto(p4);

   // outro modo de arranjos dos pontos.
   p = cria_ponto(2, 30);
   q = cria_ponto(9, 15);

   vertices = retangulo_vertices(p, q);

   p1 = cria_ponto(2, 15);
   assert(eq_ponto(vertices[0], p1));
   printf("%s\t", ponto_to_str(vertices[0]));

   p2 = cria_ponto(2, 30);
   assert(eq_ponto(vertices[1], p2));
   printf("%s\t", ponto_to_str(vertices[1]));

   p3 = cria_ponto(9, 30);
   assert(eq_ponto(vertices[2], p3));
   printf("%s\t", ponto_to_str(vertices[2]));

   p4 = cria_ponto(9, 15);
   assert(eq_ponto(vertices[3], p4));
   printf("%s\t", ponto_to_str(vertices[3]));

   destroi_array_de_pontos(vertices, 4);
   destroi_ponto(p); destroi_ponto(q);
   destroi_ponto(p1); destroi_ponto(p2);
   destroi_ponto(p3); destroi_ponto(p4);
}

void criacao_de_forma_manual_da_array_de_pontos(void) {
   ArrayPonto array = cria_manualmente_ap(
      7, 8, 10, 5, 19, 0, 7, 5, 25, 9, 10,
      15, 3, 20, 20
   );
   printf("%s\n", ap_to_str(array, 7));
   destroi_array_de_pontos(array, 7);
}

void main(int argc, char** argv) {
   executa_testes(
      3, criacao_da_array_de_pontos, true,
      todos_vertices_do_retangulo, true,
      criacao_de_forma_manual_da_array_de_pontos, true
   );
}


