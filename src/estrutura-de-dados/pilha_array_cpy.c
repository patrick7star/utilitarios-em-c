

/* Implementação de uma pilha usando array ao invés de lista ligada, 
 * que também haverá uma implementação más não aqui. Este projeto é 
 * importante, pois é colocado em um somente arquivo, sem extensão
 * de qualquer outras dependências.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/* Definição simples do tipo de dado, e algums métodos importantes 
 * subjacentes a ele. Se for necessário alterar o tipo de dado, é está 
 * parte somente que é recomendado mudar, tanto seu apelido, como alguns 
 * métodos, depedendo de como o dado está sendo projejado. Ele não tem apen  * um tipo de nome, apesar de ser trabalhado apenas com um por toda
 * implementação. */
typedef unsigned char dado_t, DADO, Dado;

// converte ele numa string para impressão abaixo.
char* dado_to_str(dado_t x) {
   char* resultado = malloc(2 * sizeof(char));
   resultado[0] = x;
   resultado[1] = '\0';
   return resultado;
}

// base bem simples da pilha, e todos seus possíveis nomes:
typedef struct {
   // capacidade inicial.
   size_t capacidade;
   // alocação dos dados.
   dado_t* array;
   // contagem de dados líquidos.
   size_t quantidade;
} pilha_array_t, PILHA_ARRAY, PilhaArray;

pilha_array_t* cria_pa(size_t capacidade) {
   // só aceita valores maiores ou iguais a dez.
   if (capacidade < 10)
      capacidade = 10;

   pilha_array_t* stack = malloc(sizeof(pilha_array_t));

   if (stack == NULL) {
      perror("erro ao alocar uma 'pilha estática'!");
      abort();
   }
   stack->capacidade = capacidade;
   stack->quantidade = 0;
   stack->array = calloc(capacidade, sizeof(dado_t));

   if (stack->array == NULL) {
      perror("erro ao alocar a array da 'pilha'!");
      abort();
   }

   return stack;
}

bool destroi_pa(pilha_array_t* s) {
   if (s != NULL) {
      free(s->array);
      free(s);
      #ifdef _DEBUG
      puts("'pilha_array_t' destruída com sucesso.");
      #endif
      return true;
   }
   #ifdef _DEBUG
   puts("'pilha_array_t' igual à 'null', portanto não destruída!");
   #endif
   return false;
}

/* função de amortização, quando a pilha está crescendo e chega na 
 * capacidade inicial, é necessário aumentar a array que carrega os 
 * dados, e é exatamente isso que está função faz. Neste caso aqui, 
 * aumenta ela em 50% da capacidade atual. */
void amortizacao(pilha_array_t* st) {
   if (st == NULL) {
      #ifdef _DEBUG
      puts("não foi possível realizar tal alteração pois a 'pilha' é inválida!");
      #endif
      perror("'pilha_array_t' inválida!");
      abort();
   }
   size_t nova_capacidade = (double)st->capacidade * 1.50;
   st->array = realloc(st->array, nova_capacidade);
   st->capacidade = nova_capacidade;
}

bool empilha_pa(pilha_array_t* st, dado_t a) {
   if (st == NULL)
      return false;
   // se alcançou a capacidade máxima, então redimensinoar.
   if (st->capacidade == st->quantidade)
      amortizacao(st);

   size_t indice = st->quantidade;
   st->array[indice] = a;
   st->quantidade += 1;
   return true;
}

size_t tamanho_pa(pilha_array_t* st)
   { return st->quantidade; }

bool vazia_pa(pilha_array_t* st) 
   { return tamanho_pa(st) == 0; }

bool desempilha_pa(pilha_array_t* st, dado_t* vl) {
   if (st == NULL)
      return false;
   if (vazia_pa(st))
      return false;

   size_t indice = st->quantidade - 1;
   if (vl != NULL)
      *vl = st->array[indice];
   else {
      #if defined(_DEBUG)
      puts("não obter dado removido!");
      #endif
   }
   st->quantidade -= 1; 
   return true;
}

dado_t* topo_pa(pilha_array_t* st) {
   if (st == NULL)
      return NULL;
   if (vazia_pa(st))
      return NULL;

   size_t indice = st->quantidade - 1;
   return &st->array[indice];
}

void visualiza_pa(pilha_array_t* st) {
   size_t posicao = (st->quantidade - 1);
   printf("pilha: [");

   if (vazia_pa(st)) {
      puts("]");
      return ;
   }

   while (posicao > 0) {
      dado_t e = st->array[posicao];
      printf("%s, ", dado_to_str(e));
      posicao--;
   }
   // imprimindo último idem.
   dado_t e = st->array[posicao];
   printf("%s, ", dado_to_str(e));

   puts("\b\b]");
}

/* nomes clássicos e interface em inglês também. */
typedef pilha_array_t stack_t;
bool push_pa(stack_t* s, dado_t a)
   { return empilha_pa(s, a); }

bool pop_pa(stack_t* s, dado_t* a)
   { return desempilha_pa(s, a); }

dado_t* top_pa(stack_t* s) 
   { return topo_pa(s); }

bool empty_pa(stack_t* s)
   { return vazia_pa(s); }

size_t len_pa(stack_t* s)
   { return tamanho_pa(s); }


#if defined(_UT_PILHA_ESTATICA)

#include <assert.h>

void simples_testes_de_algumas_operacoes() {
   pilha_array_t* stack = cria_pa(20);

   push_pa(stack, 'M');
   push_pa(stack, 'a');
   push_pa(stack, 'p');
   assert (len_pa(stack) == 3);

   push_pa(stack, 'Z');
   push_pa(stack, 'e');
   assert (len_pa(stack) == 5);
   visualiza_pa(stack);

   dado_t vl;
   assert(pop_pa(stack, &vl));
   printf("valor removido: %s\n", dado_to_str(vl));
   assert(pop_pa(stack, &vl));
   printf("valor removido: %s\n", dado_to_str(vl));
   assert (len_pa(stack) == 3);
   visualiza_pa(stack);

   while (!empty_pa(stack)) { 
      assert(pop_pa(stack, NULL));
      visualiza_pa(stack);
   }
   destroi_pa(stack);
}

#ifdef _WIN32
#include <time.h>
#include <windows.h>

dado_t letra_randomica() {
   srand(time(NULL));
   Sleep(5);
   size_t sorteio = rand() % 26;
   return (dado_t)(sorteio + 65);
}

void excendendo_seu_limite() {
   pilha_array_t* stack = cria_pa(20);

   push_pa(stack, 'M');
   push_pa(stack, 'a');
   push_pa(stack, 'p');
   push_pa(stack, 'Z');
   push_pa(stack, 'e');
   visualiza_pa(stack);
   printf("capacidade: %zu\n", stack->capacidade);
   printf("espaço livre: %zu\n", stack->capacidade - stack->quantidade);

   for (size_t i = 1; i <= 20; i++)
      push_pa(stack, letra_randomica());

   visualiza_pa(stack);
   printf("capacidade: %zu\n", stack->capacidade);
   printf("espaço livre: %zu\n", stack->capacidade - stack->quantidade);
   destroi_pa(stack);
}
#endif

#include "teste.h"

int main(int total, char* args[], char* vars[]) 
{
   
	executa_testes(1, simples_testes_de_algumas_operacoes, true);
   #ifdef _WIN32
   excendendo_seu_limite();
   #endif
}
#endif
