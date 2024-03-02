
/* Dado de qualquer outra estrutura de dados fica, pois, caso precise 
 * fazer uma delas com outro tipo, fica só modificar a cópia deste 
 * arquivo, trocando os tipos primitivos do 'Dado'. Assim com as 
 * operações que ele realiza.
 */

// biblioteca padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* contabilizando quanto dos dados abaixos foram criados, onde quer
 * que seja. */
static size_t instancias_de_dado = 0;

/* As vezes é preciso alternar apenas o tipo primitivo de dado na cópia
 * deste arquivo. Quando se altera isso na estrutura, é necessário 
 * imediamente fazer isso em cadeia nos parâmetros da funções. Por isso
 * vamos centralizar o tipo primitivo de dado aqui para que se necessite
 * altera apenas uma vez. 
 */
typedef unsigned char TIPO_DO_DADO;

// encapsulando informação, pois alterações poderiam ser desastrosas.
size_t instancias_dado()
   { return instancias_de_dado; }

struct dado { TIPO_DO_DADO dado; };
// para dados genéricos.
typedef struct dado* Dado;

Dado cria_dado(TIPO_DO_DADO dado) {
   // instanciando dado.
   Dado item = malloc(sizeof(struct dado));

   if (item != NULL) { 
      item->dado = dado; 
      // contabilizando instância deste tipo de dado.
      instancias_de_dado++;
   }
   return item;
}

void destroi_dado(Dado d) { 
   // só destroi algo se for válido.
   if (d != NULL) {
      free(d);
      // descontabiliza também quanto deles estão na memória.
      instancias_de_dado--;
   }
}

Dado cria_dado_branco() 
   { return cria_dado(0x00); }

// faz uma cópia do 'dado' em sí.
Dado clona_dado(Dado d) 
   { return cria_dado(d->dado); }

/* cria uma array, inicialmente 'null' do tipo de dado acima. */
Dado* cria_array_dado(size_t n) {
   size_t tamanho = sizeof(Dado);
   Dado * array = (Dado*)calloc(n, tamanho);
   /* colocando qualquer coisa, posteriormente só é preciso alterar. */
   for(uint64_t p = 0; p < n; p++) 
      { array[p] = cria_dado('#'); }
   return array;
}

// destrói a array de dados.
void destroi_array_dado(Dado* a, size_t q) {
   for(uint64_t p = 0; p < q; p++) 
      { destroi_dado(a[p]); }
   free(a);
   a = NULL; // matando array de uma vez por todas!
}

void visualiza_dado(Dado d) {
   if (d == NULL) 
      { puts("não há nada aqui!"); }
   else 
      // talvez tenha-se que mudar o símbolo de formatação do dado.
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

/* encapsulamento da obtenção do valor do dado interno ao tipo
 * de embrulho Dado. */
extern TIPO_DO_DADO get_dado(Dado d) 
   { return d->dado; }

extern void set_dado(Dado d, TIPO_DO_DADO bruto) 
   { d->dado = bruto; }

// outro nome para a transformação em string, um mais curto.
extern char* dado_to_str(Dado d) 
   { return dado_para_string(d); }

extern bool eq_dado(Dado a, Dado b) 
   { return dados_iguais(a, b); }

extern bool ne_dado(Dado a, Dado b)
   { return !eq_dado(a, b); }

/* verificação dos primeiro parâmetro em relação ao segundo. */
extern bool lt_dado(Dado a, Dado b)
   { return a->dado < b->dado; }
extern bool le_dado(Dado a, Dado b)
   { return a->dado <= b->dado; }
extern bool gt_dado(Dado a, Dado b)
   { return a->dado > b->dado; }
extern bool ge_dado(Dado a, Dado b)
   { return a->dado >= b->dado; }

#include <stdarg.h>
/* destruição de vários 'dados' de forma linear, tudo colocado 
 * como argumentos de uma única função, precedido obviamente pelo
 * total de argumentos.
 */
void destroi_n_dado(size_t total, ...) {
   va_list todos_dados;
   va_start(todos_dados, total);

   for (size_t p = 1; p <= total; p++) {
      Dado atual = va_arg(todos_dados, Dado);
      // impressão de debug.
      #ifdef UT_DADO
      printf("dado: '%s'\n", dado_to_str(atual));
      #endif
      destroi_dado(atual);
   }
   va_end(todos_dados);
}

/* Série de testes unitários a serem executados baseado em funções 
 * auxiliares ou não, e métodos de específicos tipos de dados acima.
 * Antes os testes unitários eram feitos em arquivos separados, fora
 * do escopo da implementação, agora todos estarão juntos a eles, por
 * simplificação de codificação, já que é até mais fácil fazer testes
 * de funções auxiliares internas assim; como também por localização.
 */
#ifdef UT_DADO

#include "../teste.h"
#include <assert.h>

void operacoes_de_comparacoes() {
   Dado a = cria_dado('m');
   Dado b = cria_dado('j');
   Dado c = cria_dado('r');
   Dado d = cria_dado('z');
   Dado e = cria_dado('j');
   assert (ne_dado(b, a));
   assert (lt_dado(c, d));
   assert (gt_dado(a, e));
   assert (ge_dado(b, e));
   assert (le_dado(e, b));
   assert (eq_dado(e, b));
   destroi_n_dado(5, a, b, c, d, e);
}
/* Espaço para a criação de testes unitários.*/
void criacao_basica_do_dado() {
   Dado d = cria_dado('M');
   Dado d1 = cria_dado('a');
   Dado d2 = cria_dado('J');
   visualiza_dado(d);
   visualiza_dado(d1);
   visualiza_dado(d2);
   assert(d->dado == 'M');
   assert(d2->dado == 'J');
   destroi_n_dado(3, d, d1, d2);
}

void criacao_de_array_do_dado() {
   Dado* a = cria_array_dado(15);
   for(int j = 0; j < 15; j++)
      { assert(a[j]->dado == '#');  }
   destroi_array_dado(a, 15);
}

void conversao_para_string() {
   Dado x = cria_dado('k');
   Dado X = cria_dado('Z');
   printf("1ª string: '%s'\n2ª string: '%s'\n",
      dado_para_string(x), 
      dado_para_string(X)
   );
   destroi_n_dado(2, X, x);
}

void contabilizacao_de_instancias() {
   /* apenas funciona se todos demais 'dados' foram liberados 
    * corretamente da memória. */
   assert (instancias_dado() == 0);
   Dado d1 = cria_dado('m');
   printf("total: %lu dados\n", instancias_dado());
   assert(instancias_dado() == 1);

   Dado d2 = cria_dado('P');
   Dado d3 = cria_dado('.');
   Dado d4 = cria_dado('z');
   printf("total: %lu dados\n", instancias_dado());
   assert(instancias_dado() == 4);

   puts("destruindo o primeiro ...");
   destroi_dado(d1);
   printf("total: %lu dados\n", instancias_dado());
   assert(instancias_dado() == 3);

   puts("criando uma array disso ...");
   Dado* array = cria_array_dado(19);
   printf("total: %lu dados\n", instancias_dado());
   assert(instancias_dado() == (19 + 3));
   destroi_n_dado(3, d3, d4, d2);
   assert(instancias_dado() == 19);
   destroi_array_dado(array, 19);
   puts("zero dado em memória.");
   assert(instancias_dado() == 0);
}

void main() {
   executa_testes(
      5, operacoes_de_comparacoes, true,
      criacao_basica_do_dado, true,
      conversao_para_string, true,
      criacao_de_array_do_dado, true,
      contabilizacao_de_instancias, true
   );
}
#endif
