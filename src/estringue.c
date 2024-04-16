

/* A linguagem C dá um número limitado de métodos para trabalhar com
 * strings quando comparado a outras linguagens. Aqui irei implementar
 * todas elas.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

typedef char* char_t, CHAR, Char;
// macro para outra abordagem de retorno.
#define CARACTERE_NAO_ENCONTRADO 10


#ifdef _UT_STRING
static void visualiza_array(size_t* array, size_t t) {
   printf ("[");
   for (size_t p = 1; p <= t; p++)
      printf ("%lu, ", array[p - 1]);
   if (t == 0)
      puts("]");
   else
      puts("\b\b]");
}
#endif

static char* copia_trecho (char* string, size_t i, size_t f) {
   if (f < i) {
      perror ("confusão nos marcos dados!");
      abort();
   }

   // faz uma cópia do trecho específico, e a retorna.
   char* trecho = string + i;
   size_t L = f - i;
   char* palavra = malloc (L * sizeof(char));

   // limpando e copiando em seguida ...
   memset (palavra, '\0', L);
   memmove (palavra, trecho, L);
   // retornando a cópia.
   return palavra;
}

typedef struct { char** lista; size_t total; } OutcomePalavras;
typedef OutcomePalavras OutcomeStrings;

static OutcomeStrings reparte_string_by_char(char* string, const char sep) 
{
   /* Separa em string à partir de qualquer caractére dado, seja ele 
    * 'imprimível' ou não.
    */
   const char SEPARADOR = sep;
   char* ptr_string = string;
   size_t contagem = 0;
   size_t posicoes[strlen(string)];

   // contando o total de palavras na string.
   while ((ptr_string = strchr (ptr_string, SEPARADOR)) != NULL) {
      size_t T = strlen(string);
      size_t t = strlen(ptr_string);
      size_t atual_posicao = T - t + 1;

      // contando o espaço em branco, e inserido a posição dele na string.
      posicoes[contagem++] = atual_posicao;
      // indo a próximo caractére...
      ptr_string++;
   }
   #ifdef _PALAVRAS
   printf ("quantidade: %lu\nposições na string:", contagem);
   visualiza_array(posicoes, contagem);
   #endif

   // repartindo ela em palavras e axexando na lista.
   size_t size_ptr_char =  sizeof (char*);
   // o total de espaços branco mais um é o total de 'palavras'.
   size_t total = contagem + 1;
   char** lista = malloc (total * size_ptr_char);
   bool existe_palavras = contagem >= 1;

   // cuidando das pontas ...
   if (existe_palavras) {
      // primeira palavra:
      // descotando a posição que começa a próxima palavra.
      char* first_word = copia_trecho (string, 0, posicoes[0] - 1);
      lista[0] = first_word;
      // última palavra:
      char* last_word = copia_trecho (
         string, 
         posicoes[contagem - 1], 
         strlen(string)
      );
      lista[total - 1] = last_word;

      #ifdef _PALAVRAS
      printf ("\tprimeira palavra: '%s'\n", first_word);
      printf ("\túltima palavra: '%s'\n", last_word);
      #endif
   }

   for (size_t i = 1; i < contagem; i++) {
      size_t inicio = posicoes[i - 1];
      // descotando a posição que começa a próxima palavra.
      size_t final = posicoes[i] - 1;
      char* word = copia_trecho (string, inicio, final);
      lista[i] = word;

      #ifdef _PALAVRAS
      printf ("\tpalavra: '%s'\n", word);
      #endif
   }
   return (OutcomePalavras) {lista, total};
}

OutcomePalavras palavras(char* string) {
   /* Pega todos trechos entre espaços em branco, sejam eles palavras 
    * legítimas ou não. */
   const char ESPACO_SEPARADOR = ' ';
   return reparte_string_by_char (string, ESPACO_SEPARADOR);
}

OutcomeStrings split_linhas(char* conteudo) {
   /* Dado uma grande string, está separada com quebras-de-linhas, aqui
    * será retornado uma cópia de todas linhas do conteúdo(sendo este 
    * provavalmente vindo de algum arquivo). */
   const char ESPACO_SEPARADOR = '\n';
   return reparte_string_by_char (string, ESPACO_SEPARADOR);
}

char* concatena_strings(uint8_t quantia, ...) {
   /* Cocatena uma quantia limitada de strings, dado a quantia delas. 
    * Nota: Como tal argumentos para função só podem ser manualmente o 
    *    tipo de inteiros que ele aceita é um de 8 bits, ou seja, só cabem
    *    apenas 256 argumentos tal função. Bastante, porém limitada, mas 
    *    mesmo com isso é um baita de quebra-galho para o tipo existente. */
   va_list argumentos;
   va_start (argumentos, quantia);
   size_t comprimento = 0;
   char* referencias[UINT8_MAX];

   for (size_t i = 1; i <= quantia; i++) {
      char* strptr = va_arg (argumentos, char*);   
      comprimento += strlen (strptr);
      // guardando a string para futura concatenação.
      referencias[i - 1] = strptr;

      #ifdef _CONCATENA_STRINGS
      printf ("string extraída: '%s'\n", strptr);
      #endif
   }
   va_end (argumentos);
   // contando caractére nulo no final.
   comprimento++;

   #ifdef _CONCATENA_STRINGS
   printf ("total de strings dadas: %u\n", quantia);
   printf ("comprimento calculado: %lu\n", comprimento);
   #endif

   char* grande_string = malloc (quantia * sizeof (char) * comprimento);
   size_t cursor = 0;
   // criando string concatenada...
   for (size_t i = 1; i <= quantia; i++) {
      char* atualstr = referencias[i - 1];
      while (*atualstr != '\0') {
         grande_string[cursor++] = *atualstr;
         atualstr++; 
      }
   }
   grande_string[comprimento - 1] = '\0';
   return grande_string;
}

#ifdef _UT_STRING
// bibliotecas essenciais:
#include <stdint.h>
#include "teste.h"
#include "dados_testes.h"
#include <stdlib.h>

void testes_basico_da_reparticao_em_palavras(void) {
   for (size_t i = 1; i <= 5; i++) {
      char* phrase = frases_i[i - 1];
      printf ("\nsobre: '%s'\n", phrase);
      palavras(phrase);
   }
}

void visualizacao_da_reparticao_em_palavras(void) {
   OutcomePalavras result = palavras (frase_ii);

   puts ("listando o resultado: ...");
   for (size_t q = 1; q <= result.total; q++)
      printf ("\t==>  '%s'\n", result.lista[q - 1]);
}

void experimento_concatenacao_de_multiplas_strings(void) {
   char* result = concatena_strings (
      4, frutas[3], frutas[5], 
         frutas[7], frutas[8]
   );
   printf ("a concatenação: '%s'\n", result);
}

int main(int qtd, char* args[], char* vars[]) {
   executa_testes(
      3, testes_basico_da_reparticao_em_palavras, true,
         visualizacao_da_reparticao_em_palavras, true,
         experimento_concatenacao_de_multiplas_strings, true
   );

   return EXIT_SUCCESS;
}
#endif

