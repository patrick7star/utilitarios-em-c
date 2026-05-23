#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>

#include "hashtable_ref.h"
#include "aleatorio.h"
#include "progresso.h"
#include "primitivos.h"
#include "memoria.h"

#define MAX 4000

static void conteudo_arquivo(char*);
static void varredura_de_vogais_deste_arquivo(void);
static void frequencia_do_alfabeto_no_arquivo(void);
static bool libera(GenT);

int main(void) {

   varredura_de_vogais_deste_arquivo();
   frequencia_do_alfabeto_no_arquivo();

   return EXIT_SUCCESS;
}

static bool libera(GenT X)
   { free(X); return true; }

static void frequencia_do_alfabeto_no_arquivo(void)
{
   HashTable dicio = new_ht(hash_char, eq_char);
   char conteudo[MAX], vogal, *chave = NULL;
   int *vl = NULL, *contador = NULL, q, t;
   ToString f, g; Drop l;

   conteudo_arquivo(conteudo);
   print_ht(dicio, f, g);

   f = debug_char;
   g = debug_int;
   l = libera;
   t = strlen(conteudo);

   for (q = 0; q < t; q++) 
   {
      if (!isalpha(conteudo[q]))
         continue;

      vogal = conteudo[q];
       vl = get_ht(dicio, &vogal);

       if (vl != NULL) (*vl)++;

       else{
         contador = box_int(0);
         chave = box_char(vogal);

         if (add_ht(dicio, chave, contador))
            printf("Letra %c adicionado com valor %d.\n", *chave, *contador);
       }
   }

   print_ht(dicio, f, g);
   drop_i_ht(dicio, l, l);
   puts("Deconstrutor finalizado com sucesso.");
}

static void varredura_de_vogais_deste_arquivo(void)
{
   HashTable dicio = new_ht(hash_char, eq_char);
   char vogais[] = "aeiou";
   int contadores[] = {0, 0, 0, 0, 0}, q, t, *vl;
   char conteudo[MAX], vogal;
   ToString f, g;

   conteudo_arquivo(conteudo);
   print_ht(dicio, f, g);

   f = debug_char;
   g = debug_int;
   t = strlen(conteudo);

   add_ht(dicio, &vogais[0], &contadores[0]);
   add_ht(dicio, &vogais[1], &contadores[1]);
   add_ht(dicio, &vogais[2], &contadores[2]);
   add_ht(dicio, &vogais[3], &contadores[3]);
   print_ht(dicio, f, g);

   for (q = 0; q < t; q++) {
      vogal = conteudo[q];

       vl = get_ht(dicio, &vogal);

       if (vl != NULL)
         (*vl)++;
   }

   print_ht(dicio, f, g);
   destroi_ht(dicio);
}

static void conteudo_arquivo(char* output)
{
   memset(output, 0x00, MAX);

   int result, cursor = 0;
   const char* pathname = "tests/frequencia_de_letras_do_dicionario.c";
   FILE* arquivo = fopen(pathname, "r");

   if (arquivo == NULL)
      perror(strerror(errno));
   else
      puts("Arquivo aberto com sucesso.");

   do {
      output[cursor] = fgetc(arquivo);
      cursor++;

   } while (!feof(arquivo));
   puts("Foi tudo passado para a string.");

   if (fclose(arquivo) == -1)
      perror(strerror(errno));
   else
      puts("Stream fechado com sucesso.");
}
