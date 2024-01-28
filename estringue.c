

/* A linguagem C dá um número limitado de métodos para trabalhar com
 * strings quando comparado a outras linguagens. Aqui irei implementar
 * todas elas.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const size_t INICIAL = 100;

typedef struct {
   // array que armazena os caractéres.
   char caracteres[INICIAL];

   // quantidade de caractéres que a array interna da estrutura permite.
   size_t capacidade;

   // cursor onde adicionar mais perdaços de string.
   size_t cursor;
} string_t, *String, *STRING;

String cria_str(const char* conteudo) {
   size_t tamanho_string = strlen(conteudo);
   size_t capacity;

   // escolher capacidade baseado se a string excede a padrão.
   if (tamanho_string >= INICIAL)
      capacity = tamanho_string + 1;
   else
      capacity = INICIAL;

   // muito específico para esta máquina.
   String s = malloc(capacity + 8 + 8);
   // String s = malloc(sizeof(string_t));

   // só faz as operações abaixo, se e somente se, ter sido alocado algo.
   if (s == NULL) {
      s->capacidade = capacity;
      // preenchendo com fim's de linha.
      memset(s->buffer, '\0', s->capacidade);

      while (*conteudo) {
         s->caracteres[s->cursor] = conteudo[0];
         s->cursor += 1;
         conteudo++;
      }
   }
   return s;
}

String cria_vazia_str() 
   { return cria_str(""); }

void destroi_str(String s) {
   free(s->buffer);
   free(s);
}

size_t tamanho_str(String s) 
   { return s->cursor; }

size_t vazia_str(String s)
   { return s->cursor == 0; }

void insere_str(String s, char _char) {
   s->buffer[s->cursor] = _char;
   s->cursor += 1;
}

void concatena_str(String s, char* string) {
   while (*string) { 
      insere_str(s, string[0]); 
      string++; 
   }
}

typedef char* char_t, CHAR, Char;
// macro para outra abordagem de retorno.
#define CARACTERE_NAO_ENCONTRADO 10

/* Primeira abordagem, retornar um endereço para o caractére removido ou
 * indexado. A segunda retorna sempre um caractére, ou o valor que o macro 
 * acima define como não encontrado/removido e etc.
 */
char_t retira_str(String s) {
   /* Por que não dá apenas o endereço do caractére, já que ele não é 
    * apagada em tal operação? Bem, justamente por isso, sim ele não é 
    * apagado na operação, porém pode ser modificado futuramente com a 
    * expansão da array interna, ou mesmo na inserção de algo novo, lembre
    * se, tal posição foi removida. */
   if (vazia_str(s))
      return NULL;
   char* removido = malloc(1); 
   removido = s->buffer[s->cursor];
   s->cursor -= 1;
   return removido;
}

char_t get_str(String s, size_t i) {
   if (i >= s->cursor || vazia_str(s)) 
      // não pode exceder o 'cursor', que é a última posição.
      return NULL;
   /* neste caso ele retorna a um endereço, é seu trabalho copiar ou não 
    * o caractére dado. */
   return &s->buffer[i];
}

char retira_i_str(String s) {
   if (vazia_str(s))
      return CARACTERE_NAO_ENCONTRADO;
   /* retorna uma cópia do caractére na última posição da string, esta é 
    * a remoção. */
   s->cursor -= 1;
   return s->buffer[s->cursor + 1];
}

char get_i_str(String s, size_t i) {
   if (i >= s->cursor || vazia_str(s)) 
      // não pode exceder o 'cursor', que é a última posição.
      return CARACTERE_NAO_ENCONTRADO;

   return s->buffer[i];
}

bool set_str(String s, char ch, size_t i) {
   if (vazia_str(s) || i >= s->cursor)
      return false;
   s->buffer[i] = ch;
   // confirma operação.
   return true;
}

void imprime_str(String s) {
   printf("string: \'");
   for (size_t i = 0; i <= f; i++) {
      char* char_ = &s->buffer[i]; 
      printf("%c", *char_);
   }
   puts("\'");
}

typedef struct {
   // a string inteira.
   unsigned char* string;

   // onde nela é relevante.
   size_t inicio; size_t final;
} slice_t;

static void imprime_slice(slice_t a) {
   for (size_t i = a.inicio; i <= a.final; i++) {
      // preenche espaços com pontos prá melhor visualização(debug mode).
      #ifdef _DEBUG
      if (a.string[i] == ' ') {
         printf(".");
         continue;
      }
      #endif
      printf("%c", a.string[i]);
   }
   puts("");
}

static bool compara_slice_e_str(slice_t a, unsigned char* s) {
   for (size_t i = 0; i < strlen(s); i++) {
      if (s[i] != a.string[a.inicio + i])
         return false;
   }
   // se chegar até aqui.
   return true;
}

typedef struct { size_t* indices; size_t total; } indices_t;

static indices_t indices_buscado(unsigned char* string, unsigned char* padrao) {
   size_t t = strlen(padrao);
   size_t u = strlen(string);
   size_t contagem = 0;
   size_t capacidade = 13;
   size_t indices[capacidade]; 

   /* marcando os índices de acordo com a iteração. */
   for (size_t i = 0; i < u; i++) {
      slice_t e = {string, i, i + t};

      if (compara_slice_e_str(e, string)) {
         indices[contagem] = i;
         #ifdef _DEBUG
         printf("%lu\n", i);
         #endif
         contagem++;
      }
   }
   return (indices_t){ indices, contagem + 1 };
}

static slice_t* split(unsigned char* string, unsigned char* padrao) {
   indices_t array = indices_buscado(string, padrao);

   if (array.total == 0)
      return NULL;

   for (size_t k = 0; k < array.total - 1; k++) {
      slice_t e = { string, array.indices[k], array.indices[k + 1]};
      imprime_slice(e);
   }

   return NULL;
}

#ifdef _UT_STRING

char silly_poem[] = "roses are red; violets are blue; i see you big pretty eyes... as a huge cloud comes me soon";

unsigned char frase_i[] = "hoje será um bom dia!";

#include <stdint.h>

void iterando_slice_string() {
   slice_t s = { silly_poem, 6, 28};

   imprime_slice(s);
   for (size_t p = s.inicio; p <= s.final; p++)
      printf("|%c| ", s.string[p]);
   puts("\b\n");

   slice_t si = { frase_i, 6, 15};
   for (size_t p = si.inicio; p <= si.final; p++)
      printf("|%c|(%x) ", si.string[p], (uint8_t)si.string[p]);
   puts("\b\n");

}

void visualizando_slices() {
   puts(frase_i);
   slice_t s1 = { frase_i, 5, 8};
   imprime_slice(s1);
   slice_t s0 = { frase_i, 5, 9};
   imprime_slice(s0);

   slice_t s2 = { frase_i, 13, 15};
   imprime_slice(s2);

   slice_t s3 = { frase_i, 10, 11 };
   imprime_slice(s3);

   slice_t s4 = { frase_i, 0, 3};
   imprime_slice(s4);
}

#include "teste.h"
#include <stdlib.h>

int main(int qtd, char* args[], char* vars[]) {

   executa_testes(
      2, visualizando_slices, true,
      iterando_slice_string, true
   );

   return EXIT_SUCCESS;
}
#endif

