
/*
  Um bom modo de organizar dados tabelados. O
 módulo possuí uma estrutura onde você pega 
 todo 'rol' de dados, cede um 'rótulo' a ele
 e toda vez que impresso será visualizado fechado
 por caractéres `Unicode` de uma maneira bem
 formatada no terminal. A estrutura `Coluna`
 que proporciona isso, também aceita a 
 impressão de outras juntas, formando assim uma
 bonita tabela.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "listarray_ref.h"

typedef struct {
   // o que o dado representa.
   char* rotulo;

   // tupla contendo array e seu tamanho.
   vetor_t rol;

   // registro para largura máxima das formatações.
   uint8_t largura;

   /* 'fat pointer' que converte tipo de dado genérico para
    * uma string. Tipo um trait bound.*/
   char* (*__str__)(generico_t);
} Coluna;

typedef char* (*ToString)(generico_t);

Coluna cria_coluna (char* rotulo, size_t t, generico_t data, ToString fn) 
{
   // buscando a formatação com a mais comprida string.
   uint8_t width = 0;
   for (size_t p = 1; p <= t; p++) {
      generico_t ptr = data + p - 1;
      // char* string = tostring(data[p - 1]);
      char* string = tostring(ptr);
      size_t length_str = strlen(string);

      if (length_str > width)  
         width = length_str;
      free (string);
   } 
   // buscando o maior também baseado no rótulo.
   uint8_t T = strlen(rotulo);
   width = T > width? T: width;

   return (Coluna) {
      .__str__ = tostring,
      .rotulo = rotulo,
      .rol = (Array){data, t}, 
      .largura = width
   };
}


#ifdef _UT_IMPRESSAO
#include <assert.h>

static char* float_to_string(void* ptr) {
   char* string = malloc(10 * sizeof(char));
   float v = *((float*)ptr);
   sprintf(string, "%5.5f",  v);
   return string;
}

static char* i8_to_str(void* ptr) {
   char* string = malloc(10 * sizeof(char));
   int8_t v = *((int8_t*)ptr);
   sprintf(string, "%d",  v);
   return string;
}

void instancia_basica_da_impressao(void) {
   float array_float[] = {3.1415, 1234.56, 0.099, 3.71, 0.000012, 1.58, 9999.99, 1.0};
   float* ptr_array = array_float;
   Coluna c1 = cria_coluna("massa(kg)", 8, ptr_array, float_to_string);   
   printf ("comprimento calculado=%u\n", c1.largura);
   assert (c1.largura == 9);

   int8_t array_i8[] = {-100, -10, -1, 0, 1, 10, 100};
   Coluna c2 = cria_coluna("divídas", 7, array_i8, i8_to_str);   
   printf ("comprimento calculado=%u\n", c2.largura);
   assert(c2.largura == 8);
}

void main(void) {
   // executando testes ...
   instancia_basica_da_impressao(); 
}
#endif
