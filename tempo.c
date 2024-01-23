

/* ferramentas importante de tempo como Cronometro e Temporizador. Os
 * "métodos" dela serão um pouco diferente e limitada comparada outras 
 * linguagens, pois o C não é uma linguagem OO. 
 */

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

// todos pesos necessários para passar um tempo preciso.
enum tipo_de_tempo { 
   // tempois inteiros:
   Segundo, 
   Minuto, 
   Hora, 
   // frações de segundos:
   Miliseg,
   Microseg, 
   Nanoseg 
};

// nome mais curto e visível.
typedef enum tipo_de_tempo TEMPO_TIPO;

#include <stdio.h>
#include <threads.h>

#include "tempo/calculo_potencia.c"

/* pausa a thread principal é extremamente complicado de se fazer
 * com a função da biblioteca padrão, então aqui vou cria um 
 * menos complicadinho. 
 */
void breve_pausa(TEMPO_TIPO tipo, uint16_t qtd) {
   // só é permitido valores menores que mil, pois qualquer coisa 
   // acima quebra os limite do 'Tipo de Tempo'.
   if (qtd > 1000)
      { perror("não pode passar para o tipo."); }

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
#include "tempo/temporizador.c" // segundo.

#if defined(_UT_TEMPO)

void alocacao_e_desalocacao() {
   Cronometro c = cria_cronometro();
   // pausa.
   breve_pausa(Segundo, 2);
   breve_pausa(Miliseg, 700);
   destroi_cronometro(c, true);

   Cronometro c1 = cria_cronometro();
   breve_pausa(Miliseg, 26);
   destroi_cronometro(c1, true);
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

int main(int qtd, char* argumentos[], char* envp[]) 
{
   clock_t inicio = clock();
   alocacao_e_desalocacao();
   // [aviso!]função consome muito tempo, e bastante verbosa também.
   // visualizacao_dos_marcos();
   clock_t fim = clock();

   double tempo_do_processo = (fim - inicio) / CLOCKS_PER_SEC;
   printf("clocks por seg: %ld\ndecorrido: %lf\n", 
   CLOCKS_PER_SEC, tempo_do_processo);
   
   return EXIT_SUCCESS;
}

#endif
