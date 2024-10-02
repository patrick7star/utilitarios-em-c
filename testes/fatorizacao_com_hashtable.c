/* Simples algoritmo de fatorização, entretanto, usando da hashtable para
 * armazenar os fatores(na verdade sua forma fatorada). Cada número será
 * representado como um mapa. */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "hashtable_ref.h"

size_t localizacao(generico_t dt, size_t c) {
   size_t n = *((size_t*)dt);
   return n % c; 
}

bool igualdade(generico_t a, generico_t b) {
   size_t x = *((size_t*)a);
   size_t y = *((size_t*)b);

   return x == y;
}

bool e_um_numero_primo(size_t n) {
/* Algoritmo "porco" para isso, porém não é o foco do código.*/
   size_t i = 2;
   while (i < n) {
      if (n % i == 0)
         return false;
      i++;
   }
   return true;
} 

size_t* box(size_t x) {
   size_t* dado;

   dado = malloc(sizeof(size_t));
   *dado = x;

   return dado;
}

char* stringfy(generico_t dt) {
   size_t vl = *((size_t*)dt);
   char* fmt = malloc(15 * sizeof(char));

   sprintf(fmt, "%zu", vl);
   return fmt;
}

void pausa() {
   const size_t ms = 200000000;
   const size_t us = 300000;
   struct timespec t = {0, ms};
   struct timespec r = {0, us};

   nanosleep(&t, &r);
}

size_t dividir(size_t* n, size_t d, bool visualiza) {
/* Divide o número pelo divisor, se for divisível é claro, até não poder 
 * mais; retorna o total de divisões feitas. */
   size_t q = 0;

   while (*n % d == 0 && *n > 0) { 
      if (visualiza) {
         printf("\tnúmero: %zu\n", *n);
         pausa();
      }
      *n /= d; q++; 
   }

   return q;
}

void pula_para_primo(size_t* P, bool visualiza) {
   while(!e_um_numero_primo(*P)) {
      if (visualiza) {
         printf("\tprimo: %zu\n", *P);
         pausa();
      }
      *P += 1;
   }
}

HashTable fatoracao(size_t N, bool visualiza) {
   // A chave é um primo, o valor é a frequência dele no número.
   HashTable F = cria_ht(localizacao, igualdade);
   size_t prime= 2;

   do {
      pula_para_primo(&prime, visualiza);

      if (visualiza)
         printf("Primo a dividir: %zu | %zu\n", N, prime);

      size_t freq = dividir(&N, prime, visualiza);

      /* Coloca primo e sua potência no dicionário de fatoração ...*/
      if (e_um_numero_primo(prime) && freq > 0)
         insere_ht(F, box(prime), box(freq));
   } while(prime++ < N);

   return F;
}

int main(void) {

   HashTable num_100 = fatoracao(100, true);
   HashTable num_360 = fatoracao(360, true);
   HashTable num_1024 = fatoracao(1024, true);

   printf("Qtd. de primos(100): %zu\n", tamanho_ht(num_100));
   printf("Qtd. de primos(360): %zu\n", tamanho_ht(num_360));

   imprime_ht(num_100, stringfy, stringfy);
   imprime_ht(num_360, stringfy, stringfy);
   imprime_ht(num_1024, stringfy, stringfy);

   HashTable num_a = fatoracao(365002, true);
   imprime_ht(num_a, stringfy, stringfy);

   HashTable num_b = fatoracao(1031324, false);
   imprime_ht(num_b, stringfy, stringfy);

   destroi_ht(num_100);
   destroi_ht(num_360);
   destroi_ht(num_1024);
   destroi_ht(num_a);
   destroi_ht(num_b);

   return EXIT_SUCCESS;
}
