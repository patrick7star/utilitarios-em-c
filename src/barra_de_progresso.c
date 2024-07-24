

/* Implementação da famosa barra de progresso em C. Não será a implementação
 * completa. */

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tgmath.h>

/* ~~~ ~~~ ~~~ ~~~ ~~~   Progresso Simples ~~~ ~~~ ~~~ ~~ ~~~~ ~~~ ~~~ */

/* A impressão da barra se dá por dada variação percentual. */
typedef struct progresso_simples {
   /* valores atual e o que têm que ser atingido para ser finalizado. */
   size_t atual;
   size_t total;

   // quantos blocos de comprimento têm a 'barra'.
   uint8_t comprimento;

   /* marca a atual porcentagem do 'progresso' para computar a variação. */
   float marco;

} *Progresso, *PS;

bool esgotado_bps (Progresso p)
   { return p->atual >= p->total; }

Progresso novo_bps(size_t total, uint8_t qb) {
   // desabilitando o buffer da saída padrão. para futuras impressões.
   setbuf(stdout, NULL);
   size_t tamanho = sizeof(struct progresso_simples);
   Progresso instancia = malloc(tamanho);
   instancia->atual = 0;
   instancia->total = total;
   if (qb > 1)
      { instancia->comprimento = qb; }
   else
      { instancia->comprimento = 50; }
   instancia->marco = 0.0;
   return instancia;
}

static char* cria_barra(float percentual, uint8_t limite) {
   /* cria uma string representando as 'barras' dos progressos, dado o 
    * percentual de preenchimento. */
   char* barra = calloc (limite + 1, sizeof (char));
   uint8_t q = (uint8_t)(percentual * (float)limite);

   for(uint8_t k = 0; k < limite; k++) { 
      if (k < q)
         // símbolo do preenchimento.
         { barra[k] = 'o'; }
      else
         // símbolo do vácuo.
         { barra[k] = '.'; }
   }
   return barra;
}

static void impressao_da_barra_bps (size_t a, size_t T, uint8_t c)
{
   float p = (float)a / (float) T;
   char * barra = cria_barra(p, c);

   printf("\r%9lu/%9lu [%s]%5.1f%%", a, T, barra, (p * 100.0));
   // quebra-de-linha pelo fim do progresso.
   if (a == T) putchar ('\n');
   // liberando a string criada da memória depois de impressa.
   free (barra);
}

void visualiza_bps(Progresso p) {
   size_t a = p->atual, t = p->total;
   float percentual = 100.0 * (float)a / (float)t;
   // diferença percentual desde da última atualização do 'marco'.
   float variacao = percentual - p->marco;
   uint8_t C = p->comprimento;
   /* cem porcentual percentual dividido pelo número de blocos de barras
    * disponíveis, assim, achamos qual é o mínimo de variação percentual 
    * necessário para que permita a impressão. */
   float fronteira = 100.0 / (float)C;

   // última impressão, caso tenha se esgotado.
   if (percentual >= 100.0) {
      impressao_da_barra_bps (t, t, C);
   } else {
      /* o mínimo percentual para um bloco inteiro de barra tenha decorrido,
       * aí, sim, coloca a visualização. */
      if (variacao > fronteira) {
         impressao_da_barra_bps (a, t, C);
         // reseta para nova contagem de variação.
         p->marco = percentual;
      }
   }
}

void atualiza_bps(Progresso p, size_t novo) 
   { p->atual = novo; }

Progresso cria_bps (size_t total) 
   /* Mesmo que a criação acima, porém a barra é rearranjada de forma 
    * automatica, assim é necessário passar apenas o argumento do 'limite
    * superior'. */
   { return novo_bps (total, 50); }


/* == == == == == == == == == == == == == == == == == == == == == == == ==
 * ~~~ ~~~ ~~~ ~~~ ~~~ Progresso Temporal ~~~ ~~~ ~~~ ~~ ~~~~ ~~~ ~~~ ~~~
 * == == == == == == == == == == == == == == == == == == == == == == == = */

typedef struct progresso_tempo {
   /* valores atual e o que têm que ser atingido para ser finalizado. */
   size_t atual; size_t total;

   // informa se o progresso está esgotado.
   bool esgotado;

   // quantos blocos de comprimento têm a 'barra'.
   uint8_t comprimento;

   /* marcador da variação de tempo.*/
   time_t inicio;

} *ProgressoTemporal, *PT, progresso_t;

PT novo_bpt(size_t total, uint8_t qB) {
   size_t tamanho = sizeof(struct progresso_tempo);
   PT instancia = malloc (tamanho);

   // desabilitando o buffer da saída padrão para futuras impressões.
   setbuf(stdout, NULL);

   instancia->atual = 0;
   instancia->total = total;
   instancia->esgotado = false;

   if (qB > 1)
      { instancia->comprimento = qB; }
   else
      { instancia->comprimento = 50; }

   // marcando o íncio da contagem de tempo.
   instancia->inicio = time(NULL);
   // retornando instância válida.
   return instancia;
}

progresso_t novo_estatico_bpt(size_t total, uint8_t qB) {
   struct progresso_tempo instancia;
   // desabilitando o buffer da saída padrão para futuras impressões.
   setbuf(stdout, NULL);

   instancia.atual = 0;
   instancia.total = total;
   instancia.esgotado = false;

   if (qB > 1)
      { instancia.comprimento = qB; }
   else
      { instancia.comprimento = 50; }

   // marcando o íncio da contagem de tempo.
   instancia.inicio = time(NULL);
   // retornando instância válida.
   return instancia;
}

static void impressao_da_barra (size_t a, size_t T, float p, uint8_t C) 
{
   float percentual = (float)a / (float) T;
   char * barra = cria_barra(p, C);

   printf("\r%9lu/%9lu [%s]%5.1f%%", a,T, barra, (p * 100.0));
   // quebra-de-linha pelo fim do progresso.
   if (a == T) putchar ('\n');
   // liberando a string criada da memória depois de impressa.
   free (barra);
}

// impressão da barra, quando necessário.
void visualiza_bpt(PT p) {
   size_t a = p->atual, t = p->total;
   float percentual = (float)a / (float)t;
   uint8_t c = p->comprimento;

   // última impressão, caso tenha se esgotado.
   if (percentual >= 1.0) {
      impressao_da_barra (t, t, percentual, c);
      p->esgotado = true;
      return;
   }

   const double delimitado = 1.0;
   time_t f = time(NULL), i = p->inicio;
   double diferenca = difftime (f, i);
   bool progresso_nao_acabou = !p->esgotado;
   bool tempo_ultrapassou_delimitado = (diferenca >= delimitado);

   /* o mínimo percentual para um bloco inteiro de barra tenha decorrido,
    * aí, sim, coloca a visualização. */
   if (tempo_ultrapassou_delimitado && progresso_nao_acabou) 
   {
      impressao_da_barra (a, t, percentual, c);
      // reseta para nova contagem de variação.
      p->inicio = time(NULL);
   }
}

// atualiza o valor que está o progresso.
void atualiza_bpt(PT p, size_t novo) { 
   /* não faz alteração se o progresso esgotou-se.*/
   if (p->esgotado) { return; }
   p->atual = novo; 
}

void visualiza_e_atualiza_bpt (PT bp, size_t novo) {
   atualiza_bpt (bp, novo);
   visualiza_bpt (bp);
}

void destroi_bps(Progresso bp) { free(bp); }

void destroi_bpt(PT p) { free(p); }

// aceita qualquer um dos temporizadores.
bool esgotado_bpt(PT bp) 
   { return bp->esgotado; }

PT cria_bpt (size_t total) 
   /* Novo modo de instânciar a 'barra temporal', aqui não precisa se
    * definir um tamanho aleatóriamente, cabível na tela, portanto é 
    * necessário apenas o parâmetro total para instanciar-la. */
   { return novo_bpt (total, 30); }

/* === === === === === === === === === === === === === === === === === ==
 *                      Testes Unitários 
 * === === === === === === === === === === === === === === === === === ==*/
#ifdef _UT_BARRA_DE_PROGRESSO
#include <unistd.h>

size_t units_MiB (uint8_t n) { return n * pow(2, 20); }

void novos_metodos_do_progresso_temporal (void) {
   size_t T = units_MiB(25);
   PT bP = cria_bpt(T);

   for (size_t p = 1; p <= T; p++)
      visualiza_e_atualiza_bpt (bP, p);

   destroi_bpt (bP);
}

void reevendo_bps (void) {
   size_t total = units_MiB(255);
   Progresso barra = novo_bps (total, 48);

   for (size_t k = 2; k <= total; k++) {
      atualiza_bps(barra, k);
      visualiza_bps(barra);
   }

   destroi_bps (barra);
}

void barra_de_progresso_na_stack(void) {
   size_t TOTAL = units_MiB(25);
   progresso_t bp = novo_estatico_bpt(TOTAL, 40);

   for (size_t q = 1; q <= TOTAL; q++)
      visualiza_e_atualiza_bpt (&bp, q);
}

void main (void) {
   // novos_metodos_do_progresso_temporal();
   // reevendo_bps();
   barra_de_progresso_na_stack();

}
#endif
