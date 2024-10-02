
/* Estrutura de dados, porém recebe valores genéricos(pointeiros de qualquer
 * tipo). O que importa aqui é o comportamento FIFO. Como possui um dado
 * pointeiro genérico(*void) não tem visualização, como já dito, apenas o 
 * comportamento importa nesta versão.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#define INVALIDA NULL
#define INICIALMENTE 20u
// tamanho do genérico(um ponteiro do tipo void).
#define SzG sizeof(generico_t)

// ponteiro 'void' aceita qualquer referência passada para ela.
typedef void* generico_t;
typedef struct { generico_t* dados; size_t total; } Array;

typedef struct fila_de_array_referencia {
   // tamanho inicial da array.
   size_t capacidade;

   // local da próxima inserção.
   size_t frente;

   // total de itens contidos nela no momento.
   size_t quantidade;

   // containers para alocação dos dados.
   generico_t* array;
} fila_t;


fila_t* cria_com_capacidade_fa(size_t capacity) {
   if (capacity < 2) {
      perror("não é possível criar tal com uma capacidade tão baixa.");
      abort();
   }
   size_t T = sizeof(fila_t);
   fila_t* instancia = malloc (T);

   if (instancia != INVALIDA) {
      instancia->capacidade = capacity;
      instancia->quantidade = 0;
      instancia->frente = 0;

      // tenta alocar a array, a parte mais importante.
      instancia->array = malloc (capacity * T);

      #ifdef ALOCACAO_E_DEALOCACAO
      puts("array generica alocada sucessamente.");
      #endif

      // se não tiver alocado corretamente, libera 'fila' e retorna 'null'.
      if (instancia == INVALIDA) 
         { free (instancia); return NULL; }

      #ifdef ALOCACAO_E_DEALOCACAO
      puts("instância foi criada com sucesso.");
      #endif
   }
   return instancia;
}

fila_t* cria_fa(void) 
   { return cria_com_capacidade_fa(INICIALMENTE); }

bool destroi_fa(fila_t* q) {
   if (q != INVALIDA) {
      // libera array interna e a 'fila', então confirma liberação.
      free (q->array);
      free (q);

      #ifdef ALOCACAO_E_DEALOCACAO
      puts("tanto a array, como a 'fila' foram sucedidas na desalocação.");
      #endif

      return true;
   }
   #ifdef ALOCACAO_E_DEALOCACAO
   puts("a 'fila' passada é inválida.");
   #endif
   // diz que foi um fracasso a liberação.
   return false;
}

size_t tamanho_fa(fila_t* q) {
	if (q != INVALIDA)
		return q->quantidade;

   // interrompe programa em caso de erro.
	perror ("'fila' passada é um dado inválido.");
	abort();
}

bool vazia_fa(fila_t* q) 
	{ return tamanho_fa(q) == 0; }

generico_t primeiro_fa(fila_t* q) {
	if (q == INVALIDA) return NULL;
	if (vazia_fa(q))	return NULL;
	return q->array[q->frente];
}

static void redimensiona(fila_t* Q, size_t nC) {
   size_t t = tamanho_fa(Q);
   size_t C = Q->capacidade;
   size_t f = Q->frente;
   generico_t* new_array = malloc(nC * SzG);
   generico_t* old_array = Q->array;

   /* Se o total de itens for menos que um quarto da capacidade, então
    * encolhe a 'array interna' à metade. */
   for (size_t k = 0; k < t; k++) {
      /* Percorrendo à partir da 'frente' da 'fila'.*/
      generico_t data = old_array[(f + k) % C]; 
      new_array[k] = data;
   }

   /* livrando-se da antiga, assim deixando um 'dangling pointer' na 
    * estrutura, então retornando a nova. Deste modo, é esperado que 
    * o campo liberado no escopo maior que este, pegue a nova array de
    * dados retornada. */
   free(old_array);
   /* Realiza um 'swaping' array internas e, atualiza a capacidade. Também 
    * a 'frente' voltou a ser a posição zero. */
   Q->capacidade = nC;
   Q->frente = 0;
   Q->array = new_array;
}

static void aumenta_fila_automaticamente(fila_t* Q) {
	size_t t = Q->quantidade;
	size_t C = Q->capacidade;

   /* verifica se e necessário redimensionar? Se for o caso, então faz com
    * que a nova tenha o dobro do tamanho da anterior. */
   if (t >= C) { redimensiona(Q, 2 * C); }
}

bool coloca_fa(fila_t* q, generico_t e) {
	if (q == INVALIDA) return false;
   /* Processo de amortização, se os itens já tiverem enchido todo container
    * ele trata de dobrar a atual capacidade para adição de mais itens.*/
   aumenta_fila_automaticamente(q);

   // novos nomes para facilitar(alinhar) o cálculo.
	size_t f = q->frente;
	size_t t = q->quantidade;
	size_t C = q->capacidade;

	q->array[(f + t) % C] = e;
	q->quantidade += 1;

	return true;
}

static void encolhe_fila_automaticamente(fila_t* Q) {
   size_t C = Q->capacidade;
   size_t t = Q->quantidade;
   bool acima_do_limite_inferior = (t > 2);
   bool abaixo_do_limite_superior = (t <= (C / 4));

   // ainda não está pronto para reduzir, ou não é possível fazer-lô.
   if (acima_do_limite_inferior && abaixo_do_limite_superior) {
      /* libera a antiga da memória e, atribui a nova, também atualiza a
       * nova capacidade da 'array interna'. */
      redimensiona(Q, C / 2);
      puts("houve redução!");
   }
}

generico_t retira_fa(fila_t* q) {
   // só é possível remover filas vázias.
	if (vazia_fa(q)) { return NULL; }

   /* Encolhe a array interna caso tenha ultrapassado um limite mínimo, no
    * caso aqui um quarto da 'capacidade', a ação aqui fica em cortar tal
    * capacidade pela metade. */
   encolhe_fila_automaticamente(q);
	
   // obtendo endereço do elemento na 'frente' da 'fila'.
   size_t f = q->frente;
   size_t C = q->capacidade;
	generico_t removido = q->array[f];

	/* computa índice seguinte. Sim, nada é removido, sobre-escrita(pelo
	 * menos ainda) na remoção, apenas o marcador da 'frente' dela, avança 
	 * uma casa, respeitando sua 'capacidade'. */
	q->frente = (f + 1) % C;
	q->quantidade -= 1;

	return removido;
}

size_t vacuo_fa(fila_t* q) 
   /* Verifica a diferença entre a capacidade e o total de itens
    * adicionado, vamos chamar isso de "espaço vago". Depedendo do tamanho
    * da 'fila', isso ocupa um bucado de memória, por isso é importante
    * ter tal valor em mente. */
   { return q->capacidade - q->quantidade; }

void print_fila_fa(fila_t* q, char* (__str__)(generico_t)) 
{
   /* Dado o método de transforma o tipo de dado numa string, imprimirá 
    * qualquer array contendo dados "homogêneos". */
	printf("[");
   // se não houver itens, apenas terminar e "fechar o colchete".
	if (vazia_fa (q)) { puts("]"); return; }

   // como é uma fila, irei remover todos 'n' itens e inserir novamente.
   // Tais operações são feitas em tempo constante, por quê não?
   size_t p = 0;
   size_t f = q->frente;
   size_t C = q->capacidade;
   size_t t = tamanho_fa(q);

	while (p < t) 
   {
      generico_t data = q->array[(f + p++) % C];
      char* valor_str = __str__(data);

      printf("%s, ", valor_str);
      free(valor_str);
   }
	puts("\b\b]");
}

size_t capacidade(fila_t* q)
   /* Retorna a capacidade interna. Sim, claro,... poderiamos obter o 
    * mesmo valor apenas usando o método 'vácuo' somado com o número de
    * itens(o método 'tamanho'). */
   { return q->capacidade; }

/* Testando todos métodos, funções, e dados abstratos acima. Deixando
 * bem referênciado esta parte, pois fica fácil descartar -- além de 
 * ser necessário se os tipos forem trocados, do contrário o programa
 * não compila; se copiado para vários projetos. Caso também esta parte
 * futuramente for colocada num subdiretório, e os tipos serem trocados
 * apenas comentar tal declaração pré-processada para não incluir o que
 * pode conflitar.
 */
#ifdef _UT_FILA_ARRAY
#include "teste.h"
#include "aleatorio.h"
#include <string.h>

// funções auxiliares para os testes:
void visualiza_char_fa(fila_t* q) {
   char* to_string(generico_t dt) {
      char* str = malloc(3 * sizeof(char));
      memset(str, '\0', 3);
      *str = *((char*)dt);
      return str;
   }
   print_fila_fa(q, to_string);
}

void visualiza_int_fa(fila_t* q) {
   char* to_string(generico_t dt) {
      char* s = malloc(10 * sizeof(char));
      sprintf(s, "%d", *((int*)dt));
      return s;
   }
   print_fila_fa(q, to_string);
}

void visualiza_interno_fa(fila_t* Q, void (*print)(generico_t)) 
{
	size_t C = Q->capacidade, i = 0;
   size_t F = Q->frente, T = Q->quantidade;
   bool houve_iteracao = false;
   generico_t* array = Q->array;

   setbuf(stdout, false);
	printf("[");

   while (i < C) {
      generico_t dt = array;
      // movendo atravez da array ...
      array = array + i;
      bool posicao_com_dado_valido = { 
         /* condição se a posição da 'frente' for menor que a 'traseira',
          * numa orientação esquerda à direita. */
         (F <= i && i <= (F + T)) ||
         /* Em caso de o continuo de dados estiver quebrado na array, ou 
          * seja, à parte final no começo da array, e a parte inicial
          * depois desta, porém separada por um longo arranjo de dados
          * inválidos. */
         (i >= 0 && i <= (F + T) % C)
      };

      if (!houve_iteracao)
         { houve_iteracao = true; }

      if (posicao_com_dado_valido)
         print(dt); 
      else 
         printf("--, ");
      i++;
   }

   if (houve_iteracao)
      puts("\b\b]");
   else
      puts("]");
}

void visualiza_bool_fa(fila_t* Q) {
   char* to_string(generico_t dt) {
      char* s = malloc(16 * sizeof(char));
      bool* a = (bool*)dt;
      const char* vS = bool_to_str(*a);
      strcpy(s, vS);
      return s;
   }
   print_fila_fa(Q, to_string);
}

void adiciona_em_series_chars(fila_t* Q, size_t t) {
   for (size_t i = 1; i <= t; i++)
   {
      size_t size = sizeof(char);
      char* valor = malloc(size);
      *valor = (char)alfabeto_aleatorio();
      coloca_fa(Q, valor);
   }
   assert (tamanho_fa(Q) >= t);
}

void adiciona_em_series_booleanos(fila_t* Q, size_t t) {
   for (size_t i = 1; i <= t; i++)
   {
      bool* valor = malloc(sizeof(bool));
      *valor = logico();
      coloca_fa(Q, valor);
   }
   assert (tamanho_fa(Q) >= t);
}

/* Abaixo ficarão os testes em sí. */
void instancia_de_caracteres(void) {
   fila_t* Q = cria_fa();

   visualiza_char_fa (Q);
   assert (vazia_fa (Q));

   puts("começando a inserção ...");
   char entradas[] = {'m', 'a', 'Z', 'p'};
   for (size_t k = 1; k <= 4; k++) {
      char* pointer = &entradas[k - 1];
      coloca_fa (Q, pointer);
   }
   visualiza_char_fa (Q);
   assert (tamanho_fa (Q) == 4);

   puts ("remoção em série:");
   for (size_t k = 4; k > 0; k--) {
      printf ("removido: %c\n", *((char*)retira_fa(Q)));
      visualiza_char_fa (Q);
   }

   destroi_fa (Q);
}

void instancia_de_inteiros_simples(void) {
   fila_t* Q = cria_com_capacidade_fa(6);

   visualiza_char_fa (Q);
   assert (vazia_fa (Q));

   int entradas[] = {-1, 2, -3, 4, -5};

   for (size_t k = 1; k <= 5; k++)
      coloca_fa (Q, (int*)&entradas[k - 1]);

   visualiza_char_fa (Q);
   assert (tamanho_fa (Q) == 5);

   puts ("remoção em série:");
   for (size_t k = 5; k > 0; k--) {
      printf ("removido: %d\n", *((int*)retira_fa(Q)));
      visualiza_int_fa (Q);
   }
   destroi_fa (Q);
}

void testando_valores_genericos(void) {
   puts("usando caractéres: ");
   instancia_de_caracteres();
   puts("\n\nusando inteiros de 32-bits: ");
   instancia_de_inteiros_simples();
}

void visualizando_escolhimento_e_expansao(void) {
   void println_bool(generico_t dt) {
      bool* ptr = dt; bool valor = *ptr;
      const char* valor_str = bool_to_str(valor);
      printf("%s, ", valor_str);
   }

   fila_t* queue = cria_com_capacidade_fa(6);

   visualiza_interno_fa(queue, println_bool);
   adiciona_em_series_booleanos(queue, 3);
   visualiza_interno_fa(queue, println_bool);

   adiciona_em_series_booleanos(queue, 4);
   visualiza_interno_fa(queue, println_bool);

   adiciona_em_series_booleanos(queue, 6);
   visualiza_interno_fa(queue, println_bool);
   printf(
      "espaços restantes e (total): %lu(%lu)\n", 
      vacuo_fa(queue), tamanho_fa(queue)
   );

   puts("\nverificando diminuação...");
   for (size_t i = 1; i <= 8; i++) {
      bool* remocao = retira_fa(queue);
      free(remocao);
      visualiza_interno_fa(queue, println_bool);
   }
   printf(
      "espaços restantes e (total): %lu(%lu)\n", 
      vacuo_fa(queue), tamanho_fa(queue)
   );

   visualiza_interno_fa(queue, println_bool);
   visualiza_bool_fa(queue);
   assert(destroi_fa(queue));
}

void encolhimento_automatico(void) {
   char* println(generico_t dt) {
      char* fmt = malloc (15 * sizeof(char));
      memset(fmt, '\0', 15);
      sprintf(fmt, "%d", *((int*)dt));
      return fmt;
   }

   int E[] = {-9999, -999, -99, -9, 0, 9, 99, 999, 9999};
   int E_i[] = {5, 10, 15, 20, 25, 30, 35, 40, 45};
   fila_t* queue = cria_com_capacidade_fa(6);

   for (size_t i = 1; i <= 9; i++) { 
      int* input = E + (i - 1);
      coloca_fa(queue, input); 
      printf(
         "total de itens: %lu\tcapacidade: %lu\n", 
         tamanho_fa(queue), capacidade(queue)
      );
   }
   print_fila_fa(queue, println);
   for (size_t i = 1; i <= 9; i++) { 
      int* input = E_i + (i - 1);
      coloca_fa(queue, input); 
      printf(
         "total de itens: %lu\tcapacidade: %lu\n", 
         tamanho_fa(queue), capacidade(queue)
      );
   }
   print_fila_fa(queue, println);

   for (size_t i = 14; i >= 1; i--) {
      int* removido = retira_fa(queue); 
      printf(
         "remoção: '%4d' \ttotal de itens: %lu\tcapacidade: %lu\n", 
         *removido, tamanho_fa(queue), capacidade(queue)
      );
      if (i % 3 == 0)
         print_fila_fa(queue, println);
   }

   assert(destroi_fa(queue));
}


int main(int total, char* argumentos[], char* variaveis[]) {
   executa_testes_a(
      true, 3, 
			testando_valores_genericos, true,
         visualizando_escolhimento_e_expansao, true,
         encolhimento_automatico, true
   );
   return EXIT_SUCCESS;
}

#endif
