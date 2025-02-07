#include "memoria.h"
// Biblioteca padrão do C:
#include <iso646.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <stdio.h>


bool* box_bool (bool valor) {
/* O algoritmo é tão simples que este simplesmente servirá de base para 
 * todos demais abaixo. */
   const int sz = sizeof(bool);
   bool* obj = malloc(sz);

   *obj = valor;
   return obj;
}

int* box_int (int valor) {
   const int sz = sizeof(int);
   int* obj = malloc(sz);

   *obj = valor;
   return obj;
}

float* box_float (float valor) {
   const int sz = sizeof(float);
   float* obj = malloc(sz);

   *obj = valor;
   return obj;
}

double* box_double (double valor) {
   const int sz = sizeof(double);
   double* obj = malloc(sz);

   *obj = valor;
   return obj;
}

char* box_char (char valor) {
   const int sz = sizeof(char);
   char* obj = malloc(sz);

   *obj = valor;
   return obj;
}

int8_t* box_i8 (int8_t valor) {
   const int sz = sizeof(int8_t);
   int8_t* obj = malloc(sz);

   *obj = valor;
   return obj;
}

wchar_t* box_wchart (wchar_t valor) {
   const int sz = sizeof(wchar_t);
   wchar_t* obj = malloc(sz);

   *obj = valor;
   return obj;
}

size_t* box_sizet (size_t valor) {
   const int sz = sizeof(size_t);
   size_t* obj = malloc(sz);

   *obj = valor;
   return obj;
}

char* box_str(char* str) {
/* Este no mínimo serve para uma cópia do gerador de string Unicodes. Ele
 * diferente dos demais, pois trata na verdade de uma array. Neste caso,
 * é feito uma cópia, e ainda fixado de vez um caractére nulo para que 
 * a string realmente seja garantida. */
   int n = strlen(str) + 1;
   const int sz = sizeof(char);
   char* obj = malloc(n * sz);

   strcpy(obj, str);
   obj[n] = '\0';
   return obj;
}

wchar_t* box_wcs(wchar_t* str) {
   int n = wcslen(str) + 1;
   const int sz = sizeof(wchar_t);
   wchar_t* obj = malloc(n * sz);

   wcscpy(obj, str);
   obj[n] = L'\0';
   return obj;
}

uint8_t* box_uint8t (unsigned char valor) {
   const int sz = sizeof(unsigned char);
   unsigned char* obj = malloc(sz);

   *obj = valor;
   return obj;
}

uint16_t* box_uint16t (unsigned short valor) {
   const int sz = sizeof(unsigned short);
   unsigned short* obj = malloc(sz);

   *obj = valor;
   return obj;
}

int16_t* box_int16t (signed short valor) {
   const int sz = sizeof(short int);
   short int* obj = malloc(sz);

   *obj = valor;
   return obj;
}

uint32_t* box_uint32t (unsigned int valor) {
   const int sz = sizeof(int);
   unsigned int* obj = malloc(sz);

   *obj = valor;
   return obj;
}

int64_t* box_int64t (signed long valor) {
   const int sz = sizeof(long int);
   long* obj = malloc(sz);

   *obj = valor;
   return obj;
}

void tipo_nao_suportado(void* type)
   { perror("tipo não suportado"); abort(); }

uint8_t* clona_array(void* array, int sz, int n)
{
/*  Pega uma array de tipo 'sz'(seu tanto de bytes), e comprimento 'n', e
 * realiza uma cópia de todos seus elementos. Então por fim retorna esta 
 * array clonada. */
   uint8_t* pointer = (uint8_t*)array;
   uint8_t* clone = malloc(n * sz);

   memmove(clone, pointer, n * sz);
   return clone;
}

void alterna(void* a, void* b, int sz)
{
   uint8_t* ptr_c = malloc(sz);
   uint8_t* ptr_a = (uint8_t*)a;
   uint8_t* ptr_b = (uint8_t*)b;

   memcpy(ptr_c, ptr_a, sz);
   memcpy(ptr_a, ptr_b, sz);
   memcpy(ptr_b, ptr_c, sz);
   ptr_a += sz;
   ptr_b += sz;
}

#ifdef __unit_tests__
/* == == == == == == == == == == == == == == == == == == == == == == == == =
 *                         Testes Unitários 
 * == == == == == == == == == == == == == == == == == == == == == == == = */
#include <limits.h>
#include <locale.h>
#include "macros.h"

int main(int qtd, char* args[], char* vars[]) 
{
   setlocale(LC_CTYPE, "en_US.UTF-8");
   char A = 'v';
   bool D = true;
   wchar_t F[7] = L"joêlho";

   char* a = nova_box(A);
   uint8_t* b = nova_box((uint8_t)UCHAR_MAX);
   wchar_t* c = nova_box(L'ê');
   bool* d = nova_box(D);
   int64_t* e = nova_box(LONG_MIN / 2);
   wchar_t* f = nova_box(F);
   char* g = nova_box("queijo");
   int8_t* h = box_i8(CHAR_MIN + 1);

   printf("a: %c\t\t~ %ld bytes\n", *a, sizeof(*a));
   printf("b: %u\t\t~ %ld bytes\n", *b, sizeof(*b));
   printf("c: %lc\t\t~ %ld bytes\n", *c, sizeof(*c));
   printf("d: %s\t\t~ %ld bytes\n", bool_to_str(*d), sizeof(*d));
   printf("e: %ld\t\t~ %ld bytes\n", *e, sizeof(*e));
   printf("f: %ls\t\t~ %ld bytes\n", f, sizeof(f));
   printf("g: %s\t\t~ %ld bytes\n", g, sizeof(g));
   printf("h: %d\t\t~ %ld bytes\n", *h, sizeof(*h));

   return EXIT_SUCCESS;
}
#endif
