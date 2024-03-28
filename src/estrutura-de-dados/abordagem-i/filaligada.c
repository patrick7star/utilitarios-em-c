
/* criando uma simples fila-ligada com o tipo também embutido aqui. Se desejar 
 * trocar o tipo dela futuramente, apenas altere as linhas iniciais. 
 */

typedef char Dado;

#include <stdio.h>
#include <stdlib.h>

char* dado_to_str(Dado d) {
   char* string = malloc(sizeof(char) * 3);
   sprintf(string, "%c", d);
   return string;
}

typedef struct no_modelo {
   Dado dado;
   struct no_modelo* seta;
} no_t;

#include <stdbool.h>

no_t* cria_no(Dado x) {
   no_t* node = malloc(sizeof(no_t));

   if (node != NULL) {
      node->dado = x;
      node->seta = NULL;
   }
   return node;
}

bool destroi_no(no_t* m) {
   free(m);
   m = NULL;
   return true;
}

typedef struct fila_ligada {
   no_t* inicio;
   no_t* fim; 
   size_t tamanho;
} fila_ligada_t;

fila_ligada_t* cria_fl() {
   fila_ligada_t* fila = malloc(sizeof(fila_ligada_t));

   if (fila != NULL) {
      fila->inicio = NULL;
      fila->fim = NULL;
      fila->tamanho = 0;
   }
   return fila;
}

bool objeto_valido(fila_ligada_t* f) 
   { return f != NULL; }

size_t tamanho_fl(fila_ligada_t* f) 
   { return f->tamanho; }

bool vazia_fl(fila_ligada_t* f) 
   { return tamanho_fl(f) == 0; }

bool insere_fl(fila_ligada_t* f, Dado d) {
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

Dado* remove_fl(fila_ligada_t* f) {
   if (vazia_fl(f))
      return NULL;
   
   // apenas referênciando o próximo depois do "primeiro".
   no_t* remocao = f->inicio;
   f->inicio = f->inicio->seta;
   // na heap para retirar deste escopo.
   Dado* obj = malloc(sizeof(Dado));
   *obj = remocao->dado;
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

Dado* final_fl(fila_ligada_t* f) {
   if (!vazia_fl(f)) 
      return &f->fim->dado;
   return NULL;
}
Dado* comeco_fl(fila_ligada_t* f) {
   if (!vazia_fl(f)) 
      return &f->inicio->dado;
   return NULL;
}


void visualiza_fl(fila_ligada_t* f) {
   if (!objeto_valido(f))
      return;
   if (vazia_fl(f))
      puts("[]");
   else {
      printf("[");
      no_t* atual = f->inicio;

      while (atual != NULL) {
         printf("%s, ", dado_to_str(atual->dado));
         atual = atual->seta;
      }
      printf("\b\b]\n");
   }
}


#ifdef _UT_FILA_LIGADA

void todas_operacoes_basicas_simples() {
   fila_ligada_t* fila = cria_fl();
   visualiza_fl(fila);
   insere_fl(fila, 'm');
   printf("start: %c\tend: %c\n", *comeco_fl(fila), *final_fl(fila));
   insere_fl(fila, 'z');
   insere_fl(fila, 'i');
   printf("start: %c\tend: %c\n", *comeco_fl(fila), *final_fl(fila));
   insere_fl(fila, 'L');
   printf("start: %c\tend: %c\n", *comeco_fl(fila), *final_fl(fila));
   visualiza_fl(fila);
   printf("%c\n", *remove_fl(fila));
   printf("%c\n", *remove_fl(fila));
   visualiza_fl(fila);
   destroi_fl(fila);
}

int main(int total, char* args[], char* vars[]) {

   todas_operacoes_basicas_simples();
   return EXIT_SUCCESS;
}

#endif

