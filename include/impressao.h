
#ifndef __IMPRESSAO_H__
#define __IMPRESSAO_H__
#include "listaarray_ref.h"
#include <stdint.h>

 /* Dado uma lista contento itens que podem ser impressos pela função 'f',
  * então será arranjados na tela de modo a otimizar o espaço. */
 void listar (vetor_t* lista, ToString f); 

 /* Impressão de arrays dos tipos mais básicos da linguagem, dado o seu
  * comprimento. */
 void imprime_array_int    (int*    array, int length);
 void imprime_array_bool   (bool*   array, int length);
 void imprime_array_float  (float*  array, int length);
 void imprime_array_double (double* array, int length);

 // Apenas referentes a strings de qualquer tipo:
 void imprime_array_char         (char*    array,   int length);
 void imprime_array_str          (char*    array[], int total);
 void imprime_array_char_unicode (wchar_t* array,   int length);
 void imprime_array_str_unicode  (wchar_t* array[], int total);

 /* Todas variações de inteiros que existem: */
 void imprime_array_i8  (int8_t*    array, int length);
 void imprime_array_u8  (uint8_t*   array, int length);
 void imprime_array_u16 (uint16_t*  array, int length);
 void imprime_array_i16 (int16_t*   array, int length);
 void imprime_array_u32 (uint32_t*  array, int length);
 void imprime_array_u64 (uint64_t*  array, int length);
 void imprime_array_i64 (int64_t*   array, int length);

 /* Mesmo que todas acimas, porém aceita qualquer tipo(include elas). */
 #define imprime_array(A, N) _Generic((A), \
   double*: imprime_array_double, \
   float*:  imprime_array_float, \
   int*:    imprime_array_int, \
   char*:   imprime_array_char, \
   bool*:   imprime_array_bool, \
   char**:   imprime_array_str, \
   \
   int8_t*:   imprime_array_i8, \
   uint8_t*:  imprime_array_u8, \
   int16_t*:  imprime_array_i16, \
   uint16_t*: imprime_array_u16, \
   uint32_t*: imprime_array_u32, \
   int64_t*:  imprime_array_i64, \
   uint64_t*: imprime_array_u64 \
 )(A, N)
 // Mesmo macro acima, porém com um nome diferente, em inglês.
#define print_array(A, N) imprime_array(A, N)

 // Cores disponíveis:
 enum Cores { 
    Vermelho = 0x0000001, Verde = 0x0000010, Amarelo = 0x0000100, 
    Azul = 0x0001000, Violeta = 0x0010000, AzulMarinho = 0x0100000, 
    SemCor = 0x1000000
 };
 /* Vários apelidos para diferência strings coloridas das normais. Na raíz,
  * elas são as mesmas coisas, entranto, as coloridas são mais cumpridas 
  * para acomodar o protocolo ANSI. */
 typedef char* StringColorido, *StringColorida, *StrColorida, *SC;

 /*   Faz uma copia da string com os devidos protocolos ANSI para que fique
  * com a pigmentação escolhida. Por fazer uma cópia, é preciso liberar
  * posteriormente.
  *   Cores inválidas faram o programa ser interrompido. Também note que,
  * por causa da inserção do protocolo, a versão colorida é sempre maior
  * que a original. */
 StrColorida colori_string (char* s, enum Cores c);
 /* Também muda a cor do texto, e sem precisar alocar um mero byte de memória
  * na heap. Entretanto, existe um limite para o tamanho da string, assim,
  * como também não é thread-safe(TS). */
 StrColorida muda_cor_da_string (char* s, enum Cores c);

 /*   Transforma as arrays dos seguintes tipos numa string(aquele famoso 
  * 'stringfy'). 
  *   E claro, abaixo tem um macro que reune todas as chamadas de forma
  * automática, com um nome mais curto e genérico.
  */
 char*  array_i8_to_str  (signed char* array, int n);
 char*  array_u8_to_str  (unsigned char* array, int n);
 char*  array_i16_to_str (signed short* array, int n);
 char*  array_u16_to_str (unsigned short* array, int n);
 char*  array_i32_to_str (int* array, int n);
 char*  array_u32_to_str (unsigned int* array, int n);
 char*  array_i64_to_str (signed long* array, int n);
 char*  array_u64_to_str (unsigned long* array, int n);
 char*  array_f32_to_str (float* array, int n);
 char*  array_f64_to_str (double* array, int n);

 #define array_to_string(ARRAY, LEN) _Generic((ARRAY), \
   signed char*:     array_i8_to_str,                  \
   unsigned char*:   array_u8_to_str,                  \
   signed short*:    array_i16_to_str,                 \
   unsigned short*:  array_i16_to_str,                 \
   signed int*:      array_i32_to_str,                 \
   unsigned int*:    array_u32_to_str,                 \
   signed long*:     array_i64_to_str,                 \
   unsigned long*:   array_u64_to_str,                 \
   float*:           array_f32_to_str,                 \
   double*:          array_f64_to_str                  \
 )(ARRAY, LEN)
 // Já está em inglês, porém outro nome bom seria este:
 #define stringfy_array(A, L) array_to_string(A, L)

 /*   Criação de checklist pra visualização. Um modo bem interessante de você
  * organizar qualquer coisa que está, tem, possui e etc; e seu estado oposto
  * antagonico. */
 typedef struct ListaDeChecagem CheckList;
 typedef struct Checagem Check;

 CheckList nova_ldc(void); 
 bool adiciona_ldc(CheckList* list,  Check* obj);
 void visualiza_ldc(CheckList* list); 
 
#endif
