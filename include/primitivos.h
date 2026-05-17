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

#endif
