

// biblioteca padrão em C(libs muito utilizadas.):
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <threads.h>

// seus módulos:
#include "../teste.h"
#include "../tempo.h"
#include "../legivel.h"


void cronometro_instancia_basica() {
   CRONOMETRO c = cria_cronometro();
   // pausa.
   breve_pausa(Segundo, 2);
   breve_pausa(Miliseg, 700);
   destroi_cronometro(c, true);

   CRONOMETRO c1 = cria_cronometro();
   breve_pausa(Miliseg, 26);
   destroi_cronometro(c1, true);
}

void visualizacao_dos_marcos() {
   CRONOMETRO c = cria_cronometro();

   breve_pausa(Segundo, 1);

   for (int i = 0; i < 95; i++) { 
      marca(c);
      breve_pausa(Miliseg, 50); 
   }

   // invocando a função que faz a listagem.
   visualiza_marcos(c);
}

void tentando_criar_um_temporizador() {
   TEMPORIZADOR t = cria_temporizador(Segundo, 2);

   for(int k = 5; k > 0; k--) {
      printf(
         "%dª informação posterior ... (%s)\n", 
         k, bool_str(esgotado(t))
      );
      breve_pausa(Miliseg, 500);
   }

   printf("temporizador esgotado? %s\n", bool_str(esgotado(t)));
   assert (esgotado(t));

   destroi_temporizador(t);
}

#include <math.h>

void uso_casual_do_temporizador() {
   uint64_t contagem = 0;
   TEMPORIZADOR t = cria_temporizador(Miliseg, 500);

   while (!esgotado(t)) {
      contagem++;
      if (contagem % (uint64_t)pow(10, 5) == 0)
         printf("contagem em (%lu).\n", contagem);
   }
   assert(esgotado(t));
   destroi_temporizador(t);
}

void limitacao_de_temporizadores_por_vez() {
   TEMPORIZADOR t1 = cria_temporizador(Segundo, 3);
   TEMPORIZADOR t2 = cria_temporizador(Miliseg, 700);
   TEMPORIZADOR t3 = cria_temporizador(Nanoseg, 870);
   assert (instancias_temporizador() == 1);
   assert (t2 == NULL);
   assert (t3 == NULL);
   destroi_temporizador(t1);
}

void verificacao_do_percentual() {
   TEMPORIZADOR t = cria_temporizador(Segundo, 4);
   
   while (!esgotado(t)) {
      printf("percentual do progresso %0.0lf%%\n", 100 * percentual(t));
      breve_pausa(Miliseg, 600);
   }
   destroi_temporizador(t);
}

void formatacao_do_cronometro() {
   Cronometro c = cria_cronometro();

   breve_pausa(Segundo, 1);
   marca(c);
   puts(cronometro_to_str(c));

   breve_pausa(Segundo, 3);
   marca(c);
   puts(cronometro_to_str(c));

   breve_pausa(Segundo, 2);
   marca(c);
   puts(cronometro_to_str(c));

   breve_pausa(Segundo, 1);
   marca(c);
   puts(cronometro_to_str(c));
}

void main(int argc, char** argv) {
   /*
   executa_teste(
      "quebra-galho de tradução de tempo",
      funcao_conversao_de_tempo
   );*/

   executa_teste_interruptor(
      "instância e simples operações com o cronômetro",
      cronometro_instancia_basica, true
   );

   executa_teste_interruptor(
      "visualização dos registros feitos(sob condições)",
      visualizacao_dos_marcos, true
   );

   executa_teste_interruptor(
      "vendo se é possível instânciar um Temporizador",
      tentando_criar_um_temporizador, true
   );

   executa_teste_interruptor(
      "usando o Temporizador, como ele é geralmente usado",
      uso_casual_do_temporizador, true
   );
   executa_teste_interruptor(
      "proibição, temporária, da criação de vários Temporizadores",
      limitacao_de_temporizadores_por_vez, true
   );

   executa_teste_interruptor(
      "o progresso percentual do Temporizador",
      verificacao_do_percentual, true
   );

   executa_teste_interruptor(
      "formatação de string do Cronômetro",
      formatacao_do_cronometro, true
   );
}


