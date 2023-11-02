

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <threads.h>
#include <time.h>
#include "c-language/tempo.h"
#include <unistd.h>


uint8_t aleatorio(uint8_t i, uint8_t f) {
   srand(time(NULL));
   return (uint8_t)(rand() % (f - i)) + i;
}

struct argumentos {
   uint8_t* array;
   uint64_t i, f;
};

thrd_start_t preenche(struct argumentos* args) {
   uint64_t inicio = (*args).i;
   uint8_t selecao = aleatorio(1, 10);

   while (inicio <= (*args).f) {
      args->array[inicio] = 3;
      inicio++;
   }
   return thrd_success;
}

void preenche_veloz(uint8_t* array, uint64_t total) {
   thrd_t todas[4];
   
   // cinco faixas de 0 à 1/5, 1/5 à 2/5, e assim até 4/5 para 5.
   uint64_t F = total / 5;

   struct argumentos args_i = { array, 0, F };
   thrd_create(&todas[0], (thrd_start_t)&preenche, &args_i);
   puts("1ª thread criada.");

   struct argumentos args_ii = { array, F, 2*F };
   thrd_create(&todas[1], (thrd_start_t)&preenche, &args_ii);
   puts("2ª thread criada.");

   struct argumentos args_iii = { array, 2*F, 3*F};
   thrd_create(&todas[2], (thrd_start_t)&preenche, &args_iii);
   puts("3ª thread criada.");

   struct argumentos args_iv = { array, 3*F, total - 1};
   thrd_create(&todas[3], (thrd_start_t)&preenche, &args_iv);
   puts("4ª thread criada.");

   // aguarda cada uma terminar.
   for (int k = 0; k <= 3; k++) {
      int indice = (k + 1);
      thrd_join(todas[k], NULL);
      printf("%dª thread terminou.\n", indice);
   }
}

void preenche_hiperveloz(uint64_t total) {
   puts("subprocesso criado com sucesso.");
   uint8_t* array_ii;
   uint8_t* array_i;

   // chocando um subprocesso ...
   pid_t processo = fork();

   if (processo == 0) {
      // no caso do subprocesso.
      array_i = (uint8_t*)calloc(total / 2, sizeof(uint8_t));
      preenche_veloz(array_i, total/2);
      free(array_i);
   }

   puts("função 'preenche_hiperveloz' terminou.\n");
}

void preenche_antigo(uint8_t* array, uint64_t total) {
   srand(time(NULL));
   uint64_t s = (uint64_t)(rand() % 100) + 1;
   for (uint64_t q = total; q > 0; q--)
      array[q - 1] = s;
}


int main() {
   uint8_t* array_inutil;
   uint64_t q = 700 * potencia(2, 20);

   for (int n = 5; n > 0; n--) {
      array_inutil = (uint8_t*)calloc(q, sizeof(uint8_t));
      preenche_veloz(array_inutil, q);
      // preenche_antigo(array_inutil, q);
      free(array_inutil);
      puts("tudo foi liberado.");
   }

   /*
   // método novo, jeito novo ...
   preenche_hiperveloz(q);
   */

   puts("pressione ENTER novamente para sair do programa.");
   getchar();

   return EXIT_FAILURE;
}
