

// biblioteca padrão do C:
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

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

static void alimenta_semente() {
   // pega oito bytes da entropia do sistema no Linux("inicialmente" mais lento).
   // abandona função em caso de já acionamento desta 'alimentação'.
   if (ACIONADO) return;

   // versão para Linux:
   #ifdef _POSIX_SOURCE
   uint64_t acumulador = 0;
   uint8_t* bytes = lendo_64_bits();

   for(uint64_t p = 1; p <= 8; p++) {
      uint64_t potencia = (uint64_t)pow(2.0, (double)p);
      acumulador = bytes[p - 1] * potencia;
   }
   srand(acumulador);
   // versão para Windows:
   #elif _WIN64
   // alimenta com o próprio endereço da variável ou função.
   srand((size_t)&semente);
   #endif

   // dizendo que função já foi chamada anteriormente.
   ACIONADO = true;
   // librando de array de oite bytes.
   free(bytes);
}

size_t inteiro_positivo(size_t a, size_t b) {
   alimenta_semente();

   if (a > b)
      return inteiro_positivo(b, a);
   else if (a == b)
      return b;

   return ((rand() % SIZE_MAX) % (b - a + 1)) + a;
}

unsigned char ascii_char_aleatorio() {
   alimenta_semente();

	unsigned char byte = rand() % 256;
   /* selecionando só caractéres válidos. ou seja, aqueles acima de 32. */
   return (unsigned char)(byte % (126-33) + 33);
}

bool logico() { alimenta_semente(); return rand() % 10 >= 5; }

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

struct par { size_t j; size_t i; };

static bool par_distinto(struct par P, struct par* array, size_t t) {
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

#ifdef _POSIX_SOURCE
#include <unistd.h>
#include <string.h>

char* palavra_aleatoria() {
   /*   O algoritmo para ser eficiente fará da seguinte forma, ao invés de 
    * carregar todas palavras na memória, contabilizará o tamanho do 
    * arquivo(em bytes) e sorteará um número entre 0(o ínicio) e o total 
    * de bytes(o tamanho do arquivo mencionado). Este número sorteado 
    * randomicamente será onde começará a lê caractéres. Com a ajuda de
    * dois demarcadores, um para a primeira quebra-de-linha, e o outro para
    * segunda. Feito isso, o que restá é posicionar o cursor do arquivo
    * onde foi marcado a primeira quebra-de-linha, e lê até a segunda. E
    * pronto, aí está a palavra lida, apenas resta retorna-lá. 
    *
    *   NOTA: ainda precisa-se de uma abordagem para tratar como pegar as 
    * palavras no começo e no final do arquivo, digo, literalmente, a
    * primeira e a última palavra. O algoritmo apresentado não cuida deste
    * caso. */
   #ifdef _DEBUG_PALAVRA_ALEATORIA
   puts("a função 'palavra_aleatoria' iniciou.");

   void letra_lida_e_onde(size_t p, char _char) {
      printf("cursor: %lu\tcaractére: '%c'\n", p, _char);
      sleep(1);
   }
   #endif

   const char caminho[] = "/usr/share/dict/american-english";
   FILE* arquivo = fopen(caminho, "rt");
   size_t inicio, fim, nl = 0;
   size_t cursor = inteiro_positivo(0, 900000);
   char letra;
   bool bloco_executado = false;

   // sorteio um byte aleatoriamente no arquivo.
   fseek(arquivo, cursor, SEEK_SET);

   // buscando dois caractéres de quebra-de-linha.
   while (nl  < 2) {
      letra = getc(arquivo);
      #ifdef _DEBUG_PALAVRA_ALEATORIA
      letra_lida_e_onde(ftell(arquivo), letra);
      #endif

      if (letra == '\n') {
         if (!bloco_executado) {
            // marca o ínicio da palavra.
            inicio = ftell(arquivo);
            bloco_executado = true;
         } else
            // marca o fim dela.
            fim = ftell(arquivo);
         // conta uma quebra-de-linha.
         nl++;
      }
   }

   // extraindo o conteúdo entre tais posições...
   size_t byte = sizeof(char);
   /* como estamos tratando de palavras aqui, cinquenta bytes é mais do que 
    * o necessário,... ou você conhece uma palavra(em inglês) que necessite
    * mais do que isso. 
    */
   char* buffer = malloc(50 * byte);
   memset(buffer, '\0', 50);
   size_t qtd = fim - (inicio + 1);

   #ifdef _DEBUG_PALAVRA_ALEATORIA
   printf("serão lidos %lu caractéres\n", qtd);
   #endif
   // lendo entre o que foi marcado pelas iterações...
   fseek(arquivo, inicio, SEEK_SET);
   size_t lido = fread(buffer, byte, qtd, arquivo);

   #ifdef _DEBUG_PALAVRA_ALEATORIA
   printf("o que foi lido: '%s'\n", buffer);
   #endif

   if (lido != qtd) {
      perror ("não leu-se todos bytes demandados.");
      abort();
   }

   fclose(arquivo);
   return buffer;
}
#endif

/* Testando todos estrutura, métodos e funções declarados e implementados
 * acima. Esta parte abaixo pode futuramente ser colocada em outra arquivo
 * e incluído aqui, não faz a menor diferença, e por cima deixa tal arquivo
 * mais limpo. 
 */
#ifdef _UT_ALEATORIO
#include "legivel.h"

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

void sorteio_de_palavra() {
   for (size_t i = 1; i <= 10; i++) {
      char* resultado = palavra_aleatoria();
      todos_caracteres(resultado);
      printf("conteúdo: %s\n\n", resultado);
   }
}

void testando_funcao_getc() {
   char caminho[] = "src/aleatorio.c";
   FILE* arq = fopen(caminho, "rt");
   size_t cursor = inteiro_positivo(50, 100);
   printf("posição selecionada: %lu\n", cursor);
   fseek(arq, cursor, SEEK_SET);

   for (size_t k = 1; k <= 50; k++) {
      char _char; 
      fread(&_char, sizeof(char), 1, arq);
      if (_char != '\n')
         printf("caractére: '%c'\tcursor: %lu\n", _char, ftell(arq));
      else
         printf("caractére: '\\n'\tcursor: %lu\n", ftell(arq));
   }
   fclose(arq);
}

void tempo_do_primeiro_inteiro_positivo_sorteado() {
   size_t total = 45000000;
   printf("sorteio de %s números aleatórios:\n", valor_legivel(total));
   time_t inicio = time(NULL), final;

   while (total > 0) {
      // disparo do número à cada milhão ...
      if (total % 1000000 == 0)
         printf("escolha=%lu\n", inteiro_positivo(1, 1000));
      total--;
   }
   final = time(NULL);

   printf("levou %0.1fseg\n", difftime(final, inicio));
}

#ifdef _POSIX_SOURCE
#include "teste.h"
#include "tempo.h"

void obtendo_tempo_de_sorteio_da_palavra() {
   Cronometro contagem = cria_cronometro();

   for (size_t k = 1; k <= 45000; k++) 
       palavra_aleatoria(); 

   marca(contagem);
   printf("decorrido: %s\n",cronometro_to_str(contagem));

}
#endif // _POSIX_SOURCE

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
      5, testando_funcao_getc, true,
         sorteio_de_palavra, true,
         // consome muito CPU e memória, sempre deixe desabilitada:
         obtendo_tempo_de_sorteio_da_palavra, true,
         // consome muito CPU e tempo, sempre desabilitada:
         tempo_do_primeiro_inteiro_positivo_sorteado, false,
         distribuicao_de_sorteios, false
   );
   #endif
   return EXIT_SUCCESS;
}
#endif // _UT_ALEATORIO
