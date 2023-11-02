
/* Dado de qualquer outra estrutura de dados
 * fica, pois, caso precise fazer uma delas
 * com outro tipo, fica só modificar a cópia
 * deste arquivo, trocando os tipos primitivos
 * do 'Dado'. 
 *
 * Dado modificado para caber direito numa estrutura de tabela de 
 * dispersão, onde tal dado será uma 'entrada' da estrutura, contendo
 * dois valores, a chave(para busca e comparação) e o valor, que é o
 * dado em sí para se guardar.
 */
// biblioteca padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


typedef char* CHAVE;
typedef int64_t VALOR;

struct dado { 
   char chave[50]; 
   VALOR dado; 
};

// para dados genéricos.
typedef struct dado* Dado;

Dado cria_dado(CHAVE ch, VALOR v) {
   // quanto ocupa em bytes.
   size_t tamanho = sizeof(struct dado);
   // instanciando dado.
   Dado item = (Dado)malloc(tamanho);
   if (item != NULL) { 
      //memcpy(item->chave, ch, strlen(item->chave));
      strcpy(item->chave, ch);
      item->dado = v; 
   }
   return item;
}

void destroi_dado(Dado d) { 
   if (d != NULL) { free(d); }
}

Dado cria_dado_branco() 
   { return cria_dado("...cHaVe-Em-BrAnCa...", -1); }

// faz uma cópia do 'dado' em sí.
Dado clona_dado(Dado d) 
   { return cria_dado(d->chave, d->dado); }

/* cria uma array, inicialmente 'null'
 * do tipo de dado acima. */
Dado* cria_array_dado(uint64_t n) {
   size_t tamanho = sizeof(Dado);
   Dado * array = (Dado*)calloc(n, tamanho);
   /* colocando qualquer coisa, posteriormente
    * só é preciso alterar. */
   for(uint64_t p = 0; p < n; p++) 
      { array[p] = cria_dado_branco(); }
   return array;
}

// destrói a array de dados.
void destroi_array_dado(Dado* a, uint64_t q) {
   for(uint64_t p = 0; p < q; p++) 
      { destroi_dado(a[p]); }
   free(a);
   //printf("todos %llu 'dados' foram destruídos.", q);
}

void visualiza_dado(Dado d) {
   if (d == NULL) 
      { puts("não há nada aqui!"); }
   // talvez tenha-se que mudar o símbolo
   // de formatação do dado.
   else 
      { printf("'%s': %ld", d->chave, d->dado); }
}

// verifica se dois dados são iguais.
bool dados_iguais(Dado a, Dado b) {
   if (a == NULL || b == NULL) 
      { return false; }
   return strcmp(a->chave, b->chave) == 0;
}

/* transforma o tipo de dado numa string.*/
char* dado_para_string(Dado d) {
   if (d == NULL) 
      return "none";
   size_t t = sizeof(char);
   char* resultado = (char*)calloc(100, t);
   sprintf(resultado, "'%s': %ld", d->chave, d->dado);
   return resultado;
}
// nome mais simplificado para função acima.
char* dado_to_str(Dado d)
   { return dado_para_string(d); }

// acessa os valores de um dado.
CHAVE get_chave_dado(Dado d) { return d->chave; }

VALOR get_valor_dado(Dado d) { return d->dado; }
