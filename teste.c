
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
