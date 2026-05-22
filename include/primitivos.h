/* Aqui faremos algumas funções principais que serão anexadas tanto no módulo
 * da tabela de dispersão, como no de conjuntos. Aqui ficarão as hash e
 * funções de igualdade, de acordo com o exigido em tais coleções, para que
 * facilite um pouco o processo, e não tenha que ficar fazendo elas a todo
 * momento.
 */

#ifndef __PRIMITIVOS_H__
#define __PRIMITIVOS_H__
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "definicoes.h"

 size_t hash_string  (GenT s, size_t cp);
 bool   eq_string    (GenT a, GenT b);
 size_t hash_u64     (GenT key, size_t cp);
 bool   eq_u64       (GenT a, GenT b);
 size_t hash_i64     (GenT key, size_t cp);
 bool   eq_i64       (GenT a, GenT b);
 size_t hash_u32     (GenT key, size_t cp);
 bool   eq_i32       (GenT a, GenT b);
 size_t hash_i32     (GenT key, size_t cp);
 bool   eq_i32       (GenT a, GenT b);
 size_t hash_u16     (GenT a, size_t cp);
 bool   eq_u16       (GenT a, GenT b);
 size_t hash_i16     (GenT a, size_t cp);
 bool   eq_i16       (GenT a, GenT b);
 size_t hash_u8      (GenT key, size_t cp);
 bool   eq_u8        (GenT a, GenT b);
 size_t hash_i8      (GenT a, size_t cp);
 bool   eq_i8        (GenT a, GenT b);
 size_t hash_bool    (GenT x, size_t cp);
 bool   eq_bool      (GenT a, GenT b);
 size_t hash_char    (GenT x, size_t cp);
 bool   eq_char      (GenT a, GenT b);
 size_t hash_byte    (GenT x, size_t cp);
 bool   eq_byte      (GenT a, GenT b);
 size_t hash_sizet   (GenT key, size_t cp);
 bool   eq_sizet     (GenT a, GenT b);

 char* debug_u8      (GenT x);
 char* debug_i8      (GenT x);
 char* debug_u16     (GenT x);
 char* debug_i16     (GenT x);
 char* debug_u32     (GenT x);
 char* debug_i32     (GenT x);
 char* debug_u64     (GenT x);
 char* debug_i64     (GenT x);
 char* debug_f32     (GenT x);
 char* debug_f64     (GenT x);
 char* debug_string  (GenT x);
 // Algo mais idiomatico do C:
 char* debug_char    (GenT x);
 char* debug_void    (GenT x);
 char* debug_int     (GenT x);
 char* debug_long    (GenT x);
 char* debug_short   (GenT x);
 char* debug_double  (GenT x);
 char* debug_float   (GenT x);

/* Nomes mais apropriados do C. Será apenas um "embrulho" das chamadas acima.
 * Logo, deixarei a implementação a mais compacta o possível. Nem todas terão,
 * a medida que uso com frequência, ou terceiros usam tal biblioteca, irei
 * colocando mais, por enquanto, só isto está ótimo!
 */
char* debug_int(GenT x);
char* debug_long(GenT x);
char* debug_short(GenT x);
char* debug_double(GenT x);
char* debug_float(GenT x);

#endif
