
#include "terminal.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* imprime o separador entre os testes
 * executados. */
static void nome_do_teste(char *nome) {
   uint8_t str_length = strlen(nome);
   Dimensao d = dimensao_terminal();
   uint8_t largura = d[1] - 3;
   char* barra = calloc(largura, 1);

   // fazendo separador.
   for (uint8_t k = 0; k <= largura; k++) 
      { barra[k] = '-'; }

   // margens do texto.
   barra[5] = ' ';
   barra[5 + str_length + 1] = ' ';

   // colocando título no separado.
   for (uint8_t k = 0; k < str_length; k++) {
      if (k < str_length && nome[k] != '\0')
         { barra[k + 6] = nome[k]; }
   }

   printf("\n%s\n\n", barra);
   // não é preciso mais desta string, já foi impressa, então libera.
   // A dimensão criada também não é mais necessária.
   destroi_dimensao(d);
   free(barra);
}
/* contabilizando testes sem nomes/ ou mensagens passadas. */
static uint8_t testes_contagem = 0;

/* executa o teste, dado a função que o faz; também é preciso do nome 
 * que ele tem, pode ser o nome da função, ou não. */
void executa_teste(char* nome, void (*funcao)()) {
   // imprime o nome do programa.
   if (strlen(nome) == 0) { 
      testes_contagem += 1;
      char mensagem_padrao[100];
      char* referencia = mensagem_padrao;
      sprintf(
         referencia, 
         "%dº teste anônimo", 
         testes_contagem
      );
      nome_do_teste(mensagem_padrao); 
   } else 
      { nome_do_teste(nome); }
   // executa função passada.
   funcao();
   // espaço para outro possível teste, ou só o 'shell'.
   puts("");
}

// conta quantos usos a função abaixo tem.
static uint8_t contagem_de_usos = 0;
/* ótima ferramenta de 'debug', onde indica a linha e o arquivo onde 
 * possível erro pode está. */
void debug_aqui() {
   //puts("o erro está bem... ... ...aqui!");
   printf("o erro está bem ... ... ...aqui!(%d)\n", contagem_de_usos + 1);
   contagem_de_usos++;
   //printf("[%s]:o erro está bem ... ... ...aqui!\n", __FILE__);
}

/* computando o número de substrings que podem se geradas, dada a 
 * string(na verdade seu comprimento, que será calculado internamente). 
 */
uint64_t total_substrings(char* str) {
   uint64_t n = strlen(str);
   return (n * n - n) / 2;
}
// copia trecho de string.
char* copia_substring(uint8_t i, uint8_t f, char* str) {
   size_t tamanho = strlen(str);
   /* índices ultrapassam limites da string. */
   bool transborda_string = (
      (f - i) > (tamanho - 1) ||
      (f >= tamanho) ||
      (i >= tamanho)
   );
   char* copia = (char*)malloc(tamanho);
   memset(copia, ' ', tamanho);

   if (i > f)
      { return copia_substring(f, i, str); }

   if (copia == NULL || transborda_string || i == f)
      return NULL;


   for(uint8_t p = i; p <= f; p++)
      { copia[p - i] = str[p]; }

   return copia;
}
/* gera todas substrings que uma string pode ter.*/
StringArray todas_substrings(char* str) {
   uint8_t largura = strlen(str);
   // total de possíveis resultados.
   uint64_t total = total_substrings(str);
   uint8_t altura = (uint8_t)total;
   StringArray array;
   array = cria_string_array(altura, largura, Invisivel); 

   for (uint8_t l = 1; l < largura; l++) {
      for(uint8_t p = 0; p < largura-l; p++){
         char* aux = copia_substring(p, (p + l), str);
         strcpy(array[total - 1], aux);
         free(aux);
         total -= 1;
      }
   }

   return array;
}

/* "bate" a correspodência entre duas strings, tenha elas ou não o 
 * mesmo comprimento. */
bool strings_correspodentes(char *str1, char *str2) {
  /* o modo aqui de achar alguma correspodência é, verificar todas 
   * fatias da menor string, com a maior. Estas fatias a serem 
   * verificadas, sempre partem do começo de ambas até a menor. */ 
   /* supõe que a primeira é menor.*/
   uint8_t c = strlen(str1); 
   uint8_t C = strlen(str2);
   /* se não for retorno recursivo com argumentos
    * invertidos é aplicado, para que se encaixe
    * perfeitamente na função. */
   if (c > C)
      { return strings_correspodentes(str1, str2); }

   for(uint8_t t = 2; t < c; t++) {
      uint8_t resultado = memcmp(str1, str2, t);
      // ou seja, igual!
      if (resultado == 0) {
         return true;
      }
   }
   // se chegar aqui, não há qualquer correspodência entre as strings.
   return false;
}

#include <time.h>

enum submultiplo_de_tempo { NANOSEG, MICROSEG, MILISEG, SEG };
typedef enum submultiplo_de_tempo ORDEM;

/* modificação da função 'sleep' para aceitar de 
 * forma facil todos arrangos possíveis de tempo
 * do sistema. */
static void pausa(ORDEM escala, uint16_t t) {
   size_t tamanho = sizeof(struct timespec);
   struct timespec* duracao;
   duracao = (struct timespec*)malloc(tamanho);
   /* converte cada tempo em 'nanosegundos', dado
    * sua ordem, claro que se for 'nanoseg', então
    * é passado direto como argumento. */
   switch (escala) {
      case NANOSEG:
         duracao->tv_nsec = t;
         duracao->tv_sec = 0;
         if (t > 1000)
            perror("só valor menores que mil nesta ORDEM.");
         break;
      case MICROSEG:
         duracao->tv_nsec = t * 1000;
         duracao->tv_sec = 0;
         if (t > 1000)
            perror("só valor menores que mil nesta ORDEM.");
         break;
      case MILISEG:
         duracao->tv_nsec = t * 1000000;
         duracao->tv_sec = 0;
         if (t > 1000)
            perror("só valor menores que mil nesta ORDEM.");
         break;
      case SEG:
         duracao->tv_nsec = t * 100;
         duracao->tv_sec = t;
         break;
   }
   /* espera em 'nanosegundos', mesmo que 
    * não tenha sido isso inicialmente, porém
    * lembre-se: alguns milhares de 'nanosegundos' são
    * alguns meros 'microsegundos', e alguns
    * milhares de 'microsegundos' são só alguns 
    * 'milisegundos', e assim em diante até 'segundos'. */
   nanosleep(duracao, NULL);
}

/* série de testes */
void espera_pouco_segundos() {
   printf("tempo em espera...");
   fflush(stdout);
   pausa(SEG, 5); 
   puts("passado 8seg.");
}

void intervalo_de_milisegundos() {
   for(uint8_t k = 1; k <= 23; k++) {
      puts("passado 300 milisegundos.");
      fflush(stdout);
      pausa(MILISEG, 300);
   }
}

/* o mesmo que o 'executa_teste', porém este pode ser facilmente
 * desativado se for preciso, apenas mudando o parâmetro. 
 */
void executa_teste_interruptor(char* descricacao, 
  void (*funcao)(), bool acionado) 
{
   // não executa nada se for falso.
   if (strlen(descricacao) == 0) { 
      testes_contagem += 1;
      char mensagem_padrao[100];
      char* referencia = mensagem_padrao;
      sprintf(
         referencia, 
         "%dº teste anônimo", 
         testes_contagem
      );
      nome_do_teste(mensagem_padrao); 
         
   } else 
      nome_do_teste(descricacao);

   if (acionado)
      // quebra-de-linha no final.
      { funcao(); puts(""); }
   else
      puts("DESATIVADO TEMPORIAMENTE!");
}

const char* bool_to_str(bool valor) {
   if (valor)
      return "verdadeiro";
   else
      return "falso";
}

// por motivos de compatibilidade, deixaremos o nome antigo.
const char* bool_str(bool valor)
   { return bool_to_str(valor); }

#include <stdarg.h>
#include "tempo.h"
#include "legivel.h"

typedef void(*Fn)(void);

void executa_testes(const uint8_t total, ...) {
   Cronometro medicao = cria_cronometro();

   va_list args;
   // como também conta o valor lógico se é para executa-lá no momento.
   va_start(args, total);

   for (uint8_t t = 1; t <= 2 * total; t+=2) {
      // primeiro pega a função.
      Fn funcao = va_arg(args, Fn);
      // a confirmação se é para executa-lá, ou não.
      bool e_para_executar = va_arg(args, int);

      if (e_para_executar) {
         executa_teste("", funcao);
         marca(medicao);
      }
   }

   va_end(args);

   // informando tempo total da execução.
   double tempo_total = marca(medicao);
   if (tempo_total <= 0)
      puts("\nexecução \"instântanea\".");
   else 
      printf(
         "\ntodos testem levaram %s\n", 
         tempo_legivel(tempo_total)
      );
}

char* concatena_literais_str(const uint8_t total, ...) {
   char juncao[1000];
   char* resultado = juncao;

   va_list strings;
   va_start(strings, total);

   for (uint8_t t = 1; t <= total; t++) {
      char* s = va_arg(strings, char*);
      strcat(juncao, s);
   }

   // retornando referência da string estática criada.
   return resultado;
}

bool str_to_bool(char* s) {
   bool e_verdadeiro = {
      !strcmp(s, "v") || 
      !strcmp(s, "true") || 
      !strcmp(s, "t") || 
      !strcmp(s, "verdadeiro")
   };

   bool e_falso = {
      !strcmp(s, "f") || 
      !strcmp(s, "false") || 
      !strcmp(s, "falso")
   };

   if (e_verdadeiro)
      return true;
   else if (e_falso)
      return false;
   else
      { perror("a string não é válida!"); abort(); }
}

/* transforma um algarismo(valor de 0 à 9) na sua versão caractére. Se
 * o valor passado exceder isso, gerará um erro. */
char algarismo(uint8_t alg) {
   switch (alg) {
      case 0:
         return '0';
      case 1:
         return '1';
      case 2:
         return '2';
      case 3:
         return '3';
      case 4:
         return '4';
      case 5:
         return '5';
      case 6:
         return '6';
      case 7:
         return '7';
      case 8:
         return '8';
      case 9:
         return '9';
      default:
         perror("não é válido para tal número(apenas 0 à 9).");
         abort();
   }
}

#include <assert.h>
/* verifica se um número inteiro positivo é potência de 2, por
 * obviamente, divisões sucessivas de dois. */
static bool potencia_de_dois(size_t n) {
   // o valor zero não é válido aqui.
   assert (n > 0);

   while (n > 0) {
      // se houver um que 2 não divide, então n não é potência de 2.
      if (n % 2 != 0)
         return false;
      n /= 2;
   }
   // se chegar até aqui, então todas divisões por dois foram bem sucedidas, logo é uma potência de 2.
   return true;
}

#include <tgmath.h>
/* O modo de computar os digitos binários são seguinte: Pegado o valor
 * decimal(um inteiro de base 10), nós contabilizando o total de 
 * dígitos na base dois que seu valor produziria. Criamos uma array
 * com mais ou menos este tamanho para colocar todos dígitos binários
 * correspodentes. Então criamos uma string dinâmica, com o mesmo 
 * tamanho da array com dígitos obviamente, iteramos todos valores
 * que representam dígitos na array, porém, acessando primeiro o último
 * elemento da array(o mais a direita), pois o modo de acha-lôs é 
 * invertido, colocando último "dígito" acessado na array como o 
 * primeiro na array de caractéres(string), e o primeiro dígito na 
 * array como o último "dígito" na string. 
 */
extern char* binario_str(size_t decimal) {
   if (decimal == 0) return "0000";
   uint8_t qtd_digitos = floor(log2(decimal)) + 1;
   /* criando a VLA depois de calcular a quantidade de dígitos exata.*/
   uint8_t digitos[qtd_digitos];
   // acrescenta mais uma 'vagão' na string para o caractére nulo.
   char* bitpattern = malloc((qtd_digitos + 1) * sizeof(char));
   // já marcando a string com o caractére 'nulo'.
   bitpattern[qtd_digitos] = '\0';

   /* os retos da divisão por 2 dão o equivalente em binário, porém
    * numa ordem inversa. Também é necessário dividir o valor inicial
    * por 2, já que deixando inalterado, produziria sempre o mesmo
    * dígito. */
   for (size_t i = 0; i < qtd_digitos; i++, decimal /= 2) 
      digitos[i] = decimal % 2; 

   /* o primeiro na string é o último na 'array de dígitos', e assim em
    * diante. */
   for (size_t k = 0; k < qtd_digitos; k++) 
      bitpattern[k] = algarismo(digitos[qtd_digitos - (k + 1)]);

   return bitpattern;
}

static void imprime_debug(uint8_t* array, size_t t) {
   if (t == 0) 
      { printf("[]\n"); return; }

   printf("[");
   for (size_t k = 0; k < t; k++)
      printf("%d, ", array[k]);
   printf("\b\b]\n");
}

/* Descobre mesmo o padrão binário do valor, provavelmente escrito em
 * complemento de dois, para especificamente 8-bits.*/
uint8_t* binario_complemento_de_dois_oito_bits(uint8_t n) {
   uint8_t* lista_de_bits = malloc(8 * sizeof(uint8_t));
   // mascaras necessárias para operações AND com todos bits.
   uint8_t mascaras[] = { 
      0x01, 0x02, 0x04, 0x08, 
      0x10, 0x20, 0x40, 0x80
   };

   /* A captura de bits será feita da direita para esquerda. */
   for (size_t i = 0; i < 8; i++) {
      /* extração do bit é simples, uso a máscara compatível ao seu 
       * atual algarismo para manter o valor apenas do algarismo desejado
       * e zerar os demais, então aplico um shift à direita, de acordo
       * quanto tal algarismo está da ponta direita. 
       * A indexação inversa é necessária pois está maquina, e as demais
       * usadas são todas little-endian.
       */
      // indexando na última casa, já que a CPU é 'little-endian'.
      lista_de_bits[8 - (i + 1)] = (n & mascaras[i]) >> i;
   }
   return lista_de_bits;
}

/* inverte a ordem dos itens na array. O algoritmo para fazer tal coisa
 * é o seguinte: executa o swap entre todos pares de pontas da array,
 * tipo, primeira troca com a última, a segunda com a penúltima, a 
 * terceira com a antipenúltima e assim em diante. O total de swaps
 * é metade da array obviamente. 
 */
static void inverte_array(uint8_t* array, size_t t) {
   for (size_t p = 0; p < (t / 2); p++) {
      uint8_t salvo = array[p];
      array[p] = array[t - (p + 1)];
      array[t - (p + 1)] = salvo;
   }
}

uint8_t* binario_complemento_de_dois(size_t n) {
   // computando o total de dígitos binários necessários.
   uint8_t qtd = floor(log2(n)) + 1;
   /* Todos valores serão armazenados numa array de bytes, pois os 
    * únicos valores que serão extraídos estão são 0 e 1. */
   uint8_t* lista_de_bits = malloc(qtd * sizeof(uint8_t));
   // mascaras necessárias para operações AND com todos bits.
   size_t mascaras[qtd]; 

   // complementando máscaras...
   for (size_t p = 1; p <= qtd; p++)
      mascaras[p - 1] = (size_t)pow(2, p - 1);

   /* A captura de bits será feita da direita para esquerda. */
   for (size_t i = 0; i < qtd; i++) {
      /* extração do bit é simples, uso a máscara compatível ao seu 
       * atual algarismo para manter o valor apenas do algarismo desejado
       * e zerar os demais, então aplico um shift à direita, de acordo
       * quanto tal algarismo está da ponta direita. 
       * A indexação inversa é necessária pois está maquina, e as demais
       * usadas são todas little-endian.
       */
      // indexando na última casa, já que a CPU é 'little-endian'.
      lista_de_bits[i] = (n & mascaras[i]) >> i;
   }
   // faz uma inversão...
   return lista_de_bits;
}

#ifdef UT_TESTE
#include "teste.h"

void teste_conversao_binaria_antiga_implementacao() {
   for (uint8_t i = 0; i <= 16; i++)
      printf("%d ===> %s\n",i, binario_str(i));
}

void amostra_da_nova_implementacao_de_binario() {
   for (uint8_t n = 1; n < 16; n++) {
      uint8_t* resultado =  binario_complemento_de_dois_oito_bits(n);
      imprime_debug(resultado, 8);
   }
}

void extracao_de_bits_implementacao_geral() {
   for (uint8_t n = 1; n < 16; n++) {
      uint8_t* resultado =  binario_complemento_de_dois_oito_bits(n);
      uint8_t* outro_resultado = binario_complemento_de_dois(n);
      printf("antiga: ");
      imprime_debug(resultado, 8);
      inverte_array(outro_resultado, 8);
      printf(" atual: ");
      imprime_debug(outro_resultado, 8);
      puts("");
   }
}

void verificando_obtendo_de_potencias_de_dois() {
   // mascaras necessárias para operações AND com todos bits.
   uint8_t qtd = 30;
   // complementando máscaras...
   for (size_t p = 1; p <= qtd; p++) {
      size_t n = (size_t)pow(2, p - 1);
      printf("%2luº ---> %9lu -->%31s\n", p, n, binario_str(n));
   }
}

int main(int qtd, char* argumentos[], char* envp[]) {
   executa_testes(
      4, teste_conversao_binaria_antiga_implementacao, false,
      amostra_da_nova_implementacao_de_binario, false,
      extracao_de_bits_implementacao_geral, true,
      // iteração para gerar máscaras funciona!
      verificando_obtendo_de_potencias_de_dois, false
   );

   return EXIT_SUCCESS;
}
#endif 

