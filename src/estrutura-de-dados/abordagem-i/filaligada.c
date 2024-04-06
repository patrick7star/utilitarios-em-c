
/* criando uma simples fila-ligada com o tipo também embutido aqui. Se desejar 
 * trocar o tipo dela futuramente, apenas altere as linhas iniciais. 
 */

// bibliotecas do C:
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// tal ponteiro pode assumir qualquer valor.
typedef void* generico_t;
// representação de tal objeto não sendo referênciado.
#define INVALIDO NULL

typedef struct no_modelo {
   generico_t dado;
   struct no_modelo* seta;
} no_t;

no_t* cria_no(generico_t x) {
   no_t* node = malloc(sizeof(no_t));

   if (node != NULL) {
      node->dado = x;
      node->seta = NULL;
   }
   return node;
}

bool destroi_no(no_t* m) {
   if (m == INVALIDO) return false;
   free(m);
   m = NULL;
   return true;
}

typedef struct fila_ligada {
   // total de itens nela:
   size_t tamanho;

   // referência as suas pontas:
   no_t* inicio;
   no_t* fim; 

} fila_ligada_t;

fila_ligada_t* cria_fl() {
   size_t t = sizeof (fila_ligada_t);
   fila_ligada_t* fila = malloc (t);

   if (fila != INVALIDO) {
      fila->inicio = NULL;
      fila->fim = NULL;
      fila->tamanho = 0;
   }
   return fila;
}

static bool objeto_valido(fila_ligada_t* f) 
   { return f != INVALIDO; }

size_t tamanho_fl(fila_ligada_t* f) 
   { return f->tamanho; }

bool vazia_fl(fila_ligada_t* f) 
   { return tamanho_fl(f) == 0; }

bool insere_fl(fila_ligada_t* f, generico_t d) {
   if (objeto_valido(f)) {
      no_t* novo = cria_no(d);

      if (vazia_fl(f)) {
         f->inicio = novo;
         f->fim = novo;
      } else {
         f->fim->seta = novo;
         f->fim = novo;
      }
      f->tamanho++;
      return true;
   }
   return false;
}

generico_t remove_fl(fila_ligada_t* f) {
   if (vazia_fl(f))
      return NULL;
   
   // apenas referênciando o próximo depois do "primeiro".
   no_t* remocao = f->inicio;
   f->inicio = f->inicio->seta;
   // na heap para retirar deste escopo.
   generico_t obj = remocao->dado;
   // descontabiliza...
   f->tamanho--;

   return obj;
}

bool destroi_fl(fila_ligada_t* f) {
   if (objeto_valido(f)) {
      while (!vazia_fl(f)) {
         free(remove_fl(f));
      }
      free(f);
      return true;
   }
   return false;
}

generico_t final_fl(fila_ligada_t* f) {
   if (!vazia_fl(f)) 
      return &f->fim->dado;
   return NULL;
}
generico_t comeco_fl(fila_ligada_t* f) {
   if (!vazia_fl(f)) 
      return &f->inicio->dado;
   return NULL;
}

#ifdef _UT_FILA_LIGADA

// várias entradas para seus testes.
char entradas[] = { 'm', 'z', 'i', 'L' };

void visualiza_char_fl(fila_ligada_t* f) {
   if (!objeto_valido(f))
      return;
   if (vazia_fl(f))
      puts("fila-ligada: []");
   else {
      printf("fila-ligada: [");
      no_t* atual = f->inicio;

      while (atual != NULL) {
         char* removido = (char*)atual->dado;
         printf("%c, ", *removido);
         atual = atual->seta;
      }
      printf("\b\b]\n");
   }
}

void mostra_pontas_da_fila(fila_ligada_t* q) {
   char inicio = *((char*)comeco_fl (q));
   char fim = *((char*)final_fl (q));

   printf("start: %c\tend: %c\n", inicio, fim);
}

void todas_operacoes_basicas_simples() {
   fila_ligada_t* fila = cria_fl();
   visualiza_char_fl(fila);
   insere_fl(fila, &entradas[0]);
   mostra_pontas_da_fila (fila);

   insere_fl(fila, &entradas[1]);
   insere_fl(fila, &entradas[2]);
   insere_fl(fila, &entradas[3]);
   mostra_pontas_da_fila (fila);
   visualiza_char_fl(fila);

   printf("removido: %c\n", *((char*)remove_fl(fila)));
   printf("removido: %c\n", *((char*)remove_fl(fila)));
   visualiza_char_fl(fila);
}

int main(int total, char* args[], char* vars[]) {
   todas_operacoes_basicas_simples();
   return EXIT_SUCCESS;
}
#endif

