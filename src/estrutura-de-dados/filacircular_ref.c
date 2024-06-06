
/* Implementação da estrutura 'lista ligada circular' em C. Novamente esta
 * é a abordagem genérica de tal. Se pode usar qualquer referência, porém
 * alguns métodos que exigem a cópia impossíveis aqui. */
#include "../../../include/estrutura-de-dados/abordagem-i/filacircular.h"
#include <errno.h>
#include <stdio.h>


/* ~~~ ~~~ ~~~ ~~~ ~~~ ~~ Implementação do Nó ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ */
 
typedef struct nodulo_unico {
   // dado genérico que tal nódulo guarda.
   generico_t dado;

   // único endereço referenciado(apontado) pela estrutura.
   struct nodulo_unico* seta;   
} nodulo_t;

static nodulo_t* cria_nodulo(generico_t dt) {
   nodulo_t* instancia = malloc(sizeof(nodulo_t));
   
   if (instancia != NULL) {
      instancia->seta = NULL;
      instancia->dado = dt;
   } else {
      /* Esta implementação será diferenciada das demais. Tal, quando 
       * houver um erro de 'alocação dinâmica', interrompe imediatamente
       * o programa e gera um erro.*/
      perror("não foi possível gerar um 'nódulo' na heap.");
      abort();
   }
   return instancia;
}

static void destroi_nodulo(nodulo_t* no) {
   if (no != NULL) {
      // perdendo referência do próximo 'nó'.
      no->seta = NULL;
      // "esquecendo" referência do 'dado'.
      no->dado = NULL;
      free(no);
   } else {
      perror("não é possível desalocar um 'nó' inválido.");
      abort();
   }
}

/* ~~~ ~~~ ~~~ ~~~ ~~~ Implementação do Lista ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ */
struct lista_circular {
   // referência a calda da lista.
   nodulo_t* fim;  

   // contagem do total itens na lista.
   size_t contagem;
};

FilaCircular cria_fc(void) {
   size_t size = sizeof(struct lista_circular);
   FilaCircular queue = malloc(size);

   if (queue != NULL) {
      queue->fim = NULL;
      queue->contagem = 0;
   } else {
      perror("não foi possível gerar uma 'fila circular' na heap.");
      abort();
   }
   return queue;
}

size_t tamanho_fc(FilaCircular q)
   { return q->contagem; }

bool vazia_fc(FilaCircular q)
   { return q->contagem == 0; }

generico_t primeiro_fc(FilaCircular q) {
   if (q == NULL) {
      perror("não é possível inserir numa 'fila' inválida!");
      abort();
   } else if (vazia_fc(q))
      // em caso de está vázia, retorna um 'null'.
      return NULL;

   // Como é circular, o 'ponteiro' que aponta o fim da 'lista/fila', 
   // tem como seu próximo elemento o primeiro da 'fila'.
   nodulo_t* comeco = q->fim->seta;
   // retorna uma cópia da referência interna do 'nódulo'.
   return comeco->dado;
}

void insere_fc(FilaCircular q, generico_t dt) {
   bool uma_fila_invalida = (q == NULL);
   // só é possível inserir em objetos válidos.
   if (uma_fila_invalida) {
      perror("não é possível inserir numa 'fila' inválida!");
      abort();
   } else {
      nodulo_t* novo = cria_nodulo(dt);

      if (vazia_fc(q)) {
         // novo 'nó' referência ele próprio, ele é a calda, ele é o começo.
         novo->seta = novo;
      } else {
         //  Primeiro o novo 'item' referência o primeiro item da 'fila',
         // então 'a calda' da 'fila' referência este novo 'nó'.
         novo->seta = q->fim->seta;
         q->fim->seta = novo;
      }
      // marca a referência da 'calda' como o novo 'nó' criado, basicamente
      // a estrutura FIFO.
      q->fim = novo; 
      q->contagem += 1; 
   }
}

generico_t retira_fc(FilaCircular q) {
   if (q == NULL) {
      perror("não é possível inserir numa 'fila' inválida!");
      abort();
   }

   // retorna um 'item' inválido se estiver vázia.
   if (vazia_fc(q)) return NULL;
   else {
      nodulo_t* remocao = q->fim->seta;
      // pegando o dado da remoção.
      generico_t dado_removido = remocao->dado;

      // referenciando o próximo depois da remoção.
      q->fim->seta = remocao->seta;
      // descontabilizando remoção...
      q->contagem -= 1;
      // destruindo nódulo alocado.
      destroi_nodulo(remocao);

      return dado_removido;
   }
}

void rotaciona_fc(FilaCircular q) {
   /* Vale notar que o sentido que ela gira aqui é o anti-horario.*/
   if (q == NULL) {
      perror("não é possível rotacionar numa 'fila circular' inválida!");
      abort();
   }
   /* Só rotaciona se houver mais de um item na 'fila'. */
   if (q->contagem > 0)
      q->fim = q->fim->seta;
}

void destroi_fc(FilaCircular q) {
   if (q == NULL) {
      perror("não é possível destruir numa 'fila' inválida!");
      abort();
   }
   while (!vazia_fc(q)) {
      retira_fc(q);
   }
   free(q); q = NULL;
}

#ifdef UT_FILA_CIRCULAR
#include <assert.h>
#include <string.h>
#include "../../../include/dados_testes.h"
#include "teste.h"

void criacao_e_destruicao(void) {
   FilaCircular queue = cria_fc();
   assert (queue != NULL);
   puts("alocação foi bem sucedida.");
   destroi_fc(queue);
   puts("desalocação foi bem sucedida.");
}

void visualiza_fc_de_strings(FilaCircular Q) {
   if (vazia_fc(Q)) { puts("{}"); return; }

   nodulo_t* atual = Q->fim->seta;

   printf("{ <-- ");
   for (size_t i = 1; i <= tamanho_fc(Q); i++) {
      char* dt = (char*)atual->dado;
      printf("%s, ", dt);
      atual = atual->seta;   
   }
   puts("\b\b -->}\n");
}


void efetuando_operacoes(void) {
   const char* strings_input[] = {
      frutas[4], frutas[8], boys_names[1],
      boys_names[4], girls_names[5], veiculos[10]
   };
   FilaCircular queue = cria_fc();

   assert (vazia_fc(queue));
   printf ("total de itens: %lu\n", tamanho_fc(queue));

   for (size_t i = 1; i <= 6; i++) {
      const char* ptr = strings_input[ i - 1 ];
      insere_fc(queue, ptr);
      printf ("total de itens: %lu\n", tamanho_fc(queue));
   }
   visualiza_fc_de_strings(queue);

   loop_infinito {
      // abandona o loop se a 'fila' está vázia.
      if (vazia_fc(queue)) break;

      generico_t r = retira_fc(queue);
      visualiza_fc_de_strings(queue);
   }
   destroi_fc(queue);
}

void verificando_rotacao(void) {
   FilaCircular queue = cria_fc();
   const char* strings_input[] = {
      veiculos[4], frutas[6], boys_names[5],
      boys_names[2], girls_names[9], 
      veiculos[1], legumes[7]
   };

   for (size_t i = 1; i <= 7; i++) {
      const char* ptr = strings_input[i - 1];
      insere_fc(queue, ptr);
      printf ("total de itens: %lu\n", tamanho_fc(queue));
   }
   visualiza_fc_de_strings(queue);

   char* first_data = primeiro_fc(queue);
   char first_original[20]; 
   strcpy(first_original, first_data);

   printf("first: '%s'\n", first_data);
   rotaciona_fc(queue);
   visualiza_fc_de_strings(queue);

   first_data = primeiro_fc(queue);
   printf("first: '%s'\n", first_data);
   rotaciona_fc(queue);
   visualiza_fc_de_strings(queue);

   first_data = primeiro_fc(queue);
   printf("first: '%s'\n", first_data);
   rotaciona_fc(queue);
   visualiza_fc_de_strings(queue);

   printf("onde começou: '%s'\n", first_original);
   for (size_t i = 1; i <= 4; i++) {
      rotaciona_fc(queue);
      first_data = primeiro_fc(queue);
   }
   visualiza_fc_de_strings(queue);
   assert (strcmp(first_data, first_original) == 0);
   destroi_fc(queue);
}

void visualiza_fc_de_int32(FilaCircular Q) {
   if (vazia_fc(Q)) { puts("{}"); return; }

   nodulo_t* atual = Q->fim->seta;

   printf("{ <-- ");
   for (size_t i = 1; i <= tamanho_fc(Q); i++) {
      int* dt = atual->dado;
      printf("%d, ", *dt);
      atual = atual->seta;   
   }
   puts("\b\b -->}\n");
}

void estressando_rotacao(void) {
   FilaCircular queue = cria_fc();
   int entradas[] = {-99999, -9999, -999, -99, -9, 9, 99, 999, 9999, 99999};

   for (size_t i = 1; i <= 10; i++) {
      int* ptr = &entradas[i - 1];
      insere_fc(queue, ptr);
   }
   visualiza_fc_de_int32(queue);
   for (size_t i = 1; i <= 100000; i++) {
      // impressão raramente acontece.
      if (i % 2857 == 0)
         visualiza_fc_de_int32(queue);
      rotaciona_fc(queue);
   }

   visualiza_fc_de_int32(queue);
   destroi_fc(queue);
}

void main(void) {
   executa_testes (
      4, criacao_e_destruicao, true,
         efetuando_operacoes, true,
         verificando_rotacao, true,
         // [nota] desativado, pois o 'output' é gigantesco.
         estressando_rotacao, true
   ); 
}
#endif
