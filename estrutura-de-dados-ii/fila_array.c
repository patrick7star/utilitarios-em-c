

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
char* dado_to_str(Dado x) {
	// seis algarismos pois pode ir até 66 mil, mais o 'caractére nulo'.
	char* string = malloc(6 * sizeof(char));
	sprintf(string, "%d", x);
	return string;
}

Dado clona_dado(Dado* x) {
	return *x;
}

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
	if (q == NULL) {
		#ifdef _DEBUG_ALOCACAO_E_DESTRUICAO
		puts("não há 'fila' alguma para desalocar(dado inválido).");
		#endif
		return false;
	} else {
		free(q->array);
		free(q);
		q = NULL;
		return true;
	}
}

size_t tamanho_fa(FilaArray* q) {
	if (q != NULL)
		return q->quantidade;
	perror("'fila' passada é um dado inválido.");
	abort();
}

bool vazia_fa(FilaArray* q) 
	{ return tamanho_fa(q) == 0; }

Dado* primeiro(FilaArray* q) {
	if (q == NULL)
		return NULL;
	if (vazia_fa(q))	
		return NULL;
	return &q->array[q->frente];
}

static void redimensiona(FilaArray* q) {
	// não gosto desta coisa de ficar dobrando.
	size_t nova = 2 * q->capacidade;
	Dado* nova_array = calloc(nova, sizeof(Dado));

	// copiando valores da antiga.
	size_t t = q->quantidade;
	size_t c = q->capacidade;

	for (size_t k = 1, f = q->frente; k <= t; k++) {
		size_t posicao = (f + (k - 1)) % c;
		Dado clone = clona_dado(&q->array[posicao]);
		nova_array[k - 1] = clone;
	}
}

bool coloca_fa(FilaArray* q, Dado e) {
	if (q == NULL)
		return false;

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

static void visualiza_fa(FilaArray* q) {
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
		printf("%s, ", dado_to_str(a));
		p = (p + 1) % C;
		t--;
	}
	puts("\b\b]");
}

static void visualiza_interna_fa(FilaArray* q) {
	printf("fila-array(crua): [");

	for (size_t f = 0; f < q->capacidade; f++) {
		Dado a = q->array[f];
		char* dadostr = dado_to_str(a);
		size_t fim = (q->frente + q->quantidade) % q->capacidade - 1;

		if (f == q->frente && !vazia_fa(q))
			printf("|%s|, ", dadostr);
		else if (f == fim && !vazia_fa(q))
			printf("|%s|, ", dadostr);
		else
			printf("%s, ", dadostr);
	}
	puts("\b\b]");
}

// série de testes unitários das estruturas, funções e métodos acimas:
#ifdef _UT_FILA_ARRAY

#include <assert.h>

void uma_instancia_vazia() {
	fila_array_t* queue = cria_fa();
	visualiza_fa(queue);
	assert (vazia_fa(queue));
	assert (destroi_fa(queue));
}

void como_fica_apos_multiplas_operacoes() {
	fila_array_t* queue = cria_capacidade_fa(10);
	assert (vazia_fa(queue));

	coloca_fa(queue, 15);
	assert (!vazia_fa(queue));

	coloca_fa(queue, 3);
	coloca_fa(queue, 7);
	coloca_fa(queue, 1);
	assert ( tamanho_fa(queue) == 4 );

	visualiza_fa(queue);
	visualiza_interna_fa(queue);

	coloca_fa(queue, 17);
	assert ( tamanho_fa(queue) == 5 );
	visualiza_fa(queue);
	visualiza_interna_fa(queue);

	free(retira_fa(queue));
	free(retira_fa(queue));
	assert ( tamanho_fa(queue) == 3 );
	visualiza_fa(queue);
	visualiza_interna_fa(queue);

	assert (destroi_fa(queue));
	assert (!destroi_fa(queue));
}

int main(int total, char* args[], char* vars[]) 
{
	uma_instancia_vazia();
	como_fica_apos_multiplas_operacoes();
}
#endif

