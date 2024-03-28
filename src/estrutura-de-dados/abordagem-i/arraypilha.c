

/* Implementação de uma pilha usando array ao invés de lista ligada, que também
 * haverá uma implementação más não aqui.
 * Este projeto é importante, pois é colocado em um somente arquivo, sem extensão
 * de qualquer outras dependências.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/* Definição simples do tipo de dado, e algums métodos importantes subjacentes 
 * a ele. Se for necessário alterar o tipo de dado, é está parte somente que 
 * é recomendado mudar, tanto seu apelido, como alguns métodos, depedendo de
 * como o dado está sendo projejado. */
typedef unsigned char dado_t;

// converte ele numa string para impressão abaixo.
char* dado_to_str(dado_t x) {
   char* resultado = malloc(2 * sizeof(char));
   resultado[0] = x;
   resultado[1] = '\0';
   return resultado;
}

// base bem simples da pilha.
typedef struct {
   // capacidade inicial.
   size_t capacidade;
   // alocação dos dados.
   dado_t* array;
   // contagem de dados líquidos.
   size_t quantidade;
} pilha_t;

pilha_t* cria_ps(size_t capacidade) {
   // só aceita valores maiores ou iguais a dez.
   if (capacidade < 10)
      capacidade = 10;

   pilha_t* stack = malloc(sizeof(pilha_t));

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

bool destroi_ps(pilha_t* s) {
   if (s != NULL) {
      free(s->array);
      free(s);
      #ifdef _DEBUG
      puts("'pilha_t' destruída com sucesso.");
      #endif
      return true;
   }
   #ifdef _DEBUG
   puts("'pilha_t' igual à 'null', portanto não destruída!");
   #endif
   return false;
}

/* função de amortização, quando a pilha está crescendo e chega na capacidade 
 * inicial, é necessário aumentar a array que carrega os dados, e é exatamente
 * isso que está função faz. Neste caso aqui, aumenta ela em 50% da capacidade
 * atual. */
void amortizacao(pilha_t* st) {
   if (st == NULL) {
      #ifdef _DEBUG
      puts("não foi possível realizar tal alteração pois a 'pilha' é inválida!");
      #endif
      perror("'pilha_t' inválida!");
      abort();
   }
   size_t nova_capacidade = (double)st->capacidade * 1.50;
   st->array = realloc(st->array, nova_capacidade);
   st->capacidade = nova_capacidade;
}

bool empilha_ps(pilha_t* st, dado_t a) {
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

size_t tamanho_ps(pilha_t* st)
   { return st->quantidade; }

bool vazia_ps(pilha_t* st) 
   { return tamanho_ps(st) == 0; }

bool desempilha_ps(pilha_t* st, dado_t* vl) {
   if (st == NULL)
      return false;
   if (vazia_ps(st))
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

dado_t* topo_ps(pilha_t* st) {
   if (st == NULL)
      return NULL;
   if (vazia_ps(st))
      return NULL;

   size_t indice = st->quantidade - 1;
   return &st->array[indice];
}

void visualiza_ps(pilha_t* st) {
   size_t posicao = (st->quantidade - 1);
   printf("pilha: [");

   if (vazia_ps(st)) {
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
typedef pilha_t stack_t;
bool push_ps(stack_t* s, dado_t a)
   { return empilha_ps(s, a); }

bool pop_ps(stack_t* s, dado_t* a)
   { return desempilha_ps(s, a); }

dado_t* top_ps(stack_t* s) 
   { return topo_ps(s); }

bool empty_ps(stack_t* s)
   { return vazia_ps(s); }

size_t len_ps(stack_t* s)
   { return tamanho_ps(s); }


#if defined(_UT_PILHA_ESTATICA)

#include <assert.h>

void simples_testes_de_algumas_operacoes() {
   pilha_t* stack = cria_ps(20);

   push_ps(stack, 'M');
   push_ps(stack, 'a');
   push_ps(stack, 'p');
   assert (len_ps(stack) == 3);

   push_ps(stack, 'Z');
   push_ps(stack, 'e');
   assert (len_ps(stack) == 5);
   visualiza_ps(stack);

   dado_t vl;
   assert(pop_ps(stack, &vl));
   printf("valor removido: %s\n", dado_to_str(vl));
   assert(pop_ps(stack, &vl));
   printf("valor removido: %s\n", dado_to_str(vl));
   assert (len_ps(stack) == 3);
   visualiza_ps(stack);

   while (!empty_ps(stack)) { 
      assert(pop_ps(stack, NULL));
      visualiza_ps(stack);
   }
   destroi_ps(stack);
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
   pilha_t* stack = cria_ps(20);

   push_ps(stack, 'M');
   push_ps(stack, 'a');
   push_ps(stack, 'p');
   push_ps(stack, 'Z');
   push_ps(stack, 'e');
   visualiza_ps(stack);
   printf("capacidade: %zu\n", stack->capacidade);
   printf("espaço livre: %zu\n", stack->capacidade - stack->quantidade);

   for (size_t i = 1; i <= 20; i++)
      push_ps(stack, letra_randomica());

   visualiza_ps(stack);
   printf("capacidade: %zu\n", stack->capacidade);
   printf("espaço livre: %zu\n", stack->capacidade - stack->quantidade);
   destroi_ps(stack);
}
#endif

int main(int total, char* args[], char* vars[]) 
{
   simples_testes_de_algumas_operacoes();
   excendendo_seu_limite();
}
#endif
