
/* Passar o cronômetro para aqui, onde fica mais organizado a distinção
 * entre seus métodos, e outras funções do 'módulo tempo', sem falar
 * que facilita a leitura.
 */

// total de registros que é possível fazer.
#define TOTAL_REGISTROS 100

struct cronometro {
   time_t inicio; 
   time_t marcos[TOTAL_REGISTROS];

   // quantos registros foram feitos.
    uint8_t qtd;
};
// tamanho da 'struct' acima.
#define CLOCK_SIZE sizeof(struct cronometro)
// nomes melhores para estrutura.
typedef struct cronometro *Cronometro;

Cronometro cria_cronometro() {
   Cronometro novo = malloc(CLOCK_SIZE);

   // se alocado corretamente.
   if (novo != NULL) {
      novo->inicio = time(NULL);
      novo->qtd = 0;
      #ifdef _DEBUG_CRIA_CRONOMETRO
      puts("cronômetro foi alocado com sucesso.");
      #endif
   } else { 
      perror("cronômetro não foi criado corretamente."); 
      abort();
   }
   return novo;
}

#include <stdio.h>
#include <stdbool.h>
#include "../legivel.h"

void destroi_cronometro(Cronometro c, bool info) {
   //if (info) {
   #ifdef _DEBUG_DESTROI_CRONOMETRO
      time_t fim = time(NULL);
      double diferenca = difftime(fim, c->inicio);
      printf(
         "\ntempo decorrido: %s\nregistros feitos: %d\n",
         tempo_legivel(diferenca), c->qtd
      );
   // }
   #endif

   if (c != NULL)
      free(c);
   else {
      #ifdef _DEBUG_DESTROI_CRONOMETRO
      puts("o tal cronômetro não existe!");
      #endif
   }
}

double marca(Cronometro c) {
   if (c->qtd >= TOTAL_REGISTROS) 
      perror("não é possível mais fazer registros.");
   
   c->marcos[c->qtd] = time(NULL);
   c->qtd++;

   // retorna a diferença decorrida até agora.
   return difftime(time(NULL), c->inicio);
}

void visualiza_marcos(Cronometro c) {
   for(int total = c->qtd; total > 0; total--) {
      // tempo decorrido desde o ínicio.
      double t = difftime(c->marcos[total - 1], c->inicio);

      // só funciona para contagem na faixa de segundos e uma hora.
      if (t > 0 && t < 3600.0) {
         int ordem = abs(total - c->qtd) + 1;
         int min = (int)t / 60;
         int seg = (int)t % 60;
         printf("%3dº. [%02d:%02d]\n", ordem, min, seg);
      }
   }
   puts("\b\b");
}

double variacao(Cronometro c) {
   // quanto variou do último registro marcado, se houver algum é claro.
   if (c->qtd == 0)
      perror("nenhum registro foi marcado.");

   time_t f = time(NULL);
   time_t i = c->marcos[c->qtd - 1];
   return difftime(f, i);
}

char* cronometro_to_str(Cronometro c) {
   /* transforma o cronômetro num formato string. */
   char* resultado_fmt = (char*)calloc(30, sizeof(char));
   double t = marca(c);

   // desconsiderando tempo marcado.
   c->qtd--;

   sprintf(
      resultado_fmt, 
      "cronômetro (%s, %3d)", 
      tempo_legivel(t), c->qtd
   );
   // retornando a slice-string transformada.
   return resultado_fmt;
}
