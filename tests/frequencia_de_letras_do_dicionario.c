#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "hashtable_ref.h"
#include "aleatorio.h"
#include "progresso.h"


/* === === === === === === === === === === === === === === === === === ==
 *                      Métodos das Chaves e Valores
 * === === === === === === === === === === === === === === === === === ==*/
// Traços para os valores:
char* sizet_to_str(generico_t a) {
   size_t* ptr = (size_t*)a;
   int sz = 13 * sizeof(char);
   char* result = malloc(sz);

   sprintf(result, "%lu", *ptr);
   return result;
}
size_t* box_i(size_t a) {
   int sz = sizeof(size_t);
   size_t* dt = malloc(sz);

   if (dt != NULL) 
      *dt = a;
   return dt;
}
bool sizet_del(generico_t b) 
   { free(b); return true; }

// Traços para as chaves:
char* string_to_str(generico_t e) {
   char* ptr = (char*)e;
   int t = strlen(ptr) + 4;
   char* result = malloc(t * sizeof(char));

   sprintf(result, "\"%s\"", ptr);
   return result;
}
size_t string_hash(generico_t a, size_t b) {
   char* s = (char*)a;
   int t = strlen(s);
   return (s[0] * s[t/2] * s[t - 1]) % b;
}
bool string_eq(generico_t a, generico_t b) {
   char* str_a = (char*)a;
   char* str_b = (char*)b;

   return strcmp(str_a, str_b) == 0;
}
bool string_del(generico_t a) 
   { free(a); return true; } 

char* nova_str(int n) {
   const int sz = sizeof(char);
   char* string = malloc(n * sz);

   memset(string, '\0', n);
   return string;
}

/* === === === === === === === === === === === === === === === === === ==
 *                Estrutura de Frequencia de Letras 
 * === === === === === === === === === === === === === === === === === ==*/
#include <ctype.h>

typedef HashTable LetraFrequencia, LF;

char* box_char(char simbolo) {
   char* obj = malloc(sizeof(char));

   *obj = simbolo;
   return obj;
}

size_t key_hash_lf(generico_t X, size_t T)
{
   char letra = *((char*)X);
   char minuscula = tolower(letra);

   // printf("letra: %c em %d\n", minuscula, minuscula / (int)T);
   return (int)minuscula / T;
}

bool key_eq_lf(Generico a, Generico b) 
{
   char letra_a = tolower(*((char*)a));
   char letra_b = tolower(*((char*)b));
   // const char* result = letra_a == letra_b ? "true": "false";

   // printf("%c == %c? %s\n", letra_a, letra_b, result);
   return letra_a == letra_b;
}

bool key_and_value_drop_lf(generico_t e) 
   { free(e); return true; }

LetraFrequencia cria_lf(void)
{
   HashTable dicio = cria_ht(key_hash_lf, key_eq_lf);
   const int T = 97 + 26;

   for (int k = 97; k < T; k++)
   {
      char* key = box_char((char)k); 
      char letter = k;
      size_t* value = box_i(0);

      sprintf(key, "%c", letter);
      insere_ht(dicio, key, value);
   }
   return dicio;
}

void analiza_palavra(LF table, char* word) {
   // printf("word: %s\n", word);
   generico_t old_value = obtem_ht(table, word);

   if (old_value != NULL) {
      size_t* new_value = old_value;

      *new_value += 1;
      // atualiza_ht(table, word, new_value);
   } else {
   }
}

/* === === === === === === === === === === === === === === === === === ==
 *                Bloco Principal de Execução do Programa
 * === === === === === === === === === === === === === === === === === ==*/
int main(void) {
   HashTable M = new_ht(string_hash, string_eq);
   const size_t N = 500;
   PG bar = cria_bp(Simples, N, 53);

   puts("Tabela e barra-de-progresso criadas.");

   for (size_t k = 1; k < N; k++) 
   {
      char* X = palavra_aleatoria();
      size_t * Y = box_i(1);
      bool result = add_ht(M, X, Y);

      if (!result) {
         size_t* valor = get_ht(M, X);
         *valor += 1;
      }
      atualiza_bp(&bar, k);
      visualiza_bp(&bar);
   }
   printf("\nTotal de itens inseridos: %zu\n", len_ht(M));

   LetraFrequencia lf = cria_lf();
   IterHT iter = cria_iter_ht(M);

   puts("Mais frequêntes palavras sorteadas:");
   imprime_ht(lf, string_to_str, sizet_to_str);

   while (!consumido_iter_ht(iter)) {
      IterOutputHT output = next_ht(iter);
      char* ch = (char*)output.key;
      size_t* vl = (size_t*)output.value;

      if (*vl > 2) 
         printf("\t- \"%s\": %zu\n", ch, *vl);
      analiza_palavra(lf, ch);
   }

   destroi_iter_ht(iter);
   puts("\nFinalmente, frequência de letras referente à todas palavras:");
   imprime_ht(lf, string_to_str, sizet_to_str);
   destroi_ht(M);
   // destroi_interno_ht(lf, string_del, string_del);

   return EXIT_SUCCESS;
}
