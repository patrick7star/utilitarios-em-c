

/*
 *   Estas estruturas não só diferem na implementação da primeira linha de
 * estruturas de dados, achadas no outro diretório(com um valor romano
 * com uma unidade a menos), já que tem tudo o que se precisa neste módulo,
 * sem precisar de qualquer módulo externo, a não ser na parte de testes.
 *
 * NOTA: uma coisa que quero testar é colocar a descrição dos algoritmos
 * em funções e métodos dentro do escopo deles ao invés de em cima da 
 * função para ficar algo mais "limpo".
 */
#include <stdint.h>

// o tipo de dado desta estrutura trabalha é de cópia.
typedef uint16_t dado_t, Dado, DADO;

#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>

// métodos necessários do 'dado' acima:
static char* dado_to_str(Dado* x) {
	// seis algarismos pois pode ir até 66 mil, mais o 'caractére nulo'.
	char* string = malloc(6 * sizeof(char));
	sprintf(string, "%d", *x);
	return string;
}

// clonagem deste tipo de dado.
static Dado clona_dado(Dado* x) { return *x; }

// máximo de caractéres que ele pode ocupar em impressão.
static Dado dado_string_extensao() { return 5; }

// retira em sí o valor do tipo Dado.
static Dado valor_do_dado(Dado* x) { return *x; }

#include <stddef.h>

typedef struct {
	// quantidade de elementos líquidos(módulo entre remoções e inserções).
	size_t quantidade;

	// capacidade da array(seu tamanho inicialmente) e o próprio arranjo.
	dado_t* array;
	size_t capacidade;

	// vagão do primeiro elemento dela.
	size_t frente;
} 
// todos os nomes possíveis para esta estrutura de dado:
fila_array_t, FILA_ARRAY, FilaArray;


FilaArray* cria_capacidade_fa(size_t capacidade) {
	size_t tamanho_bytes = sizeof(FILA_ARRAY);
	fila_array_t* fila = malloc(tamanho_bytes);

	// alocação não pode falhar.
	if (fila != NULL) {
		fila->quantidade = 0;	
		fila->frente = 0;
		fila->capacidade = capacidade;
		fila->array = calloc(capacidade, tamanho_bytes);
		// criação da array também não pode falhar.
		if (fila->array == NULL) {
			#ifdef _DEBUG_ALOCACAO_E_DESTRUICAO
			puts("houve erro ao alocar a array da 'fila'.");
			#endif
			free(fila);
			return NULL;
		}
		#ifdef _DEBUG_ALOCACAO_E_DESTRUICAO
		puts("'fila' alocada com sucesso.");
		#endif
	}
	return fila;
}

FilaArray* cria_fa() {
	/* cria uma arrray com a capacidade padrão. A implementação disso é 
	 * apenas chamar a função acima dando uma constante definidada também
	 * aqui.
	 */
	const uint8_t CAPACIDADE_PADRAO = 30;
	return cria_capacidade_fa(CAPACIDADE_PADRAO);
}

#include <stdbool.h>

bool destroi_fa(FilaArray* q) {
   /* não consegue contornar algo como double-free, então não conte como
    * garantido tal ação. */
	if (q == NULL) {
		#ifdef _DEBUG_ALOCACAO_E_DESTRUICAO
		puts("não há 'fila' alguma para desalocar(dado inválido).");
		#endif
      // confirma não desalocação, já que não foi necessário.
		return false;
	} else if (q->array == NULL) {
      #ifdef _DEBUG_ALOCACAO_E_DESTRUICAO
      puts("[erro em desalocação] não existe uma array para desalocar.");
      #endif
      // liberando estrutura, e confirmando ação como não completa.
      free(q);
      return false;
   }
   #ifdef _DEBUG_ALOCACAO_E_DESTRUICAO
   puts("destruindo array interna, e então a fila.");
   #endif
   free(q->array);
   q->array = NULL;
   free(q);
   q = NULL;
   return true;
}

size_t tamanho_fa(FilaArray* q) {
	if (q != NULL)
		return q->quantidade;
	perror("'fila' passada é um dado inválido.");
	abort();
}

bool vazia_fa(FilaArray* q) 
	{ return tamanho_fa(q) == 0; }

Dado* primeiro_fa(FilaArray* q) {
	if (q == NULL)
		return NULL;
	if (vazia_fa(q))	
		return NULL;
	return &q->array[q->frente];
}

static void redimensiona(FilaArray* q) {
	// copiando valores da antiga.
	size_t t = q->quantidade;
	size_t C = q->capacidade;
   size_t F = q->frente;

   #ifdef _DEBUG_REDIMENSIONA
   printf("a redimensão da array foi acionada.(%lu/%lu)\n", t, C);
   #endif

	// não gosto desta coisa de ficar dobrando.
   size_t dado_size = sizeof(Dado);
	Dado* nova_array = calloc(2 * C, dado_size);


	for (size_t k = 1, f = F; k <= t; k++) {
		size_t posicao = (f + (k - 1)) % C;
		Dado clone = clona_dado(&q->array[posicao]);
		nova_array[k - 1] = clone;
	}
   // pegando esta, descartando a outra.
   q->array = nova_array;
   // atualizando nova capacidade.
   q->capacidade = 2 * C;
}

bool coloca_fa(FilaArray* q, Dado e) {
	if (q == NULL) return false;
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

Dado* retira_fa(FilaArray* q) {
	if (vazia_fa(q))
		return NULL;
	
	Dado* removido = malloc(sizeof(dado_t)); 
	// verifica se a alocação foi bem sucedida.
	if (removido == NULL) {
		#ifdef _DEBUG_INSERCAO_OU_REMOCAO
		puts("erro ao alocar 'dado' para clonagem.");
		#endif
		return NULL;
	}
	// cuida tanto de casos de cópia como de referência.
	*removido = clona_dado(&q->array[q->frente]);

	/* computa índice seguinte. Sim, nada é removido, sobre-escrita(pelo
	 * menos ainda) na remoção, apenas o marcador da 'frente' dela, avança 
	 * uma casa, respeitando sua 'capacidade'. 
	 */
	q->frente = (q->frente + 1) % q->capacidade;
	q->quantidade -= 1;

	return removido;
}

#include <assert.h>
#include <string.h>

char* queue_to_str_fa(FilaArray* q) {
   // total de itens dela:
   size_t T = q->quantidade;
   /* Primeiro, o máximo de caractéres que o Dado exige mais a vírgula 
    * e o espaço, tudo isso multiplicado pelo total de itens na fila. Então,
    * os dois separadores, e por último mais o caractére nulo. */
   size_t max = dado_string_extensao();
   size_t total = (max + 2) * T + 2 + 1;
   char* resultado_fmt = calloc(total, sizeof(char));

   /* Por ser uma estrutura de fila, o modo como farei é usando um artificio
    * apenas válido para este tipo. Vamos remover a quantidade de itens
    * nela inserindo novamente o valor removido no fim, mas antes disso 
    * convertemos o dado para string e ficamos com isso. No final deste 
    * processo o primeiro item é o mesmo que o anterior, assim como o último
    * a ordem que também pegamos as strings de dados convertidos segue a 
    * forma da fila inicialmente, então é só concatena para finalizar o
    * trabalho. Obviamente não é todo bom, e terá que mudar no futuro pois
    * está fila implementada por array também redimensiona(se expande ou 
    * encolhe) de acordo com o seu tamanho e o total líquido de elementos,
    * logo tal processo pode ser não linear. */

   // começo da string.
   strcat(resultado_fmt, "[");

   while (T > 0) {
      Dado* remocao = retira_fa(q);
      char* dadostr = dado_to_str(remocao);
      strcat(resultado_fmt, dadostr);
      strcat(resultado_fmt, ", ");
      // removendo isso depois de concatenado.
      free(dadostr);
      assert (coloca_fa(q, valor_do_dado(remocao)));
      T--;
   }
   // finalizando a string.
   strcat(resultado_fmt, "\b\b]");

   return resultado_fmt;
}

void visualiza_fa(FilaArray* q) {
	printf("fila-array: [");

	if (vazia_fa(q)){
		puts("]");
		return;
	}

	size_t t = q->quantidade;
	size_t p = q->frente;
	size_t C = q->capacidade;

	while (t > 0) {
		Dado a = q->array[p];
		printf("%s, ", dado_to_str(&a));
		p = (p + 1) % C;
		t--;
	}
	puts("\b\b]");
}

// série de testes unitários das estruturas, funções e métodos acimas:
#ifdef _UT_FILA_ARRAY
static void visualiza_interna_fa(FilaArray* q) {
   // atributos da array simplificado.
   size_t C = q->capacidade;
   size_t Q = q->quantidade;
   size_t F = q->frente;

   // impressão do começo dela:
	printf("fila-array-crua(%lu/%lu): [", Q, C);

	for (size_t f = 0; f < C; f++) {
		Dado a = q->array[f];
		char* dadostr = dado_to_str(&a);
      // computando posição de fim da fila na array.
      size_t fim = ((F + Q) % C) - 1;

		if (f == q->frente && !vazia_fa(q))
			printf("|%s|, ", dadostr);
		else if (f == fim && !vazia_fa(q))
			printf("|%s|, ", dadostr);
		else
			printf("%s, ", dadostr);
	}
	puts("\b\b]");
}

void uma_instancia_vazia() {
	fila_array_t* queue = cria_fa();
	visualiza_fa(queue);
	assert (vazia_fa(queue));
	assert (destroi_fa(queue));
}

void como_fica_apos_multiplas_operacoes() {
	fila_array_t* queue = cria_capacidade_fa(10);
   puts("fila vázia no momento.");
	assert (vazia_fa(queue));

   puts("inserindo alguns valores ...");
	coloca_fa(queue, 15);
	assert (!vazia_fa(queue));

	coloca_fa(queue, 3);
	coloca_fa(queue, 7);
	coloca_fa(queue, 1);
	assert ( tamanho_fa(queue) == 4 );

   puts("visualização dela de dois modos:");
	visualiza_fa(queue);
	visualiza_interna_fa(queue);

	coloca_fa(queue, 17);
	assert ( tamanho_fa(queue) == 5 );
	visualiza_fa(queue);
	visualiza_interna_fa(queue);

   puts("removendo itens dela ...");
	free(retira_fa(queue));
	free(retira_fa(queue));
	assert ( tamanho_fa(queue) == 3 );
	visualiza_fa(queue);
	visualiza_interna_fa(queue);

   puts("destruição da fila...");
	assert (destroi_fa(queue));
}

static Dado valor_randomico() { return rand() % 100; }

void verificando_expansao_de_capacidade() {
	fila_array_t* queue = cria_capacidade_fa(9);

	assert (vazia_fa(queue));
	visualiza_interna_fa(queue);

   for (size_t i = 1; i <= 10; i++) {
      Dado X = valor_randomico(); 
      printf("\nvalor sorteado=%d\n", X);
      assert (coloca_fa(queue, X));
      visualiza_interna_fa(queue);
   }
   visualiza_interna_fa(queue);

   puts("\n\nnova leva de inserções... :");
   for (size_t i = 1; i <= 13; i++) {
      Dado X = valor_randomico(); 
      printf("\nvalor sorteado=%d\n", X);
      assert (coloca_fa(queue, X));
      visualiza_interna_fa(queue);
   }
   visualiza_interna_fa(queue);

	assert (destroi_fa(queue));
}

void removendo_todos_itens() {
	fila_array_t* queue = cria_capacidade_fa(9);

	visualiza_interna_fa(queue);

   for (size_t i = 1; i <= 14; i++) {
      Dado X = valor_randomico(); 
      assert (coloca_fa(queue, X));
   }

   while (tamanho_fa(queue) > 0) {
      Dado* remocao = retira_fa(queue);
      visualiza_fa(queue);
      visualiza_interna_fa(queue);
      printf("removido: '%u'\n\n", *remocao);
   }

   puts("preenchendo novamente, a mesma fila ...");
   for (size_t i = 1; i <= 14; i++) {
      Dado X = valor_randomico(); 
      assert (coloca_fa(queue, X));
      visualiza_interna_fa(queue);
      visualiza_fa(queue);
   }

   puts("\nnovas remoções, mais não todas ... ...");
   while (tamanho_fa(queue) > 5) {
      visualiza_fa(queue);
      visualiza_interna_fa(queue);
      Dado* remocao = retira_fa(queue);
      printf("removido: '%u'\n\n", *remocao);
   }

   puts("terminando de remover o restante ... ...");
   while (tamanho_fa(queue) > 0) {
      visualiza_interna_fa(queue);
      visualiza_fa(queue);
      Dado* remocao = retira_fa(queue);
      free(remocao);
   }
   puts("resultado final, as duas visualizações:");
   visualiza_interna_fa(queue);
   visualiza_fa(queue);

   destroi_fa(queue);
}

void transformacao_da_fila_em_string() {
   puts("transformação da fila em string:");
	fila_array_t* queue = cria_capacidade_fa(9);

   for (size_t i = 1; i <= 14; i++) {
      Dado X = valor_randomico(); 
      assert (coloca_fa(queue, X));
      char *string = queue_to_str_fa(queue);
      puts(string);
      free(string);
   }
   destroi_fa(queue);
}

int main(int total, char* args[], char* vars[]) 
{
   // adicionando entropia na função randômica.
   srand((size_t)&main);

	uma_instancia_vazia();
	como_fica_apos_multiplas_operacoes();
   verificando_expansao_de_capacidade();
   removendo_todos_itens();
   transformacao_da_fila_em_string();
}
#endif

