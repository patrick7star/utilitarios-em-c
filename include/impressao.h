
#ifndef __IMPRESSAO_H__
#define __IMPRESSAO_H__
#include "listaarray_ref.h"
#include <stdint.h>

 /* Dado uma lista contento itens que podem ser impressos pela função 'f',
  * então será arranjados na tela de modo a otimizar o espaço. */
 void listar (vetor_t* lista, ToString f); 

/* === === === === === === === === === === === === === === === === === === =
 *                   Impressão de Arrays de Primitivos
 * === === === === === === === === === === === === === === === === === === */
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

/* === === === === === === === === === === === === === === === === === === =
 *                   Coloração e Formatação de Strings
 * 
 * Nota: os códigos são os equivalentes hexadecimais daqueles dado na tabela
 *       ANSI de cores e efeitos.
 * === === === === === === === === === === === === === === === === === === */
 enum Formatacao { 
    // Todas cores disponíveis:
    Preto = 0x1e, Vermelho = 0x1f, Verde       = 0x20, Amarelo = 0x21, 
    Azul  = 0x22, Violeta  = 0x23, AzulMarinho = 0x24, Branco  = 0x25,
    SemCor = 0x26,
    // Efeitos disponíveis:
    Negrito = 1, Italico = 3, Piscante = 5, Frio = 2, Sublinhado = 4, 
    Riscado = 9, Normal = 22
 };

 /* O proposito de tais arrays é somente para iteração das definições acima.
  * Isso é útil tanto para testes unitários, quanto aplicações. */
 const enum Formatacao TODAS_CORES[] = {
   Preto, Vermelho, Verde, Amarelo, Azul, 
   Violeta, AzulMarinho, Branco
 };
 const enum Formatacao TODOS_EFEITOS[] = {
   Negrito, Italico, Piscante, Frio, Sublinhado,
   Riscado, Normal
 };

 /* Vários apelidos para diferência strings coloridas das normais. Na raíz,
  * elas são as mesmas coisas, entranto, as coloridas são mais cumpridas 
  * para acomodar o protocolo ANSI. */
 typedef char* StringColorido, *StringColorida, *StrColorida, *SC;
 typedef enum Formatacao EnumFmt;

 /*   Faz uma copia da string com os devidos protocolos ANSI para que fique
  * com a pigmentação escolhida. Por fazer uma cópia, é preciso liberar
  * posteriormente.
  *   Cores inválidas faram o programa ser interrompido. Também note que,
  * por causa da inserção do protocolo, a versão colorida é sempre maior
  * que a original. */
 StrColorida colori_string    (char* s, enum Formatacao c);
 /* Também muda a cor do texto, e sem precisar alocar um mero byte de memória
  * na heap. Entretanto, existe um limite para o tamanho da string, assim,
  * como também não é thread-safe(TS). */
 StrColorida muda_cor_da_string (char* s, enum Formatacao c);

 /* Duas versões da mesma coisa. A primeira, retorna uma string alocada 
  * dinamicamente, com o mesmo conteúdo do que a anterior, porém com o 
  * código ANSI acrescentada pra se parecer formatada. */
 StrColorida aplica_formatacao (char* In, EnumFmt In_c, EnumFmt In_e);

 /*   Várias funções acimas com um 'output' dado via argumento. O retorno é 
  * apenas um valor lógico dizendo que foi feito algo ou não. 
  *   A recursa em alteração pode ser por causa de algum argumento inválido,
  * seja o enum, ou os ponteiros para os respectivos buffers. 
  */
 bool colori_string_i (const char* In, enum Formatacao c, char* Out);
 bool aplica_formatacao_i
   (const char* In_s, EnumFmt In_c, EnumFmt In_e, char* Out);

 /* Aplica os efeitos acima nas próprias strings de entradas. As próprias 
  * referencias dadas são novamente retornadas, porém, se você tem controle
  * do 'argumento' dado, isso é despresível. */
 StrColorida aplica_formatacao_ii (char* IO_str, EnumFmt In_c, EnumFmt In_e);
 StrColorida colori_string_ii(char* IO_str, enum Formatacao In_cor);


/* === === === === === === === === === === === === === === === === === === =
 *                  Stringficação de Arrays de Todos Tipos
 *                         Inteiros que Existem
 * === === === === === === === === === === === === === === === === === === */
 /*   E claro, abaixo tem um macro que reune todas as chamadas de forma
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

/* === === === === === === === === === === === === === === === === === === =
 *                         Lista de Checagem  

 *   Criação de checklist pra visualização. Um modo bem interessante de você
 * organizar qualquer coisa que está, tem, possui e etc; e seu estado oposto
 * antagonico. 
 * === === === === === === === === === === === === === === === === === === */
 typedef struct ListaDeChecagem CheckList;
 typedef struct Checagem Check;

 CheckList nova_ldc(void); 
 bool adiciona_ldc(CheckList* list,  Check* obj);
 void visualiza_ldc(CheckList* list); 
 
#endif
