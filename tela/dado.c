
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
#include "../ponto.h"

/* contabilizando quanto dos dados abaixos foram criados, onde quer
 * que seja. */
uint64_t instancias_de_dados = 0;

// encapsulando informação, pois alterações poderiam ser desastrosas.
uint64_t instancias_dado()
   { return instancias_de_dados; }

struct dado { ArrayPonto dado; uint64_t qtd; };
// para dados genéricos.
typedef struct dado* Dado;

Dado cria_dado(ARRAY_PONTO a, uint64_t tamanho) {
   // instanciando dado.
   Dado item = malloc(sizeof(struct dado));
   if (item != NULL) { 
      // inicialmente tudo, pontos na coordenada (0,0).
      item->dado = a;
      item->qtd = tamanho;
      // contabilizando instância deste tipo de dado.
      instancias_de_dados++;
   }
   return item;
}

void destroi_dado(Dado d) { 
   // só destroi algo se for válido.
   if (d != NULL) {
      destroi_array_de_pontos(d->dado, d->qtd);
      free(d);
      d = NULL;
      // descontabiliza também quanto deles estão na memória.
      instancias_de_dados--;
   }
}

Dado cria_dado_branco() 
   { return cria_dado(cria_array_de_pontos(10), 10); }

// faz uma cópia do 'dado' em sí.
Dado clona_dado(Dado d) 
   { return cria_dado(d->dado, d->qtd); }

/* cria uma array, inicialmente 'null'
 * do tipo de dado acima. */
Dado* cria_array_dado(uint64_t n) { return NULL; }

// destrói a array de dados.
void destroi_array_dado(Dado* a, uint64_t q) { }

void visualiza_dado(Dado d) {
   if (d == NULL) 
      { puts("não há nada aqui!"); }
   // talvez tenha-se que mudar o símbolo
   // de formatação do dado.
   else 
      { visualiza_ap(d->dado, d->qtd); }
}

// verifica se dois dados são iguais.
bool dados_iguais(Dado a, Dado b) { return false; }

/* transforma o tipo de dado numa string.*/
char* dado_para_string(Dado d) {
   size_t t = sizeof(char);
   char* resultado = (char*)calloc(20, t);
   if (d != NULL)
      sprintf(resultado, "%s", ap_to_str(d->dado, d->qtd));
   else
      sprintf(resultado, "%s", "none");
   return resultado;
}
