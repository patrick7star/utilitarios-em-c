/*   Modo direto, e mais limpo, de alocar simples variáveis, ou agregados na
 * parte heap do layout de memória. Principalmente para os principais 
 * tipos primitivos do C. 
 *
 *   Observe que o prefixo box tem como meta emitar o Rust, onde, este 
 * ponteiro inteligente nele diz colocar qualquer tipo de dado na 'heap' 
 * da memória; claro que isso que ele gera aqui não é um smart-pointer,
 * o retorno é memória 'pura' ao invés. Aqui segue o mesmo esquema, porém 
 * mais limitado por causa do C em sí. Existe até uma parte genérica, porém 
 * não é possível dobrar todas limitações que a linguagem impõe.
 */

#ifndef __MEMORIA_H__
#define __MEMORIA_H__
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

 // Para os tipos primitivos mais básicos:
 bool*    box_bool   (bool    vl);
 char*    box_char   (char  vl);
 int8_t*  box_i8     (int8_t  vl);
 int*     box_int    (int     vl);
 wchar_t* box_wchart (wchar_t vl);
 float*   box_float  (float   vl);
 double*  box_double (double  vl);
 size_t*  box_sizet  (size_t  vl);

 // Colocar 'narrow' /e 'wide' na heap.
 char*    box_str (char* str   );
 wchar_t* box_wcs (wchar_t* str);

 /* Para todos os demais tipos de inteiros. Observe que alguns estão 
  * omitidos de propósito(uint64, int32_t, int8_t,...), isso porque já são 
  * abordados, uito por todos estes tipos serem contrações de outros nomes 
  * no C. Na parte da apelidação, abordamos eles. */
 uint8_t*  box_uint8t  (uint8_t  vl);
 uint16_t* box_uint16t (uint16_t vl);
 int16_t*  box_int16t  (int16_t  vl);
 uint32_t* box_uint32t (uint32_t vl);
 int64_t*  box_int64t  (int64_t  vl);

 // Clona qualquer array, dado o tanto em bytes do seu tipo, e comprimento.
 uint8_t* clona_array (void* array, int sz, int n);
 // Alterna de dois valores genéricos, dado eles, e o seu tipo(tanto em bytes).
 void alterna (void* a, void* b, int sz);

 // Apelidos que as funções principais acima também podem ser chamadas:
 // Wide strings(Unicode):
 #define  box_ustr            box_wcs
 #define  box_unicode_string  box_wcs
 #define  box_wide_str        box_wcs
 // Narrow strings:
 #define  box_string      box_str 
 #define  box_narrow_str  box_str 
 #define  box_ascii_str   box_str 
 // Valores decimais:
 #define  box_f32   box_float
 #define  box_f64   box_double 
 /* Todos tipos de inteiros. Estou no máximo abordando todas possíveis 
  * escritas que tentarei, sem falar colocando a sintáxe do Rust que é 
  * indiscutivelmente mais ergonetrica. */
 #define  box_u8       box_uint8t  
 #define  box_uint8    box_uint8t   
 #define  box_byte     box_uint8t   
 #define  box_int8t    box_i8    
 #define  box_int8     box_i8    
 #define  box_u16      box_uint16t  
 #define  box_uint16   box_uint16t  
 #define  box_int16    box_int16t    
 #define  box_i16      box_int16t    
 #define  box_i32      box_int       
 #define  box_int32t   box_int  // Omitido.
 #define  box_int32    box_int
 #define  box_u32      box_uint32t  
 #define  box_uint32   box_uint32t  
 #define  box_uint64t  box_sizet // Omitido.
 #define  box_uint64   box_sizet    
 #define  box_u64      box_sizet    
 #define  box_usize    box_sizet  // Presuposto pode mudar no futuro.
 #define  box_sz       box_sizet    
 #define  box_int64    box_int64t   
 #define  box_i64      box_int64t
 #define  box_isize    box_int64t  // Presuposto pode mudar no futuro.
 // Sobre caractéres Unicode:
 #define box_wc box_wchart 

 void tipo_nao_suportado(void* type);
 // Apelido em Inglês:
 #define new_box nova_box
 /* Grande algromerador de gnéricos que indica a função correta baseado no
  * tipo de retorno e lista de parâmetros. */
 #define nova_box(OBJ) _Generic((OBJ), \
   bool:          box_bool,            \
   char:          box_char,            \
   unsigned char: box_uint8t,          \
   int8_t:        box_int8t,           \
   uint16_t:      box_uint16t,         \
   int16_t:       box_int16t,          \
   uint32_t:      box_uint32t,         \
   int:           box_int,             \
   int64_t:       box_int64t,          \
   size_t:        box_sizet,           \
   double:        box_double,          \
   float:         box_float,           \
   char*:         box_str,             \
   wchar_t*:      box_wcs,             \
   default:       tipo_nao_suportado   \
 )(OBJ)

#endif

