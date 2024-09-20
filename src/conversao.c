/* Aqui ficam a transformação de vários tipos de inteiros em bytes, em dois
 * principais tipos de ordem: little endian and big endian. Existe a versão
 * padrão também, que respeita a plataforma que ela está sendo chamada. 
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#define to_bytes(X, Y) _Generic((X), \
   uint16_t: u16_to_bytes, \
   int16_t: i16_to_bytes, \
   uint32_t: u32_to_bytes, \
   int32_t: i32_to_bytes, \
   uint64_t: u64_to_bytes, \
   int64_t: i64_to_bytes \
)(X, Y)

void print_array(uint8_t* array, int t)
{
   putchar('[');
   for (int i = 1; i <= t; i++)
      printf("%d, ", array[i - 1]);
   printf("\b\b]\n");
}

bool arrays_iguais(uint8_t* a, uint8_t* b, int t) {
/* O algoritmo consiste no seguinte, supõe que todos itens são iguais, caso
 * ache algum valor que não bate, então desconfirma hipotese inicial, caso 
 * contrário confirma. */
   #ifdef __debug__
   print_array(a, t);
   print_array(b, t);
   #endif 

   for (int i = 1; i <= t; i++)
   {
      if (a[i - 1] != b[i - 1])
         return false;
   }
   return true;
}

bool maquina_little_endian(void) {
/* Se o processador tem uma ordem "little endian", o valor de 11, por exemplo,
 * só pode ter o byte relevante no começo, os outros três sendo iguais a 
 * zero. */
   uint8_t array_do_valor_onze[] = {0xB, 0x0, 0x0, 0x0};
   int valor_arbitrario = 11;
   int* pointer = &valor_arbitrario;
   uint8_t* array_de_bytes = (uint8_t*)pointer;

   return arrays_iguais(array_do_valor_onze, array_de_bytes, 4);
}

bool maquina_big_endian(void) {
/* Tenta verificar se a ordem dos bytes é "little endian", se não confirmar,
 * só pode ser o oposto, "big endian". Aqui não aplicarei a negativa do 
 * código acima, pelo contrário, copiarei ele, e só mudarei a array de 
 * comparação, prá justamente mostrar que tal algoritmo funciona. */
   uint8_t array_do_valor_onze[] = {0x0, 0x0, 0x0, 0xb};
   int valor_arbitrario = 11;
   int* pointer = &valor_arbitrario;
   uint8_t* array_de_bytes = (uint8_t*)pointer;

   return arrays_iguais(array_do_valor_onze, array_de_bytes, 4);

}

void int_to_bytes(int input, uint8_t* output) {
/* Assumindo que tal tem 4 bytes, então este é o tamanho da array trabalhada
 * neste algoritmo. A ordem é a da máquina, a transformação é direta, então
 * antes de utilizar o resultada aqui, apenas verifica, com as funções dadas
 * acima, se ela é little-endian ou big-endian. Os bytes capturados, via 
 * endereço da variável passada, serão copiadas para o buffer também passado
 * por argumento. */
   int size = sizeof(int);
   /* Pega endereço que armazena chuck de bytes do inteiro de 32-bits. */
   int* pointer = &input;
   // Converte o pointeiro(chuck) para bytes...
   uint8_t* bytes = (uint8_t*)pointer;

   // Copia tais bytes para o buffer de saída do escopo.
   memcpy(output, bytes, size);
}

int from_bytes_to_int(uint8_t* input) {
/*   Pega a array de 4 bytes que representa um inteiro, então converte ela 
 * para o respectivo inteiro. A array de bytes tem que representar a mesma
 * ordem da atual máquina(big ou little)-endian, se não o resultado pode 
 * ser indefinido com uma entrada não válida. 
 *
 *   A conversão é o mesmo método * que o algoritmo anterior, porém, ao 
 * inverso. Aqui o pointeiro de bytes serão passados, converteremos para um 
 * pointeiro do determinado tipo desta * função, e no fim, retornamos o valor
 * que ele forma. */
   int* pointeiro = (int*)input;

   return *pointeiro;
}

void u16_to_bytes(uint16_t input, uint8_t* output) {
/* Quase o mesmo algoritmo que é aplicado para o inteiro(padrão do * C, que 
 * é com sinal, e de 32-bits). O que muda aqui é a array de output tem que 
 * ter apenas dois bytes, e a entrada é um inteiro positivo de 2 bytes. */
   int size = sizeof(uint16_t);
   /* Pega endereço que armazena chuck de bytes do inteiro de 32-bits. */
   uint16_t* pointer = &input;
   // Converte o pointeiro(chuck de bytes) para equivalente de 8-bits...
   uint8_t* bytes = (uint8_t*)pointer;

   // Copia tais bytes para o buffer de saída do escopo.
   memcpy(output, bytes, size);
}

uint16_t from_bytes_to_u16(uint8_t* input) {
/* Apenas muda os tipos de pointeiros que está se convertendo... */
   uint16_t* pointeiro = (uint16_t*)input;
   return *pointeiro;
}

void i16_to_bytes(int16_t input, uint8_t* output) {
   int size = sizeof(int16_t);
   int16_t* pointer = &input;
   uint8_t* bytes = (uint8_t*)pointer;

   memcpy(output, bytes, size);
}

int16_t from_bytes_to_i16(uint8_t* input) {
   int16_t* pointeiro = (int16_t*)input;
   return *pointeiro;
}

/*   Com estas foram a primeira feita, porém com o nome original, não vale a
 * pena copiar o código, apenas fazer a chamadas internas com os mesmos
 * argumentos. Refazendo nesta parte só por um caso de consistências com 
 * os tipos precedentes. 
 *
 *   Note que 'int32_t' é só um apelido para 'int'. 
 */
void i32_to_bytes(int32_t input, uint8_t* output)
   { int_to_bytes(input, output);}

int32_t from_bytes_to_i32(uint8_t* input)
   { return from_bytes_to_int(input); }

void u32_to_bytes(uint32_t input, uint8_t* output) {
   int size = sizeof(uint32_t);
   uint32_t* pointer = &input;
   uint8_t* bytes = (uint8_t*)pointer;

   memcpy(output, bytes, size);
}

uint32_t from_bytes_to_u32(uint8_t* input) {
   uint32_t* pointeiro = (uint32_t*)input;
   return *pointeiro;
}

void u64_to_bytes(uint64_t input, uint8_t* output) {
   int size = sizeof(uint64_t);
   uint64_t* pointer = &input;
   uint8_t* bytes = (uint8_t*)pointer;

   memcpy(output, bytes, size);
}

uint64_t from_bytes_to_u64(uint8_t* input) {
   uint64_t* pointeiro = (uint64_t*)input;
   return *pointeiro;
}

void i64_to_bytes(int64_t input, uint8_t* output) {
   int size = sizeof(int64_t);
   int64_t* pointer = &input;
   uint8_t* bytes = (uint8_t*)pointer;

   memcpy(output, bytes, size);
}

int64_t from_bytes_to_i64(uint8_t* input) {
   int64_t* pointeiro = (int64_t*)input;
   return *pointeiro;
}

void sizet_to_bytes(size_t input, uint8_t* output) {
   int size = sizeof(size_t);
   size_t* pointer = &input;
   uint8_t* bytes = (uint8_t*)pointer;

   memcpy(output, bytes, size);
}

size_t from_bytes_to_sizet(uint8_t* input) {
   size_t* pointeiro = (size_t*)input;
   return *pointeiro;
}

#ifdef __UT_CONVERSAO__
#include <locale.h>
#include <time.h>
#define bool_to_str(X) X?L"true": L"false"

int main(void) {
   setlocale(LC_CTYPE, "");

   bool resultado = maquina_little_endian();
   wprintf(L"É está máquina 'little endian'? %ls\n", bool_to_str(resultado));
   resultado = maquina_big_endian();
   wprintf(L"É está máquina 'big endian'? %ls\n", bool_to_str(resultado));

   uint8_t bytes[] = {0xB, 0xE, 0xE, 0xF};
   int convertido = from_bytes_to_int(bytes);
   printf("BEEF ==> %d\n", convertido);

   uint8_t exemplo[] = {0xf, 0x0, 0x0, 0x0};
   convertido = from_bytes_to_int(exemplo);
   assert (convertido == 15);
   printf("[0xf, 0, 0, 0] ==> %d\n", convertido);

   printf("\ntamanho do 'time_t': %zu\n", sizeof(time_t));
   time_t timestamp = time(NULL);
   printf("%lld\n", timestamp);
   uint8_t bytes_tt[8];
   sizet_to_bytes(timestamp, bytes_tt);
   print_array(bytes_tt, sizeof(time_t));
   time_t reconvertido_tt = from_bytes_to_sizet(bytes_tt);
   printf("reconvertendo novamente ==> %lld\n", reconvertido_tt);
   assert(timestamp == reconvertido_tt);

   int16_t n = -5320;
   uint8_t bytes_n[2];
   to_bytes(n, bytes_n);
   print_array(bytes_n, 2);
   printf("reconvertido:%d\n", from_bytes_to_i16(bytes_n));
}
#endif