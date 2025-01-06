

/* ferramentas importante de tempo como Cronometro e Temporizador. Os
 * "métodos" dela serão um pouco diferente e limitada comparada outras 
 * linguagens, pois o C não é uma linguagem OO. 
 */

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include "tempo.h"


#include "tempo/calculo_potencia.c"

/* Pausa a thread principal é extremamente complicado de se fazer
 * com a função da biblioteca padrão, então aqui vou cria um 
 * menos complicadinho. 
 */
void breve_pausa(TEMPO_TIPO tipo, uint16_t qtd) 
{
	#ifdef __linux__
   /* Só é permitido valores menores que mil, pois qualquer coisa 
    * acima quebra os limite do 'Tipo de Tempo'. */
   if (qtd > 1000)
      { perror("não pode passar para o tipo."); abort(); }

   struct timespec periodo;
   switch(tipo) {
      // tempos inteiros(maior que um segundo):
      case Segundo:
         periodo.tv_sec = qtd;
         periodo.tv_nsec = 0;
         break;
      case Minuto:
         periodo.tv_sec = qtd * 60;
         periodo.tv_nsec = 0;
         break;
      case Hora:
         periodo.tv_sec = qtd * 3600;
         periodo.tv_nsec = 0;
         break;
      // fração de segundos:
      case Miliseg:
         periodo.tv_sec = 0;
         periodo.tv_nsec = qtd * potencia(10, 6);
         break;
      case Microseg:
         periodo.tv_sec = 0;
         periodo.tv_nsec = qtd * potencia(10, 3);
         break;
      case Nanoseg:
         periodo.tv_sec = 0;
         periodo.tv_nsec = qtd;
         break;
   }

   // aplicando o tempo depois de configurado.
   thrd_sleep(&periodo, NULL);
	#elif defined(_WIN32)
	perror("Não implementado para Windows ainda.");
	abort();
	#endif
}

/* Importando o restante do código para cá. Funções, estruturas, 
 * variáveis globais tudo está sendo puxado. O arquivo está num 
 * diretório com o nome deste arquivo 'tempo'.
 * Observe que a ordem que as importações são feitas são importantes,
 * já que, podem importar bibliotecas que são utilizados nas demais
 * importações, ou tem funções essenciais nas próximo. A ordem seguem
 * como foram recortadas nestes arquivo, e colada em outros. Não
 * podem ser mudadas.
 */
#include "tempo/cronometro.c"   // primeiro
#ifdef __linux__
#include <threads.h>
#include "tempo/temporizador.c" // segundo.
#elif defined(_WIN32)
#include "tempo/temporizador-win.c" // segundo.
#endif


/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 *
 *                       Testes Unitários 
 *
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---*/
#if defined(_UT_TEMPO)
#include <assert.h>
#ifdef __linux__
#include <unistd.h>
#elif defined(_WIN32)
#include <locale.h>
#endif

#ifdef __linux__
void alocacao_e_desalocacao() {
   Cronometro c = cria_cronometro();
   // pausa.
   breve_pausa(Segundo, 2);
   breve_pausa(Miliseg, 700);
   destroi_cronometro(c);

   Cronometro c1 = cria_cronometro();
   breve_pausa(Miliseg, 26);
   destroi_cronometro(c1);
}

void visualizacao_dos_marcos() {
   Cronometro clock = cria_cronometro();
   breve_pausa(Segundo, 1);

   for (int i = 0; i < 95; i++) { 
      marca(clock);
      if (i % 6 != 0)
         printf("registrado\t");
      else
         puts("");
      breve_pausa(Miliseg, rand() % 150 + 50); 
   }
   // invocando a função que faz a listagem.
   puts("\nregistros feitos:");
   visualiza_marcos(clock);
}

void stringficacao_do_temporizador() {
   Temporizador timer = cria_temporizador(Microseg, 20);
   assert(timer->reutilizacoes == 0);

   breve_pausa(Microseg, 9);
   breve_pausa(Nanoseg, 500);
   puts(timer_to_str(timer));

   breve_pausa(Microseg, 6);
   breve_pausa(Nanoseg, 100);
   puts(timer_to_str(timer));

   recomecar(timer);
   assert(timer->reutilizacoes == 1);

   breve_pausa(Microseg, 3);
   puts(timer_to_str(timer));

   breve_pausa(Microseg, 12);
   puts(timer_to_str(timer));

   breve_pausa(Microseg, 8);
   puts(timer_to_str(timer));

   destroi_temporizador(timer);

   Temporizador timer_seg = cria_temporizador(Segundo, 8);

   breve_pausa(Segundo, 3);
   puts(timer_to_str(timer_seg));

   breve_pausa(Segundo, 4);
   puts(timer_to_str(timer_seg));

   breve_pausa(Segundo, 2);
   puts(timer_to_str(timer_seg));

   destroi_temporizador(timer_seg);
}

void dois_tais_ate_o_esgotamento(void) {
   Temporizador a = cria_temporizador(Segundo, 8);
   Temporizador b = cria_temporizador(Segundo, 13);

   while (!(esgotado(a) && esgotado(b))) {
      char* fmt_a = timer_to_str(a);
      char* fmt_b = timer_to_str(b);

      printf("a ==> %s \tb ==> %s\n", fmt_a, fmt_b);
      free(fmt_b); free(fmt_a);

      breve_pausa(Miliseg, 700);
   }
   // Após o esgotamento:
   char* fmt_a = timer_to_str(a);
   char* fmt_b = timer_to_str(b);
   printf("a ==> %s \tb ==> %s\n", fmt_a, fmt_b);
   free(fmt_b); free(fmt_a);

   destroi_temporizador(a);
   destroi_temporizador(b);
}

void reutilizacao_do_temporizador(void) {
   Timer obj = cria_temporizador(Segundo, 5);
   bool temporizador_esta_esgotado = esgotado(obj);

   assert (obj->reutilizacoes == 0);
   do {
      temporizador_esta_esgotado = esgotado(obj);

      char* fmt = timer_to_str(obj);
      printf("%s\n", fmt);
      free(fmt);

      breve_pausa(Miliseg, 800);
   } while(!temporizador_esta_esgotado);

   recomecar(obj);
   assert (obj->reutilizacoes == 1);
   do {
      temporizador_esta_esgotado = esgotado(obj);

      char* fmt = timer_to_str(obj);
      printf("%s\n", fmt);
      free(fmt);

      breve_pausa(Miliseg, 800);
   } while(!temporizador_esta_esgotado);

   recomecar(obj);
   assert (obj->reutilizacoes == 2);
   do {
      temporizador_esta_esgotado = esgotado(obj);

      char* fmt = timer_to_str(obj);
      printf("%s\n", fmt);
      free(fmt);

      breve_pausa(Miliseg, 800);
   } while(!temporizador_esta_esgotado);
   destroi_temporizador(obj);
}

void disparando_o_cronometro(void) {
   Cronometro tool = cria_cronometro();
   double entrada[] = {3.51, 5.81, 7.23, 8.01, 14.2};
   size_t cursor = 0;
   
   while(cursor < 5) {
      // Registra a variação:
      marca(tool);
      // Impressão:
      char* fmt = cronometro_to_str(tool);
      printf("%s \t ~%3.2lfseg\n", fmt, entrada[cursor]);
      free(fmt);
      // Pausa deterministica:
      sleep((int)entrada[cursor++]);
   }
   destroi_cronometro(tool);
}

void teste_desconhecido_i() {
   clock_t inicio = clock();
   alocacao_e_desalocacao();
   // [aviso!]função consome muito tempo, e bastante verbosa também.
   // visualizacao_dos_marcos();
   clock_t fim = clock();

   double tempo_do_processo = (fim - inicio) / CLOCKS_PER_SEC;
   printf("clocks por seg: %ld\ndecorrido: %lf\n", 
   CLOCKS_PER_SEC, tempo_do_processo);
}

void teste_desconhecido_ii(void) {
   clockid_t tipo = CLOCK_MONOTONIC;
   struct timespec inicio, fim;

   clock_getres(tipo, &inicio);
   nanosleep(&(struct timespec){7, 15000}, NULL);
   clock_getres(tipo, &fim);

   printf("inicio: %ldseg %ldns\nfim: %ldseg %ldns\n",
      inicio.tv_sec, inicio.tv_nsec, fim.tv_sec, fim.tv_nsec);
   size_t decorrido_seg = fim.tv_sec - inicio.tv_sec;
   size_t decorrido_nano = fim.tv_nsec - inicio.tv_nsec;
   printf("%ldseg %ldns\n", decorrido_seg, decorrido_nano);
}
#elif defined(_WIN32)
#include <synchapi.h>

#define CABECALHO_DA_FUNCAO(void) ({\
	printf( \
		"\n\n[linha: %d] Função em execução agora: '%s'\n", \
		__LINE__, __FUNCTION__ \
	);\
})

void alocacao_e_desalocacao() {
	CABECALHO_DA_FUNCAO();
   Cronometro c = cria_cronometro();
   // pausa.
	Sleep(2700);
   destroi_cronometro(c);

   Cronometro c1 = cria_cronometro();
	Sleep(26);
   destroi_cronometro(c1);
}

void visualizacao_dos_marcos() {
	CABECALHO_DA_FUNCAO();

   Cronometro clock = cria_cronometro();
	Sleep(1000);

   for (int i = 0; i < 95; i++) { 
      marca(clock);
      if (i % 6 != 0)
         printf("registrado\t");
      else
         puts("");
		Sleep(rand() % 150 + 50);
   }
   // invocando a função que faz a listagem.
   puts("\nregistros feitos:");
   visualiza_marcos(clock);
}

void stringficacao_do_temporizador() {
	CABECALHO_DA_FUNCAO();

   Temporizador timer = cria_temporizador(Miliseg, 20);
   assert(timer->reutilizacoes == 0);

	Sleep(500);
   puts(timer_to_str(timer));

	Sleep(6);
   puts(timer_to_str(timer));

   recomecar(timer);
   assert(timer->reutilizacoes == 1);

	Sleep(3);
   puts(timer_to_str(timer));

	Sleep(12);
   puts(timer_to_str(timer));

	Sleep(8);
   puts(timer_to_str(timer));

   destroi_temporizador(timer);

   Temporizador timer_seg = cria_temporizador(Segundo, 8);

	Sleep(3000);
   puts(timer_to_str(timer_seg));

	Sleep(4000);
   puts(timer_to_str(timer_seg));

	Sleep(2000);
   puts(timer_to_str(timer_seg));

   destroi_temporizador(timer_seg);
}

void dois_tais_ate_o_esgotamento(void) {
	CABECALHO_DA_FUNCAO();

   Temporizador a = cria_temporizador(Segundo, 8);
   Temporizador b = cria_temporizador(Segundo, 13);

   while (!(esgotado(a) && esgotado(b))) {
      char* fmt_a = timer_to_str(a);
      char* fmt_b = timer_to_str(b);

      printf("a ==> %s \tb ==> %s\n", fmt_a, fmt_b);
      free(fmt_b); free(fmt_a);

		Sleep(700);
   }
   // Após o esgotamento:
   char* fmt_a = timer_to_str(a);
   char* fmt_b = timer_to_str(b);
   printf("a ==> %s \tb ==> %s\n", fmt_a, fmt_b);
   free(fmt_b); free(fmt_a);

   destroi_temporizador(a);
   destroi_temporizador(b);
}

void reutilizacao_do_temporizador(void) {
	CABECALHO_DA_FUNCAO();

   Timer obj = cria_temporizador(Segundo, 5);
   bool temporizador_esta_esgotado = esgotado(obj);

   assert (obj->reutilizacoes == 0);
   do {
      temporizador_esta_esgotado = esgotado(obj);

      char* fmt = timer_to_str(obj);
      printf("%s\n", fmt);
      free(fmt);

		Sleep(800);
   } while(!temporizador_esta_esgotado);

   recomecar(obj);
   assert (obj->reutilizacoes == 1);
   do {
      temporizador_esta_esgotado = esgotado(obj);

      char* fmt = timer_to_str(obj);
      printf("%s\n", fmt);
      free(fmt);

		Sleep(800);
   } while(!temporizador_esta_esgotado);

   recomecar(obj);
   assert (obj->reutilizacoes == 2);
   do {
      temporizador_esta_esgotado = esgotado(obj);

      char* fmt = timer_to_str(obj);
      printf("%s\n", fmt);
      free(fmt);

		Sleep(800);
   } while(!temporizador_esta_esgotado);
   destroi_temporizador(obj);
}

void disparando_o_cronometro(void) {
	CABECALHO_DA_FUNCAO();

   Cronometro tool = cria_cronometro();
   double entrada[] = {3.51, 5.81, 7.23, 8.01, 14.2};
   size_t cursor = 0;
   
   while(cursor < 5) {
      // Registra a variação:
      marca(tool);
      // Impressão:
      char* fmt = cronometro_to_str(tool);
      printf("%s \t ~%3.2lfseg\n", fmt, entrada[cursor]);
      free(fmt);
      // Pausa deterministica:
      // sleep((int)entrada[cursor++]);
		Sleep((DWORD)entrada[cursor++]);
   }
   destroi_cronometro(tool);
}
#endif

int main(int qtd, char* argumentos[], char* envp[]) 
{
	setlocale(LC_CTYPE, "en_US.UTF-8");
	/* Os códigos estão sendo chamados de forma crua, porque a módulo que 
	 * provém testes precisa deste módulo como depedência. */
   alocacao_e_desalocacao();
   visualizacao_dos_marcos();
   stringficacao_do_temporizador();
   dois_tais_ate_o_esgotamento();
   reutilizacao_do_temporizador();
   disparando_o_cronometro();

   return EXIT_SUCCESS;
}
#endif
