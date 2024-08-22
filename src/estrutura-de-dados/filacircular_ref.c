/* Implementação da estrutura 'lista ligada circular' em C. Novamente esta
 * é a abordagem genérica de tal. Se pode usar qualquer referência, porém
 * alguns métodos que exigem a cópia impossíveis aqui. 
 */
#include "filacircular_ref.h"
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
#include <malloc.h>
#endif

/* === === === === === === === === === === === === === === === === === ==
 *                      Implementação do Nó
 * === === === === === === === === === === === === === === === === === ==*/
typedef struct nodulo_unico {
   // dado genérico que tal nódulo guarda.
   generico_t dado;

   // único endereço referenciado(apontado) pela estrutura.
   struct nodulo_unico* seta;   
} nodulo_t, *Node;

static Node cria_nodulo(generico_t dt) {
   const size_t size = sizeof(nodulo_t);
   Node instancia = malloc(size);
   
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

/* === === === === === === === === === === === === === === === === === ==
 *                      Implementação do Lista 
 * === === === === === === === === === === === === === === === === === ==*/
struct lista_circular {
   // referência a calda da lista.
   Node fim;  

   // contagem do total itens na lista.
   size_t contagem;
};

FilaCircular cria_fc(void) {
   const size_t size = sizeof(struct lista_circular);
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

size_t comprimento_fc(FilaCircular q)
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
   Node novo = cria_nodulo(dt);

   if (uma_fila_invalida) 
   {
      // Liberando 'nó' inútil...
      free(novo);
      // Só é possível inserir em objetos válidos.
      perror("não é possível inserir numa 'fila' inválida!");
      abort();
   } 

   if (vazia_fc(q)) {
      // Novo 'nó' referência ele próprio, ele é a calda, ele é o começo.
      novo->seta = novo;
   } else {
      /*  Primeiro o novo 'item' referência o primeiro item da 'fila',
       * então 'a calda' da 'fila' referência este novo 'nó'. */
      novo->seta = q->fim->seta;
      q->fim->seta = novo;
   }
   /* marca a referência da 'calda' como o novo 'nó' criado, basicamente
    * a estrutura FIFO. */
   q->fim = novo; 
   q->contagem += 1; 
}

generico_t retira_fc(FilaCircular q) {
   if (q == NULL) {
      perror("Não é possível inserir numa 'fila' inválida!");
      abort();
   }

   if (vazia_fc(q)) 
   // retorna um 'item' inválido se estiver vázia.
      return NULL;

   Node remocao = q->fim->seta;
   // pegando o dado da remoção.
   generico_t dado_removido = remocao->dado;

   // referenciando o próximo depois da remoção.
   q->fim->seta = remocao->seta;
   // descontabilizando remoção...
   q->contagem -= 1;
   // destruindo nódulo alocado.
   free(remocao);

   return dado_removido;
}

bool rotaciona_fc(FilaCircular q) {
   /* Vale notar que o sentido que ela gira aqui é o anti-horario.*/
   if (q == NULL) {
      perror("não é possível rotacionar numa 'fila circular' inválida!");
      abort();
   }

   // Uma operação inócua numa 'fila' vázia.
   if (vazia_fc(q)) return false;

   /* Só rotaciona se houver mais de um item na 'fila'. */
   q->fim = q->fim->seta;
   return true;
}

static void remove_e_destroi_todos_itens(FilaCircular q, Drop del) {
/* Aplica a operação de remoção até que a 'fila' tenha zero itens. Como
 * tal operação retorna o 'dado' que ele guarda, usa a função desalocação
 * de tal para liberar-lô, em caso de tal função ser inválida(null), nenhuma
 * desalocação de 'dado' será feita.*/
   Drop desaloca = del;

   while (comprimento_fc(q) > 0) 
   {
      generico_t dt = retira_fc(q);
      
      if (desaloca != NULL)
         desaloca(dt);
   }
   assert (comprimento_fc(q) == 0);
}

void destroi_fc(FilaCircular q) {
/* Desaloca todos itens que ela retém, entretanto, não libera os 'dados'
 * contido em cada 'nó', apenas destrói os 'nós'. Portanto, se você passou
 * objetos dinâmicos, chamar tal função fará com que seu programa vaze
 * memória. */
   if (q == NULL) {
      perror("não é possível destruir numa 'fila' inválida!");
      abort();
   }
   
   remove_e_destroi_todos_itens(q, NULL);
   free(q); q = NULL;
}

void destroi_inner_fc(FilaCircular q, Drop del) 
{
/* Desalocação espaço que 'fila' ocupa, e também dos dados adicionados
 * nela -- supondo é claro, que tal foram adicionados na heap. */
   if (q == NULL) {
   // Interrompe o programa em caso de uma 'fila' inválida.
      perror("não é possível destruir numa 'fila' inválida!");
      abort();
   }

   remove_e_destroi_todos_itens(q, del);
   // Libera 'fila', e faz o ponteiro passado totalmente 'nulo'.
   free(q); q = NULL;

   #ifdef __linux__
   const int QTD_DE_BYTES = 0;

   /* Ordem para aplicar todos 'free' chamados neste escopo, e nas funções
    * que ele chama, uma liberação de memória imediata. */ 
   malloc_trim(QTD_DE_BYTES);
   #endif
}

void imprime_fc(FilaCircular q, ToString f) {
   const size_t LIMITE = 10000;
   size_t total = comprimento_fc(q);
   bool nao_ha_elementos = vazia_fc(q);
   nodulo_t* current = q->fim;

   if (total >= LIMITE) {
      perror("O máximo de exibição é 10 mil itens."); 
      abort();
   }
   
   printf("Fila Circular(%zu): [..", total);

   if (nao_ha_elementos) {
   // Finaliza a impressão da 'fila' sem elementos.
      printf("\b\b]\n");
      return;
   }

   do {
      generico_t data = current->dado;
      char* data_str = f(data);

      // Imprime o dado "estringuificado", então libera a formatação.
      printf("%s, ", data_str);
      free(data_str);
      // Percorre a atual fila-circular...
      current = current->seta;

   } while (--total > 0);

   // Finalização da impressão da 'fila'.
   printf("\b\b..]\n");
}

size_t tamanho_fc(FilaCircular q, size_t sz) {
   size_t n = comprimento_fc(q);
   size_t a = sizeof(FilaCircular) + sizeof(struct lista_circular);
   size_t b = sizeof(Node) + sizeof(nodulo_t);
   size_t c = sizeof(q->fim->dado) + sz;

   /* O calculo do tamanho da estrutura dinâmica é a seguinte: a soma do
    * tamanho da estrutura da 'fila', mais o tamanho do pointeiro dela; 
    * a soma entre o tamanho do dado(que é passado como argumento), mais
    * o 'nó' que armazena, somando com o tamanho dos pointeiros de cada,
    * todo este último multiplicado pelo total de itens nela. */
   return a + (b + c) * n;
}

#ifdef UT_FILA_CIRCULAR
/* === === === === === === === === === === === === === === === === === ==
 *                     Testes Unitários da 
 *                Fila Circular e outras Funções
 * === === === === === === === === === === === === === === === === === ==*/
#include <assert.h>
#include <string.h>
#include <tgmath.h>
#include <limits.h>
#include "dados_testes.h"
#include "teste.h"
#include "tempo.h"
#include "legivel.h"
#include "progresso.h"

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
   for (size_t i = 1; i <= comprimento_fc(Q); i++) {
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
   printf ("total de itens: %lu\n", comprimento_fc(queue));

   for (size_t i = 1; i <= 6; i++) {
      generico_t data = (char*)strings_input[ i - 1 ];
      insere_fc(queue, data);
      printf ("total de itens: %lu\n", comprimento_fc(queue));
   }
   visualiza_fc_de_strings(queue);

   loop_infinito {
      // abandona o loop se a 'fila' está vázia.
      if (vazia_fc(queue)) break;

      char* r = retira_fc(queue);
      printf("\t\b\b\bremovido: '%s'\n\n", r);

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
      char* data = (char*)strings_input[i - 1];
      insere_fc(queue, data);
      printf ("total de itens: %lu\n", comprimento_fc(queue));
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
   for (size_t i = 1; i <= comprimento_fc(Q); i++) {
      int* dt = atual->dado;
      printf("%d, ", *dt);
      atual = atual->seta;   
   }
   puts("\b\b -->}\n");
}

static char* int32_to_str(generico_t dt) {
   char screen[15];
   int data_int = *((int*)dt);
   char* inteiro_fmt;
   int char_sz = sizeof(char), len;

   sprintf(screen, "%d", data_int);
   // Copiando a string para array.
   len = strlen(screen);
   inteiro_fmt = malloc((len + 2) * char_sz);
   // Limpando e copiando...
   memset(inteiro_fmt, '\0', len + 2);
   memcpy(inteiro_fmt, screen, len);

   return inteiro_fmt;
}

void simples_visualizacao_da_fila(void) {
   FilaCircular queue = cria_fc();
   int entradas[] = {-99999, -9999, -999, -99, -9, 9, 99, 999, 9999, 99999};

   for (size_t i = 1; i <= 10; i++) {
      generico_t ptr = &entradas[i - 1];
      insere_fc(queue, ptr);
   }

   imprime_fc(queue, int32_to_str);
   destroi_fc(queue);
}

void estressando_rotacao(void) {
   FilaCircular queue = cria_fc();
   int entradas[] = {-99999, -9999, -999, -99, -9, 9, 99, 999, 9999, 99999};
   const size_t TOTAL = 100000;

   for (size_t i = 1; i <= 10; i++) {
      generico_t ptr = &entradas[i - 1];
      insere_fc(queue, ptr);
   }

   for (size_t i = 1; i <= TOTAL; i++) {
      // impressão raramente acontece.
      if (i % 2857 == 0) 
         imprime_fc(queue, int32_to_str);
      rotaciona_fc(queue);
   }


   imprime_fc(queue, int32_to_str);
   destroi_fc(queue);
}

static bool desaloca_int32(generico_t a)
   { free(a); a = NULL; return true; }

void nao_quebra_em_liberar_dados_de_fila_vazias(void) {
   puts("Criado com sucesso, porém sem dados interno.");
   FilaCircular queue = cria_fc();
   printf("Liberando fila-circular, e seus dados internos ... ");
   destroi_inner_fc(queue, desaloca_int32);
   puts("feito com sucesso.");
}

int main(void) {
   executa_testes (
      6, criacao_e_destruicao, true,
         efetuando_operacoes, true,
         verificando_rotacao, true,
         simples_visualizacao_da_fila, true,
         nao_quebra_em_liberar_dados_de_fila_vazias, true,
         estressando_rotacao, true
   ); 

   return EXIT_SUCCESS;
}
#endif
