
/* Estrutura de dados, porém recebe valores genéricos(pointeiros de qualquer
 * tipo). O que importa aqui é o comportamento FIFO. Como possui um dado
 * pointeiro genérico(*void) não tem visualização, como já dito, apenas o 
 * comportamento importa nesta versão.
 */

// ponteiro 'void' aceita qualquer referência passada para ela.
typedef void* generico_t;

#include <stdlib.h>
#define INVALIDA NULL
#define INICIALMENTE 20u

typedef struct fila_de_array {
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
   size_t T = sizeof (struct fila_de_array);
   fila_t* instancia = malloc (T);

   if (instancia != INVALIDA) {
      instancia->capacidade = capacity;
      instancia->quantidade = 0;
      instancia->frente = 0;

      // tenta alocar a array, a parte mais importante.
      instancia->array = malloc (capacity * sizeof(T));
      // se não tiver alocado corretamente, libera 'fila' e retorna 'null'.
      if (instancia == INVALIDA) 
         { free (instancia); return NULL; }
   }
   return instancia;
}

fila_t* cria_fa(void) {
   return cria_com_capacidade_fa(INICIALMENTE);
}

#include <stdbool.h>

bool destroi_fa(fila_t* q) {
   if (q != INVALIDA) {
      // libera array interna e a 'fila', então confirma liberação.
      free (q->array);
      free (q);
      return true;
   }
   // diz que foi um fracasso a liberação.
   return false;
}

#include <stdio.h>
#include <assert.h>

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

static void redimensiona(fila_t* q) {
	// copiando valores da antiga.
	size_t t = q->quantidade;
	size_t C = q->capacidade;
   size_t F = q->frente;

	// não gosto desta coisa de ficar dobrando.
   size_t dado_size = sizeof(generico_t);
   generico_t* nova_array = calloc(2 * C, dado_size);


	for (size_t k = 1, f = F; k <= t; k++) {
		size_t posicao = (f + (k - 1)) % C;
      generico_t referencia = q->array[posicao];
		nova_array[k - 1] = referencia;
	}
   // pegando esta, descartando a outra.
   q->array = nova_array;
   // atualizando nova capacidade.
   q->capacidade = 2 * C;
}

bool coloca_fa(fila_t* q, void* e) {
	if (q == INVALIDA) return false;
   // verifica/e executa uma possível expansão para colocar mais itens.
   if (q->quantidade == q->capacidade) redimensiona(q);

   // novos nomes para facilitar(alinhar) o cálculo.
	size_t f = q->frente;
	size_t t = q->quantidade;
	size_t C = q->capacidade;

	q->array[(f + t) % C] = e;
	q->quantidade += 1;

	return true;
}

generico_t retira_fa(fila_t* q) {
   // só é possível remover filas vázias.
	if (vazia_fa(q)) return NULL;
	
   // obtendo endereço do elemento na 'frente' da 'fila'.
   size_t f = q->frente;
	generico_t removido = q->array[f];

	/* computa índice seguinte. Sim, nada é removido, sobre-escrita(pelo
	 * menos ainda) na remoção, apenas o marcador da 'frente' dela, avança 
	 * uma casa, respeitando sua 'capacidade'. 
	 */
	q->frente = (q->frente + 1) % q->capacidade;
	q->quantidade -= 1;

	return removido;
}

#ifdef _UT_FILA_ARRAY

static void visualiza_char_fa(fila_t* q) {
	printf("fila-array: [");

	if (vazia_fa (q)){
		puts("]");
		return;
	}
   // como é uma fila, irei remover todos 'n' itens e inserir novamente.
   // Tais operações são feitas em tempo constante, por quê não?
   size_t contagem = tamanho_fa (q);

	while (contagem > 0) {
		char* valor = retira_fa (q);
		printf("%c, ", *((char*)valor));
      assert (coloca_fa (q, valor));
		contagem--;
	}
	puts("\b\b]");
}

static void instancia_de_caracteres(void) {
   fila_t* Q = cria_fa();

   visualiza_char_fa (Q);
   assert (vazia_fa (Q));

   char entradas[] = {'m', 'a', 'Z', 'p'};
   for (size_t k = 1; k <= 4; k++)
      coloca_fa (Q, &entradas[k - 1]);
   visualiza_char_fa (Q);
   assert (tamanho_fa (Q) == 4);

   puts ("remoção em série:");
   for (size_t k = 4; k > 0; k--) {
      printf ("removido: %c\n", *((char*)retira_fa(Q)));
      visualiza_char_fa (Q);
   }

   destroi_fa (Q);
}

static void visualiza_int_fa(fila_t* q) {
	printf("fila-array: [");

	if (vazia_fa (q)){ puts("]"); return; }
   // como é uma fila, irei remover todos 'n' itens e inserir novamente.
   // Tais operações são feitas em tempo constante, por quê não?
   size_t contagem = tamanho_fa (q);
   generico_t dado;

	while (contagem > 0) {
      dado = retira_fa (q);
		printf ("%d, ", *((int*)dado));
      assert (coloca_fa (q, dado));
		contagem--;
	}
	puts("\b\b]");
}

static void instancia_de_inteiros_simples(void) {
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

int main(int total, char* argumentos[], char* variaveis[]) {
   instancia_de_caracteres();
   instancia_de_inteiros_simples();
   return EXIT_SUCCESS;
}

#endif
