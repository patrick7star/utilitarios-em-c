// Definições das estruturas, dados, métodos, e funções abaixo:
#include "listaarray_ref.h"
// Apenas bibliotecas padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* Sem específicar a capacidade, será o valor abaixo que será
 * automaticamente redimensionado para o arranjo de dados. */
#define INICIAL 200
#define MINIMO_TAMANHO 3
/* Modo mais limpo de dizer que um tipo de dado é 'inválido'. */
#define INVALIDA NULL

struct array_lista {
   // quantia inicial da array e total de itens nela guardados.
   size_t quantia;
   size_t capacidade;

   // array de containers, este, armazena o endereço do dado e seu tamanho.
   generico_t* array;
};

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
   }
   return lista;
}

ArrayLista cria_al(void)
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

static void redimensiona(ArrayLista L, size_t nC) {
/* Basicamente a solução foi copiada da implementação da 'fila-array' que
 * é: faz uma função que redimensiona a 'array interna' da lista para o
 * tamanho desejado, em outras funções específicas, chamadas em ambos
 * métodos que altera a estrutura da estrutura, chamam tais funções, que
 * baseado em premisas lógicas determinadas, redimensionam para mais ou
 * para menos tal capacidade interna, já fazem quase tudo, inclusive
 * atualizar os valores. */
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
// Retorna valor interno que contabiliza o líquido de inserções.
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

char* to_string_al(ArrayLista L, ToString fn) {
/* Se você fornercer a função que gera uma string para o tipo de dado, tal
 * função pode construir uma  string da estrutura em sí. */
   size_t t = tamanho_al(L);
   size_t size_vetor;
   char* resultado_fmt;
   size_t max_length = 10;
   const char* rotulo = "array-lista";

   if (t >= 1000) {
      /* Por ser um processamento pesado, tanto na criação da string, com
       * a medição do comprimento delas inicialmente, colocarei um limite
       * de mil itens. Como impressões gerais não chegam a isso, pois tal
       * código é geralmente usado para debbuging, é um bom limite. Até
       * não achar um algoritmo mais leve para fazer isso, fica aí tal,
       * sendo atualizado(dobrado) toda vez que o programa "quebrar" por
       * causa de tal limitação. */
      perror("não formata dado com tantos itens(mais de 1 mil).");
      abort();
   }

   // Computando o a string com maior comprimento.
   for (size_t i = t; i > 0; i--) {
      generico_t dado = L->array[t - i];
      // Olhando as strings, e registrando o tamanho da maior entre elas.
      char* dado_str = fn(dado);
      size_t length = strlen(dado_str);

      if (length > max_length)
         { max_length = length; }
      free(dado_str);
   }

   /* Calculando necessário de caractéres, mais uns extras para não quebrar,
    * e alocando a string com tal valor. */
   size_vetor = strlen(rotulo) + (t + 1) * (max_length + 2) * sizeof(char);
   resultado_fmt = (char*)malloc (size_vetor);

   #ifdef _TO_STRING
   printf(
      "maior comprimento: %lu\ntotal de caractéres: %lu\n",
      max_length, size_vetor
   );
   #endif

   // Se está dentro do limite, então definir a primeira parte.
   strcpy(resultado_fmt, rotulo);
   strcat(resultado_fmt, ": [");

   if (vazia_al(L))
      { strcat(resultado_fmt, "]"); }
   else {
      for (size_t i = t; i > 0; i--) {
         generico_t dado = L->array[t - i];
         char* dado_str = fn(dado);
         // Concatena o dado...
         strcat(resultado_fmt, dado_str);
         // Concatena o separador...
         strcat(resultado_fmt, ", ");
         // Liberando a string gerada, depois de copiada.
         free(dado_str);
      }
      // Retira espaço e vírgular(separador) do excesso.
      strcat(resultado_fmt, "\b\b]\0");
   }
   return resultado_fmt;
}

ArrayLista cria_de_al(int Q, ...) {
/* Função que aloca uma 'lista' da quantidade definida manualmente de
 * pointeiros. Ela serve mais de auxiliar para o macro abaixo. */
   va_list dados_seq;
   ArrayLista lista = cria_com_capacidade_al(Q);

   va_start(dados_seq, Q);
   for (int i = 1; i <= Q; i++) {
      generico_t datum = va_arg(dados_seq, generico_t);
      insere_al(lista, datum);
   }
   va_end(dados_seq);

   return lista;
}

void destroi_todas_al(int qtd, ...) {
/* Dada todas listas instânciadas, esta função desaloca todas, em sequência.
 * Se faltou com uma, o programa é interrompido. */
   va_list LISTAS;

   va_start(LISTAS, qtd);
   for (int i = 1; i <= qtd; i++) {
      generico_t L = va_arg(LISTAS, ArrayLista);

      if (destroi_al(L)) {
         perror("[erro]Não foi possível desalocar esta!");
         abort();
      };
   }
   va_end(LISTAS);
}

/* === === === === === === === === === === === === === === === === === === === === === === =
 *                      Iterador e seus métodos
 *
 * Observação: todo código foi copiada da implementação de pilha, e apenas
 * se trocou os sufixos e alguns rótulos, além de alguns toques leve na
 * implementação, campos que não se usam foram desativados, e "locomoção"
 * na "array interna" é diferente aqui. Comentários foram retirados,
 * justamente, para nota-se que aqui não é um código original. Busque o
 * original para qualquer outro problema não sintático.
 * === === === === === === === === === === === === === === === === === === === === === == */
 #include "lista-array/iteration.c"


bool destroi_interno_al(Vetor l, Drop g) {
/* Desalocador da lista, mas que também desaloca memória interna alocada
 * inserida nela, claro, se o desconstrutor 'g' do tipo homogênio interno
 * for cedido também. */
   if (l == NULL || g == NULL)
   // Caso algum dos argumentos não for válido, então não prossegue na
   // liberação, e somente retorna a negação da operação.
      return false;

   IterAL iteracao = cria_iter_al(l);

   while (consumido_iter_al(&iteracao))
       g(next_al(&iteracao).item);

   destroi_al(l);
   return true;
}

void imprime_lista_al(Vetor l, ToString f) {
/* Formatação da lista apenas, passado a função que converte o tipo genérico
 * que ela retém inicialmente, numa string. Sim, existe um limite de itens
 * que está função iprime, você tem que ativar ela se quer a impressão de
 * tudo, caso contrário um erro será emitido. */
   if (l == NULL || f == NULL)
      return;
   else if (tamanho_al(l) > (int)1.0e4) {
      perror("não é possível imprimir com está quantia de itens.");
      abort();
   }

   IterAL iteracao = cria_iter_al(l);
   char* fmt;
   IterOutputAL out;

   printf("ListaArray(%d) [", (int)tamanho_al(l));

   if (vazia_al(l))
      { puts("]"); return; }

   while (!consumido_iter_al(&iteracao)) {
      out = next_al(&iteracao);
      fmt = f(out.item);
      printf("%s, ", fmt);
      free(fmt);
   }
   printf("\b\b]\n");
}

/* === === === === === === === === === === === === === === === === === === === === === === =
 *                         Tradução pra Interface em Inglês.
 *
 *   Nome dos principais métodos e estruturas em inglês. Na verdade, todas funções são 
 * apenas encapsulações(wrappers) dos métodos originais.
 * === === === === === === === === === === === === === === === === === === === === === == */
 #include "lista-array/translate.c"

#if defined(__unit_tests__)
/* === === === === === === === === === === === === === === === === === === === === === === =
 *                         Testes Unitários
 *
 * Testando todos métodos, funções, e dados abstratos acima. Deixando
 * bem referênciado esta parte, pois fica fácil descartar -- além de
 * ser necessário se os tipos forem trocados, do contrário o programa
 * não compila; se copiado para vários projetos. Caso também tal trecho,
 * futuramente, for colocada num subdiretório, e os tipos serem trocados
 * apenas comentar tal declaração pré-processada para não incluir o que
 * pode conflitar.
 * === === === === === === === === === === === === === === === === === === === === === == */
 #include "lista-array/tests.c"
#endif
