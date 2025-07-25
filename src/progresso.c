// Declaração das funções e tipos de dados abaixo:
#include "progresso.h"
// Biblioteca padrão do C:
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Atributos do objeto. Alguns valores variam por plataforma.
#ifdef _WIN32
const char BARRA = '*';
#elif defined(__linux__)
const char BARRA = 'o';
#endif
const char VACUO = '.';
const size_t COMPRIMENTO = 45;

/* == == == == == == == == == == == == == == == == == == == == == == == === 
 *                          Progresso Simples 
 * == == == == == == == == == == == == == == == == == == == == == == === */
// Apelido mais usado abaixo:
typedef PS simple_progress_t; 

bool esgotado_bps (RefPS p)
/* Verifica se */
   { return p->atual >= p->total; }

simple_progress_t cria_bps(size_t total, uint8_t comprimento) {
   simple_progress_t instancia;

   instancia.atual = 0;
   instancia.total = total;
   if (comprimento > 1)
      instancia.comprimento = comprimento;
   else
      instancia.comprimento = COMPRIMENTO;
   instancia.marco = 0.0;
   return instancia;
}

static char* cria_barra(float percentual, uint8_t limite) {
/* Cria uma string representando as 'barras' dos progressos, dado o 
    * percentual de preenchimento. */
   char* barra = calloc (limite + 1, sizeof (char));
   uint8_t q = (uint8_t)(percentual * (float)limite);

   for(uint8_t k = 0; k < limite; k++) { 
      if (k < q)
         // símbolo do preenchimento.
         { barra[k] = BARRA; }
      else
         // símbolo do vácuo.
         { barra[k] = VACUO; }
   }
   return barra;
}

static void impressao_da_barra_bps (size_t a, size_t T, uint8_t c)
{
   float p = (float)a / (float) T;
   char * barra = cria_barra(p, c);

   #ifdef _WIN32
   printf("\r%9zu/%9zu [%s]%5.1f%%", a, T, barra, (p * 100.0));
   fflush(stdout);
   #elif defined(__linux__)
   printf("\r%9lu/%9lu [%s]%5.1f%%", a, T, barra, (p * 100.0));
   fflush(stdout);
   #endif
   // quebra-de-linha pelo fim do progresso.
   if (a == T) { putchar ('\n'); }
   // liberando a string criada da memória depois de impressa.
   free (barra);
}

void visualiza_bps(RefPS p) {
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

void atualiza_bps(RefPS p, size_t novo) 
   { p->atual = novo; }

simple_progress_t cria_padrao_bps (size_t total) 
/* Mesmo que a criação acima, porém a barra é rearranjada de forma 
 * automatica, assim é necessário passar apenas o argumento do 'limite
 * superior'. */
   { return cria_bps (total, COMPRIMENTO); }

/* == == == == == == == == == == == == == == == == == == == == == == == === 
 *                          Progresso Temporal
 * == == == == == == == == == == == == == == == == == == == == == == == == */
// Apelido usado na implementação abaixo:
typedef PT time_progress_t;

time_progress_t cria_bpt(size_t total, uint8_t qB) {
   time_progress_t instancia;

   instancia.atual = 0;
   instancia.total = total;
   instancia.esgotado = false;

   if (qB > 1)
      { instancia.comprimento = qB; }
   else
      { instancia.comprimento = 50; }

   // marcando o íncio da contagem de tempo.
   instancia.inicio = time(NULL);
   // time(&instancia.inicio);
   // retornando instância válida.
   return instancia;
}

static void impressao_da_barra (size_t a, size_t T, float p, uint8_t C) 
{
/* Faz a formatação da barra, dados seu atual valor, o total, percentual 
 * disso alcançado, e o comprimento da 'barra de progresso'. */
   char * barra = cria_barra(p, C);

   #ifdef _WIN64
   printf("\r%9zu/%9zu [%s]%5.1lf%%", a,T, barra, (p * 100.0));
   fflush(stdout);
   #elif defined(__linux__)
   printf("\r%9lu/%9lu [%s]%5.1lf%%", a,T, barra, (p * 100.0));
   fflush(stdout);
   #endif
   // quebra-de-linha pelo fim do progresso.
   if (a == T) putchar ('\n');
   // liberando a string criada da memória depois de impressa.
   free (barra);
}

void visualiza_bpt(RefPT p) {
/* Imprime a 'barra de progresso' quando decorrido um determinado intervalo.
 * Essencial essa impressão quando necessário, pois imprimir a cada iteração
 * de algum loop, usaria excessivamente muito recurso da máquina. */
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
void atualiza_bpt(RefPT p, size_t novo) { 
   /* não faz alteração se o progresso esgotou-se.*/
   if (p->esgotado) { return; }
   p->atual = novo; 
}

void visualiza_e_atualiza_bpt (RefPT bp, size_t novo) {
/* Atualiza e mostra a barra na mesma chamada. */
   atualiza_bpt (bp, novo);
   visualiza_bpt (bp);
}

bool esgotado_bpt(RefPT bp) 
   { return bp->esgotado; }

time_progress_t cria_padrao_bpt (size_t total) 
/* Novo modo de instânciar a 'barra temporal', aqui não precisa se
 * definir um tamanho aleatóriamente, cabível na tela, portanto é 
 * necessário apenas o parâmetro total para instanciar-la. */
   { return cria_bpt (total, COMPRIMENTO); }

/* == == == == == == == == == == == == == == == == == == == == == == == == =
 *                         Temporizador Genérico
 * == == == == == == == == == == == == == == == == == == == == == == == = */
PG cria_bp(TipoDeProgresso t, size_t total, uint8_t comprimento) {
   PG instancia;
   instancia.classe = t;

   switch (instancia.classe) {
   case Temporal:
      instancia.progresso.temporal = cria_bpt(total, comprimento);
      break;
   case Simples:
      instancia.progresso.simples = cria_bps(total, comprimento);
   };
   return instancia;
}

bool esgotado_bp(RefPG a) {
   switch(a->classe) {
   case Temporal:
      return esgotado_bpt(&a->progresso.temporal);
   case Simples:
      return esgotado_bps(&a->progresso.simples);
   };
}

bool atualiza_bp(RefPG a, size_t v) {
   switch(a->classe) {
   case Temporal:
      atualiza_bpt(&a->progresso.temporal, v);
   case Simples:
      atualiza_bps(&a->progresso.simples, v);
   };
   return true;
}

void visualiza_bp(RefPG a) {
   switch(a->classe) {
   case Temporal:
      visualiza_bpt(&a->progresso.temporal);
   case Simples:
      visualiza_bps(&a->progresso.simples);
   };
}

bool atualiza_e_visualiza_bp(RefPG a, size_t v)
{
   switch(a->classe)
   {
   case Temporal:
      atualiza_bpt(&a->progresso.temporal, v);
      visualiza_bpt(&a->progresso.temporal);
   case Simples:
      atualiza_bps(&a->progresso.simples, v);
      visualiza_bps(&a->progresso.simples);
   };
   return true;
}

/* == == == == == == == == == == == == == == == == == == == == == == == == =
 *                         Interface em Inglês
 *
 *   Não só em inglês os nomes ficam mais simples e legíveis, como também
 * possibilita uma compreensão ainda mais universal de tal estrutura
 * com outras por aí.
 * == == == == == == == == == == == == == == == == == == == == == == == = */
 PG new_bp(TipoDeProgresso t, size_t n, uint8_t c)
   { return cria_bp(t, n, c); }

 bool finished_bp(RefPG a) 
   { return esgotado_bp(a); }

 bool update_bp(RefPG a, size_t v) 
   { return atualiza_bp(a, v); }

 void print_bp(RefPG a) 
   { visualiza_bp(a); }

 bool update_e_print_bp(RefPG b, size_t v)
   { return atualiza_e_visualiza_bp(b, v); }

/* == == == == == == == == == == == == == == == == == == == == == == == === 
 *                     Testes Unitários e seus Auxiliares
 * = == == == == == == == == == == == == == == == == == == == == == == == */
#ifdef __unit_tests__
/* Testes, em suas respectivas plataformas, estão separados entre os 
 * definitivos macros. */
void taxa_de_aumento(size_t* t) 
{
   size_t T = *t;

   // Taxas de crescimento dependendo do progresso.
   if (T < 100)
      *t += 5;
   else if (T >= 100 && T < 200)
      *t += 3;
   else if (T >= 200 && T <= 400)
      *t += 2;
   else 
      (*t)++;
}

#ifdef __linux__
#include "teste.h"
#include "tempo.h"
#include <assert.h>
#include <time.h>
#include <unistd.h>
/* Testes que foram definidos espeicificamente para plataformas Unix. */

/* Converte um total de megabytes in bytes. */
const uint64_t L = 30;
const struct timespec PAUSA = {0, 600000000};

void progresso_simples() {
   ProgressoSimples p = cria_bps(L, 0);

   for (size_t k = 1; k <= L; k++) 
   { 
      atualiza_bps(&p, k);
      visualiza_bps(&p); 
      // sleep(1); 
      nanosleep(&PAUSA, NULL);
   }
   assert(esgotado_bps(&p));
}

void progresso_simples_varios_tamanhos() {
   ProgressoSimples pI = cria_bps(L, 4);
   ProgressoSimples pII = cria_bps(L, 10);
   ProgressoSimples pIII = cria_bps(L, 20);

   for (uint64_t k = 0; k <= L; k++) { 
      atualiza_bps(&pI, k);
      visualiza_bps(&pI); 
      nanosleep(&PAUSA, NULL);
   }
   assert(esgotado_bps(&pI));

   for (uint64_t k = 0; k <= L; k++) { 
      atualiza_bps(&pII, k);
      visualiza_bps(&pII); 
      nanosleep(&PAUSA, NULL);
   }
   assert(esgotado_bps(&pII));

   for (uint64_t k = 0; k <= L; k++) { 
      atualiza_bps(&pIII, k);
      visualiza_bps(&pIII); 
      nanosleep(&PAUSA, NULL);
   }
   assert(esgotado_bps(&pIII));
}

void progresso_temporal() {
   PT p = cria_bpt(L, 32);

   for (uint64_t k = 0; k <= L; k++) { 
      atualiza_bpt(&p, k);
      visualiza_bpt(&p); 
      nanosleep(&PAUSA, NULL);
   }
}

void duas_chamadas_ao_mesmo_tempo_pg(void)
{
   size_t i = 0, T = 500;
   const size_t ms = 400;
   PG bar = cria_bp(Simples, T, 50);

   do {
      taxa_de_aumento(&i);
      atualiza_e_visualiza_bp(&bar, i);
      breve_pausa(Miliseg, ms);

   } while (i < T);
}

#elif defined(_WIN32)
/* Série de testes definidas especificamente para o Windows. Nas demais
 * plataformas, ele basicamente não serão compilados. */
#include <windows.h>

size_t units_MiB (uint8_t n) 
   { return n * pow(2, 20); }

void novos_metodos_do_progresso_temporal (void) {
   size_t T = units_MiB(25);
   time_progress_t progresso = cria_padrao_bpt(T);

   for (size_t p = 1; p <= T; p++)
      visualiza_e_atualiza_bpt (&progresso, p);
}

void reevendo_bps (void) {
   size_t total = units_MiB(255);
   simple_progress_t barra = cria_bps (total, 48);

   for (size_t k = 2; k <= total; k++) {
      atualiza_bps(&barra, k);
      visualiza_bps(&barra);
   }
}

void teste_de_conversao_estatica(void) {
   size_t TOTAL = units_MiB(100);
   simple_progress_t bp = cria_bps(TOTAL, 70);

   printf("Total a carregar é %zu\n", TOTAL);
   for (size_t t = 1; t <= TOTAL; t++) {
      atualiza_bps(&bp, t);
      visualiza_bps(&bp);
   }
}

void uso_simples_da_barra_generica(void) {
   size_t i = 0, T = 500;
   PG a = cria_bp(Simples, T, 50);
   const size_t ms = 100;

   do {
      taxa_de_aumento(&i);
      atualiza_bp(&a, i);
      visualiza_bp(&a);
      Sleep(ms);
   } while (i < T);

   T = 300; i = 0;
   PG b = cria_bp(Temporal, T, 20);
   do {
      taxa_de_aumento(&i);
      atualiza_bp(&b, i);
      visualiza_bp(&b);
      Sleep(ms);
   } while (i < T);
}
#endif

#ifdef __unit_tests__
int main (void) {
   #ifdef _WIN64
   #include <locale.h>
      // Configuração apenas para o terminal do Windows.
      setlocale(LC_CTYPE, "en_US.UTF-8");

      novos_metodos_do_progresso_temporal();
      reevendo_bps();
      teste_de_conversao_estatica();
      uso_simples_da_barra_generica();

   #elif defined(__linux__)
      puts("Śérie de testes no Linux.\n");

      executa_testes (
         4, progresso_simples, true,
            progresso_simples_varios_tamanhos, true,
            progresso_temporal, true,
            // Desativado, pois consome muito tempo:
            duas_chamadas_ao_mesmo_tempo_pg, false
      );
   #endif

   return EXIT_SUCCESS;
}
#endif
#endif
