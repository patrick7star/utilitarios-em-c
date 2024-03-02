

/* Implementação da famosa barra de progresso em C. Não será a implementação
 * completa. */

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Progresso Simples. */

/* A impressão da barra se dá por dada variação percentual. */
typedef struct progresso_simples {
   /* valores atual e o que têm que ser 
    * atingido para ser finalizado. */
   uint64_t atual;
   uint64_t total;
   // informa se o progresso está esgotado.
   bool esgotado;
   // quantos blocos de comprimento têm a 'barra'.
   uint8_t comprimento;
   /* marca a atual porcentagem do 'progresso'
    * para computar a variação. */
   float marco;
} *Progresso;

// criação do progresso da barra.
Progresso novo_bps(uint64_t total, uint8_t qb) {
   // desabilitando o buffer da saída padrão. para futuras impressões.
   setbuf(stdout, NULL);
   size_t tamanho = sizeof(struct progresso_simples);
   Progresso instancia = malloc(tamanho);
   instancia->atual = 0;
   instancia->total = total;
   instancia->esgotado = false;
   if (qb > 1)
      { instancia->comprimento = qb; }
   else
      { instancia->comprimento = 50; }
   instancia->marco = 0.0;
   return instancia;
}

// cria barra que é o símbolo do 'progresso'.
static char* cria_barra(float percentual, uint8_t limite) {
   char *barra = (char*)calloc(limite, 1);
   uint8_t q = (uint8_t)(percentual * (float)limite);
   for(uint8_t k = 0; k < limite; k++) { 
      if (k < q)
         { barra[k] = 'o'; }
      else
         { barra[k] = '.'; }
   }
   return barra;
}

// impressão da barra, quando necessário.
void visualiza_bps(Progresso p) {
   float percentual, diferenca, fronteira;
   uint8_t x, c, a, t;
   a = p->atual; t = p->total;

   percentual = (float)a / (float)t;

   // última impressão, caso tenha se esgotado.
   if (percentual >= 1.0)
   {
      char * barra = cria_barra(percentual, c);
      printf(
         "\r%lu/%lu [%s]100.0%%\n", 
         p->atual, p->total, barra
      );
      p->esgotado = true;
      return;

   }

   // diferença percentual desde da última 
   // atualização do 'marco'.
   diferenca = 100*(percentual - p->marco);
   c = p->comprimento;
   /* cem porcentual percentual dividido 
    * pelo número de blocos de barras
    * disponíveis, assim, achamos qual 
    * é o mínimo de variação percentual necessário
    * para que permita a impressão. */
   fronteira = 100.0/(float)c;

   /* o mínimo percentual para um bloco
    * inteiro de barra tenha decorrido,
    * aí, sim, coloca a visualização. */
   if (diferenca >= fronteira || p->marco == 0
     && !p->esgotado) 
   {
      // reseta para nova contagem de variação.
      p->marco = percentual;
      char * barra = cria_barra(percentual, c);
      printf(
         "\r%lu/%lu [%s]%5.1f%%", 
         p->atual, p->total, 
         barra, percentual*100
      );
   }
}

// atualiza o valor que está o progresso.
void atualiza_bps(Progresso p, uint64_t novo) 
   { p->atual = novo; }


#include <time.h>

struct progresso_tempo {
   /* valores atual e o que têm que ser 
    * atingido para ser finalizado. */
   uint64_t atual;
   uint64_t total;
   // informa se o progresso está esgotado.
   bool esgotado;
   // quantos blocos de comprimento têm a 'barra'.
   uint8_t comprimento;
   /* marcador da variação de tempo.*/
   time_t inicio;
};

/* Progresso Temporal. */
typedef struct progresso_tempo* ProgressoTemporal;
// apelido mais curto para futuras instancias.
typedef struct progresso_tempo* PT;

PT novo_bpt(uint64_t total, uint8_t qb) {
   // desabilitando o buffer da saída padrão.
   // para futuras impressões.
   setbuf(stdout, NULL);
   size_t tamanho = sizeof(struct progresso_tempo);
   PT instancia = malloc(tamanho);
   instancia->atual = 0;
   instancia->total = total;
   instancia->esgotado = false;
   if (qb > 1)
      { instancia->comprimento = qb; }
   else
      { instancia->comprimento = 50; }
   instancia->inicio = time(NULL);
   return instancia;
}

// impressão da barra, quando necessário.
void visualiza_bpt(PT p) {
   uint8_t x, c, a, t;
   a = p->atual; t = p->total;
   float percentual = (float)a / (float)t;
   c = p->comprimento;

   // última impressão, caso tenha se esgotado.
   if (percentual >= 1.0)
   {
      char * barra = cria_barra(percentual, c);
      printf(
         "\r%lu/%lu [%s]100.0%%\n", 
         p->atual, p->total, barra
      );
      p->esgotado = true;
      return;
   }

   /* A cada meio segundo é preciso mudar.*/
   double fronteira = 0.5;
   time_t final = time(NULL);
   double diferenca = difftime(final, p->inicio);

   /* o mínimo percentual para um bloco
    * inteiro de barra tenha decorrido,
    * aí, sim, coloca a visualização. */
   if (diferenca >= fronteira && !p->esgotado) 
   {
      // reseta para nova contagem de variação.
      p->inicio = time(NULL);
      char * barra = cria_barra(percentual, c);
      printf(
         "\r%lu/%lu [%s]%5.1f%%", 
         p->atual, p->total, 
         barra, (percentual * 100)
      );
   }
}

// atualiza o valor que está o progresso.
void atualiza_bpt(PT p, uint64_t novo) { 
   /* não faz alteração se o progresso esgotou-se.*/
   if (p->esgotado) { return; }
   p->atual = novo; 
}

extern void destroi_bps(Progresso bp) { free(bp); }
extern void destroi_bpt(PT p) { free(p); }

// aceita qualquer um dos temporizadores.
extern bool esgotado_bpt(PT bp) 
   { return bp->esgotado; }
extern bool esgotado_bps(Progresso bp) 
   { return bp->esgotado; }
