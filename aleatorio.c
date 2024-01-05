

// biblioteca padrão do C:
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>


#ifdef _POSIX_SOURCE // apenas para Linux.
uint8_t byte_aleatorio() {
   FILE *arq = fopen("/dev/urandom", "rb");
   uint8_t b = 0;
   if (arq != NULL) 
      { fread(&b, sizeof(uint8_t), 1, arq); }
   fclose(arq);
   return b;
}

uint8_t* lendo_64_bits() {
   FILE *arq = fopen("/dev/urandom", "rb");
   uint8_t* bytes = calloc(8, sizeof(uint8_t));

   if (arq != NULL) { 
      int total_lido;
      total_lido = fread(bytes, sizeof(uint8_t), 8, arq); 
      if (total_lido != 8)
         { perror("não foi lido 8 bytes"); }
      if (bytes == NULL)
         { perror("não foi possível lê nada."); }
   }
   fclose(arq);
   return bytes;
}
#endif

unsigned char ascii_char_aleatorio() {
	uint8_t byte;
	#if defined(_POSIX_SOURCE)
   byte = byte_aleatorio();
	#elif defined(_WIN64)
	srand(time(NULL));
	byte = rand() % 256;
	#endif
   // selecionando só caractéres válidos.
   // ou seja, aqueles acima de 32.
   return (unsigned char)(byte % (126-33) + 33);
}

#include <math.h>

uint64_t inteiro_positivo(uint64_t a, uint64_t b) {
   if (a > b)
      return inteiro_positivo(b, a);
   else if (a == b)
      return b;
   uint8_t* bytes = lendo_64_bits();
   if (bytes == NULL)
      perror("array de bytes inválida(null)");
   uint64_t acumulador = 0;
   for(uint64_t p = 0; p < 8; p++) {
      uint64_t potencia = (uint64_t)pow(2.0, (double)p);
      acumulador = bytes[p] * potencia;
   }
   return (acumulador % (b - a + 1)) + a;
}

#include <stdbool.h>

bool logico() {
	#if defined (_WIN64)
	// sorteio de 5 até 9(incluso) são verdadeiro.
	srand(time(NULL));
	return (rand() % 10) >= 5;
	#elif defined(_POSIX_SOURCE)
   if (byte_aleatorio() > 256/2) 
      { return true; }
   else 
      { return false; }
	#else
	perror("sistema desconhecido!");
	abort();
	#endif 
}

#include <ctype.h>

unsigned char alfabeto_aleatorio() {
   uint8_t code = ascii_char_aleatorio();
   if (isalpha(code))
      { return (unsigned char)code; }
   else
      { return alfabeto_aleatorio(); }
}

uint64_t* array_inteiro_aleatoria(uint64_t n, uint64_t a, uint64_t b) {
   uint64_t* array = calloc(n, sizeof(uint64_t));

   for (uint64_t q = 1; q <= n; q++) 
      array[q - 1]= inteiro_positivo(a, b);

   return array;
}

char* palavra_aleatoria() {
#ifdef _POSIX_SOURCE
	perror("também não implementado em LINUX(ainda)!");
#else
   perror("não implementado para não Linux!");
   abort();
#endif
}

struct par { size_t j; size_t i; };

bool par_distinto(struct par P, struct par* array, size_t t) {
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

/* O algoritmo consiste em realizar o total de swaps em metade do
 * tamanho da array, sendo tais posições escolhidas aleatóriamente. */
extern void embaralha(uint8_t* array, size_t t) {
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

// todos testes unitários do módulo.
#ifdef UT_ALEATORIO

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

int main(int qtd, char** argumentos, char* envp[]) {
	funcao_de_embaralho();
   return EXIT_SUCCESS;
}
#endif
