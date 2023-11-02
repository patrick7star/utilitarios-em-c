

// biblioteca padrão do C:
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


uint8_t byte_aleatorio() {
   FILE *arq = fopen("/dev/urandom", "rb");
   uint8_t b = 0;
   if (arq != NULL) 
      { fread(&b, sizeof(uint8_t), 1, arq); }
   fclose(arq);
   return b;
}

uint8_t* lendo_64_bits() {
   FILE *arq = fopen("/dev/urandom", "rb");
   uint8_t* bytes = (uint8_t*)calloc(8, sizeof(uint8_t));

   if (arq != NULL) { 
      int total_lido;
      total_lido = fread(bytes, sizeof(uint8_t), 8, arq); 
      if (total_lido != 8)
         { perror("não foi lido 8 bytes"); }
      if (bytes == NULL)
         { perror("não foi possível lê nada."); }
   }
   fclose(arq);
   return bytes;
}

unsigned char ascii_char_aleatorio() {
   uint8_t b = byte_aleatorio();
   // selecionando só caractéres válidos.
   // ou seja, aqueles acima de 32.
   return (unsigned char)(b % (126-33) + 33);
}

#include <math.h>

uint64_t inteiro_positivo(uint64_t a, uint64_t b) {
   if (a > b)
      { return inteiro_positivo(b, a); }
   else if (a == b)
      { return b; }
   uint8_t* bytes = lendo_64_bits();
   if (bytes == NULL)
      { perror("array de bytes inválida(null)"); }
   uint64_t acumulador = 0;
   for(uint64_t p = 0; p < 8; p++) {
      uint64_t potencia = (uint64_t)pow(2.0, (double)p);
      acumulador = bytes[p] * potencia;
   }
   return (acumulador % (b - a + 1)) + a;
}

#include <stdbool.h>
bool logico() {
   uint8_t sorteio = byte_aleatorio();
   if (sorteio > 256/2) 
      { return true; }
   else 
      { return false; }
}

#include <ctype.h>
unsigned char alfabeto_aleatorio() {
   uint8_t code = ascii_char_aleatorio();
   if (isalpha(code))
      { return (unsigned char)code; }
   else
      { return alfabeto_aleatorio(); }
}
