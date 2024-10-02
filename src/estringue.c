

/* A linguagem C dá um número limitado de métodos para trabalhar com
 * strings quando comparado a outras linguagens. Aqui irei implementar
 * todas elas.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
// Definições das funções abaixo:
#include "estringue.h"

// macro para outra abordagem de retorno.
#define CARACTERE_NAO_ENCONTRADO 10
#define SIZE_STR sizeof(struct string_dinamica)
#define WCHAR_SIZE sizeof(wchar_t)

// Sinifica que todas ocorrências serão feitas.
const int64_t TODOS = -1;

#ifdef _UT_STRING
static void visualiza_array(size_t* array, size_t t) {
   printf ("[");
   for (size_t p = 1; p <= t; p++)
      printf ("%lu, ", array[p - 1]);
   if (t == 0)
      puts("]");
   else
      puts("\b\b]");
}
#endif

static char* copia_trecho (char* string, size_t i, size_t f) {
   if (f < i) {
      perror ("confusão nos marcos dados!");
      abort();
   }

   // faz uma cópia do trecho específico, e a retorna.
   char* trecho = string + i;
   size_t L = f - i;
   char* palavra = malloc (L * sizeof(char));

   // limpando e copiando em seguida ...
   memset (palavra, '\0', L);
   memmove (palavra, trecho, L);
   // retornando a cópia.
   return palavra;
}

static OutcomeStrings reparte_string_by_char(char* string, const char sep) 
{
/* Separa em string à partir de qualquer caractére dado, seja ele 
 * 'imprimível' ou não.
 */
   const char SEPARADOR = sep;
   char* ptr_string = string;
   size_t contagem = 0;
   size_t posicoes[strlen(string)];

   // contando o total de palavras na string.
   while ((ptr_string = strchr (ptr_string, SEPARADOR)) != NULL) {
      size_t T = strlen(string);
      size_t t = strlen(ptr_string);
      size_t atual_posicao = T - t + 1;

      // contando o espaço em branco, e inserido a posição dele na string.
      posicoes[contagem++] = atual_posicao;
      // indo a próximo caractére...
      ptr_string++;
   }
   #ifdef _PALAVRAS
   printf ("quantidade: %lu\nposições na string:", contagem);
   visualiza_array(posicoes, contagem);
   #endif

   // repartindo ela em palavras e axexando na lista.
   size_t size_ptr_char =  sizeof (char*);
   // o total de espaços branco mais um é o total de 'palavras'.
   size_t total = contagem + 1;
   char** lista = malloc (total * size_ptr_char);
   bool existe_palavras = contagem >= 1;

   // cuidando das pontas ...
   if (existe_palavras) {
      // primeira palavra:
      // descotando a posição que começa a próxima palavra.
      char* first_word = copia_trecho (string, 0, posicoes[0] - 1);
      lista[0] = first_word;
      // última palavra:
      char* last_word = copia_trecho (
         string, 
         posicoes[contagem - 1], 
         strlen(string)
      );
      lista[total - 1] = last_word;

      #ifdef _PALAVRAS
      printf ("\tprimeira palavra: '%s'\n", first_word);
      printf ("\túltima palavra: '%s'\n", last_word);
      #endif
   }

   for (size_t i = 1; i < contagem; i++) {
      size_t inicio = posicoes[i - 1];
      // descotando a posição que começa a próxima palavra.
      size_t final = posicoes[i] - 1;
      char* word = copia_trecho (string, inicio, final);
      lista[i] = word;

      #ifdef _PALAVRAS
      printf ("\tpalavra: '%s'\n", word);
      #endif
   }
   return (OutcomePalavras) {lista, total};
}

OutcomePalavras palavras(char* string) {
   /* Pega todos trechos entre espaços em branco, sejam eles palavras 
    * legítimas ou não. */
   const char ESPACO_SEPARADOR = ' ';
   return reparte_string_by_char (string, ESPACO_SEPARADOR);
}

OutcomeStrings split_linhas(char* conteudo) {
/* Dado uma grande string, está separada com quebras-de-linhas, aqui
 * será retornado uma cópia de todas linhas do conteúdo(sendo este 
 * provavalmente vindo de algum arquivo). */
   const char ESPACO_SEPARADOR = '\n';
   return reparte_string_by_char (conteudo, ESPACO_SEPARADOR);
}

char* concatena_strings(uint8_t quantia, ...) {
/* Cocatena uma quantia limitada de strings, dado a quantia delas. 
 *
 * Nota: Como tal argumentos para função só podem ser manualmente o 
 *    tipo de inteiros que ele aceita é um de 8 bits, ou seja, só cabem
 *    apenas 256 argumentos tal função. Bastante, porém limitada, mas 
 *    mesmo com isso é um baita de quebra-galho para o tipo existente. */
   va_list argumentos;
   va_start (argumentos, quantia);
   size_t comprimento = 0;
   char* referencias[UINT8_MAX];

   for (size_t i = 1; i <= quantia; i++) {
      char* strptr = va_arg (argumentos, char*);   
      comprimento += strlen (strptr);
      // guardando a string para futura concatenação.
      referencias[i - 1] = strptr;

      #ifdef _CONCATENA_STRINGS
      printf ("string extraída: '%s'\n", strptr);
      #endif
   }
   va_end (argumentos);
   // contando caractére nulo no final.
   comprimento++;

   #ifdef _CONCATENA_STRINGS
   printf ("total de strings dadas: %u\n", quantia);
   printf ("comprimento calculado: %lu\n", comprimento);
   #endif

   char* grande_string = malloc (quantia * sizeof (char) * comprimento);
   size_t cursor = 0;
   // criando string concatenada...
   for (size_t i = 1; i <= quantia; i++) {
      char* atualstr = referencias[i - 1];
      while (*atualstr != '\0') {
         grande_string[cursor++] = *atualstr;
         atualstr++; 
      }
   }
   grande_string[comprimento - 1] = '\0';
   return grande_string;
}

static wchar_t* clona_static_string_slice(const char* s) {
   size_t T = strlen(s) + 1;
   size_t size = sizeof(wchar_t);
   wchar_t* nova_str = malloc(T * size);

   for (size_t i = 1; i <= T; i++)
      mbstowcs(nova_str, s, T);
   return nova_str;
}

String from_str(const char* s) {
/* Cria uma string dado uma literal string. Ele literalmente clona ela, ou
 * seja, o ponteiro 'char*' segurado por ela não é a literal escrita em 
 * tempo de compilação.
 */
   size_t T = strlen(s) + 1;
   String instancia = malloc(SIZE_STR); 

   if (instancia != NULL) {
      instancia->total = T;
      instancia->quantia = T - 1;
      instancia->caracteres = clona_static_string_slice(s);
   }
   return instancia;
}

String cria_str(void) {
/* Cria uma string com comprimento nulo, entretanto, o pointeiro a string 
 * criada não é exatamente nulo, porém não contém conteúdo algum nela. */
   const size_t T = 10;
   String instancia = malloc(SIZE_STR); 
   wchar_t* string_na_heap = malloc(T * WCHAR_SIZE);

   // Zerando ela...
   // memset(string_na_heap, '\0', T);
   wmemset(string_na_heap, L'\0', T);

   if (instancia != NULL) {
      instancia->total = 10;
      instancia->quantia = 0;
      instancia->caracteres = string_na_heap;
   }
   return instancia;
}

String cria_com_capacidade_str(size_t cP) {
/* Cria uma string com comprimento nulo, entretanto, o pointeiro a string 
 * criada não é exatamente nulo, porém não contém conteúdo algum nela. */
   String instancia = malloc(SIZE_STR); 
   wchar_t* string_na_heap = malloc(cP * WCHAR_SIZE);

   // Zerando ela...
   wmemset(string_na_heap, L'\0', cP);

   if (instancia != NULL) {
      instancia->total = cP;
      instancia->quantia = 0;
      instancia->caracteres = string_na_heap;
   }
   return instancia;
}

bool destroi_str(String s) {
/* Destrói a instância, se falhar em destruir algum dos seus componentes, 
 * retorna o resultado da operação como 'falso'(falho); se tudo foi bem 
 * sucedido, o resultado é apenas okay('verdadeiro'). */
   if (s != NULL) {
      free(s->caracteres);
      free(s);
      return true;
   }
   return false;
}

bool vazia_str(String s)
/* O nome é bem explicativo! Nenhum caractére nela, logo vázia. */
   { return s->quantia == 0; }

size_t comprimento_str(String s)
/* Assim como a função 'strlen', este não contabiliza o caractére nulo. */
   { return s->quantia; }

size_t vacuo_str(String s) 
/* Quanto falta para a 'string' fazer um redimensionamento com mais uma,
 * ou algumas, inserções. */
   { return s->total - s->quantia; }

static void adiciona_um_caractere_unicode(String s, wchar_t caractere) {
   size_t capacidade = s->total;
   size_t Q = comprimento_str(s);
   
   if (Q < capacidade) {
   // Apenas adiciona se houver espaço.
      s->caracteres[s->quantia++] = caractere;
   } else {
      size_t nova_capacidade = 1.10 * (double)s->quantia;
      size_t size = nova_capacidade * WCHAR_SIZE;

      #ifdef _UT_STRING
      puts(
         "Não foi possível inserir nada mais, então redimensionado"
         " em 10% do atual."
      );
      printf("atualização de capacidade.\n\t%lu >> %lu\n", s->quantia,
            nova_capacidade);
      #endif

      s->caracteres = (wchar_t*)realloc(s->caracteres, size);
      s->total = nova_capacidade;
      // Chama recursivamente para inserção, pós redimensionamento.
      adiciona_um_caractere_unicode(s, caractere);
   }
}

void adiciona_str(String s, char c) {
// Adiciona um simples 'narrow caractére' no fim da string.
   wchar_t cU;

   // Apenas adiciona se houver espaço.
   mbtowc(&cU, &c, 2 * sizeof(char)); 
   adiciona_um_caractere_unicode(s, cU);
}

void anexa_str(String s, char* a) {
/* Um modo inicialmente fácil de realizar isso é, converter para uma string
 * Unicode, então iterar cada caractére, equanto adiciona na estrutura. 
 * Farei assim agora, depois atualiza para algo mais eficiente. */
   wchar_t* conversao = clona_static_string_slice(a);
   wchar_t* pointer = conversao;

   while (*pointer) {
      adiciona_um_caractere_unicode(s, *pointer);
      pointer++;
   }
   // Liberando, pois já foi iterado, assim copiada.
   free(conversao);
}

static size_t* busca_todas_ocorrencias_do_caractere(String s, 
  wchar_t _char, int64_t N, bool comeca_na_esquerda)
{
/*   Varre a array interna da string em busca de todas possíveis ocorrências
 * do caractére unicode dado. Pode-se configurar a partida da busca, assim
 * como a quantidade de buscas, sim, por incoerência com o nome. 
 *   O retorno é uma array, onde o primeiro item é o total de índices, e os
 * demais são eles em sí. */
   size_t* indices = NULL, total = 0, Q = s->quantia; 
   wchar_t* array = s->caracteres;

   /* Contando o número de ocorrências, pelo acaso da simetria do item
    * analisado, tal contagem serve tanto para a versão regular(esquerda-
    * à-direita), como o inverso. */
   for (size_t k = 1; k <= Q; k++) {
      if (array[k - 1] == _char) 
         { total++; }

      /* Se foi marcado como 'nem todas ocorrências', quebra em específico
       * ponto da contagem. */
      if (N != TODOS && total == N) { break; }
   }

   #ifdef _UT_STRING
   printf("Total de índices encontrados: %lu\n", total);
   #endif

   if (comeca_na_esquerda) {
      indices = malloc((total + 1) * sizeof(size_t));
      indices[0] = total;
      size_t p = 0, k = 1;

      // Agora sim, uma busca para alocação da posição com o caractére.
      while (total > 0) {
         if (array[p] == _char) { 
            total--;
            indices[k++] = p;
         }
         p++;
      }
   } else {
      indices = malloc((total + 1) * sizeof(size_t));
      indices[0] = total;
      size_t p = Q - 1, k = 1;

      while (total > 0) {
         if (array[p] == _char) { 
            total--;
            indices[k++] = p;
         }
         p--;
      }
   }
   return indices;
}


// Funções de strings retiradas do módulo testes:
uint64_t total_substrings(char* str) {
/* Computando o número de substrings que podem se geradas, dada a 
 * string(na verdade seu comprimento, que será calculado internamente). 
 */
   uint64_t n = strlen(str);
   return (n * n - n) / 2;
}

char* copia_substring(size_t i, size_t f, char* str) {
/* Dado um intervalo válido, ou seja, um valor inteiro menor e maior, 
 * respectivamente; e um endereço do começo de alguma string, tal função 
 * produz a copia de tal trecho. 
 */
   size_t t = strlen(str);
   size_t total_de_bytes = sizeof(char) * t;
   /* índices ultrapassam limites da string. */
   bool transborda_string = (
      (f - i) > (t - 1) ||
      (f >= t) ||
      (i >= t)
   );
	size_t tamanho = f - i;
   char* copia = malloc(tamanho);
   const char NULO = '\0';

   memset(copia, NULO, total_de_bytes);

   if (i > f)
   // Caso esteja invertido, apenas conserta o mal entendido.
      { return copia_substring(f, i, str); }

   if (copia == NULL || transborda_string || i == f)
   /* Nega cópia nos seguintes casos: O intervalo passado transborda os
    * limites da string dada; a string é inválida; o intervalo dado é 
    * nulo. */
      return NULL;

   for(uint8_t p = i; p <= f; p++)
   // Copiando caractére à caractére da posição dada.
      { copia[p - i] = str[p]; }

   return copia;
}

char* concatena_literais_str(int total, ...) {
   const size_t maximo = 1000;
   char* resultado = malloc (maximo * sizeof (char));
   memset (resultado, '\0', maximo);

   va_list strings;
   va_start(strings, total);

   for (uint8_t t = 1; t <= total; t++) {
      char* s = va_arg(strings, char*);
      strcat(resultado, s);
   }
   va_end(strings);

   // retornando referência da string estática criada.
   return resultado;
}

void preenche_str(String s, char c, int n)
{	
	while (n-- > 0) 
		adiciona_str(s, c);
}

#ifdef _UT_STRING
/* === === === === === === === === === === === === === === === === === ==
 *                       Testes 
 *                            Unitários
 * === === === === === === === === === === === === === === === === === ==*/
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include "teste.h"
#include "dados_testes.h"

void testes_basico_da_reparticao_em_palavras(void) {
   for (size_t i = 1; i <= 5; i++) {
      char* phrase = (char*)frases_i[i - 1];
      printf ("\nsobre: '%s'\n", phrase);
      palavras(phrase);
   }
}

void visualizacao_da_reparticao_em_palavras(void) {
   OutcomePalavras result = palavras ((char*)frase_ii);

   puts ("listando o resultado: ...");
   for (size_t q = 1; q <= result.total; q++)
      printf ("\t==>  '%s'\n", result.lista[q - 1]);
}

void experimento_concatenacao_de_multiplas_strings(void) {
   char* result = concatena_strings (
      4, frutas[3], frutas[5], 
         frutas[7], frutas[8]
   );
   printf ("a concatenação: '%s'\n", result);
}

void criacao_simples_de_uma_instancia_String(void) {
   String sI = cria_str(); 
   printf("Seu tamanho: %lu\n", sI->total);

   String sII = from_str("padlock");
   printf("Seu tamanho: %lu\n", sII->total);

   assert (destroi_str(sI));
   assert (destroi_str(sII));
}

void imprime_string(String s) {
   size_t T = s->quantia;

   putchar('\"');
   for (size_t p = 1; p <= T; p++)
      printf("%lc", s->caracteres[p - 1]);
   putchar('\"');
   putchar('\n');
}

void diversos_tipos_basicos_de_insercao_String(void) {
   String X = cria_str();
   char input[] = "mAnBpCqDeRgS";
   char* ptr_input = input;

   while (*ptr_input) {
      printf("Sobrando: %lu\n", vacuo_str(X));
      adiciona_str(X, *ptr_input);
      imprime_string(X);
      ptr_input++;
   }
   
   puts("Anexando toda uma string ...");
   printf("Comprimento: %lu\n", comprimento_str(X));
   anexa_str(X, "A1B2C3Z9X88Y777");
   imprime_string(X);
   printf("Comprimento: %lu\n", comprimento_str(X));
   printf("Sobrando: %lu\n", vacuo_str(X));
   destroi_str(X);
}

void visualiza_int_array(size_t* A) {
   /* Apenas a declaração abaixo já faz a iteração futura inócuo, porque
    * se não há array, logo o acesso de um NULL é convertido a zero, 
    * entretanto, para quem não é muito familiar com C, fica difícil de ler
    * tal declaração, por isso da "declaração de saída" abaixo. */
   if (A == NULL) { return; }

   size_t t = A[0];

   putchar('[');
   for (size_t i = 1; i <= t; i++) { 
      if (i != t)
         printf("%lu, ", A[i]); 
      else
         printf("%lu", A[i]); 
   }
   putchar(']'); 
   putchar('\n');
}

void motor_de_busca_de_ocorrencias(void) {
   String amostra_i = from_str("banana");
   String amostra_ii = from_str("paralelepipedo");
   size_t* (*busca)(String, wchar_t, int64_t, bool);
   busca = busca_todas_ocorrencias_do_caractere;

   size_t* posicoes_i = busca(amostra_i, L'a', TODOS, true);
   size_t* posicoes_ii = busca(amostra_i, L'a', TODOS, false);
   puts("Primeiro esquerda-à-direita, depois o inverso, resultados:");
   imprime_string(amostra_i);
   visualiza_int_array(posicoes_i);
   visualiza_int_array(posicoes_ii);
   free(posicoes_ii); free(posicoes_i);
   puts("Agora, sem contabilizar todas ocorrências:");
   posicoes_i = busca(amostra_i, L'a', 2, true);
   posicoes_ii = busca(amostra_i, L'a', 1, false);
   visualiza_int_array(posicoes_i);
   visualiza_int_array(posicoes_ii);
   free(posicoes_ii); free(posicoes_i);

   posicoes_i = busca(amostra_ii, L'e', TODOS, true);
   posicoes_ii = busca(amostra_ii, L'e', TODOS, false);
   puts("\nPrimeiro esquerda-à-direita, depois o inverso, resultados:");
   imprime_string(amostra_ii);
   visualiza_int_array(posicoes_i);
   visualiza_int_array(posicoes_ii);
   free(posicoes_ii); free(posicoes_i);
   puts("Agora, sem contabilizar todas ocorrências:");
   posicoes_i = busca(amostra_ii, L'e', 2, true);
   posicoes_ii = busca(amostra_ii, L'e', 1, false);
   visualiza_int_array(posicoes_i);
   visualiza_int_array(posicoes_ii);
   free(posicoes_ii); free(posicoes_i);

   posicoes_i = busca(amostra_i, L'a', 0, true);
   visualiza_int_array(posicoes_i);
   posicoes_i = busca(amostra_i, L'a', 0, false);
   visualiza_int_array(posicoes_i);

   destroi_str(amostra_ii);
   destroi_str(amostra_i);
}

void preenchimento_da_string(void) {
	String obj = cria_str();

	preenche_str(obj, '-', 5);
	imprime_string(obj);
	preenche_str(obj, '+', 13);
	imprime_string(obj);

	destroi_str(obj);
}

int main(int qtd, char* args[], char* vars[]) 
{
   executa_testes_a(
      true, 7, 
			testes_basico_da_reparticao_em_palavras, true,
         visualizacao_da_reparticao_em_palavras, true,
         experimento_concatenacao_de_multiplas_strings, true,
         // Testes específicos do tipo String:
         criacao_simples_de_uma_instancia_String, true,
         diversos_tipos_basicos_de_insercao_String, true,
         motor_de_busca_de_ocorrencias, true,
			preenchimento_da_string, true
   );

   return EXIT_SUCCESS;
}
#endif

