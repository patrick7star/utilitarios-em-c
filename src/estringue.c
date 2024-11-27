

/* A linguagem C dá um número limitado de métodos para trabalhar com
 * strings quando comparado a outras linguagens. Aqui irei implementar
 * todas elas.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include <stddef.h>
// Definições das funções abaixo:
#include "estringue.h"

// macro para outra abordagem de retorno.
#define CARACTERE_NAO_ENCONTRADO 10
#define SIZE_STR sizeof(struct string_dinamica)
#define WCHAR_SIZE sizeof(wchar_t)

// Sinifica que todas ocorrências serão feitas.
const int64_t TODOS = -1;
// Tupla de retorno que retorna 'posições na array' e seu 'comprimento'.
typedef struct posicoes_dos_padroes { ptrdiff_t* array; int size; } 
   Posicoes;

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

static ListaStrings reparte_string_by_char(char* string, const char sep) 
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
   return (ListaStrings) {lista, total};
}

ListaStrings palavras(char* string) {
   /* Pega todos trechos entre espaços em branco, sejam eles palavras 
    * legítimas ou não. */
   const char ESPACO_SEPARADOR = ' ';
   return reparte_string_by_char (string, ESPACO_SEPARADOR);
}

ListaStrings split_linhas(char* conteudo) {
/* Dado uma grande string, está separada com quebras-de-linhas, aqui
 * será retornado uma cópia de todas linhas do conteúdo(sendo este 
 * provavalmente vindo de algum arquivo). */
   const char ESPACO_SEPARADOR = '\n';
   return reparte_string_by_char (conteudo, ESPACO_SEPARADOR);
}

char* concatena_strings(int quantia, ...) 
{
/* Cocatena uma quantia limitada de strings, dado a quantia delas. 
 *
 * Nota: Como tal argumentos para função só podem ser manualmente o 
 *    tipo de inteiros que ele aceita é um de 8 bits, ou seja, só cabem
 *    apenas 256 argumentos tal função. Bastante, porém limitada, mas 
 *    mesmo com isso é um baita de quebra-galho para o tipo existente. */
   assert(quantia > 0);

   va_list argumentos;
   va_start (argumentos, quantia);
   size_t comprimento = 0;
   char* referencias[UINT8_MAX];

   for (int i = 1; i <= quantia; i++) 
   {
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
   for (size_t i = 1; i <= quantia; i++) 
   {
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
/* == == == == == == == == == == == == == == == == == == == == == == == == 
 *                      Funções Auxiliares para
 *                         Strings Puras em C
 *
 *   Tal função altera ou gera raw-strings em C(array de caractéres), ao 
 * invés da estrutura string acima. Há vários métodos e funções que outras
 * linguagem tem como padrão, porém o C não coloca elas.
 * == == == == == == == == == == == == == == == == == == == == == == ==  */
char* maiuscula_ascii(char* str)
{
/* Faz a string, multibyte, para toda uppercase(maiúscula). */
   size_t t = strlen(str);
   const int m = 0x61 - 0x41;
   size_t sz = sizeof(char);
   size_t size = (t + 1) * sz;
   char* result = malloc(size);

   for (size_t i = 0; i < t; i++)
   {
      int code = str[i];

      if (islower(code))
         result[i] = (char)(code - m);
      else
         result[i] = (char)code;
   }

   result[t] = '\0';
   return result;
}

char* minuscula_ascii(char* str)
{
/* Faz uma string ascii toda para lowercase(minúscula). */
   size_t t = strlen(str);
   const int m = 0x61 - 0x41;
   size_t sz = sizeof(char);
   size_t size = (t + 1) * sz;
   char* result = malloc(size);

   for (size_t i = 0; i < t; i++)
   {
      int code = str[i];

      if (isupper(code))
         result[i] = (char)(code + m);
      else
         result[i] = (char)code;
   }

   result[t] = '\0';
   return result;
}

char* capitaliza_ascii(char* str)
{
/* Acha o primeiro caractére ASCII minúsculo na string, então coloca de 
 * forma capitalizada. Se o primeiro caractére já está 'capitalizado', 
 * então nada é alterado.
 */
   size_t t = strlen(str);
   const int m = 0x61 - 0x41;
   size_t size = (t + 1) * sizeof(char);
   char* result = malloc(size);
   bool acionado = false; 

   for (size_t i = 0; i < t; i++)
   {
      int code = str[i];

      if (isupper(code))
         acionado = true;

      if (islower(code) && !acionado)
      {
         result[i] = (char)(code - m);
         acionado = true;
      } else
         result[i] = (char)code;
   }

   result[t] = '\0';
   return result;
}

char* alterna_case_ascii(char* str)
{
/* Alterna a capitalização de cada letra, se ela está minúscula, então 
 * fica maiúscula, e vice-versa. */
   size_t t = strlen(str);
   const int M = 0x61 - 0x41;
   size_t size = (t + 1) * sizeof(char);
   char* result = malloc(size);

   for (size_t i = 0; i < t; i++)
   {
      int code = str[i];

      // Apenas realização a alteração nos símbolos alfabéticos do ASCII.
      if (isalpha(code))
      {
         if (islower(code))
            result[i] = (char)(code - M);
         else
            result[i] = (char)(code + M);
      } else
         result[i] = code;
   }

   result[t] = '\0';
   return result;
}

char* extrai_letras_ascii(char* str)
{
/* Apara qualquer outro caractére não alfabético -- tem que ser um caractére
 * ASCII, da string. As letras são colocadas na ordem que são encontradas.
 */ 
   size_t t = strlen(str);
   size_t size = (t + 1) * sizeof(char);
   char* result = malloc(size);
   // Cursor da resultante, e do argumento passado.
   size_t k = 0, p = 0;

   while (p < t) {
      if (isalpha(str[p]) || str[p] == '.')
         result[k++] = str[p];
      p++;
   }
   result[k] = '\0';

   return result;
}

char* extrai_digitos_ascii(char* str)
{
/* O mesmo que acima, porém agora para números. Neste caso aqui também se 
 * o valor for um decimal, será pego, más não garante se a sintaxe de tal
 * está correta. */ 
   size_t t = strlen(str);
   size_t size = (t + 1) * sizeof(char);
   char* result = malloc(size);
   // Cursor da resultante, e do argumento passado.
   size_t k = 0, p = 0;

   while (p < t) {
      if (isdigit(str[p]) || str[p] == '.')
         result[k++] = str[p];
      p++;
   }
   result[k] = '\0';

   return result;
}

char* repete_caractere_ascii(char p, int qtd)
{
/* Cria uma string baseada no caractére dado, para que seja preenchida por
 * ele 'qtd' de vezes demandada. */
   char* fmt = malloc(qtd * sizeof(char));

   for (int i = 0; i < qtd; i++) fmt[i] = p;
   fmt[qtd] = '\0';

   return fmt;
}

static Posicoes acha_posicoes(char* str, char* pattern)
{
   Posicoes result;
   const int N = 20, sz = sizeof(ptrdiff_t);
   int cursor = 0;
   ptrdiff_t*  posicoes = malloc(N * sz);
   char* aux_str = str, *ptr;

   /* Acha posições de todos padrões, e registra todos eles em ordem de 
    * encontro. Só para o loop quando a busca não achar nenhum começo de
    * sub-estringue, retornando assim um ponteiro nulo. */
   do {
      // Redimensiona array caso precise.
      if (cursor >= N) 
      {
         // Aumenta em 20% da capicidade atual.
         int nc = (int)((float)cursor * 1.20);
         posicoes = realloc(posicoes, nc * sz);
      }

      if (strlen(pattern) > 1)
         ptr = strstr(aux_str, pattern);
      else
         ptr = strchr(aux_str, *pattern);

      // Abandona loop em caso da procura não ter encontrada nada.
      if (ptr == NULL) 
         break;
      else {
         // Posição relativa ao ínicio da estringue(s).
         ptrdiff_t d = ptr -str;

         #ifdef __debug__
         /* Mostrando um trecho de onde está na estringue geral, e também
          * o ínicio do padrão encontrado, e quanto correu da última 
          * posição.
          */
         // Quanto passou do último trecho da sub-estringue.
         ptrdiff_t c = ptr - aux_str;
         char trecho[N];
         memcpy(trecho, aux_str + c, N * sizeof(char));
         if (strlen(aux_str) > N)
            printf("\t-->'%s...'\t%ld\n", trecho, d);
         else
            printf("\t-->'%s'\t%ld\n", trecho, d);
         #endif

         posicoes[cursor++] = d;
         /* Adianta o trecho da estringue para depois do padrão novo
          * achado, mais uma casa adiante. */
         aux_str = str + (d + 1);
      }
   } while(aux_str != NULL);

   result.array = malloc(cursor * sz);
   result.size = cursor;

   assert (result.array != NULL);
   memcpy(result.array, posicoes, cursor * sz);
   // Não mais necessária tal alocação.
   free(posicoes);

   #ifdef __debug__
   printf("Encontrado(%d): {", cursor);

   for (int k = 0; k < result.size; k++)
      printf("%ld, ", result.array[k]);

   puts("\b\b}\n");
   #endif

   return result;
}

char* substitui_ascii(char* s, char* a, char* b)
{
/*   Troca todos padrões(a) na string(s) pelo o novo padrão(b). 
 *   O algoritmo consiste no seguinte: primeiro é registrado todos inicios
 * de posições de cada padrão na estringue(s), na ordem que foram achados
 * é claro, então depois a mesma estringue(s) é varrida até a primeira 
 * posição registrada -- isso se ter sido encontrado alguma; neste ponto 
 * ao invés de apenas copiar os ordinários caractéres da estringue(s), ele
 * copia a estringue de substituição(b). Se tal estringue(b) for menor que
 * o padrão anterior(estringue a), então os demais caractéres faltantes
 * serão preenchidos com um "símbolo neutro". Terminado tal substituição
 * do primeiro, o cursor muda para próxima posição encontrada. */
   Posicoes posicoes = acha_posicoes(s, a);
   // Comprimento do padrão/e o substituto que será substituído.
   const int ta = strlen(a), tb = strlen(b), t = strlen(s);
   const int sz = sizeof(char);
   // Aumenta em 25% o atual tamanho da array de caractéres.
   const int calculo = (int)((float)t * 1.25);
   char* output = malloc(calculo * sz);
   /* Cursor de escrita na estringue(x), de leitura(y), e por último, o 
    * cursor da próxima posição na array. */
   int w = 0, r = 0, p = 0;

   #ifdef __debug__
   printf("\nAtual tamanho: %d\n", t);
   printf("Cálculo da alocação: %d\n", calculo);
   #endif

   while (p < posicoes.size || r < t) 
   {
      // Se encontrar um 'padrão', começar reescreve-lo...
      if (r == posicoes.array[p]) 
      {
         #ifdef __debug__
         printf("Atual posição trabalhada: %ld\n", posicoes.array[p]);
         #endif

         for (int i = 0; i < tb; i++)
            output[w + i] = b[i];
         /* Cursor avança o quanto foi escrito. Já o segundo avança quanto
          * foi lido da string original. O primeiro é o 'substítuto', o 
          * segundo é o 'padrão' dado como argumento. */
         w += tb; r += ta;
         // Próxima posição a analizar.
         p++;

         if (tb == 0)
         // Em caso de o substítuto ser uma estringue vázia, retira um
         // avanço no 'cursor de escrita' para não ficar um espaço em 
         // branco a mais onde será substituido.
            w -= 1;

      } else 
         // Um caractére lido, um caractére escrito.
         output[w++] = s[r++];
   }
   // Liberando array de posições alocadas...
   free(posicoes.array);
   // Crava o final dela.
   output[w] = '\0';
   return output;
}

ListaStrings reparte_ascii(char* str, char* pattern)
{
/* Retorna lista de estringues baseado no separador dado. Se não houver
 * nenhum dos padrões dados o retorno será uma lista com tamanho nulo, e
 * nenhuma cópia de estringue. */
   Posicoes pontos = acha_posicoes(str, pattern);
   int sz = sizeof(char*);
   // Total de pontos que referência cada padrão encontrado na estringue.
   // Também o total de espaços reservados para as cópias de estringues.
   size_t t = pontos.size;
   size_t q = t + 1;
   // Comprimento do 'padrão' a achar.
   int x = strlen(pattern);
   // 'Lista' contendo todos trechos copiados.
   ListaStrings result;

   // A literal númerica um representa a parte alocada manualmente.
   result.lista = (char**)malloc(q * sz);
   result.total = 0;

   /* Faz a cópia para cada intervalo obtido. */
   for (size_t p = 0; p < t; p++) 
   {
      // Pula também o "divisor" dado.
      int i = pontos.array[p] + x, f;

      /* Define o final normalmente, até que chega na última, neste caso,
       * fica algo entre tal posição e o restante da estringue. */
      if (p < (t - 1))
         f = pontos.array[p + 1];
      else
         f = strlen(str);

      assert(f != 0);
      result.lista[p] = copia_trecho(str, i, f);
      result.total += 1;
   }

   // Liberar a alocação se não foi encontrado nada.
   if (result.total == 0)
      free(result.lista);
   return result;
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
   ListaStrings result = palavras ((char*)frase_ii);

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

void fazendo_strings_maiusculas_e_minusculas(void)
{
   puts("Transformação das strings ...");
   for (int i = 1; i <= GIRLS_NAMES; i++)
   {
      char* q = (char*)girls_names[i - 1];
      char* r = maiuscula_ascii(q);
      char* s = minuscula_ascii(r);

      printf("\b\b\b\b\t'%s' ===> '%s', '%s'\n", q, r, s);
      free(r); free(s);
   }
}

void capitalizacao_das_strings(void) 
{
   puts("Transformação das strings ...");

   for (int i = 1; i <= FRUITS; i++)
   {
      char* q = (char*)fruits[i - 1];
      char* r = capitaliza_ascii(q);

      printf("\b\b\b\b\t'%s' ===> '%s'\n", q, r);
      free(r);
   }
}

void metodos_de_extracao(void)
{
   const char* inputs[] = {
      " 501Estou# falando desta % aqui.&|",
      "kda15amI.4ll5", "__#c87A()s-a8+$$",
      "aaaa5eee_$$5---kfnas0"
   };

   puts("Antes ===> depois:");

   char* a = (char*)inputs[0];
   char* b = extrai_letras_ascii(a);
   printf("'%s' ===> '%s'\n", a, b);
   assert(strcmp(b, "Estoufalandodestaaqui.") == 0);
   free(b);

   a = (char*)inputs[1];
   b = extrai_digitos_ascii(a);
   printf("'%s' ===> '%s'\n", a, b);
   assert(strcmp(b, "15.45") == 0);
   free(b);

   a = (char*)inputs[2];
   b = extrai_letras_ascii(a);
   printf("'%s' ===> '%s'\n", a, b);
   free(b);

   a = (char*)inputs[3];
   b = extrai_digitos_ascii(a);
   printf("'%s' ===> '%s'\n", a, b);
   assert(strcmp(b, "550") == 0);
   free(b);
}

void criacao_de_string_homogenea(void)
{
   char tipos[] = {'+', '-', '=', '*'};
   char* aux = NULL, *simbolo = tipos;

   do {
      aux = repete_caractere_ascii(*simbolo, 31); 
      puts(aux);
      free(aux);
   } while(*simbolo++);
}

void buscando_por_padroes_na_estringue(void)
{
   char* exemplo = {
      "Today is the day, for a lot of thing, it is gonna be okay, because "
      "it is already okay, is she here?! For get more stuff, you need "
      "go away, right now! Look there a lot of weed, let's go!"
   };
   printf("\"%s\"(%lu)\n", exemplo, strlen(exemplo));

   Posicoes a = acha_posicoes(exemplo, "is");
   Posicoes b = acha_posicoes(exemplo, "okay");
   Posicoes c = acha_posicoes(exemplo, "e");
   Posicoes d = acha_posicoes(exemplo, "a");

   free(a.array); free(b.array); free(c.array); free(d.array);
}

void metodo_de_substituicao_de_string(void)
{
   char* exemplo = {
      "Today is the day, for a lot of thing, it is gonna be okay, because "
      "it is already okay, is she here?! For get more stuff, you need "
      "go away, right now!"
   };
   char* padroes[] = { "is", "okay", ",", "stuff" };
   const int TOTAL = sizeof(padroes) / sizeof(char*);

   printf("\"%s\"(%lu)\n", exemplo, strlen(exemplo));
   
   for (int i = 0; i < TOTAL; i++)
   {
      char* item = padroes[i];
      char* output_a = substitui_ascii(exemplo, item, "**");
      char* output_b = substitui_ascii(exemplo, item, "******");
      char* output_c = substitui_ascii(exemplo, item, "*");
      char* output_d = substitui_ascii(exemplo, item, "");

      printf("\n - mesmo tamanho: '%s'\n", output_a);
      printf("\n - maior: '%s'\n", output_b);
      printf("\n - apenas um: '%s'\n", output_c);
      printf("\n - vázio: '%s'\n", output_d);

      free(output_d);
      free(output_c);
      free(output_b);
      free(output_a);
   }
}

void reparticao_da_strings_dado_padrao(void)
{
   char* inputs[] = {
      "gato&&cachorro&&comédia&&porta&&alicáte&&pessoas",
      "gato.cachorro.comédia.porta.alicáte.pessoas",
      "gato.cachorro&.comédia&.porta.&alicáte&.pessoas",
      "gato.cachorro.comédia.porta.alicáte.pessoas",
      // Uma estringue vázia.
      ""
   };
   char* padroes[] = {"&&", ".", "&.", "***", "#"};

   for (int k = 0; k < 5; k++)
   {
      ListaStrings output = reparte_ascii(inputs[k], padroes[k]);
      size_t n = output.total;

      printf("\n\"%s\"\nTodos %lu resultados:\n", padroes[k], n);

      for (int i = 0; i < output.total; i++) {
         printf("\t- '%s'\n", output.lista[i]);
         free(output.lista[i]);
      }
      // free(output.lista);
   }
}

int main(int qtd, char* args[], char* vars[]) 
{
   executa_testes_a(
     false, 7, 
			testes_basico_da_reparticao_em_palavras, true,
         visualizacao_da_reparticao_em_palavras, true,
         experimento_concatenacao_de_multiplas_strings, true,
         // Testes específicos do tipo String:
         criacao_simples_de_uma_instancia_String, true,
         diversos_tipos_basicos_de_insercao_String, true,
         motor_de_busca_de_ocorrencias, true,
			preenchimento_da_string, true
   );

   executa_testes_a(
     true, 8,
      fazendo_strings_maiusculas_e_minusculas, false,
      capitalizacao_das_strings, false,
      metodos_de_extracao, true,
      criacao_de_string_homogenea, true,
      buscando_por_padroes_na_estringue, true,
      metodo_de_substituicao_de_string, true,
      reparticao_da_strings_dado_padrao, true,
      experimento_concatenacao_de_multiplas_strings, true
   );

   return EXIT_SUCCESS;
}
#endif

