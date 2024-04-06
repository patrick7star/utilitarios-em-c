

/* Simples implementação genérica da array-list.
 *   Se ficar muito boa se juntará as estruturas de dados tipo(iii) no
 * projeto utilitários, pois é uma array dinâmica que serve incrivelmente
 * para qualquer projeto que precisa armazenar um grande número de objetos,
 * seja eles qualquer que tipo.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define INICIAL 200
#define INVALIDA NULL

typedef struct array_lista {
   // quantia inicial da array e total de itens nela guardados.
   size_t quantia;
   size_t capacidade;

   // array de containers, este, armazena o endereço do dado e seu tamanho.
   // void* array[INICIAL];
   void** array;
} ArrayLista, Vetor, vetor_t;

ArrayLista* cria_al() {
   ArrayLista* lista = malloc(sizeof(ArrayLista));

   if (lista != INVALIDA) {
      lista->capacidade = INICIAL;
      lista->quantia = 0;
      lista->array = malloc(INICIAL * sizeof(void*));
      // pula alocações que não foram realizadas.
      if (lista->array == NULL) return NULL;
   }
   return lista;
}

#include <stdbool.h>
#include <stdio.h>

bool destroi_al(ArrayLista* l) {
   if (l == INVALIDA) return false;
   free (l->array);
   free (l);
   return true;
}

bool redimensiona(ArrayLista* l) {
   size_t nova_capacidade = 2 * l->capacidade;
   size_t Q = l->quantia;
   size_t tamanho = sizeof (void*);
   void** nova_array = malloc(2 * nova_capacidade * tamanho);
   size_t C = l->capacidade;
   void** ptr_array_antiga = l->array;

   // só realiza a rotina abaixo se a 'quantia' atingiu a 'capacidade'.
   if (Q < C) return false;

   // se livrando a referência a antiga ...
   l->array = NULL;
   // copiando ...
   for (size_t k = 1; k <= C; k++)
      nova_array[k - 1] = ptr_array_antiga[k - 1];
   // liberando antiga...
   #ifdef _REDIMENSIONA
   puts ("a liberação da antiga array foi sucedida.");
   #endif
   free (ptr_array_antiga);
   // então atribuindo nova array a instância ...
   l->array = nova_array;
   // atualizando nova capacidade...
   l->capacidade = nova_capacidade;
   // confirma como feita o redimensionamento.
   return true;
}

bool insere_al(ArrayLista* l, void* dado) {
   /* verifica se antes é preciso redimensionar a array interna para caber
    * um novo item inserido. */
   redimensiona(l);
   // achando o local para armazenar, e coloca lá.
   size_t  indice = l->quantia;
   l->array[indice] = dado;
   // contabiliza item adicionado.
   l->quantia += 1;
   return true;
}

bool vazia_al(ArrayLista* l) 
   // verifica se está sem qualquer item.
   { return l->quantia == 0; }

size_t tamanho_al(ArrayLista* l) 
   // retorna valor interno que contabiliza inserções.
   { return l->quantia; }

void* remove_al(ArrayLista* l) {
   // não faz nada com lista vázias.
   if (vazia_al(l)) return NULL;

   size_t t = l->quantia;
   void* remocao = l->array[t - 1];
   l->quantia -= 1;
   return remocao;
}

void* indexa_al(ArrayLista* l, size_t indice) {
   /* Retorna item na posição 'indice' da array, se o valor de índice for
    * inválido retorna 'null'. 
    */
   size_t Q = l->quantia;

   // só indexa valores válidos, de zero ao total de inserções menos um.
   if (indice > Q) { return NULL; }

   return l->array[indice];
}

void* remove_indice_al (ArrayLista* l, size_t indice) {
   /* Retorna item na posição 'indice' da array, se o valor de índice for
    * inválido retorna 'null'. 
    */
   size_t Q = l->quantia;
   void* remocao = NULL;

   if (indice >= Q) {
      perror ("o índice ultrapassa as fronteiras da array.");
      abort ();
   }

   for (size_t i = 0; i < Q; i++) {
      if (i == indice) {
         remocao = l->array[i];
         // copiando a sucessora para esta.
         for (size_t j = indice; j < (Q - 1); j++)
            l->array[j] = l->array[j + 1];
         l->array[Q - 1] = NULL;
         l->quantia -= 1;
         break;
      }
   }
   return remocao;
}

#if defined(_UT_ARRAY_LISTA)
#include "teste.h"

void visualizacao_array_list_char(vetor_t* l) {
   size_t t = tamanho_al(l);
   char* caractere;

   if (vazia_al(l)) {
      puts("array-lista: []");
      return;
   }

   if (l->quantia > 7)
      // só fica difícil de contar valores maiores que sete.
      printf ("array-lista(%lu): [", l->quantia);
   else
      printf ("array-lista: [");

   for (size_t i = t; i > 0; i--) {
      caractere = l->array[t - i];
      printf ("%c, ", *caractere);
   }
   puts ("\b\b]");
}
void visualizacao_array_list_int(vetor_t* l) {
   size_t t = tamanho_al(l);
   int* caractere;

   if (vazia_al(l)) {
      puts("array-lista: []");
      return;
   }

   if (l->quantia > 7)
      // só fica difícil de contar valores maiores que sete.
      printf ("array-lista(%lu): [", l->quantia);
   else
      printf ("array-lista: [");

   for (size_t i = t; i > 0; i--) {
      caractere = l->array[t - i];
      printf ("%d, ", *caractere);
   }
   puts ("\b\b]");
}

#include <assert.h>

void demonstracao_com_caracteres() {
   vetor_t* lista = cria_al();
   char m = 'x', n = 'A', p = 'J';

   insere_al(lista, &p);
   insere_al(lista, &n);
   insere_al(lista, &m);
   visualizacao_array_list_char(lista);

   char* removido = remove_al(lista);
   printf ("removido: '%c'\n", *removido);
   removido = remove_al(lista);
   printf ("removido: '%c'\n", *removido);

   assert (tamanho_al(lista) == 1);
   assert (*((char*)remove_al(lista)) == 'J');
   assert (vazia_al(lista));

   destroi_al(lista);
}

void demonstracao_com_inteiros() {
   ArrayLista* outra_lista = cria_al();
   int entradas[] = {39, 73, 15, 101};

   for (size_t k = 1; k <= 4; k++)
      insere_al (outra_lista, &entradas[k - 1]);

   while (!vazia_al(outra_lista)) {
      printf ("removido: %d\n", *((int*)remove_al(outra_lista)));
      visualizacao_array_list_int(outra_lista);
   }
      
   destroi_al(outra_lista);
}

void demonstracao_com_strings() {
   ArrayLista* lista = cria_al();
   char* entradas[] = { "cerca", "porta", "cadeado", "mala"};

   for (size_t k = 1; k <= 4; k++)
      insere_al (lista, entradas[k - 1]);

   while (!vazia_al(lista)) {
      char* string = remove_al(lista);
      printf ("removido: '%s'\n", string);
   }
   destroi_al(lista);
}

void verificado_o_que_foi_coletado(int total, char* argumentos[]) {
   ArrayLista* lista = cria_al();
   char** entradas = argumentos;

   for (size_t k = 1; k <= total; k++)
      insere_al (lista, entradas[k - 1]);

   size_t inicial = tamanho_al(lista);
   for (size_t k = 1; k <= 10; k++)
      printf (
         "removido ==> %s(%lu)\n", 
         (char*)remove_al(lista),
         tamanho_al (lista)
      );
   assert (inicial - 10 == tamanho_al(lista));
   puts ("experimentando indexação ...");
   for (size_t k = 1; k <= tamanho_al(lista) / 2; k++)
      printf ("%lu) %s\n", k, (char*)indexa_al(lista, k));
   destroi_al(lista);
}

void visualiza_array_list_string(vetor_t* l) {
   size_t t = tamanho_al(l);
   char* string;

   if (vazia_al(l)) {
      puts("array-lista: []");
      return;
   }

   // só fica difícil de contar valores maiores que sete.
   printf ("array-lista(%lu): [", l->quantia);

   for (size_t i = 1; i <= t; i++) {
      string = l->array[i - 1];
      printf ("%s, ", string);
   }
   puts ("\b\b]");
}

void remocao_em_pontos_criticos (void) {
   ArrayLista* lista = cria_al();
   char* entradas[] = { "cerca", "porta", "cadeado", "mala", "bolsa"};

   for (size_t k = 1; k <= 5; k++)
      insere_al (lista, entradas[k - 1]);
   visualiza_array_list_string (lista);

   puts ("removendo o terceiro(cadeado) ...");
   remove_indice_al (lista, 2);
   visualiza_array_list_string (lista);

   puts ("removendo o primeiro(cerca) ...");
   remove_indice_al (lista, 0);
   visualiza_array_list_string (lista);

   destroi_al(lista);
}

int main(int total, char* argumentos[], char* variaveis[]) {
   executa_testes (
      4, demonstracao_com_inteiros, true,
      demonstracao_com_caracteres, true,
      demonstracao_com_strings, true,
      remocao_em_pontos_criticos, true
   );
   // verificado_o_que_foi_coletado, true

   return EXIT_SUCCESS;
}
#endif
