#include "primitivos.h"
#include <string.h>

size_t hash_string(GenT s, size_t cp) 
{
   size_t t = strlen(s);
   char* string;
   size_t x = 0, y = 0, z = SIZE_MAX;

   if (t >= 3) 
   {
      string = s;
      x = (size_t)string[t / 2];
      y = (size_t)string[t - 1];
      z = (size_t)string[0];
   }
      
   return (t*x*y*z + (t + y + z+ x)) % cp;
} 

bool eq_string(GenT a, GenT b) 
{
   char* str_a = a;
   char* str_b = b;

   return strcmp(str_a, str_b) == 0;
}

size_t hash_u64 (GenT key, size_t cp) 
   { return *((uint64_t*)key) % cp; }

bool eq_u64(GenT a, GenT b) 
   { return *((uint64_t*)a) == *((uint64_t*)b); }

size_t hash_i64 (GenT key, size_t cp) 
   { return *((int64_t*)key) % cp; }

bool eq_i64(GenT a, GenT b) 
   { return *((int64_t*)a) == *((int64_t*)b); }

size_t hash_u32 (GenT key, size_t cp) 
   { return *((uint32_t*)key) % cp; }

bool eq_u32(GenT a, GenT b) 
   { return *((uint32_t*)a) == *((uint32_t*)b); }

size_t hash_i32 (GenT key, size_t cp) 
   {  return *((int32_t*)key) % cp;  }

bool eq_i32(GenT a, GenT b) 
   { return *((int*)a) == *((int*)b); }

size_t hash_u16(GenT a, size_t cp) 
   { return *((uint16_t*)a) % cp; }

bool eq_u16(GenT a, GenT b) 
   { return *((uint16_t*)a) == *((uint16_t*)b); }

size_t hash_i16(GenT a, size_t cp) 
   { return *((int16_t*)a) % cp; }

bool eq_i16(GenT a, GenT b) 
   { return *((int16_t*)a) == *((int16_t*)b); }

size_t hash_u8 (GenT key, size_t cp) 
   { return *((uint8_t*)key) % cp; }

bool eq_u8(GenT a, GenT b) 
   { return *((uint8_t*)a) == *((uint8_t*)b); }

size_t hash_i8(GenT a, size_t cp) 
   { return *((int8_t*)a) % cp; }

bool eq_i8(GenT a, GenT b) 
   { return *((int8_t*)a) == *((int8_t*)b); }

size_t hash_bool(GenT x, size_t cp) 
   { return 0 ? *((bool*)x): 1; }

bool eq_bool(GenT a, GenT b) 
   { return *((bool*)a) == *((bool*)b); }

size_t hash_char(GenT x, size_t cp) 
   { return hash_i8(x, cp); }

bool eq_char(GenT a, GenT b)
   { return eq_i8(a, b); }

size_t hash_byte(GenT x, size_t cp) 
   { return hash_u8(x, cp); }

bool eq_byte(GenT a, GenT b)
   { return eq_u8(a, b); }

bool eq_sizet(GenT a, GenT b) 
   { return eq_u64(a, b); }

size_t hash_sizet (GenT key, size_t cp) 
   { return hash_u64(key, cp); }
