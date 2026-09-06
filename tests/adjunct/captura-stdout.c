#include "captura-stdout.h"
// Módulos auxiliares:
#include "reveste-arvore.h"
// Bibliotec padrão do C:
#include <stdio.h>
#include <string.h>
#include <errno.h>
// API do Linux:
#include <fcntl.h>
#include <unistd.h>

static void conteudo_capturada(int lido);

/* Formatação padrão do erro dado, mostra a linha e o texto da mensagem
 * de erro. */
#define ERRO_MSG(RETORNO) ({                                   \
   char format[100], *erro_msg = NULL;                         \
   int line = __LINE__;                                        \
   if(RETORNO == -1) {                                         \
      erro_msg = strerror(errno);                              \
      sprintf(format, "line: %d | (%s)\n", line, erro_msg);    \
      perror(format);                                          \
   } \
});

void captura_visualizacao_da_arvore(Tree arvore, ArvorePrint viewer)
{ 
   const int MAX = string_buffer_size();
   int lido, tubos[2];
   const int size = MAX * MAX;
   const int STDOUT_COPY = dup(STDOUT_FILENO);
   char* buffer = string_buffer();
   auto visualiza_arvore = viewer;

   ERRO_MSG(pipe(tubos));
   ERRO_MSG(dup2(tubos[1], STDOUT_FILENO));

   visualiza_arvore(arvore);
   lido = read(tubos[0], buffer, size); 

   ERRO_MSG(lido);
   ERRO_MSG(dup2(STDOUT_COPY, STDOUT_FILENO));
   ERRO_MSG(close(tubos[0]));
   ERRO_MSG(close(tubos[1]));
   conteudo_capturada(lido);
}

static void conteudo_capturada(int lido)
{
   char* string = string_buffer();

   printf("Foram lido %d bytes.\n", lido);
   printf("O que foi capturado:\n\t'''\n%s\n\t'''\n\n", string);
   fflush(stdout);
}
