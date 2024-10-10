
#ifndef __CONVERSAO_H__
#define __CONVERSAO_H__
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

 #define to_bytes(X, Y) _Generic((X), \
   uint16_t: u16_to_bytes, \
   int16_t: i16_to_bytes, \
   uint32_t: u32_to_bytes, \
   int32_t: i32_to_bytes, \
   uint64_t: u64_to_bytes, \
   int64_t: i64_to_bytes \
 )(X, Y)

 /* Várias funções retornam de forma direta a serialização as seguinte tupla
  * abaixo, os bytes e o total.
  *
  * Nota: os bytes que geralmente retornam são da memória dinâmica, logo 
  *       precisarão ser desalocados. 
  */
 struct Bytes { uint8_t* bytes; size_t total; };

 // Método para desalocar a estrutura acima.
 void free_bytes (struct Bytes*);

 /* Verificam a ordem dos bytes da máquina que está usando, já que as arrays
  * convertidas abaixo seguirão tal, importante chamar tal antes de usar
  * a serialização dos inteiros abaixo. */
 bool maquina_little_endian (void); 
 bool maquina_big_endian    (void);

 /* Primeiro feito, feito para o inteiro básico, e principal tipo da 
  * linguagem C. */
 void int_to_bytes     (int      In, uint8_t* Out); 
 int from_bytes_to_int (uint8_t* In); 
 /* Ciclo de serialização para outros tipos de dados primitivos que há 
  * na linguagem C. */
 void   bool_to_bytes        (bool     In, uint8_t* Out);
 void   double_to_bytes      (double   In, uint8_t* Out); 
 double from_bytes_to_double (uint8_t* In);
 void   float_to_bytes       (float    In, uint8_t* Out);
 float  from_bytes_to_float  (uint8_t* In);

 /* Transforma um inteiro de determinado tipo numa array de bytes. O tamanho
  * da array será do determinado tipo, use 'sizeof' macro para verifcar. 
  * Abaixo em escala crescente todas conversões: 
  */
 void u8_to_bytes    (uint8_t  In, uint8_t* Out);
 void i8_to_bytes    (int8_t   In, uint8_t* Out);
 void u16_to_bytes   (uint16_t In, uint8_t* Out);
 void i16_to_bytes   (int16_t  In, uint8_t* Out);
 void i32_to_bytes   (int32_t  In, uint8_t* Out);
 void u32_to_bytes   (uint32_t In, uint8_t* Out); 
 void u64_to_bytes   (uint64_t In, uint8_t* Out); 
 void i64_to_bytes   (int64_t  In, uint8_t* Out);
 void sizet_to_bytes (size_t   In, uint8_t* Out);

 /* Deserialização de uma determinada array de bytes. Novamente, segue o
  * tamanho dos tipos, se for passado uma array menor para a conversão de 
  * um tipo que não bate com o tamanho, o retorno será indefinido. Sua 
  * responsabilidade passar arrays com ao devido 'bound'. 
  */
 uint8_t  from_bytes_to_u8    (uint8_t* In);
 int8_t   from_bytes_to_i8    (uint8_t* In);
 uint16_t from_bytes_to_u16   (uint8_t* In);
 int16_t  from_bytes_to_i16   (uint8_t* In);
 int32_t  from_bytes_to_i32   (uint8_t* In);
 uint32_t from_bytes_to_u32   (uint8_t* In); 
 uint64_t from_bytes_to_u64   (uint8_t* In); 
 int64_t  from_bytes_to_i64   (uint8_t* In); 
 size_t   from_bytes_to_sizet (uint8_t* In);

 /* Serialização e deserialização de uma 'narrow' string, uma string apenas
  * com caractéres ASCII. Observe o retorno e a entrada de ambas funções.
  * Sim, não é uma pura string de entrada ou saída, e sim uma tupla que 
  * contém os bytes da string num campo, e o tanto de bytes no outro.
  *
  * Nota: não é possível apenas pegar tal tupla e colocar uma array de 
  *       caractéres com esta quantidade de caractéres, o retorno da função
  *       que serializa, codifica o comprimento da string no começo dos 
  *       bytes, tem que se levar isso em consideração, qualquer coisa 
  *       simplesmente não funcionaria.
  */
 struct Bytes string_to_bytes      (char*         In);
 char*        from_bytes_to_string (struct Bytes* In);


#endif
