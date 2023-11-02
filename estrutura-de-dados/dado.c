
/* Dado de qualquer outra estrutura de dados
 * fica, pois, caso precise fazer uma delas
 * com outro tipo, fica só modificar a cópia
 * deste arquivo, trocando os tipos primitivos
 * do 'Dado'. Assim com as operações que ele 
 * realiza.
 */

// biblioteca padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* contabilizando quanto dos dados abaixos foram criados, onde quer
 * que seja. */
uint64_t instancias_de_dados = 0;

// encapsulando informação, pois alterações poderiam ser desastrosas.
uint64_t instancias_dado()
   { return instancias_de_dados; }

struct dado { unsigned char dado; };
// para dados genéricos.
typedef struct dado* Dado;

Dado cria_dado(unsigned char dado) {
   // instanciando dado.
   Dado item = malloc(sizeof(struct dado));
   if (item != NULL) { 
      item->dado = dado; 
      // contabilizando instância deste tipo de dado.
      instancias_de_dados++;
   }
   return item;
}

void destroi_dado(Dado d) { 
   // puts("'dado' destruído."); 

   // só destroi algo se for válido.
   if (d != NULL) {
      free(d);
      // descontabiliza também quanto deles estão na memória.
      instancias_de_dados--;
   }
}

Dado cria_dado_branco() 
   { return cria_dado(0x00); }

// faz uma cópia do 'dado' em sí.
Dado clona_dado(Dado d) 
   { return cria_dado(d->dado); }

/* cria uma array, inicialmente 'null'
 * do tipo de dado acima. */
Dado* cria_array_dado(uint64_t n) {
   size_t tamanho = sizeof(Dado);
   Dado * array = (Dado*)calloc(n, tamanho);
   /* colocando qualquer coisa, posteriormente
    * só é preciso alterar. */
   for(uint64_t p = 0; p < n; p++) 
      { array[p] = cria_dado('#'); }
   return array;
}

// destrói a array de dados.
void destroi_array_dado(Dado* a, uint64_t q) {
   for(uint64_t p = 0; p < q; p++) 
      { destroi_dado(a[p]); }
   free(a);
   //printf("todos %lu 'dados' foram destruídos.", q);
}

void visualiza_dado(Dado d) {
   if (d == NULL) 
      { puts("não há nada aqui!"); }
   // talvez tenha-se que mudar o símbolo
   // de formatação do dado.
   else 
      { printf("%c", d->dado); }
}

// verifica se dois dados são iguais.
bool dados_iguais(Dado a, Dado b) {
   if (a == NULL || b == NULL) 
      { return false; }
   return a->dado == b->dado;
}

/* transforma o tipo de dado numa string.*/
char* dado_para_string(Dado d) {
   size_t t = sizeof(char);
   char* resultado = (char*)calloc(20, t);
   if (d != NULL)
      sprintf(resultado, "%c", d->dado);
   else
      sprintf(resultado, "%s", "none");
   return resultado;
}
