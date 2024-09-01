

// biblioteca padrão do C:
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <tgmath.h>
#include <assert.h>
#include <ctype.h>
#ifdef _POSIX_SOURCE
#include <unistd.h>
#include <string.h>
#endif
// Definições de constantes e funções abaixos:
#include "aleatorio.h"

/* é atribuído futuramente valores tirados de diferentes formas, 
 * depedendo do OS. */
static bool ACIONADO = false;

#ifdef _POSIX_SOURCE // apenas para Linux.
uint8_t* lendo_64_bits() {
   FILE *arq = fopen("/dev/urandom", "rb");
   uint8_t* bytes = calloc(8, sizeof(uint8_t));

   if (arq != NULL) { 
      int total_lido;
      total_lido = fread(bytes, sizeof(uint8_t), 8, arq); 
      // tem que ser garantido que foi 8 bytes.
      assert (total_lido == 8);
      if (bytes == NULL)
         { perror("não foi possível lê nada."); }
   }
   fclose(arq);
   return bytes;
   return NULL;
}
#endif


uint64_t* array_inteiro_aleatoria(uint64_t n, uint64_t a, uint64_t b) 
{
   uint64_t* array = calloc(n, sizeof(uint64_t));

   for (uint64_t q = 1; q <= n; q++) 
      array[q - 1]= inteiro_positivo(a, b);

   return array;
}

struct par { size_t j; size_t i; };

static bool par_distinto(struct par P, struct par* array, size_t t) 
{
	for (size_t m = 0; m < t; m++) {
		struct par A = array[m];
		bool um_par_igual = {
			(P.i == A.i && P.j == A.j) ||
			(P.i == A.j && P.j == A.i)
		};

		if (um_par_igual) return false;
	}
	return true;
}

static struct par selecao_randomica(size_t i, size_t f) {
	size_t p = inteiro_positivo(i, f);
	size_t q = inteiro_positivo(i, f);

	while (q == p) 
		q = inteiro_positivo(i, f);
	return (struct par) { p, q};
}

extern void embaralha(uint8_t* array, size_t t) {
   /* O algoritmo consiste em realizar o total de swaps em metade do
    * tamanho da array, sendo tais posições escolhidas aleatóriamente. */
	size_t total = t / 2;
	printf("total de swaps à fazer: %lu\n", total);
	size_t inicio = 0;
	size_t fim = t - 1;
	// seleções de pares de posições já feitos.
	struct par S[total];

	for (size_t m = 0; m < total; m++) {
		// colocando na array de seleção.
		S[m] = selecao_randomica(inicio, fim);
		struct par P = S[m];

		if (m > 0) {
			while (!par_distinto(P, S, m - 1))
				P = selecao_randomica(inicio, fim);
		}

		// realizando o swap.
		size_t p = P.i, q = P.j;
		uint8_t copia = array[p];
		array[p] = array[q];
		array[q] = copia;
		printf("swap realizados. %lu -- %lu\n", p, q);
	}
}


/* Testando todos estrutura, métodos e funções declarados e implementados
 * acima. Esta parte abaixo pode futuramente ser colocada em outra arquivo
 * e incluído aqui, não faz a menor diferença, e por cima deixa tal arquivo
 * mais limpo. 
 */
#ifdef _UT_ALEATORIO
#include "legivel.h"
#ifdef _POSIX_SOURCE
#include "teste.h"
#include "tempo.h"
#endif

static void imprime_array_uint8_t(uint8_t* a, const size_t t) {
	printf("[");
	for (size_t i = 0; i < t; i++)
		printf("%d, ", a[i]);
	puts("\b\b]");
}

void funcao_de_embaralho() {
	uint8_t array[] = {5, 9, 2, 8, 7, 1, 4, 3, 6, 0};
	size_t t = 10;

	imprime_array_uint8_t(array, t);

	embaralha(array, t);
	printf("embaralho(I): ");
	imprime_array_uint8_t(array, t);

	embaralha(array, t);
	printf("embaralho(II): ");
	imprime_array_uint8_t(array, t);
}

void todos_caracteres(char* string) {
   // visualização de caractére-por-caractére.
   while ((bool)*string) {
      char ch = string[0];
      if (ch == '\n')
         printf("|\\n|  ");
      else if (ch == '\0')
         printf("|#|  ");
      else if (ch == ' ')
         printf("|.|  ");
      else
         printf("|%c|  ", ch);
      string++;
   }
   puts("\b\b");
}

void distribuicao_de_sorteios() {
   size_t total = 45000000;
   size_t um_alg = 0, dois_algs = 0, tres_algs = 0;
   size_t contagem = total;

   printf(
      "sorteio de %s números aleatórios de 0 à 500:\n", 
      valor_legivel(total)
   );

   while (contagem > 0) {
      size_t sorteio = inteiro_positivo(0, 500);

      // disparo do número à cada milhão ...
      if (contagem % 1000000 == 0 && contagem != 0) {
         char* ja_contabilizado = valor_legivel(total-contagem);
         printf(
            "escolha=%03lu (%6s)\n", sorteio, 
            ja_contabilizado
         );
         free(ja_contabilizado);
      }

      if (sorteio < 10)
         um_alg++;
      else if (sorteio < 100)
         dois_algs++;
      else if (sorteio < 1000)
         tres_algs++;

      contagem--;
   }

   double t = total;
   printf(
      "distribuição:\n\t0-9: %0.1f%%" "\n\t10-99: %0.1f%%"
      "\n\t100-999: %0.1f%%\n",
      ((double)um_alg / t) * 100.0, 
      ((double)dois_algs / t) * 100.0, 
      ((double)tres_algs / t) * 100.0
   );
}

int main(int qtd, char* args[], char* vars[]) 
{
   #ifdef _WIN64
   puts("funções compatíveis com o Windows.");
   #elif _POSIX_SOURCE
   puts ("testes apenas para Linux:");
   executa_testes(
      7, testando_funcao_getc, false,
         sorteio_de_palavra, true,
         // consome muito CPU e memória, sempre deixe desabilitada:
         obtendo_tempo_de_sorteio_da_palavra, false,
         sorteio_de_caracteres_ascii, false,
         sorteio_de_letras_do_alfabeto, false,
         // consome muito CPU e tempo, sempre desabilitada:
         tempo_do_primeiro_inteiro_positivo_sorteado, false,
         distribuicao_de_sorteios, false
   );
   #endif
   return EXIT_SUCCESS;
}
#endif // _UT_ALEATORIO
