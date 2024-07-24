/*
  Um bom modo de organizar dados tabelados. O módulo possuí uma estrutura 
onde você pega todo 'rol' de dados, cede um 'rótulo' a ele e toda vez que 
impresso será visualizado fechado por caractéres `Unicode` de uma maneira 
bem formatada no terminal. A estrutura `Coluna` que proporciona isso, 
também aceita a impressão de outras juntas, formando assim uma bonita 
tabela.
*/

// Biblioteca padrão do C:
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
// Biblioteca deste próprio repositório:
#include "listaarray_ref.h"
#include "terminal.h"

static size_t maior_string(vetor_t* L, ToString F) {
   size_t t = tamanho_al(L);
   size_t maximo = 0;
   char* dado_fmt;

   for (size_t i = 1; i <= t; i++) {
      generico_t dado = indexa_al(L, i - 1);
      dado_fmt = dado;
      size_t comprimento = strlen(dado_fmt);

      if (comprimento > maximo)
         maximo = comprimento;
   }
   // Máximo, somado a margem.
   return maximo + 2;
}

static char* ajusta_entrada(generico_t dt, ToString f, size_t c) {
   size_t size = c * sizeof(char);
   char* string = malloc(size);
   // Formatação do dado numa string.
   char* dado_fmt = f(dt);
   // Comprimento da formatação do dado.
   size_t t = strlen(dado_fmt);

   #ifdef __debug__
   const char BRANCO = '.';
   #else
   const char BRANCO = ' ';
   #endif

   for (size_t i = 0; i < c; i++) {
      if (i < t)
         string[i] = dado_fmt[i];
      else
         string[i] = BRANCO;
   }
   string[c] = '\0';
   free(dado_fmt);
   return string;
}

void listar(vetor_t* lista, ToString f) {
/* Lista uma lista de qualquer coisa, dado seu tal lista, e seu modo de 
 * transformar o dado interno que ela porta numa string. */
   Dimensao dim = dimensao_terminal();
   size_t M = maior_string(lista, f);
   size_t C = dim[1] / M;
   size_t total = tamanho_al(lista);
   generico_t dado;
   size_t contagem = 0;

   #ifdef __debug__
   printf("\nMaior comprimento de string: %lu\n", M);
   printf("linhas=%u colunas=%u\n\n", dim[0], dim[1]);
   #endif

   while (total-- > 0) {
      dado = indexa_al(lista, contagem++);
      char* entrada = ajusta_entrada(dado, f, M);
      printf("%s", entrada);
      free(entrada);
      if (contagem % C == 0)
         putchar('\n');
   }
   // Para garantiar quebra de linha.
   putchar('\n');
}

/* === === === === === === === === === === === === === === === === === ===+
 * .......................................................................&
 * ........................Testes Unitários...............................&
 * .......................................................................&
 * === === === === === === === === === === === === === === === === === ===*/
#ifdef _UT_IMPRESSAO
#include <assert.h>
#include "dados_testes.h"

char* constchar_to_str(generico_t dt) {
   char* pointer = (char*)dt;
   size_t t = strlen(pointer);
   size_t size = (t + 1) * sizeof(char);
   char* copia = malloc(size);
   strcpy(copia, pointer);
   return copia;
}

void primeiro_exemplo_da_funcao_de_listagem(void) {
   vetor_t* L = cria_al();

   for (size_t k = 1; k <= VEICULOS; k++) 
      insere_al(L, (char*)veiculos[k - 1]);
   for (size_t k = 1; k <= BOYS_NAMES; k++) 
      insere_al(L, (char*)boys_names[k - 1]);

   listar(L, constchar_to_str);
}

void checando_um_caractere_multibyte(void) {
   const char exemplo[] = "você está certo, faça!";
   size_t t = strlen(exemplo);

   puts("Tentando identificar multibyte-chars ...");
   for (size_t p = 1; p <= t; p++) {
      char* ptr = (char*)&exemplo[p - 1];
      printf("\t>>> '%c'(%d)\n", exemplo[p - 1], mblen(ptr, t));
   }
}

int main(void) {
   // executando testes ...
   primeiro_exemplo_da_funcao_de_listagem();
   // checando_um_caractere_multibyte();
}
#endif
