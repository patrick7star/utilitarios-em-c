

/*
 * Implementação da estrutura Tela Terminal que se adequa a tela do 
 * terminal executado, e permite alguns desenhos e escribas nele.
 */

#include "terminal.h"
#include "texto.h"
#include "tela/pilha_ligada.h"
#include "tela/dado.h"

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
   PILHA_LIGADA modificacoes;
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
      screen->modificacoes = cria_pl();
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
      destroi_pl(t->modificacoes);
      free(t);
      t = NULL;
   } else 
      puts("valor inválido, então nada foi liberado.");
}

void visualiza_tela_debug(TELA t) {
   if (t->modificacoes == 0)
      puts("nenhuma alteração feita nela, por enquanto.");
   visualiza_matriz_texto_debug(t->grade);
}

#include "ponto.h"
#include <assert.h>

// faz uma linha reta entre os pontos não-colineares, com o
// símbolo dado, que tem que ser um caractére ASCII.
static void risca_linha(Tela t, Ponto a, Ponto b, char simbolo) {
   // coordenadas mais legíveis.
   uint8_t ya = a[0], yb = b[0];
   uint8_t xa = a[1], xb = b[1];
   uint8_t d = distancia_ponto(a, b);
   ArrayPonto coordenadas = cria_array_de_pontos(d);

   for (uint8_t p = 0; p < d; p++) {
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
      // registrando ponto, ou seja, não libera mais imediatamente.
      coordenadas[p] = ponto;
   }

   // inserindos pontos modificados.
   Dado dt = cria_dado(coordenadas, d);
   assert(coloca_pl(t->modificacoes, dt));
}

void risca_tela(TELA t, Ponto A, Ponto B) {
   // o tipo de 'risco' que a função abaixo aplica na tela.
   const char PINCEL = '#';
   risca_linha(t, A, B, PINCEL);
}

uint16_t alteracoes_tela(TELA t)
   { return quantidade_pl(t->modificacoes); }

uint16_t instancias_tela(void)
   { return total_de_instancias_da_tela; }

Dado mesclando_quatro_ap(Dado x, Dado y, Dado z, Dado w) {
   uint16_t qtd = x->qtd + y->qtd + z->qtd + w->qtd;
   ArrayPonto a = cria_array_de_pontos(qtd);

   uint16_t k = 0;
   for (uint16_t i = 0; i < x->qtd; i++, k++) 
      a[k] = x->dado[i];
   for (uint16_t i = 0; i < y->qtd; i++, k++) 
      a[k] = y->dado[i];
   for (uint16_t i = 0; i < z->qtd; i++, k++) 
      a[k] = z->dado[i];
   for (uint16_t i = 0; i < w->qtd; i++, k++) 
      a[k] = w->dado[i];

   return cria_dado(a, qtd);
}

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

   // remove para colocar numa só array, portanto mescla últimas alterações.
   Dado modificacao = mesclando_quatro_ap(
      retira_pl(t->modificacoes),
      retira_pl(t->modificacoes),
      retira_pl(t->modificacoes),
      retira_pl(t->modificacoes)
   );
   // inserindo na pilha o aglomerado das quatros modificações acima.
   coloca_pl(t->modificacoes, modificacao);

   destroi_array_de_pontos(vtcs, 4);
}

#include <errno.h>
#include <string.h>

static void escreve_string_horizontal(Tela t, Ponto p, char* s) {
   uint8_t x = p[1], y = p[0];
   uint8_t L = t->dimensao[1];
   uint8_t c = strlen(s);
   ArrayPonto coords = cria_array_de_pontos(c);

   if (x + strlen(s) > L)
      abort();

   for (uint8_t i = 0; i < c; i++) { 
      Ponto np = cria_ponto(y, i + x);
      set(t->grade, np, s[i]);
      coords[i] = np;
   }
   // inserindo tais modificações na pilha.
   Dado datum = cria_dado(coords, c);
   assert(coloca_pl(t->modificacoes, datum));
}

static void escreve_string_vertical(Tela t, Ponto p, char* s) {
   uint8_t x = p[1], y = p[0];
   uint8_t L = t->dimensao[0];
   uint8_t c = strlen(s);
   ArrayPonto coords = cria_array_de_pontos(c);

   if (y + strlen(s) > L)
      abort();

   for (uint8_t i = 0; i < c; i++) { 
      Ponto np = cria_ponto(i + y, x);
      set(t->grade, np, s[i]); 
      coords[i] = np;
   }
   Dado datum = cria_dado(coords, c);
   assert(coloca_pl(t->modificacoes, datum));
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

/* O algoritimo para isso está quase todo construído. É preciso apenas
 * retirar da pilha a array de pontos onde foi modificado a tela, e
 * fazer destes pontos a tela branca novamente, dependendo se ela é
 * invisível ou pincelada. */
bool desfaz_alteracao(Tela t) {
   // última alteração retirada da 'pilha' de modificações.
   Dado dado_ua = retira_pl(t->modificacoes);
   ArrayPonto aP = dado_ua->dado;
   uint16_t n = dado_ua->qtd;

   if (dado_ua == NULL)
      // nenhum dado retirado, pelo motivo que seja, então confirmar
      // como "nado feito".
      return false;

   for (uint16_t p = 1; p <= n; p++)
      set(t->grade, aP[p - 1], ' ');

   // no momento, tal array de pontos é apenas liberada. Futuramente,
   // será colocada numa outra pilha para desfazer, o que foi desfeito.
   destroi_dado(dado_ua);
   // confirma operação com feita.
   return true;
}

// visualização da tela, sem o debug.

#include "tela/pontos_de_ligamentos.c"

Ponto centro_tela(Tela t) {
   uint8_t y = t->dimensao[0] / 2;
   uint8_t x = t->dimensao[1] / 2;
   Ponto central = cria_ponto(y, x);
   return central;
}

static bool ponto_pertence_a_tela(Tela t, Ponto p) 
   { return (p[0] < t->dimensao[0]) && (p[1] < t->dimensao[1]); }

void desenha_simples_ponto(Tela t, Ponto p, unsigned char ch) {
   ArrayPonto coord = cria_array_de_pontos(1);
   coord[0] = p;
   set(t->grade, p, ch);
   Dado dt = cria_dado(coord, 1);
   assert(coloca_pl(t->modificacoes, dt));
}

bool marca_ponto(Tela t, Ponto a, unsigned char simbolo) {
   // verifica se o ponto dado é válido.
   if (ponto_pertence_a_tela(t, a)) {
      desenha_simples_ponto(t, a, simbolo);
      return true;
   }
   return false;
}

bool marca_pontos(Tela t, ArrayPonto pts, size_t q, 
  unsigned char simbolo) 
{
   return false;   
}
