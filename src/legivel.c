/* Grandezas mais legíveis:
 * Faz conversões de grandezas referentes a dados utilizados em computação,
 * ou outros campos. 
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* tempo_legivel(double segundos) {
   // variável com uma letra para agilizar codificação.
   double s = segundos;
   char* resultado = calloc(30, sizeof(char));

   // para comparação ...
   const double MINUTO = 60.0;
   const double HORA = 60 * MINUTO; 
   const double DIA = 24 * HORA;
   const double MES = 30 * DIA;
   const double MILISEG = powf(10, -3);
   const double MICROSEG = powf(10, -6);
   const double NANOSEG = powf(10, -9);

   if (s >= 1.0) {
      if (s < MINUTO)
         sprintf(resultado, "%0.1lf seg", s);
      else if (s >= MINUTO && s < HORA)
         sprintf(resultado, "%0.1lf min", s / MINUTO);
      else if (s >= HORA && s < DIA)
         sprintf(resultado, "%0.1lfh", s / HORA);
      else if (s >= DIA && s < MES)
         sprintf(resultado, "%0.1lf dias", s / DIA);
      else
         perror("não implementado para tal grandeza");
   } else {
      if (s >= MILISEG)
         sprintf(resultado, "%0.0lf ms", s * 1000.0);
      else if (s >= MICROSEG)
         sprintf(resultado, "%0.0lf \u03bcs", s * powf(10, 6));
      else if (s >= NANOSEG)
         sprintf(resultado, "%0.0lf ns", s * powf(10, 9));
      else {
			// qualquer quantia abaixo disso, será considerado zero!
			strcpy (resultado, "0seg");
		}
   }
   return resultado;
}

// #include "legivel/calculo_potencia.c"
static uint64_t potencia(uint64_t b, uint8_t e) {
/* Calcula uma pontência, porém retorna ela como um grande inteiro --
 * o maior tipo que existe, já que na falta de um deste na biblioteca
 * padrão do C, é preciso criar um. Não computa para expoentes negativos,
 * por motivos óbvio do tipo de retorno.
 */
   uint64_t produto = 1;

   for (uint8_t k = 1; k <= e; k++)   
      produto *= b;

   return produto;
}

char* tamanho_legivel(size_t bytes) { 
   // múltiplos de tamanho(equivalente em bytes).
   uint64_t KILO = potencia(2, 10);  
   uint64_t MEGA = potencia(2, 20); 
   uint64_t GIGA = potencia(2, 30);
   uint64_t TERA = potencia(2, 40);
   uint64_t PETA = potencia(2, 50);

   char* resultado_str = calloc(30, sizeof(char));
   char peso_str[5];
   float valor;

   if (bytes >= KILO && bytes < MEGA) {
      memcpy(&peso_str, "KiB", 3);
      valor = (float)bytes / (float)KILO;
   } else if (bytes >= MEGA && bytes < GIGA) {
      memcpy(&peso_str, "MiB", 3);
      valor = (float)bytes / (float)MEGA;
   } else if (bytes >= GIGA && bytes < TERA) {
      memcpy(&peso_str, "GiB", 3);
      valor = (float)bytes / (float)GIGA;

   } else if (bytes >= TERA && bytes < PETA) {
      memcpy(&peso_str, "TiB", 3);
      valor = (float)bytes / (float)TERA;
   } else {
      memcpy(&peso_str, "B's", 3);
      valor = (float)bytes;
   }

   sprintf(resultado_str, "%0.1f %s", valor, peso_str);
   return (char*)resultado_str;
}

char* valor_legivel(size_t unidades) {
   char* peso;
   double potencia;
   char* resultado_str = malloc(15);
   uint64_t u = unidades;

   if (u >= 1000 && u < pow(10, 6)) {
      peso = "mil";
      potencia = 1000;
   } else if (u >= pow(10, 6) && u < pow(10, 9)) {
      peso = "mi";
      potencia = pow(10, 6);
   } else if (u >= pow(10, 9) && u < pow(10, 12)) {
      peso = "bi";
      potencia = pow(10, 9);
   } else if (u >= pow(10, 12) && u < pow(10, 15)) {
      potencia = pow(10, 12);
      peso = "tri";
   } else 
      { peso = ""; potencia = 1; }

   double novo_valor = (double)unidades /(double)potencia;

   if (novo_valor < 1.0)
      sprintf(resultado_str, "%d%s", (uint16_t)novo_valor, peso);
   else
      sprintf(resultado_str, "%0.1f%s", novo_valor, peso);
   return resultado_str;
}

/* === === === === === === === === === === === === === === === === === ===+
 * .......................................................................&
 * ........................Testes Unitários...............................&
 * .......................................................................&
 * === === === === === === === === === === === === === === === === === ===*/
#ifdef _UT_LEGIVEL
// Biblioteca padrão em C(libs muito utilizadas.):
#include <stdbool.h>
#include <assert.h>
// Seus módulos:
#include "teste.h"

uint64_t entradas[] = {
   382, 12832, 3842394, 7712340981,
   111931512, 50123812341, 100030231892377
};

double segundos[] = {
   51.3232, 12832.15, 8328.0,
   38832.312, 0.001, 0.038,
   0.000851, 0.000000701, 190.5321
};

void legibilidade_do_tempo(void) {
   for(int p = 0; p < 9; p++)
      printf(
         "%16lf ===> %s\n", 
         segundos[p], 
         tempo_legivel(segundos[p])
      );
   // de avaliação manual?
}

void legibilidade_de_tamanhos(void) {
   for(size_t p = 1; p <= 7; p++) {
		uint64_t valor = entradas[p - 1];
		char* traducao = tamanho_legivel(valor);
		printf ("%20lu ==> %s\n", valor, traducao);
		free (traducao);
	}
}

void o_grosso_de_grande_valores(void) {
   for(int p = 0; p < 7; p++)
      printf(
         "%16lu ===> %s\n", 
         entradas[p], 
         valor_legivel(entradas[p])
      );
}

int main(void) {
   executa_testes(
      3, legibilidade_do_tempo, true,
         legibilidade_de_tamanhos, true,
		   o_grosso_de_grande_valores, true
	);

   return EXIT_SUCCESS;
}
#endif
