// Declaration of methods and functions:
#include "pilhaarray.h"
// Biblioteca padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
// Apenas para os testes:
#include "definicoes.h"

// Initial size of buffer for 'stack mode'. It recommended this not being
// larger than 10 thosands, because it starts to effect heavily the binary
// that is generated.
#define INITIAL_CAPACITY 1000

/* === === === === === === === === === === === === === === === === === === ==
 *                   How Struct is Defined
 * === === === === === === === === === === === === === === === === === === */
struct abstract_array_stack {

   GenT stack[INITIAL_CAPACITY];
   GenT* heap;

   size_t top;
   size_t capacity;

};

Stack new_pa(void) 
{
   size_t sz = sizeof(STACK);
   Stack self = malloc(sz);
   const size_t MAX = INITIAL_CAPACITY;

   if (self != NULL) 
   {
      self->heap = NULL;
      self->top = 0;
      self->capacity = MAX;
   }
   return self;
}

void push_pa(Stack s, GenT data) 
{
   size_t top = 0xdeadbeef;

   if (s == NULL || data == NULL) return;

   top = (*s).top;
   (*s).stack[top] = data;
   (*s).top += 1;
}

size_t length_pa(Stack s)
   { return (*s).top; }

bool empty_pa(Stack s)
   { return (*s).top == 0; }


GenT pop_pa(Stack s) 
{
   size_t top; GenT removed = NULL;

   if (empty_pa(s)) { return NULL; }

   top = (*s).top - 1;
   removed = (*s).stack[top];
   (*s).top -= 1;
   return removed;
}

bool destroy_pa(Stack s) 
{
   if (s == NULL) return false;

   if ((*s).capacity > INITIAL_CAPACITY)
      free((*s).heap);

   free(s);
   return true;
}

size_t size_pa(Stack s, size_t size) 
/* Size of sort of object it holds plus size of this pointer multiplied by number of 
 * items, added with size of type of struct plus pointer size. */
{ 
   int a = sizeof(GenT);
   int n = sizeof(pilha_t);
   int m = sizeof(Stack);
   int Q = (*s).top;

   /* The tally here it like size of placeholder pointer plus size of object it holds, all
    * of that multiplied by the number of itens in stack. In the end add the size of 
    * the abstract type and pointer that holds it. */
   return (a + size) * Q + n + m; 
}

float percentual_pa(Stack self)
   { return (float)(*self).top / (float)(*self).capacity; }

void print_pa(Stack s, ToString formatter) 
{
   if (empty_pa(s))
      { puts("Stack is empty!"); return; }

   size_t index = (*s).top - 1;
   GenT top = (*s).stack[index];
   size_t length = length_pa(s), n;
   char* fmt = NULL;
   float percent = percentual_pa(s) * 100.0;
   
   fmt = formatter(top);
   printf("Stack(%zu | %0.1f%%): [%s]-[", length, percent, fmt);
   fflush(stdout);
   free(fmt);

   for (n = (index - 1); n > 0; n--)
   { 
      top = (*s).stack[n];
      fmt = formatter(top);
      printf("%s, ", fmt); 
      free(fmt); 
   }
   puts("\b\b]");
}

void extend_pa(Stack s, Stack p) 
{
/* Consume all 'stack p', e insere seus items within of 'stack s'. */
   while (!empty_pa(p)) 
      push_pa(s, pop_pa(p));
   destroy_pa(p);
}

GenT top_pa(Stack s)
   { return (*s).stack[(*s).top - 1]; }

/* === === === === === === === === === === === === === === === === === ==
 *                     Unit Tests 
 * === === === === === === === === === === === === === === === === === ==*/
 #ifdef __unit_tests__
// Biblioteca padrão do C:
#include <locale.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
// API do sistema:
// Própria biblioteca:
#include "teste.h"
#include "dados-testes.h"
#include "macros.h"
#include "primitivos.h"

UNIT_TEST it_just_create_one_print_it_and_destroy_it(void);
UNIT_TEST insert_items_inside_it_so_print_stack(void);
UNIT_TEST removing_until_exhaustion(void);
UNIT_TEST calculating_size_of_the_stack_with_elements(void);

int main(int total, char* args[], char* vars[]) 
{
   executa_testes_b(
      true, 4,
         Unit(it_just_create_one_print_it_and_destroy_it, true),
         Unit(insert_items_inside_it_so_print_stack, true),
         Unit(removing_until_exhaustion, true),
         Unit(calculating_size_of_the_stack_with_elements, true)
   );

   return EXIT_SUCCESS;
}

UNIT_TEST calculating_size_of_the_stack_with_elements(void)
{
   Stack object = new_pa(); 
   int n = 0xffeeddcc;
   char* data = NULL;
   ToString debug = debug_char;

   for (n = 0, data=(char*)&alfabeto[n]; n < ALFABETO; n++, data=(char*)&alfabeto[n])
      push_pa(object, data);

   print_pa(object, debug);
   printf("Quantity of items: %zu\n", length_pa(object));
   printf("Size of struct(Stack): %zu bytes.\n", size_pa(object, sizeof(char)));
   destroy_pa(object);
}

UNIT_TEST removing_until_exhaustion(void)
{
   Stack object = new_pa(); 
   int n = 0xffeeddcc;
   char* data = NULL;
   char** names = (char**)girls_names;
   const int N = GIRLS_NAMES;

   for (n = 0, data=(char*)names[n]; n < N; n++, data=(char*)names[n])
      push_pa(object, data);

   print_pa(object, debug_string);
   printf("Quantity of items: %zu\n", length_pa(object));

   while (!empty_pa(object))
   {
      data = pop_pa(object);
      printf("\t- %s\n", data);
   }
   destroy_pa(object);
}

UNIT_TEST it_just_create_one_print_it_and_destroy_it(void)
{
   Stack input = new_pa(); 
   print_pa(input, debug_char);
   destroy_pa(input);
}

UNIT_TEST insert_items_inside_it_so_print_stack(void)
{
   Stack object = new_pa(); 
   int n = 0xffeeddcc;
   char* data = NULL;

   for (n = 0, data=(char*)&vogais[n]; n < VOGAIS; n++, data=(char*)&vogais[n])
      push_pa(object, data);

   print_pa(object, debug_char);
   printf("Quantity of items: %zu\n", length_pa(object));

   while (!empty_pa(object))
   {
      data = pop_pa(object);
      printf("\t- %c\n", *data);
   }
      
   destroy_pa(object);
}
#endif
