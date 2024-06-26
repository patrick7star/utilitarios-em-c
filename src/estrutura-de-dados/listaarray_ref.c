

/* Simples implementação genérica da array-list.
 *   Se ficar muito boa se juntará as estruturas de dados tipo(iii) no
 * projeto utilitários, pois é uma array dinâmica que serve incrivelmente
 * para qualquer projeto que precisa armazenar um grande número de objetos,
 * seja eles qualquer que tipo.
 */

// Apenas bibliotecas padrão do C:
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/* Sem específicar a capacidade, será o valor abaixo que será 
 * automaticamente redimensionado para o arranjo de dados. */
#define INICIAL 200
#define MINIMO_TAMANHO 3
/* Modo mais limpo de dizer que um tipo de dado é 'inválido'. */
#define INVALIDA NULL

// um jeito mais limpo e entendível de como chamar tal tipo.
typedef void* generico_t;

typedef struct array_lista {
   // quantia inicial da array e total de itens nela guardados.
   size_t quantia;
   size_t capacidade;

   // array de containers, este, armazena o endereço do dado e seu tamanho.
   generico_t* array;

} *ArrayLista, vetor_t;

ArrayLista cria_com_capacidade_al(size_t n) {
/* Aloca a lista, onde a 'array interna' tem uma 'capacidade inicial', ou 
 * seja, o tamanho sem amortização do total de itens é definido pelo que chama tal. Claro que tal valor inicial(a capacidade), tem que ser maior 
 * que um mínimo. */
   const size_t SIZE_LISTA = sizeof (struct array_lista);
   const size_t SIZE_GENERICO = sizeof(generico_t);
   ArrayLista lista = malloc (SIZE_LISTA);
   generico_t* containers = malloc (n * SIZE_GENERICO);

   // redefine para o mínimo necessário se não cumprir tal requisito.
   if (n < MINIMO_TAMANHO) { n = MINIMO_TAMANHO; }

   if (containers == INVALIDA) {
   // pula alocações que não foram realizadas.
      puts("[erro na alocação]liberando o que foi alocado...");
      free(lista);
      return NULL;
   }

   if (lista != INVALIDA) {
   /* referenciando a array alocada, copiando a capacidade passada como 
    * argumento, e definindo como "sem itens" a lista inicialmente. */
      lista->array = containers;
      lista->capacidade = n;
      lista->quantia = 0;


      #ifdef _ALOCACAO_E_DESALOCACAO   
      puts("a instância foi criada com sucesso.");
      #endif
   }
   return lista;
}

ArrayLista cria_al() 
/* Cria tal array com um tamanho definido automaticamente, não é gigante
 * tal valor, porém ainda pode ser relevante para programa muito 
 * compactos. */
   { return cria_com_capacidade_al(INICIAL); }

bool destroi_al(ArrayLista l) {
/* Verfica se a 'lista' passada é válida, se for o caso continua, então
 * verifica se sua 'array interna' é válida, se támbém for o caso, libera
 * ela primeiramente, então libera a lista. O resultado do procedimento
 * descrito é um valor lógico verdadeiro para o processo sem interrupção,
 * e o valor 'falso' para se houve alguma disrrupção no processo, mesmo
 * que tenha-se liberado algo. */
   if (l == INVALIDA) 
   // Não há uma lista para liberar.
      return false;
   else if (l->array == INVALIDA) {
   // liberação não se deu completamente, porém, ainda manuseável.
      free(l);
      return false;
   }

   free (l);
   return true;
}

/*
bool redimensiona(ArrayLista l) {
   size_t nova_capacidade = 2 * l->capacidade;
   size_t Q = l->quantia;
   size_t tamanho = sizeof (generico_t);
   generico_t* nova_array = malloc(2 * nova_capacidade * tamanho);
   size_t C = l->capacidade;
   generico_t* ptr_array_antiga = l->array;

   // só realiza a rotina abaixo se a 'quantia' atingiu a 'capacidade'.
   if (Q < C) return false;

   // se livrando a referência a antiga ...
   l->array = NULL;
   // copiando ...
   for (size_t k = 1; k <= C; k++)
      nova_array[k - 1] = ptr_array_antiga[k - 1];

   // liberando antiga...
   #ifdef _REDIMENSIONA
   puts ("a liberação da antiga array foi sucedida.");
   #endif
   free (ptr_array_antiga);

   // então atribuindo nova array a instância ...
   l->array = nova_array;
   // atualizando nova capacidade...
   l->capacidade = nova_capacidade;
   // confirma como feita o redimensionamento.
   return true;
}
*/

/* Basicamente a solução foi copiada da implementação da 'fila-array' que 
 * é: faz uma função que redimensiona a 'array interna' da lista para o
 * tamanho desejado, em outras funções específicas, chamadas em ambos
 * métodos que altera a estrutura da estrutura, chamam tais funções, que 
 * baseado em premisas lógicas determinadas, redimensionam para mais ou 
 * para menos tal capacidade interna, já fazem quase tudo, inclusive
 * atualizar os valores. */
static void redimensiona(ArrayLista L, size_t nC) {
   size_t t = L->quantia;
   generico_t* new_array = malloc(nC * sizeof(generico_t));
   generico_t* old_array = L->array;

   /* Se o total de itens for menos que um quarto da capacidade, então
    * encolhe a 'array interna' à metade. */
   for (size_t k = 0; k < t; k++) {
      /* Percorrendo à partir da 'frente' da 'fila'.*/
      generico_t data = old_array[k]; 
      new_array[k] = data;
   }

   /* livrando-se da antiga, assim deixando um 'dangling pointer' na 
    * estrutura, então retornando a nova. Deste modo, é esperado que 
    * o campo liberado no escopo maior que este, pegue a nova array de
    * dados retornada. */
   free(old_array);
   /* Realiza um 'swaping' array internas e, atualiza a capacidade. Também 
    * a 'frente' voltou a ser a posição zero. */
   L->capacidade = nC;
   L->array = new_array;
}

static bool encolhe_lista_automaticamente(ArrayLista l) {
   size_t t = l->quantia;
   size_t C = l->capacidade;
   bool tudo_positivo_para_reduzir = { 
      (t >= 2) && (t <= (C / 4))  
      /* Para não reduzir a modalidade de alocação inicial, se estiver 
       * nele tal capacidade não reduz, por mais que os outros estejam
       * certo para tal processamento. */
      && C != INICIAL
   };

   if (tudo_positivo_para_reduzir) {
      /* libera a antiga da memória e, atribui a nova, também atualiza a
       * nova capacidade da 'array interna'. */
      redimensiona(l, C / 2);

      #ifdef _REDIMENSIONA
      puts("houve redução!");
      #endif
      return true;
   }

   #ifdef _REDIMENSIONA
   puts("não houve redução.");
   #endif
   return false;
}

static bool estende_lista_automaticamente(ArrayLista l) {
   size_t t = l->quantia;
   size_t C = l->capacidade;
   bool preencheu_todo_espaco = (t == C);

   if (preencheu_todo_espaco) {
      /* libera a antiga da memória e, atribui a nova, também atualiza a
       * nova capacidade da 'array interna'. */
      redimensiona(l, 2 * C);

      #ifdef _REDIMENSIONA
      puts("houve redução!");
      #endif
      return true;
   }

   #ifdef _REDIMENSIONA
   puts("não houve redução.");
   #endif
   return false;
}

bool insere_al(ArrayLista l, generico_t dado) {
/* Verifica se antes é preciso redimensionar a array interna para caber
 * um novo item inserido. */
   // redimensiona(l);
   estende_lista_automaticamente(l);
   // achando o local para armazenar, e coloca lá.
   size_t indice = l->quantia;
   l->array[indice] = dado;
   // contabiliza item adicionado.
   l->quantia += 1;
   return true;
}

bool vazia_al(ArrayLista l) 
/* Retorna 'verdade' se a 'lista' está vázia, 'falso' caso o contŕario. */
   { return l->quantia == 0; }

size_t tamanho_al(ArrayLista l) 
// retorna valor interno que contabiliza o líquido de inserções.
   { return l->quantia; }


generico_t remove_al(ArrayLista l) {
/* O processo de remoção leva-se apenas como mover o 'cursor' que indica
 * a próxima posição para inserção, no caso, para trás. Se não houver 
 * qualquer item, o retorno será 'inválido'. */
   if (vazia_al(l)) return NULL;


   size_t t = l->quantia;
   generico_t remocao = l->array[t - 1];
   l->quantia -= 1;

   /* Se após tal remoção, houver muito espaço vázio sem necessidade, aqui
    * será encolhido, antes devolver o 'dado' removido. */
   encolhe_lista_automaticamente(l);
   return remocao;
}

generico_t indexa_al(ArrayLista l, size_t indice) {
/* Retorna item na posição 'indice' da array, se o valor de índice for
 * inválido retorna 'null'. Claro que é apenas possível indexar um valor
 * dentro da capacidade interna(digo no total de itens) da estrutura. Um 
 * índice fora desta faixa, apenas retorna um objeto 'inválido'. */
   size_t Q = l->quantia;

   // só indexa valores válidos, de zero ao total de inserções menos um.
   if (indice > Q) { return NULL; }

   return l->array[indice];
}

generico_t remove_indice_al (ArrayLista l, size_t p) {
/* Retorna item na posição 'indice' da array, se o valor de índice for
 * inválido retorna 'null'. Como é uma remoção no meio da 'array', então 
 * será preciso mover todos 'itens' à frente da posição do anulado, e copia
 * todas seus endereços uma posição à esquerda, para tampar o buraco. */
   size_t Q = l->quantia;
   generico_t remocao = NULL;

   // Não pode remover uma posição fora do limite, o returno é 'inválido'.
   if (p >= Q) { return INVALIDA; }

   // Salva a referência da posição a ser removida(sobreposta na verdade).
   remocao = l->array[p];
   // contabiliza 'item' removido.
   l->quantia -= 1;

   /* Copiando todos itens à frente da posição que será removida,
    * portanto à esquerda dela. */
   for (size_t j = p; j < (Q - 1); j++) 
      { l->array[j] = l->array[j + 1]; }

   return remocao;
}

size_t vacuo_al(ArrayLista l) 
/* Retorna o vázio entre a capacidade total da estrutura, e o total de
 * elementos que ele possui. */
   { return l->capacidade - l->quantia; }

/* Testando todos métodos, funções, e dados abstratos acima. Deixando
 * bem referênciado esta parte, pois fica fácil descartar -- além de 
 * ser necessário se os tipos forem trocados, do contrário o programa
 * não compila; se copiado para vários projetos. Caso também tal trecho,
 * futuramente, for colocada num subdiretório, e os tipos serem trocados
 * apenas comentar tal declaração pré-processada para não incluir o que
 * pode conflitar.
 */
#if defined(_UT_ARRAY_LISTA)
#include "teste.h"
#include <assert.h>

void visualizacao_array_list_char(vetor_t* l) {
   size_t t = tamanho_al(l);
   char* caractere;

   if (vazia_al(l)) {
      puts("array-lista: []");
      return;
   }

   if (l->quantia > 7)
      // só fica difícil de contar valores maiores que sete.
      printf ("array-lista(%lu): [", l->quantia);
   else
      printf ("array-lista: [");

   for (size_t i = t; i > 0; i--) {
      caractere = l->array[t - i];
      printf ("%c, ", *caractere);
   }
   puts ("\b\b]");
}
void visualizacao_array_list_int(vetor_t* l) {
   size_t t = tamanho_al(l);
   int* caractere;

   if (vazia_al(l)) {
      puts("array-lista: []");
      return;
   }

   if (l->quantia > 7)
      // só fica difícil de contar valores maiores que sete.
      printf ("array-lista(%lu): [", l->quantia);
   else
      printf ("array-lista: [");

   for (size_t i = t; i > 0; i--) {
      caractere = l->array[t - i];
      printf ("%d, ", *caractere);
   }
   puts ("\b\b]");
}

void demonstracao_com_caracteres() {
   vetor_t* lista = cria_com_capacidade_al(15);
   char m = 'x', n = 'A', p = 'J';
   puts("alocação ocorreu1!!!");

   insere_al(lista, &p);
   insere_al(lista, &n);
   insere_al(lista, &m);
   visualizacao_array_list_char(lista);

   char* removido = remove_al(lista);
   printf ("removido: '%c'\n", *removido);
   removido = remove_al(lista);
   printf ("removido: '%c'\n", *removido);

   assert (tamanho_al(lista) == 1);
   assert (*((char*)remove_al(lista)) == 'J');
   assert (vazia_al(lista));

   destroi_al(lista);
}

void demonstracao_com_inteiros() {
   ArrayLista outra_lista = cria_al();
   int entradas[] = {39, 73, 15, 101};

   for (size_t k = 1; k <= 4; k++)
      insere_al (outra_lista, &entradas[k - 1]);

   while (!vazia_al(outra_lista)) {
      printf ("removido: %d\n", *((int*)remove_al(outra_lista)));
      visualizacao_array_list_int(outra_lista);
   }
      
   destroi_al(outra_lista);
}

void demonstracao_com_strings() {
   ArrayLista lista = cria_al();
   char* entradas[] = { "cerca", "porta", "cadeado", "mala"};

   for (size_t k = 1; k <= 4; k++)
      insere_al (lista, entradas[k - 1]);

   while (!vazia_al(lista)) {
      char* string = remove_al(lista);
      printf ("removido: '%s'\n", string);
   }
   destroi_al(lista);
}

void verificado_o_que_foi_coletado(int total, char* argumentos[]) {
   ArrayLista lista = cria_al();
   char** entradas = argumentos;

   for (size_t k = 1; k <= total; k++)
      insere_al (lista, entradas[k - 1]);

   size_t inicial = tamanho_al(lista);
   for (size_t k = 1; k <= 10; k++)
      printf (
         "removido ==> %s(%lu)\n", 
         (char*)remove_al(lista),
         tamanho_al (lista)
      );
   assert (inicial - 10 == tamanho_al(lista));
   puts ("experimentando indexação ...");
   for (size_t k = 1; k <= tamanho_al(lista) / 2; k++)
      printf ("%lu) %s\n", k, (char*)indexa_al(lista, k));
   destroi_al(lista);
}

void visualiza_array_list_string(vetor_t* l) {
   size_t t = tamanho_al(l);
   char* string;

   if (vazia_al(l)) {
      puts("array-lista: []");
      return;
   }

   // só fica difícil de contar valores maiores que sete.
   printf ("array-lista(%lu): [", l->quantia);

   for (size_t i = 1; i <= t; i++) {
      string = l->array[i - 1];
      printf ("%s, ", string);
   }
   puts ("\b\b]");
}

void remocao_em_pontos_criticos (void) {
   ArrayLista lista = cria_al();
   char* entradas[] = { "cerca", "porta", "cadeado", "mala", "bolsa"};

   for (size_t k = 1; k <= 5; k++)
      insere_al (lista, entradas[k - 1]);
   visualiza_array_list_string (lista);

   puts ("removendo o terceiro(cadeado) ...");
   remove_indice_al (lista, 2);
   visualiza_array_list_string (lista);

   puts ("removendo o primeiro(cerca) ...");
   remove_indice_al (lista, 0);
   visualiza_array_list_string (lista);

   destroi_al(lista);
}

#include "dados_testes.h"

void redimensionamento_automatico_da_capacidade(void) {
   vetor_t* L = cria_com_capacidade_al(4);

   puts("observação da expansão da capacidade ...");
   for (size_t k = 1; k <= 5; k++) {
      size_t t = tamanho_al(L);
      // coputando a capacidade.
      size_t C = vacuo_al(L) + t;
      printf("\tcapacidade: %lu\ttotal de itens: %lu\n", C, t);

      char* input = (char*)objetos[k - 1];
      insere_al (L, input);
   }
   putchar('\t'); visualiza_array_list_string(L);

   for (size_t p = 1; p <= 5; p++) {
      size_t t = tamanho_al(L);
      // coputando a capacidade.
      size_t C = vacuo_al(L) + t;
      printf("\tcapacidade: %lu\ttotal de itens: %lu\n", C, t);

      char* input = (char*)frutas[p - 1];
      insere_al (L, input);
   }
   putchar('\t'); visualiza_array_list_string(L);

   puts("\nverificando o encolhimento dela...");
   for (size_t i = 10; i > 0; i--) {
      size_t t = tamanho_al(L);
      // coputando a capacidade.
      size_t C = vacuo_al(L) + t;
      printf("\tcapacidade: %lu\ttotal de itens: %lu\n", C, t);
      if (t % 3 == 0)
         { putchar('\t'); visualiza_array_list_string(L); }
      assert (remove_al (L) != INVALIDA);
   }

   putchar('\t'); visualiza_array_list_string(L);
   destroi_al(L);
}

int main(int total, char* argumentos[], char* variaveis[]) {
   executa_testes (
      5, demonstracao_com_inteiros, true,
         demonstracao_com_caracteres, true,
         demonstracao_com_strings, true,
         remocao_em_pontos_criticos, true,
         redimensionamento_automatico_da_capacidade, true
   );
   // verificado_o_que_foi_coletado, true

   return EXIT_SUCCESS;
}
#endif
