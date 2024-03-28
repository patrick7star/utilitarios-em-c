
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// tipos mais genéricos, então caso necessite mudar-los para outro programa,
// apenas troque aqui.
typedef char* Chave;
typedef uint16_t Valor;
// outros nomes:
typedef Chave chave_t;
typedef Valor valor_t;

struct umaentradadetabela { 
   Chave chave; 
   Valor valor;
};
// seu tamnho em bytes.
#define ENTRADA_SIZE sizeof(struct umaentradadetabela)
// apelidos mais amigaveis, e para parecer um objeto(tira os asterísticos).
typedef struct umaentradadetabela* Entrada;
typedef Entrada* ArrayEntrada;

#include <stdio.h>

static Entrada cria_entrada(Chave c, Valor v) {
   Entrada entrada = malloc(ENTRADA_SIZE);
   if (entrada != NULL) {
      # ifdef DEBUG_Entrada
      puts("'entrada' alocada com sucesso.");
      #endif
      entrada->chave = c;
      entrada->valor = v;
   }
   return entrada;
}

static bool destroi_entrada(Entrada e) {
   if (e != NULL) { 
      free(e); 
      # ifdef DEBUG_Entrada
      puts("'entrada' desalocada com sucesso.");
      #endif
      return true; 
   }
   # ifdef DEBUG_Entrada
   puts("falha ao desalocar 'entrada'.");
   #endif
   return false;
}

static bool eq_entrada(Entrada d, Entrada e) 
   { return !strcmp(e->chave, d->chave); }

#define CHAVE_VAZIA "vAlOrNenHuMnEsTAEntRAdA"
#define VALOR_VAZIO UINT16_MAX

static Entrada cria_entrada_branca() 
   { return cria_entrada(CHAVE_VAZIA, VALOR_VAZIO); }

static bool e_uma_entrada_vazia(Entrada e) {
   Entrada vazia = cria_entrada_branca();
   bool resultado = eq_entrada(e, vazia);
   destroi_entrada(vazia);
   return resultado;
}

static char* entrada_to_str(Entrada e) {
   // caso especial para 'entradas em branco'.
   if (e_uma_entrada_vazia(e)) {
      char* string = malloc(3 * sizeof(Chave));
      memset(string, '-', 2);
      string[2] = '\0';
      return string;
   }
   size_t t = strlen(e->chave);
   /* cinco caractéres para algarismos do tipo(uint16_t) mais o tamanho
    *  -- sendo este o máximo que este tipo de valor pode atingir, 
    * atual da string, o espaço e separador(dois-pontos), sem falar
    * no caractére nulo no final. 
    */
   size_t qtd = 5 + t + 2 + 1;
   char* string = calloc(qtd, sizeof(Chave));

   // fazendo a formatação e retornando...
   sprintf(string, "%s: %d", e->chave, e->valor);
   return string;
}

static ArrayEntrada cria_array_entrada(size_t n) {
   ArrayEntrada array = calloc(n, sizeof(Entrada));

   if (array != NULL) {
      for (size_t t = 0; t < n; t++)
         array[t] = cria_entrada_branca();
      # ifdef DEBUG_ArrayEntrada
      printf("'ArrayEntrada' de %ld, alocada com sucesso.\n", n);
      #endif
   }
   return array;
}

static bool destroi_array_entrada(ArrayEntrada a, size_t n) {
   if (a != NULL) {
      for (size_t p = 0; p < n; p++)
         destroi_entrada(a[p]);
      free(a);
      a = NULL;
      # ifdef DEBUG_ArrayEntrada
      printf("'ArrayEntrada'(%ld) desalocada com sucesso.\n", n);
      #endif
      return true;
   }
   # ifdef DEBUG_ArrayEntrada
   puts("falha ao desalocar 'ArrayEntrada'.");
   #endif
   return false;
}
