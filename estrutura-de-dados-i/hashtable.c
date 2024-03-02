
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*   Tipos mais genéricos, então caso necessite mudar-los para outro 
 * programa, apenas troque aqui.
 * 
 *   Hashtables com chaves genérica são basicamente impossível, não poderia
 * implementar a hash no interno da estrutura. Basicamente implementar uma
 * para cada tipo primitivo(string, caractere, inteiro, etc...). O valor
 * pode sempre ser genérico, já que ele é irrelevante para o cálculo hash.
 * Digo apenas para tipos primitivos, pois são com os que sempre usei, 
 * nunca utilizei outras estruturas com hash como chave em um mapa, mesmo
 * no Python, onde isso é extramente fácil de se chegar em tal código 
 * macarronico.
 *
 *   Novamente, como é algo genérico, então a parte de visualização fica 
 * irrelevante aqui, apenas o comportamento é importante nestes casos.
 */
typedef char* chave_t;
typedef void* valor_t;

typedef struct umaentradadetabela { 
   chave_t chave; 
   valor_t valor;
} entry_t;

static entry_t cria_entrada(chave_t c, valor_t v) {
   entry_t* entrada = malloc (sizeof(entry_t));

   if (entrada != NULL) {
      # ifdef UT_HASHTABLE
      puts("'entrada' alocada com sucesso.");
      #endif
      entrada->chave = c;
      entrada->valor = v;
   }
   return entrada;
}

static bool destroi_entrada(entry_t* e) {
   if (e != NULL) { 
      free(e); 
      # ifdef UT_HASHTABLE
      puts("'entrada' desalocada com sucesso.");
      #endif
      return true; 
   }
   # ifdef UT_HASHTABLE
   puts("falha ao desalocar 'entrada'.");
   #endif
   return false;
}

#define STRINGS_IGUAIS 0

static bool eq_entrada(entry_t* d, entry_t* e) 
   { return strcmp(e->chave, d->chave) == STRINGS_IGUAIS; }

// const char* CHAVE_VAZIA = "vAlOrNenHuMnEsTAEntRAdA";
#define CHAVE_VAZIA "vAlOrNenHuMnEsTAEntRAdA"
static Entrada cria_entrada_branca() 
   { return cria_entrada(CHAVE_VAZIA, UINT16_MAX); }

/* Testando todos métodos, funções, e dados abstratos acima. */
#ifdef UT_HASHTABLE

#include <assert.h>
void testa_entrada_tipo() {
   Entrada e = cria_entrada("string", 37);
   printf("%s:%d\n", e->chave, e->valor);
   assert(destroi_entrada(e));
}

void igualdade_de_entradas() {
   Entrada e = cria_entrada("string", 37);
   Entrada a = cria_entrada("maça", 37);
   Entrada x = cria_entrada("uva", 8);
   Entrada t = cria_entrada("string", 5);
   assert(!eq_entrada(a, e));
   assert(!eq_entrada(a, x));
   assert(eq_entrada(e, t));
   puts("todas passaram no teste de igualdade!");
   destroi_entrada(e);
   destroi_entrada(a);
   destroi_entrada(x);
   destroi_entrada(t);
}

/*
void alocacao_e_destruicao_array_entrada() {
   ArrayEntrada a = cria_array_entrada(7);
   assert(destroi_array_entrada(a, 7));
}*/

void main() {
   testa_entrada_tipo();
   igualdade_de_entradas();
   // alocacao_e_destruicao_array_entrada();
}
#endif

