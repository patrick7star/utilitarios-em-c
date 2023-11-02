

/* Grandezas mais legíveis:

   Faz conversões de grandezas referentes a dados utilizados em 
   computação, ou outros campos. 
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

char* tempo_legivel(double segundos) {
   // variável com uma letra para agilizar codificação.
   double s = segundos;
   char* resultado = (char*)calloc(30, sizeof(char));

   // para comparação ...
   double MINUTO = 60.0;
   double HORA = 60 * MINUTO; 
   double DIA = 24 * HORA;
   double MES = 30 * DIA;
   double MILISEG = powf(10, -3);
   double MICROSEG = powf(10, -6);
   double NANOSEG = powf(10, -9);

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
         sprintf(resultado, "%0.1lf ms", s * 1000.0);
      else if (s >= MICROSEG)
         sprintf(resultado, "%0.1lf \u03bcs", s * powf(10, 6));
      else if (s >= NANOSEG)
         sprintf(resultado, "%0.1lf ns", s * powf(10, 9));
      else
         perror("não implementado para tal grandeza");
   }

   return resultado;
}

#include <string.h>
#include <stdint.h>
#include "legivel/calculo_potencia.c"

char* tamanho_legivel(uint64_t bytes) { 
   // múltiplos de tamanho(equivalente em bytes).
   uint64_t KILO = potencia(2, 10);  
   uint64_t MEGA = potencia(2, 20); 
   uint64_t GIGA = potencia(2, 30);
   uint64_t TERA = potencia(2, 40);
   uint64_t PETA = potencia(2, 50);

   char* resultado_str = (char*)calloc(30, sizeof(char));
   char peso_str[5];
   float valor;

   if (bytes >= KILO && bytes < MEGA) {
      // peso_str = "KiB";
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
   return resultado_str;
}
