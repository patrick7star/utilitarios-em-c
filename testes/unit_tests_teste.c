
// biblioteca padrão em C(libs muito utilizadas.):
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
// seus módulos:
#include "../teste.h"
#include "../terminal.h"

void gera_todas_substrings() {
   char* str = "abacaxi";
   uint8_t quantia = total_substrings(str);
   printf(
      "string: \"%s\" \t\b\b chute: %ld\n", 
      str, (uint64_t)quantia
   );
   StringArray resultados = todas_substrings(str);

   for (int l = 0; l < quantia; l++) { 
      printf(
         "\t\b\b\b\b%02i ===> %s\n", 
         (l + 1), resultados[l]
      ); 
   }

   destroi_string_array(resultados, quantia);
}

void gera_todas_substrings_com_variadas_strings() {
   char strings[9][40] = {
      "independemente", "computador",
      "trem..", "porta...", "abacaxi", 
      "protrona", "lápis", "ovo~~~~~", "gramado"
   };

   for (int l = 1; l <= 9; l++) {
      char *str = strings[l - 1];
      uint8_t quantia = total_substrings(str);
      printf(
         "\nstring: \"%s\" \t\b\b chute: %ld\n", 
         str, (uint64_t)quantia
      );
      StringArray resultados = todas_substrings(str);

      for (int l = 0; l < quantia; l++) { 
         printf(
            "\t\b\b\b\b%02i ===> %s\n", 
            (l + 1), resultados[l]
         ); 
      }

      destroi_string_array(resultados, quantia);
   }
}

#include "../tempo.h"

void funcao_amostra()
   { puts("simples impressão!"); breve_pausa(Miliseg, 200); }

void diga_ola_mundo()
   { printf("olá mundo!\n"); breve_pausa(Miliseg, 600); }

void execucao_de_multiplos_testes_de_uma_vez() {
   executa_testes(
      3, funcao_amostra, true, 
      funcao_amostra, false,
      diga_ola_mundo, true
   );
}

void concatenacao_de_varias_strings_literais() {
   const char entrada[100] = "maçã-pêra-banana-abacaxi-uva";
   char* saida = concatena_literais_str(
      9, "maçã", "-", "pêra", "-", "banana", "-",
      "abacaxi", "-", "uva"
   );
   printf("resultado: '%s'\n", saida);
   assert(!strcmp(entrada, saida));
}

void main() {
   executa_teste(
      "gera todas substrings",
      gera_todas_substrings
   );
   executa_teste(
      "gera todas substrings com variadas entradas",
      gera_todas_substrings_com_variadas_strings
   );
   executa_teste(SEM_NOME, concatenacao_de_varias_strings_literais);
}
