

// biblioteca padrão em C(libs muito utilizadas.):
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

// seus módulos:
#include "../teste.h"
#include "../texto.h"
#include "../aleatorio.h"

/* série de testes unitários */
void metodos_de_visualizacao_da_matriz_texto() {
   MT m = cria_matriz_texto(cria_dimensao(30, 97), Visivel);
   visualiza_matriz_texto_debug(m);
   visualiza_matriz_texto(m, false);
   destroi_matriz_texto(m);
}

void preenchendo_vacuos_com_metodo_set_da_mt() {
   MT m = cria_matriz_texto(cria_dimensao(15, 67), Invisivel);
   assert(set(m, cria_ponto(8, 34), 'K'));
   assert(set(m, cria_ponto(3, 14), '#'));
   assert(set(m, cria_ponto(13, 28), 'y'));
   visualiza_matriz_texto_debug(m);

   /* tenta preenche todos vácuos de maneira randômica. */
   for(int k = 1; k <= 27; k++) {
      Ponto p = cria_ponto(k % 10, 2*(k % 5));
      assert(set(m, p, alfabeto_aleatorio()));
      destroi_ponto(p);
   }
   // visualizando o resultado;
   visualiza_matriz_texto_debug(m);
   destroi_matriz_texto(m);
}

void preenchendo_toda_matriz_texto_randomicamente() {
   MT m = cria_matriz_texto(cria_dimensao(30, 67), Visivel);
   visualiza_matriz_texto_debug(m);
   uint8_t Y = m->dimensao[0];
   uint8_t X = m->dimensao[1];

   /* tenta preenche todos vácuos de maneira randômica. */
   unsigned char selecao = alfabeto_aleatorio();
   for (uint16_t contagem = 0; contagem < Y*X; contagem++) {
      Ponto p = cria_ponto(
         (uint8_t)inteiro_positivo(0, 29),
         (uint8_t)inteiro_positivo(0, 66)
      );
      selecao = alfabeto_aleatorio();
      assert(set(m, p, selecao)); 
      destroi_ponto(p);
   }
   // resultado, com todos vácuos preenchidos.
   visualiza_matriz_texto_debug(m);
   destroi_matriz_texto(m);
}

#include <string.h>

void conversao_para_string() {
   MT m = cria_matriz_texto(cria_dimensao(15, 47), Invisivel);
   char texto_arbitrarios[8][47] = {
      "gosma verde",
      "adoro roxo",
      "a pretty postalcard",
      "Season of pumpinkins, it's halloween!",
      "trouble sleeping",
      "because he never give in",
      "i hate flies",
      "he comes your man! "
   };

   // escrevendo strings bem no meio dela.
   for (int i = 0; i < 8; i++) {
      char* string = texto_arbitrarios[i];
      uint8_t c = strlen(string);

      for (int k = 0; k < strlen(string); k++) {
         // ponto.
         uint8_t x = (47 - c) / 2 + k;
         Ponto posicao = cria_ponto((2*i), x);
         set(m, posicao, string[k]);
         destroi_ponto(posicao);
      }
   }

   // resultado, com todos vácuos preenchidos.
   puts("versão impressa:");
   visualiza_matriz_texto_debug(m);
   printf("versão formatada para string:\n%s", mt_to_str(m));

   destroi_matriz_texto(m);
   assert(true);
}

#include <time.h>

void formato_string_dos_pontos() {
   srand(time(NULL));

   for (int k = 0; k < 9; k++) {
      Ponto p = cria_ponto(rand() % 100, rand() % 2301);
      printf("%-18s", ponto_to_str(p));
      destroi_ponto(p);
   }
   puts("");
}


void main(int argc, char** argv) {
   executa_teste_interruptor(
      "visualização da MT", 
      metodos_de_visualizacao_da_matriz_texto,
      true
   );

   executa_teste_interruptor(
      "método de alteração de uma célula na MT",
      preenchendo_toda_matriz_texto_randomicamente,
      false
   );

   executa_teste_interruptor(
      "usando deliberadamente o método set da MT",
      preenchendo_toda_matriz_texto_randomicamente, false
   );

   executa_teste_interruptor(
      "conversão numa string",
      conversao_para_string, true
   );

   executa_teste_interruptor(
      "formato string do Ponto",
      formato_string_dos_pontos,
      true
   );
}


