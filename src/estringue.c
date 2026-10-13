// Definições das funções abaixo:
#include "estringue.h"
// Biblioteca padrão do C:
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include <stddef.h>

// Macro para outra abordagem de retorno.
#define CARACTERE_NAO_ENCONTRADO 10
#define SIZE_STR sizeof(struct string_dinamica)
#define WCHAR_SIZE sizeof(wchar_t)

// Sinifica que todas ocorrências serão feitas.
const int64_t TODOS = -1;
// Tupla de retorno que retorna 'posições na array' e seu 'comprimento'.
typedef struct posicoes_dos_padroes { ptrdiff_t* array; int size; }
   Posicoes;

/* == == == == == == == == == == == == == == == == == == == == == == == ===
 *                      Métodos do Tipo Lista String
 * == == == == == == == == == == == == == == == == == == == == == == == ==*/
void debug_lista_strings(ListaStrings* In)
{
   const char* const SEP = "\t\b\b\b\b";
   char** lista = (*In).lista;
   int m = 0xdead, TOTAL = (*In).total;

   printf("Lista Strings[%d] {\n", TOTAL);

   for (m = 0; m < TOTAL; m++)
      printf("%s- '%s'\n", SEP, lista[m]);
   puts("}\n");
}

void free_lista_strings(ListaStrings* In)
{
   #ifdef __debug__
   char** lista = (*In).lista;
   const int TOTAL = (*In).total;

   // As strings provavelmente são as únicas coisas alocadas dinâmicamente.
   // Às vezes, nem todas listas do tipos tem itens com alocação dinâmica.
   for (int n = 0; n < TOTAL; n++)
   {
      printf("O (%dº | '%s' ) foi liberado.\n", n + 1, lista[n]);
      free(lista[n]);
   }
   #endif
   free((*In).lista);
   #ifdef __debug__
   puts("Liberado a array de pointeiros (char**).");
   #endif
}

/* == == == == == == == == == == == == == == == == == == == == == == == ===
 *                      Repartição(Split) de Strings
 * == == == == == == == == == == == == == == == == == == == == == == == ==*/
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
/*   Separa em string à partir de qualquer caractére dado, seja ele algum
 * caractére 'imprimível' ou não.
 *   É preciso liberar todo conteúdo alocado, já que, inserir tal substring
 * na lista estática, copia-se o trecho que representa-a, então aloca
 * memória dinamicamente.
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
   }

   for (size_t i = 1; i < contagem; i++) {
      size_t inicio = posicoes[i - 1];
      // descotando a posição que começa a próxima palavra.
      size_t final = posicoes[i] - 1;
      char* word = copia_trecho (string, inicio, final);
      lista[i] = word;
   }
   return (ListaStrings) {lista, total};
}

ListaStrings palavras(char* string) {
/* Pega todos trechos entre espaços em branco, sejam eles palavras legítimas
 * ou não. Também é um tipo de 'split' de espaços em brancos. */
   const char ESPACO_SEPARADOR = ' ';
   return reparte_string_by_char (string, ESPACO_SEPARADOR);
}

ListaStrings split_lines(const char* conteudo) {
/* Dado uma grande string, está separada com quebras-de-linhas, aqui
 * será retornado uma cópia de todas linhas do conteúdo(sendo este
 * provavalmente vindo de algum arquivo). */
   const char ESPACO_SEPARADOR = '\n';
   char* input = (char*)conteudo;

   return reparte_string_by_char (input, ESPACO_SEPARADOR);
}

ListaStrings split_at(const char* string, const char caractere)
 { return reparte_string_by_char((char*)string, caractere); }

ListaStrings split_ascii_whitespace(const char* string)
/* O mesmo que o método 'palavras', porém com o mesmo nome em Rust. Sendo,
 * a função é apenas um 'wrap' da outra. */
   { return palavras((char*)string); }

ListaStrings split_once(const char* string, const char PADRAO) {
/* Retorna uma tupla, com tamanho dois, que contém as duas únicas partes
 * que ocorrem na primeira ocorrência de busca neste 'spliting'. */
   const int sz = sizeof(char*), szChar = sizeof(char);
   ListaStrings Out;
   char* cursor = strchr(string, PADRAO);
   ptrdiff_t m, n;

   if (cursor == NULL)
      return (ListStr) {NULL, 0};

   // Já define o resultado que é bem determinado.
   Out.lista = (char**)calloc(2, sz);
   Out.total = 2;
   // Computando comprimento da string da primeira e segunda parte.
   m = cursor - string;
   n = strlen(string) - m;
   // Alocando memória necessária para cada cópia, com locais já zerados.
   // Isso evita falta de caractéres nulos.
   Out.lista[0] = (char*)calloc(m + 1, szChar);
   Out.lista[1] = (char*)calloc(n + 1, szChar);
   // Aplicando cópias dos respectivos trechos.
   memcpy(Out.lista[0], string, m);
   memcpy(Out.lista[1], cursor + 1, n);

   return Out;
}

static char* ultima_ocorrencia
  (const char* restrict s, const char* restrict p)
{
   char* ptr = strstr(s, p);
   char* old = NULL;
   int t = strlen(p);

   while (ptr != NULL)
      { old = ptr; ptr = strstr(ptr + t, p); }
   return old;
}

static bool comeco_da_string_com_o_pattern
  (const char* restrict s, const char* restrict p)
   { return strstr(s, p) == s; }

static bool fim_da_string_com_o_pattern
  (const char* restrict s, const char* restrict p)
{
   // Encontra a primeira ocorrência da substring.
   int t = strlen(p), n = strlen(s);
   ptrdiff_t d = 0xdead;
   char* ptr = ultima_ocorrencia(s, p);

   d = ptr - s;
   #ifdef __debug__
   printf("diferença: %ld\nstrlen(p): %d\nlength: %d\n", d, t, n);
   #endif
   return d + t == n;
}

static bool argumentos_nao_aceitaveis
  (const char* restrict s, const char* restrict p)
   { return strcmp(s, "") == 0 || strcmp(p, "") == 0; }

ListStr split_matches(const char* string, const char* PATTERN)
{
/*   O mesmo que qualquer acima, porém neste caso busca o padrão uma string
 * apenas de um simples caractere ASCII. 
 * A entrada padrão que esta função reparte é algo como:
 * 
 *    "casaPATTERNlojaPATTERNmáquina música PATTERN ... livroPATTERNCaixa"
 *
 *   Portanto, uma string que começa sem o padrão, e termina sem ele também.
 * Qualque entrada que não siga tal normas, terá de algum jeito ser tratado
 * antes do processamento, caso contrário o resultado não será o desejado.
 *   O 'input' também não funciona no momento para funções que não tenha 
 * vários padrões intercalados, ou seja, que a divisão não resulte em mais de
 * três recortes.
 */
   /* Caso em que a string começa com o padrão. Enquanto não há uma solução
    * tratável, não será aceito tal formatação. */
   if (argumentos_nao_aceitaveis(string, PATTERN))
   {
      perror("Caso de entrada não é válido(argumentos 'vázios')!");
      abort();
   }

   if ( fim_da_string_com_o_pattern(string, PATTERN) ||
        comeco_da_string_com_o_pattern(string, PATTERN) 
   ){
      perror("Caso de entrada não é válido!");
      abort();
   }

   const int N = strlen(string) + 1;
   const int M = strlen(PATTERN);
   // Posições dos 'padrões' e cursores necessários.
   int posicoes[N], p = 0, m = 0, n = 0;
   char* ptr = strstr(string, PATTERN);
   const int size = sizeof(char**);
   char* input = (char*)string;
   ListStr output = {NULL, 0};

   while (ptr != NULL)
   {
      // Computa a diferença desde o começo.
      p = ptr - string; 
      // Marca a posição depois do padrão.
      posicoes[m++] = p + M;
      // String agora começa depois do padrão encontrado.
      ptr = strstr(ptr + M, PATTERN);
   }

   // Por enquanto, contagem apenas dos PATTERNs encontrados.
   output.total = m;
   output.lista = (char**)calloc(m, size);

   for (n = 0; n < output.total - 1; n++)
      output.lista[n] = copia_trecho(input, posicoes[n], posicoes[n + 1] - M);
   // Incluindo o primeiro e último, que não são contabilizados pelo loop
   // acima.
   output.lista[n++] = copia_trecho(input, 0, posicoes[0] - M);
   output.lista[n] = copia_trecho(input, posicoes[m - 1], N);
   // Contabiliza última inserção.
   output.total++;

   return output;
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

/* == == == == == == == == == == == == == == == == == == == == == == == ===
 *                   Construção da String Mutável
 * == == == == == == == == == == == == == == == == == == == == == == == ==*/
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



// Funções de strings retiradas do módulo testes:
uint64_t total_substrings(char* str) {
/* Computando o número de substrings que podem se geradas, dada a
 * string(na verdade seu comprimento, que será calculado internamente).
 */
   uint64_t n = strlen(str);
   return (n * n - n) / 2;
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
   char* fmt = malloc((qtd + 1) * sizeof(char));

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

static bool caractere_pertence(char In, char* Set)
{
   for (int n = 0; n < strlen(Set); n++)
   {
      if (In == Set[n])
         return true;
   }
   return false;
}

static void arranca_caractere_no_fim(char* input, char set[])
{
   const int T = strlen(input);
   char* output = input;
   int n = 0x00000000, p = T - (n + 1);
   char character = output[p];

   while(caractere_pertence(character, set))
   {
      output[T - (n + 1)] = '\0';
      n++;
      character = output[T - (n + 1)];
   }
}

char* strip_suffix(char* input, char* pattern)
{
/* Retorna uma nova string com os caractéres em 'pattern' arrancados do lado direito da
 * string. */
   const int N = strlen(pattern);
   char* output = malloc(N);
   int count = N;

   memset(output, 0x00, N);
   strcpy(output, input);

   while(count-- > 0)
      arranca_caractere_no_fim(output, pattern);

   return output;
}

static void move_tudo_uma_casa_a_esquerda(char* input, int length)
{
// Desloca todas caracteres da string, uma casa à esquerda.
   const int LENGTH = length;
   char* output = input;
   int count = 0;

   for (count = 0; count < LENGTH; count++)
      output[count] = output[count + 1];
}

static void arranca_caractere_no_comeco(char* input, char set[])
{
   char* output = input;
   int n = 0x00000000;
   char character = output[0];

   // Substitui partes aparadas à esquerda com um símbolo especial(temporario).
   while(caractere_pertence(character, set))
   {
      output[n] = '-';
      n++;
      character = output[n];
   }

   // Aqui, movemos 'n' vezes da onde parou à esquerda, sobreescrevendo o caractére
   // que substitui.
   while (n-- > 0)
      move_tudo_uma_casa_a_esquerda(output, strlen(input));
   // Define novo fim da string.
}

char* strip_prefix(char* input, char* pattern)
{
/* Retorna uma nova string com os caractéres em 'pattern' arrancados do lado esquerdo da
 * string. */
   const int N = strlen(pattern);
   char* output = malloc(N);
   int count = N;

   memset(output, 0x00, N);
   strcpy(output, input);

   while(count-- > 0)
      arranca_caractere_no_comeco(output, pattern);

   return output;
}

// Outros nomes paras funções acimas:
char* strip(char* input, char* pattern)
   { return strip_prefix(input, pattern); }

char* rstrip(char* input, char* pattern)
   { return strip_suffix(input, pattern); }

#ifdef __unit_tests__
/* === === === === === === === === === === === === === === === === === ==
 *                       Testes
 *                            Unitários
 * === === === === === === === === === === === === === === === === === ==*/
 #include "estringue/unitarios.c"
#endif

