

/* A linguagem C dá um número limitado de métodos para trabalhar com
 * strings quando comparado a outras linguagens. Aqui irei implementar
 * todas elas.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

