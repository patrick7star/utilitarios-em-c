

/*
 * Implementação da estrutura Tela Terminal que se adequa a tela do 
 * terminal executado, e permite alguns desenhos e escribas nele.
 */

#include "terminal.h"
#include "texto.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* Nunca serão criadas na ordem de milhares ou milhões de Telas, no
 * máximo, algumas dezenas, então apenas um byte é o suficiente para 
 * contar-lás.
 */
static uint8_t total_de_instancias_da_tela = 0;

struct tela {
   // tamanho da tela trabalhada(não pode ser modificada).
   Dimensao dimensao;

   // matriz contendo os pixel(caractéres) dos rasbíscos.
   MT grade;

   // contabilização de modificações feitas nele.
   uint16_t modificacoes;
   // apenas a última alteração é gravada por enquanto.
   // ARRAY_PONTO alteracoes;
   // total de pontos da array acima.
   uint16_t qtd_pontos;
};

// nome curto do tipo de dado.
typedef struct tela* TELA;
typedef TELA Tela;
// tamanho da estrutura.
#define TELA_SIZE sizeof(struct tela)

TELA cria_tela() {
   TELA screen = (TELA)malloc(TELA_SIZE);

   if (screen != NULL) {
      screen->dimensao = dimensao_terminal();
      /* ajustando largura, já que por algum motivo, transborda em
       * quatro caractéres a impressão da borda; e já que são apenas
       * quatro unidades, não faz tanta diferença.
       */
      screen->dimensao[1] -= 4;
      screen->grade = cria_matriz_texto(
         screen->dimensao, 
         Invisivel
      );
      puts("Matriz Texto criada com sucesso.");
      // nenhuma array referênciada inicialmente.
      // screen->alteracoes = NULL;
      screen->qtd_pontos = 0;
      screen->modificacoes = 0;
      puts("Tela criada com sucesso.");
   }

   return screen;
}

void destroi_tela(TELA t) {
   if (t != NULL) {
      puts("destrói grade de caractéres interna.");
      destroi_matriz_texto(t->grade);
      puts("destrói tupla da Dimensao.");
      puts("finalmente, libera a 'Tela'.");
      // destroi_array_de_pontos(t->alteracoes, t->qtd_pontos);
      free(t);
   } else 
      puts("valor inválido, então nada foi liberado.");
}

void visualiza_tela_debug(TELA t) {
   if (t->modificacoes == 0)
      puts("nenhuma alteração feita nela, por enquanto.");
   visualiza_matriz_texto_debug(t->grade);
}

#include "ponto.h"

// faz uma linha reta entre os pontos não-colineares, com o
// símbolo dado, que tem que ser um caractére ASCII.
static void risca_linha(Tela t, Ponto a, Ponto b, char simbolo) {
   // coordenadas mais legíveis.
   uint8_t ya = a[0], yb = b[0];
   uint8_t xa = a[1], xb = b[1];

   for (uint8_t p = 0; p < distancia_ponto(a, b); p++) {
      uint8_t y, x;
      // rabisco horizontal
      if (ya == yb) 
         { y = yb; x = xa + p; }
      // rabísco vertical.
      else if (xa == xb)
         { y = ya + p; x = xb;}
      // rabisco diagonal.
      else { 
         if (ya < yb && xa < xb) {
            y = ya + p;
            x = xa + p;
         } else if (yb < ya && xb > xa){
            y = yb + p;
            x = xb - p;
         } else { 
            perror("não trabalhado com tal configuração!");
            break;
         }
      }
      Ponto ponto = cria_ponto(y, x);
      set(t->grade, ponto, simbolo);
      destroi_ponto(ponto);
   }

   // contabilizando modificação.
   t->modificacoes++;
}

void risca_tela(TELA t, Ponto A, Ponto B) {
   // o tipo de 'risco' que a função abaixo aplica na tela.
   const char PINCEL = '#';
   risca_linha(t, A, B, PINCEL);
}

uint16_t alteracoes_tela(TELA t)
   { return t->modificacoes; }

uint16_t instancias_tela(void)
   { return total_de_instancias_da_tela; }

void circunscreve_retangulo(TELA t, Ponto a, Ponto b) {
   ARRAY_PONTO vtcs = retangulo_vertices(a, b);
   const char char_v = '&';
   const char char_h = '=';

   // parte superior.
   risca_linha(t, vtcs[0], vtcs[1], char_h);
   // lateral esquerda.
   risca_linha(t, vtcs[0], vtcs[3], char_v);
   // parte inferior.
   risca_linha(t, vtcs[3], vtcs[2], char_h);
   // lateral direita.
   risca_linha(t, vtcs[1], vtcs[2], char_v);

   destroi_array_de_pontos(vtcs, 4);
}

#include <errno.h>
#include <string.h>

static void escreve_string_horizontal(Tela t, Ponto p, char* s) {
   uint8_t x = p[1], y = p[0];
   uint8_t L = t->dimensao[1];
   uint8_t c = strlen(s);

   if (x + strlen(s) > L)
      abort();

   for (uint8_t i = 0; i < c; i++) { 
      Ponto np = cria_ponto(y, i + x);
      set(t->grade, np, s[i]);
   }
}

static void escreve_string_vertical(Tela t, Ponto p, char* s) {
   uint8_t x = p[1], y = p[0];
   uint8_t L = t->dimensao[0];
   uint8_t c = strlen(s);

   if (y + strlen(s) > L)
      abort();

   for (uint8_t i = 0; i < c; i++) { 
      Ponto np = cria_ponto(i + y, x);
      set(t->grade, np, s[i]); 
   }
}

enum sentido { Horizontal, Vertical };
typedef enum sentido SENTIDO;

void escreve_string(Tela t, Ponto p, char* s, SENTIDO onde) {
   switch (onde) {
      case Horizontal:
         escreve_string_horizontal(t, p, s);
         break;
      case Vertical:
         escreve_string_vertical(t, p, s);
         break;
   }
}


