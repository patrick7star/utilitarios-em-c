
// Importando as declarações das funções implementadas abaixo:
#include "aleatorio.h"
// Bibliote padrão do C:
#include <stdbool.h>
#include <time.h>
#include <iso646.h>
#ifdef _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#endif

// Número em que os bits internos são sempre modificados.
static bool EXECUTADO_ALGUMA_VEZ = false;
static size_t SEMENTE;

static void alimenta_a_semente(void) {
   if (not EXECUTADO_ALGUMA_VEZ) 
   {
      // Endereço de memória da própria função.
      size_t endereco = (size_t)&alimenta_a_semente;
      // Total de tempo decorrido desde da criação do Unix.
      SEMENTE = (size_t)time(NULL) | endereco;
      // Marca como já buscado alguma vez o número randômico.
      EXECUTADO_ALGUMA_VEZ = true;

      // Apenas abandona a função...
      return;
   }

   // Se passou da condicional, cai neste padrão, que é bagunçar com
   // os bits dos inteiro inicial.
   // Adicionando entropia no 'bit pattern' do número.
   SEMENTE ^= SEMENTE >> 5;
   SEMENTE ^= SEMENTE << 10;
   SEMENTE ^= SEMENTE >> 17;
   SEMENTE ^= SEMENTE << 3;
   SEMENTE ^= SEMENTE >> 12;
   SEMENTE ^= SEMENTE << 9;
   SEMENTE ^= SEMENTE >> 8;
}

extern size_t inteiro_positivo(size_t a, size_t b) {
/* O algoritmo consiste em dá alguns shifts(para qualquer lado) na
 * 'semente', e então, verificar qual o valor aleatório gerado na mexida
 * internas dos bytes de um número. */

   // Colocando mais e mais entropia no número inicial.
   alimenta_a_semente();
   // Novo valor obtido.
   size_t n = SEMENTE;

   /* O primeiro parâmetro implica no valor menor do intervalo, e o segundo
    * no maior, em caso de troca entre eles, a função entederá isso, e 
    * portanto chamará novamente a função com os valores devidamente
    * invertidos na forma correta. Valores iguais, obviamente produzirão
    * o valor dado. */
   if (a > b)
      return inteiro_positivo(b, a);
   else if (a == b)
      return b;

   //return ((rand() % SIZE_MAX) % (b - a + 1)) + a;
   return (n % (b - a + 1)) + a;
}

extern bool logico (void)
/* Sorteia um valor de 0 à 9, se estiver entre 0 à 5, retorna um valor
 * 'verdadeiro', caso contrário, 'falso' 
 */
   { return inteiro_positivo(0, 9) <= 4; }


unsigned char alfabeto_aleatorio(void) {
/* Baseado no ASCII code, entre maiúsculas e minúsculas, algum destes 
 * intervalos de letras será sorteado. No caso de maiúsculas e minúsculas,
 * a chance de sorteio de cada é 50/50. */
   int code;

   // Lance de moeda para escolher entre minúsculas e maiúsculas:
   if (logico())
      code = inteiro_positivo(0x41, 0x5a);
   else
      code = inteiro_positivo(0x61, 0x7a);

   return code;
}

char ascii_char_aleatorio(void) 
/* Selecionando só caractéres válidos. Portanto, aqueles acima de 32. */
   { return inteiro_positivo(33, 126); }

#ifdef _POSIX_SOURCE
char* palavra_aleatoria(void) {
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
   size_t inicio = SIZE_MAX, fim =SIZE_MAX, nl = 0;
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

#ifdef _UT_ALEATORIO
/* === === === === === === === === === === === === === === === === === ==
 *                            Testes Unitários
 *
 * Testando todos estrutura, métodos e funções declarados e implementados
 * acima. Esta parte abaixo pode futuramente ser colocada em outra arquivo
 * e incluído aqui, não faz a menor diferença, e por cima deixa tal arquivo
 * mais limpo. 
 * === === === === === === === === === === === === === === === === === ==*/
#include "legivel.h"
// Apenas para sistemas POSIX.
#ifdef _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <tgmath.h>
#include "teste.h"
#include "tempo.h"
#endif

void tempo_do_primeiro_inteiro_positivo_sorteado(void) {
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

void sorteio_de_caracteres_ascii(void) {
   const size_t TOTAL = 100;

   printf("todos %lu sorteios.\n", TOTAL);
   for (size_t i = 1; i <= TOTAL; i++) {
      if (i % 17 == 0)
         putchar('\n');
      printf("|%c| ", ascii_char_aleatorio());
   }
}

void sorteio_de_letras_do_alfabeto(void) {
   const size_t TOTAL = 100;

   printf("todos %lu sorteios.\n", TOTAL);
   for (size_t i = 1; i <= TOTAL; i++) {
      if (i % 17 == 0)
         putchar('\n');
      printf("|%c| ", alfabeto_aleatorio());
   }
}

void amostra_pequena_inteiros_positivos(void) {
   uint64_t total = 18;

   for(uint64_t p = 1; p <= total; p++) {
      uint64_t a = 53, b = 179;
      uint64_t x = inteiro_positivo(a, b);

      fflush(stdout);

      if (p % 5 == 0) { puts(""); }
      else { printf("%lu, ", x); }

      assert(x >= a && x<= b);
   }
}

void amostra_gigante_inteiros_positivos(void) {
   uint64_t total = 1650000;
   bool maximo_processado = false;
   bool minimo_selecionado = false;

   for(uint64_t p = 1; p <= total; p++) {
      uint64_t a = 53, b = 10502;
      uint64_t x = inteiro_positivo(a, b);

      assert(x >= a && x<= b);
      if (x == b)
         { maximo_processado = true; }
      else 
         { minimo_selecionado = true; }
   }

   puts("Sem erros com valores maiores que o pedido.");
   assert(maximo_processado);
   assert(minimo_selecionado);
   puts("Todos máximos e mínimos também foram selecionados.");
}

void distribuicao_de_valores_logicos(void) 
{
   size_t T = 7081900, verdades = 0;
   // Um porcento(1%).
   const float MINIMO = 0.01;

   for (size_t k = 1; k <= T; k++) 
      { if (logico()) verdades += 1; }

   float p_true = (float)verdades / (float)T;
   float p_false = 1.0 - p_true;
   float variacao = abs(p_true - 0.50);

   printf(
      "verdadeiros: %17.4f%% (%zu)\nfalso: %23.4f%% (%zu)\n",
      p_true * 100.0, verdades,  p_false * 100, T - verdades
   );

   assert (variacao < MINIMO);
}

static void todos_caracteres(char* texto)
{
   int t = strlen(texto);

   for (int k = 1; k <= t; k++)
      printf("\t\b\b - %c\n", texto[k - 1]);
}

void sorteio_de_palavra(void)
{
   for (size_t i = 1; i <= 10; i++) {
      char* resultado = palavra_aleatoria();
      printf("\nConteúdo: \"%s\"\n", resultado);
      todos_caracteres(resultado);
   }
}

void obtendo_tempo_de_sorteio_da_palavra(void)
{
   Cronometro contagem = cria_cronometro();

   for (size_t k = 1; k <= 45000; k++) 
       palavra_aleatoria(); 

   marca(contagem);
   printf("decorrido: %s\n",cronometro_to_str(contagem));

}

int main(int qtd, char* args[], char* vars[]) 
{
   #ifdef _WIN64
   puts("funções compatíveis com o Windows.");

   #elif __linux__
   puts ("Testes apenas para Linux:");
   executa_testes_a(
      true, 8, tempo_do_primeiro_inteiro_positivo_sorteado, false,
         sorteio_de_caracteres_ascii, true,
         sorteio_de_letras_do_alfabeto, true,
         amostra_pequena_inteiros_positivos, true,
         amostra_gigante_inteiros_positivos, false,
         distribuicao_de_valores_logicos, true,
         sorteio_de_palavra, false,
         obtendo_tempo_de_sorteio_da_palavra, false
   );
   #endif

   return EXIT_SUCCESS;
}
#endif
