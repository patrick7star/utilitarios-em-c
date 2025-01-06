
/* Passar o cronômetro para aqui, onde fica mais organizado a distinção
 * entre seus métodos, e outras funções do 'módulo tempo', sem falar
 * que facilita a leitura.
 */

#include <stdio.h>
#include <stdbool.h>
#include "legivel.h"

// total de registros que é possível fazer.
#define TOTAL_REGISTROS 100
static uint8_t instancias_do_cronometro;

struct cronometro {
   // Segundos decorridos à partir desta contagem.
   time_t inicio; 
   // Outros selos de tempo do sistema, para verificar variação.
   time_t* marcos;

   // capacidade inicial da array.
   size_t capacidade;

   // quantos registros foram feitos.
   size_t qtd;
};
// tamanho da 'struct' acima.
#define CLOCK_SIZE sizeof (struct cronometro)

Cronometro cria_cronometro() {
   Cronometro novo = malloc(CLOCK_SIZE);

   // se alocado corretamente.
   if (novo != NULL) {
      novo->inicio = time(NULL);
      novo->qtd = 0;
      novo->capacidade = TOTAL_REGISTROS;
      novo->marcos = malloc(TOTAL_REGISTROS * sizeof(time_t));

      #ifdef _DEBUG_CRIA_CRONOMETRO
      puts("cronômetro foi alocado com sucesso.");
      #endif
      // Contabilizando instância nova.
      instancias_do_cronometro++;
   } else { 
      perror("cronômetro não foi criado corretamente."); 
      abort();
   }
   return novo;
}

static void ajusta_container_de_registros(Cronometro c) {
   size_t Q = c->capacidade;
   size_t t = c->qtd;

   // verificando a necessidade de expansão.
   if (t < Q) {
      #ifdef MARCA_CRONOMETRO
      printf("não necessária uma expansão há %lu\n", Q - t);
      puts("abandonando rotina...");
      #endif
      return;
   }

   // régra básica(e quebra-galho) de amortização, dobra o atual.
   size_t nova = 2 * Q;
   /* assume que a realocação sempre é sucedida, caso contrário pode 
    * haver "vazamento de memória". Como o uso pensado para tal estrutura
    * é bem restrito e controlado, mesmo um vazamento de memória não seria
    * algo que causaria problema descontrolado a aplicação. */
   time_t* nova_array = realloc(c->marcos, nova);
   c->capacidade = nova;
   c->marcos = nova_array;
}

double marca(Cronometro c) {
/* Retorna o tempo variado desde a última marcação, o desde o início, se
 * nenhuma ainda foi marcada. */
   ajusta_container_de_registros(c);
   
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
			#ifdef _WIN32
         int ordem = total - c->qtd + 1;
			#else 
         int ordem = abs(total - c->qtd) + 1;
			#endif
         int min = (int)t / 60;
         int seg = (int)t % 60;
         printf("%3dº. [%02d:%02d]\n", ordem, min, seg);
      }
   }
   puts("\b\b");
}

double variacao(Cronometro c) {
   // quanto variou do último registro marcado, se houver algum é claro.
   if (c->qtd == 0) {
      const char* mensagem_erro = "nenhum registro foi marcado.";
      perror(mensagem_erro); abort();
   }

   time_t f = time(NULL);
   size_t Q = c->qtd;
   time_t i = c->marcos[Q - 1];
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
      "Cronômetro(%u) [%s | %3zu]", 
      instancias_do_cronometro,
      tempo_legivel(t), c->qtd
   );
   // retornando a slice-string transformada.
   return resultado_fmt;
}

uint8_t instancias_cronometro(void) 
   { return instancias_do_cronometro; }
   
void destroi_cronometro(Cronometro c) {
   #ifdef _DEBUG_DESTROI_CRONOMETRO
      time_t fim = time(NULL);
      double diferenca = difftime(fim, c->inicio);
      printf(
         "\ntempo decorrido: %s\nregistros feitos: %zu\n",
         tempo_legivel(diferenca), c->qtd
      );
   #endif

   if (c != NULL) {
      free(c);
      instancias_do_cronometro--;
      #ifdef _DEBUG_DESTROI_CRONOMETRO
      printf(
         "Desalocação realizada, agora há apenas %u instâncias.\n", 
         instancias_do_cronometro
      );
      #endif
   } else {
      #ifdef _DEBUG_DESTROI_CRONOMETRO
      puts("o tal cronômetro não existe!");
      #endif
   }
}
