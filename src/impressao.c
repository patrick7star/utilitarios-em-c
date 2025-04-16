/*
  Um bom modo de organizar dados tabelados. O módulo possuí uma estrutura 
onde você pega todo 'rol' de dados, cede um 'rótulo' a ele e toda vez que 
impresso será visualizado fechado por caractéres `Unicode` de uma maneira 
bem formatada no terminal. A estrutura `Coluna` que proporciona isso, 
também aceita a impressão de outras juntas, formando assim uma bonita 
tabela.
*/

#include "impressao.h"
// Biblioteca padrão do C:
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <wchar.h>
#include <string.h>
#include <assert.h>
// Biblioteca deste próprio repositório:
#include "terminal.h"

static size_t maior_string(vetor_t* L, ToString F) {
   size_t t = tamanho_al(L);
   size_t maximo = 0;
   char* dado_fmt;

   for (size_t i = 1; i <= t; i++) {
      generico_t dado = indexa_al(L, i - 1);
      dado_fmt = dado;
      size_t comprimento = strlen(dado_fmt);

      if (comprimento > maximo)
         maximo = comprimento;
   }
   // Máximo, somado a margem.
   return maximo + 2;
}

static char* ajusta_entrada(generico_t dt, ToString f, size_t c) {
   size_t size = c * sizeof(char);
   char* string = malloc(size);
   // Formatação do dado numa string.
   char* dado_fmt = f(dt);
   // Comprimento da formatação do dado.
   size_t t = strlen(dado_fmt);

   #ifdef __debug__
   const char BRANCO = '.';
   #else
   const char BRANCO = ' ';
   #endif

   for (size_t i = 0; i < c; i++) {
      if (i < t)
         string[i] = dado_fmt[i];
      else
         string[i] = BRANCO;
   }
   string[c] = '\0';
   free(dado_fmt);
   return string;
}

void listar(vetor_t* lista, ToString f) {
/* Lista uma lista de qualquer coisa, dado seu tal lista, e seu modo de 
 * transformar o dado interno que ela porta numa string. */
   Dimensao dim = dimensao_terminal();
   size_t M = maior_string(lista, f);
   size_t C = dim[1] / M;
   size_t total = tamanho_al(lista);
   generico_t dado;
   size_t contagem = 0;

   #ifdef __debug__
   printf("\nMaior comprimento de string: %lu\n", M);
   printf("linhas=%u colunas=%u\n\n", dim[0], dim[1]);
   #endif

   while (total-- > 0) {
      dado = indexa_al(lista, contagem++);
      char* entrada = ajusta_entrada(dado, f, M);
      printf("%s", entrada);
      free(entrada);
      if (contagem % C == 0)
         putchar('\n');
   }
   // Para garantiar quebra de linha.
   putchar('\n');
}

/* === === === === === === === === === === === === === === === === === ===+
 * ......................Impressão de Arrays..............................&
 * ......................  de Vários Tipos Primitivos.....................&
 * === === === === === === === === === === === === === === === === === ===*/
const int LIMITE = 13000;

void imprime_array_int(int* array, int length)
{
   assert(length >= 0 && array != NULL); 
   /* Números grandes de mais são totalmente desnecessário para o que isso
    * foi feito. */
   assert(length <= LIMITE);

   if (length == 0)
      { puts("[]"); return; }

   putchar('[');
   for (int p = 0; p < length; p++)
      printf("%d, ", array[p]);
   puts("\b\b]");
}

void imprime_array_bool(bool* array, int length)
{
   assert(length >= 0 && array != NULL); 
   /* Números grandes de mais são totalmente desnecessário para o que isso
    * foi feito. */
   assert(length <= LIMITE);

   if (length == 0)
      { puts("[]"); return; }

   putchar('[');
   for (int p = 0; p < length; p++)
   {
      if (array[p])
         printf("true, ");
      else
         printf("false, ");
   }
   puts("\b\b]");
}

void imprime_array_char(char* array, int length)
{
   assert(length >= 0 && array != NULL); 
   /* Números grandes de mais são totalmente desnecessário para o que isso
    * foi feito. */
   assert(length <= LIMITE);

   if (length == 0)
      { puts("[]"); return; }

   putchar('[');
   for (int p = 0; p < length; p++)
   {
      printf("'%c', ", array[p]);
   }
   puts("\b\b]");
}

void imprime_array_str(char* array[], int total)
{
   assert(total >= 0 && array != NULL); 
   /* Números grandes de mais são totalmente desnecessário para o que isso
    * foi feito. */
   assert(total <= LIMITE);

   if (total == 0)
      { puts("[]"); return; }
   
   /* Calculando o comprimento da tela, e quantas strings cabem nela. */
   struct TerminalSize dim = obtem_dimensao();
   int L = dim.colunas, x = 0, PAD = 1;
   char fmt[L];

   putchar('[');
   for (int p = 0; p < total; p++)
   {
      sprintf(fmt, "\"%s\", ", array[p]);
      x += strlen(fmt) + PAD;

      if (x >= L) {
         printf("\n%s", fmt);
         x = 0;
      } else
         printf("%s", fmt);
   }
   puts("\b\b]");
}

void imprime_array_str_unicode(wchar_t* array[], int total)
{
/* Uma cópia, com alguns retoque é claro, da função que realização a 
 * impressão de 'narrow' strings. */
   assert(total >= 0 && array != NULL); 
   assert(total <= LIMITE);

   if (total == 0)
      { wprintf(L"[]\n"); return; }
   
   struct TerminalSize dim = obtem_dimensao();
   int L = dim.colunas, x = 0, PAD = 1;
   wchar_t fmt[L];

   putchar('[');
   for (int p = 0; p < total; p++)
   {
      swprintf(fmt, L, L"\" %ls \"", array[p]);
      x += wcslen(fmt) + PAD;

      if (x >= L) {
         printf("\n%ls, ", fmt);
         x = 0;
      } else
         printf("%ls, ", fmt);
   }
   puts("\b\b]");
   // wprintf(L"\b\b]\n");
}

static bool e_caractere_ascii(wchar_t caractere)
{
   wchar_t* ptr = &caractere;
   uint8_t* bytes = (uint8_t*)ptr;

   if (bytes[0] > 0 && bytes[1] == 0 && bytes[2] == 0 && bytes[3] == 0)
      return true;
   else
      return false;
}
void imprime_array_char_unicode(wchar_t* array, int length)
{
/* Mesmo que a função 'imprime_array_char', porém para caractéres Unicodes.
 * Entretanto, aqui, símbolos mais gráficos não são colocados entre aspas
 * simples. */
   assert(length >= 0 && array != NULL); 
   assert(length <= LIMITE);

   if (length == 0)
      { puts("[]"); return; }

   putchar('[');
   for (int p = 0; p < length; p++)
   {
      wchar_t caractere = array[p];

      if (e_caractere_ascii(caractere))
         printf("'%lc', ", array[p]);
      else
         printf("%lc , ", array[p]);
   }
   puts("\b\b]");
}

void imprime_array_float(float* array, int length)
{
   assert(length >= 0 && array != NULL); 
   /* Números grandes de mais são totalmente desnecessário para o que isso
    * foi feito. */
   assert(length <= LIMITE);

   if (length == 0)
      { puts("[]"); return; }

   putchar('[');
   for (int p = 0; p < length; p++)
      printf("%0.2f, ", array[p]);
   puts("\b\b]");
}

void imprime_array_double(double* array, int length)
{
   assert(length >= 0 && array != NULL); 
   /* Números grandes de mais são totalmente desnecessário para o que isso
    * foi feito. */
   assert(length <= LIMITE);

   if (length == 0)
      { puts("[]"); return; }

   for (int p = 0; p < length; p++)
      printf("%0.4lf, ", array[p]);
   puts("\b\b]");
}

// ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~
//                      Tipos de Inteiros Específicos:
//                   8-bits, 16-bits, 32-bits, 64-bits
// ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~
typedef enum TipoPrimitivo 
   {i8, u8, i16, u16, i32, u32, i64, u64, f32, f64} TP;

static void impressao_de_item_generico(void* array, TP qual_o_tipo, int p) {
   uint8_t*  ptr_u8;       int8_t*  ptr_i8;
   uint16_t* ptr_u16;      int16_t* ptr_i16;
   uint32_t* ptr_u32;      int32_t* ptr_i32;
   uint64_t* ptr_u64;      int64_t* ptr_i64;
   float*    ptr_f32;      double*  ptr_f64;

   switch(qual_o_tipo)
   {
      case u8:
         ptr_u8 = (uint8_t*)array;
         printf("%u, ", ptr_u8[p]);
         break;
      case u16:
         ptr_u16 = (uint16_t*)array;
         printf("%u, ", ptr_u16[p]);
         break;
      case u32:
         ptr_u32 = (uint32_t*)array;
         printf("%u, ", ptr_u32[p]);
         break;
      case u64:
         ptr_u64 = (uint64_t*)array;
         printf("%lu, ", ptr_u64[p]);
         break;
      case i8:
         ptr_i8 = (int8_t*)array;
         printf("%d, ", ptr_i8[p]);
         break;
      case i16:
         ptr_i16 = (int16_t*)array;
         printf("%d, ", ptr_i16[p]);
         break;
      case i32:
         ptr_i32 = (int32_t*)array;
         printf("%d, ", ptr_i32[p]);
         break;
      case i64:
         ptr_i64 = (int64_t*)array;
         printf("%ld, ", ptr_i64[p]);
         break;
      case f32:
         ptr_f32 = (float*)array;
         printf("%0.2f, ", ptr_f32[p]);
         break;
      case f64:
         ptr_f64 = (double*)array;
         printf("%0.2lf, ", ptr_f64[p]);
         break;
      default:
         perror("Tipo não compátivel!");
         abort();
   }
}

static void imprime_array_de_qualquer_tipo(void* array, int length, TP tipo)
{
   assert(length >= 0 && array != NULL); 
   /* Números grandes de mais são totalmente desnecessário para o que isso
    * foi feito. */
   assert(length <= LIMITE);

   putchar('[');
   for (int p = 0; p < length; p++)
      impressao_de_item_generico(array, tipo, p);

   if (length == 0)
      puts("]");
   else
      puts("\b\b]");
}

void imprime_array_i8(int8_t* array, int length)
   { imprime_array_de_qualquer_tipo(array, length, i8); }
   
void imprime_array_u8(uint8_t* array, int length)
   { imprime_array_de_qualquer_tipo(array, length, u8); }

void imprime_array_i16(int16_t* array, int length)
   { imprime_array_de_qualquer_tipo(array, length, i16); }

void imprime_array_u16(uint16_t* array, int length)
   { imprime_array_de_qualquer_tipo(array, length, u16); }

void imprime_array_u32(uint32_t* array, int length)
   { imprime_array_de_qualquer_tipo(array, length, u32); }

void imprime_array_i64(int64_t* array, int length)
   { imprime_array_de_qualquer_tipo(array, length, i64); }

void imprime_array_u64(uint64_t* array, int length)
   { imprime_array_de_qualquer_tipo(array, length, u64); }

/* === === === === === === === === === === === === === === === === === ===+
 *                      Coloração de Resultados
 * === === === === === === === === === === === === === === === === === ===*/
#define ANSI_VERMELHO   "\x1b[031m"
#define ANSI_VERDE      "\x1b[032m"
#define ANSI_AMARELO    "\x1b[033m"
#define ANSI_AZUL       "\x1b[034m"
#define ANSI_VIOLETA    "\x1b[035m"
#define ANSI_MARINHO    "\x1b[036m"
#define ANSI_DESLIGADO  "\x1b[00m"

// Buffer para copia temporaria de string.
static char TEXTO[UCHAR_MAX];

static char* const cor_selecionada(enum Cores cor) {
   switch(cor) {
      case Vermelho:
         return ANSI_VERMELHO;
      case Violeta:
         return ANSI_VIOLETA;
         break;
      case AzulMarinho:
         return ANSI_MARINHO;
         break;
      case Azul:
         return ANSI_AZUL;
      case Amarelo:
         return ANSI_AMARELO;
      case Verde:
         return ANSI_VERDE;
      default:
         perror("Tipo de cor não definida aidna.");
         abort();
   }
}

StrColorida colori_string(char* In_a, enum Cores In_b) {
/*   Aplica protocolo ANSI numa cópia da string, que será posteriormente
 * retornada. A string retornada foi antes alocada dinâmicamente na memória.
 */
   int m = strlen(ANSI_VIOLETA);
   int n = strlen(In_a);
   int sz = n + 2*m + 1;
   StrColorida Out = malloc(sz);
   char* selecao = cor_selecionada(In_b);
   char* off = ANSI_DESLIGADO;

   sprintf(Out, "%s%s%s", selecao, In_a, off);
   return Out;
}

StrColorida muda_cor_da_string(char* In_a, enum Cores In_b) {
/* Faz o mesmo que acima, porém sem alocação dinâmica. A desvantagem aqui
 * é que é uma função 'thread-unsafe', e que tem limitação da quantia 
 * de caractéres que podem ser usadas. A função confrotado com o tamanho
 * superior de caractéres que ela permite, simplesmente interromperá o
 * programa. */
   int m = strlen(ANSI_VERDE);
   int comprimento = strlen(In_a);

   if (comprimento + 2*m >= UCHAR_MAX) {
      perror
         ("string passada é muito comprida para o buffer que a função usa.");
      abort();
   }

   char* selecao = cor_selecionada(In_b); 
   char* off = ANSI_DESLIGADO;
   char* Out = TEXTO;

   sprintf(Out, "%s%s%s", selecao, In_a, off);
   return Out;
}

/* === === === === === === === === === === === === === === === === === ===+
 *             Converte arrays de qualquer tipo numa string 

 *   O método usado aqui é usar a impressão da saída padrão para pegar-la, ao
 * invés de simplesmente fazer uma iteração e transformar item por item
 * concatenando tudo, como sempre foi feito. Tal novo método, será bem mais
 * sostificado.
 * === === === === === === === === === === === === === === === === === ===*/
 #include <limits.h>
 #include <fcntl.h>
 #include <unistd.h>

char* captura_impressao_de_qualquer_array
  (void* array, int n, const int sz, TP tipo)
{
/* Aplica uma impressão de uma array genérica, dado é claro seu tamanho e 
 * tipo(tanto de bytes). Entretanto, a saída padrão(stdout) está conectada
 * a um pipe local, ou seja, toda impressão transmitida para um local que
 * deve está atrelada a algum buffer que pode lê e armazenar tal impressão. 
 * Como este é todo o objetivo do código, tal buffer que captorou os bytes,
 * é copiado na heap e retornado.
 */
   const int In = 0, Out = 1, MAX = SHRT_MAX / 10;
   int tubos[2], saida_padrao, lido;
   unsigned char buffer[MAX];
   char* formatacao;

   // Deixar o stdout sem qualquer conteúdo.
   fflush(stdout);
   // Salva a saída padrão do processo antes de entubar ela em outro lugar.
   saida_padrao = dup(fileno(stdout));
   // Cria 'pipes', então conecta a saída padrão ao tubo de saída dos 'pipes'.
   pipe(tubos);
   dup2(tubos[Out], fileno(stdout));

   /* Executa função que utiliza do 'stdout', porém conecatada ao pipe, os
    * bytes serão transmitidos pra la ao invés do normal. Em seguida, após 
    * tal execução, lemos os bytes transmitidos. Como a impressão via 
    * 'printf' vem com uma quebra-de-linha, vamos anula-la. */
   imprime_array_de_qualquer_tipo(array, n, tipo);
   lido = read(tubos[In], buffer, MAX);
   buffer[lido - 1] = '\0';

   // Libera o resto que está no stdout, então recupera antiga 'saída'.
   fflush(stdout);
   dup2(saida_padrao, fileno(stdout));

   /* Aloca o necessário de memória, que é exatamente o que foi lido(mais um,
    * apenas pra ter total segurança). Copia o conteúdo do 'buffer', então
    * retorna. */
   formatacao = malloc((lido + 1) * sz);
   memcpy(formatacao, buffer, lido);

   return formatacao;
}

/* Nome mais curto para caber tudo na curta tela que uso. Também é um nome
 * com um 'pascal case', ou seja, faz um destaque bem notável. */
static char* (*CapturaDaImpressao) (void*, int, const int, TP)
   = captura_impressao_de_qualquer_array;

char* array_i8_to_str(signed char* array, int n)
   { return CapturaDaImpressao(array, n, sizeof(char), i8); }

char* array_u8_to_str(unsigned char* array, int n)
   { return CapturaDaImpressao(array, n, sizeof(uint8_t), u8); }

char* array_i16_to_str(signed short* array, int n)
   { return CapturaDaImpressao(array, n, sizeof(int16_t), i16); }

char* array_u16_to_str(unsigned short* array, int n)
   { return CapturaDaImpressao(array, n, sizeof(uint16_t), u16); }

char* array_i32_to_str(int* array, int n) 
   { return CapturaDaImpressao(array, n, sizeof(uint32_t), i32); }

char* array_u32_to_str(unsigned int* array, int n)
   { return CapturaDaImpressao(array, n, sizeof(uint32_t), u32); }

char* array_i64_to_str(signed long* array, int n)
   { return CapturaDaImpressao(array, n, sizeof(int64_t), i64); }

char* array_u64_to_str(unsigned long* array, int n)
   { return CapturaDaImpressao(array, n, sizeof(uint64_t), u64); }

char* array_f32_to_str(float* array, int n)
   { return CapturaDaImpressao(array, n, sizeof(float), f32); }

char* array_f64_to_str(double* array, int n)
   { return CapturaDaImpressao(array, n, sizeof(double), f64); }

/* === === === === === === === === === === === === === === === === === ===+
 *                Listagem de Checagem de Qualquer Lista 
 *
 *   O objetivo aqui é fazer uma visualização de checagem de qualquer tipo
 * de dado que se desejar. Apenas coloque o nome do item, e seu estado, ou
 * seja, se ele foi checado/ou não. Haverá a opção tanto de coloração, que
 * é padrão dependendo da plataforma.
 * === === === === === === === === === === === === === === === === === ===*/
struct Checagem 
   { char nome[UCHAR_MAX * 2]; bool resultado; };

struct ListaDeChecagem 
   { struct Checagem array[UCHAR_MAX * 2]; int quantia; int maior; };

static void imprime_resultado(bool resultado, bool colorido)
{
   const wchar_t POSITIVO = L'\u2714', NEGATIVO = L'\u2718'; 
   char buffer[10];
   StringColorida fmt;
   
   if (colorido) {
      sprintf(buffer, "%lc", resultado ? POSITIVO: NEGATIVO);
      fmt = colori_string(buffer, resultado ? Verde: Vermelho);
      printf("%s", fmt);
      free(fmt);
   } else 
      printf("%lc", resultado ? POSITIVO: NEGATIVO);
}

static void imprime_margem_esquerda(void)
   { printf("    "); }

static void imprime_legenda(char* legenda) 
   { printf("%s", legenda); }

static void imprime_recuo(int n, int m) {
   char formatacao[UCHAR_MAX * 2];

   memset(formatacao, '.', m - n);
   formatacao[m - n] = '\0';
   printf("%s ", formatacao);
}

static void imprime_checagem(struct Checagem* obj, const int M) 
{
   imprime_margem_esquerda();
   imprime_legenda((*obj).nome);
   imprime_recuo(strlen((*obj).nome), M);
   imprime_resultado((*obj).resultado, true);
   putchar('\n');
}

struct ListaDeChecagem nova_ldc(void) {
   return (struct ListaDeChecagem) { .quantia = 0, .maior = 0};
}

bool adiciona_ldc(struct ListaDeChecagem* list, struct Checagem* obj)
{
   if (list == NULL || obj == NULL)
      return false;

   int posicao = (*list).quantia, length = strlen((*obj).nome);
   const int size = sizeof(struct Checagem);
   struct Checagem* array = list->array;

   if (posicao + 1 >= UCHAR_MAX * 2)
      return false;

   if (length > (*list).maior)
   // Registrando um comprimento maior de string.
      (*list).maior = length + 5;

   memcpy(&array[posicao], obj, size);
   list->quantia += 1;

   // Se tudo ocorreu com sucesso.
   return true;
}

void visualiza_ldc(struct ListaDeChecagem* list) 
{
   int n = (*list).quantia;
   struct Checagem * array = (*list).array;

   printf("\nLista de todos %d itens abaixo:\n", n);
   for (int q = 1; q <= n; q++)
      imprime_checagem(&array[q - 1], (*list).maior);
}

/* === === === === === === === === === === === === === === === === === ===+
 * .......................................................................&
 * ........................Testes Unitários...............................&
 * .......................................................................&
 * === === === === === === === === === === === === === === === === === ===*/
#ifdef __unit_tests__
#include "dados_testes.h"
#include "teste.h"
#include <locale.h>

char* constchar_to_str(generico_t dt) {
   char* pointer = (char*)dt;
   size_t t = strlen(pointer);
   size_t size = (t + 1) * sizeof(char);
   char* copia = malloc(size);
   strcpy(copia, pointer);
   return copia;
}

void primeiro_exemplo_da_funcao_de_listagem(void) {
   vetor_t* L = cria_al();

   for (size_t k = 1; k <= VEICULOS; k++) 
      insere_al(L, (char*)veiculos[k - 1]);
   for (size_t k = 1; k <= BOYS_NAMES; k++) 
      insere_al(L, (char*)boys_names[k - 1]);

   listar(L, constchar_to_str);
}

void checando_um_caractere_multibyte(void) {
   const char exemplo[] = "você está certo, faça!";
   size_t t = strlen(exemplo);

   puts("Tentando identificar multibyte-chars ...");
   for (size_t p = 1; p <= t; p++) {
      char* ptr = (char*)&exemplo[p - 1];
      printf("\t>>> '%c'(%d)\n", exemplo[p - 1], mblen(ptr, t));
   }
}

void impressao_de_varios_tipos_de_array(void)
{
   int N;

   imprime_array_str((char**)veiculos, VEICULOS);
   imprime_array_str((char**)objetos, OBJETOS);

   imprime_array_char((char*)sexo, SEXO);
   N = sizeof(valores_padronizados_iv) / sizeof(int);
   imprime_array_int((int*)valores_padronizados_iv, N);

   puts("\nVerificando a versão genérica ...");
   bool resposta[] = { true, false };
   imprime_array(resposta, 2);
   imprime_array((char**)legumes, LEGUMES);
}

void impressao_de_array_gigante_de_strings(void)
{
   int N = TODAS_LISTAS_COM_STRS;
   char*** inputs = (char***)todas_listas_com_strs, *lista[N];
   int* L = (int*)TLCS_COMPRIMENTOS;
   int T = sizeof(TLCS_COMPRIMENTOS) / sizeof(int);

   printf("\nN: %llu\n", TODAS_LISTAS_COM_STRS);
   for (int i = 0, q = 0; i < T; i++)
   {
      int t = L[i];

      for (int k = 0; k < t; k++, q++) {
         printf("i: %2d | k: %2d | t: %2d | q: %2d\n", i, k, L[i], q);
         lista[q] = (char*)inputs[i][k];
      }
   }
   
   printf("Tamanho da lista: %d\n", T);
   imprime_array_str(lista, N);
}

void impressao_de_outros_tipos_de_inteiros(void)
{
   int8_t in_a[] = {-2, -1, 0, 1, 2, 3};
   uint8_t in_b[] = {53, 63, 73, 83, 93};
   int16_t in_c[] = {SHRT_MIN, -1000, 10000, -20000, 30000, SHRT_MAX};
   uint16_t in_d[] = {0, USHRT_MAX};
   int32_t in_e[] = {INT_MIN, INT_MAX};
   uint32_t in_f[] = {0, UINT_MAX};
   int64_t in_g[] = {LLONG_MIN, LLONG_MAX};
   uint64_t in_h[] = {0, ULLONG_MAX};

   puts("Inteiro byte com sinal(i8):");
   imprime_array(in_a, 6);
   puts("Inteiro de byte sem sinal(u8):");
   imprime_array(in_b, 5);
   puts("Inteiro curto sem sinal(i16):");
   imprime_array(in_c, 6);
   puts("Inteiro curto com sinal(u16):");
   imprime_array(in_d, 2);
   puts("Inteiro com sinal(i32):");
   imprime_array(in_e, 2);
   puts("Inteiro sem sinal(u32):");
   imprime_array(in_f, 2);
   puts("Inteiro longo com sinal(i64):");
   imprime_array(in_g, 2);
   puts("Inteiro longo sem sinal(u64):");
   imprime_array(in_h, 2);
}

void impressao_de_strings_e_caracteres_unicode(void)
{
   wchar_t* input[] = {
      L"Minha linda casa ... \U0001f3e0",
      L"Me and him, we \u2665, but we broke up ... \U0001f494",
      L"Irei de \U0001f686 ou de \U0001f697 ?!",
      L"Eu adotei um lindo \U0001f415 para meu lar."
   };

   puts("Impressão pra verificar se está funcionando:");
   for (int p = 0; p < 4; p++)
      printf("\t\b\b\b- %ls\n", input[p]);

   imprime_array_str_unicode(input, 4);

   wchar_t input_a[] = {
      L'\U0001f414', L'}', L'\U0001f4d2', L'\U0001f3dd', L'\U0001f4b0',
      L'\U0001f4df', L'a', L'\U0001f4bf', L'\U0001f489', L'\U0001f419',
      L'é', L'+'
   };
   const int n = sizeof(input_a) / sizeof(wchar_t);

   puts("\nImpressão de caractéres Unicode:");
   imprime_array_char_unicode(input_a, n);
}

void aplicacao_randomica_de_cores_em_pequenas_strings(void)
{
   enum Cores colors[] = {
      Azul, Violeta, Verde, AzulMarinho, Amarelo, 
      Vermelho
   };
   const int N = sizeof(colors) / sizeof(enum Cores);

   for (int i = 0; i < FRUTAS; i++) {
      char* in = (char*)frutas[i];
      enum Cores cor = colors[i % N];
      char* out = colori_string(in, cor);

      printf(" - %-30s~ %s\n", out, frases_i[i]);
      free(out);
   }

   puts("\nOutro método de coloração aplicado(há um trade-off).");
   for (int i = 0; i < OBJETOS; i++) {
      char* in = (char*)objetos[i];
      enum Cores cor = colors[i % N];
      char* out = muda_cor_da_string(in, cor);

      printf(" - %-30s~ %s\n", out, frases_i[i]);
   }
}

void transformacao_de_array_int_em_string_via_pipes(void)
{
   int input[] = {-3, 2, -1, 0, 1, -2, 3, -4};
   const int n = sizeof(input) / sizeof(int);
   char* output = array_i32_to_str(input, n);

   printf("Impressão do que foi conseguido '%s'.\n", output);

}

void conversao_de_arrays_genericas_em_string_por_pipes(void)
{
   char* output_a, *output_b, *output_c;
   uint32_t input_a[] = {9, 99, 999, 9999, 9.9999e4, 9.99999e5};
   float input_b[] = {3.14, 0.1234, 1.41849123, 5.9};
   signed char input_c[] = {127, -128, 0, 1, 33};
   const int nA = sizeof(input_a) / sizeof(uint32_t);
   const int nB = sizeof(input_b) / sizeof(float);
   const int nC = sizeof(input_c) / sizeof(char);

   output_a = array_u32_to_str(input_a, nA);
   printf("Impressão de uma array(u32): '%s'\n", output_a);
   free(output_a);

   output_b = array_f32_to_str(input_b, nB);
   printf("Impressão de uma array(f32): '%s'\n", output_b);
   free(output_b);

   output_c = array_i8_to_str(input_c, nC);
   printf("Impressão de uma array(i8): '%s'\n", output_c);
   free(output_c);
}

void stringficacao_usando_macro_geral(void)
{
   char*    Out    = NULL;
   uint32_t In_a[] = {1, 2, 3, 4, 97, 98, 99, 100};
   int      In_a2[]= {-1, 2, -3, -4, 97, -98, 99, -100};
   float    In_b[] = {3.14, -0.23, 1.44, -5.9, 12.01, -7.99, 0.01, -501.07};
   double   In_b2[]= {3.14, -0.23, 1.44, -5.9, 12.01, -7.99, 0.01, -501.07};
   uint8_t  In_c[] = {127, 128, 0, 1, 33, 57, 23, 12};
   int8_t   In_c2[]= {127, -128, 0, -1, 33, -57, 23, -12};

   Out = array_to_string(In_a, 8);
   puts(Out); free(Out);
   Out = array_to_string(In_a2, 8);
   puts(Out); free(Out);
   Out = array_to_string(In_b, 8);
   puts(Out); free(Out);
   Out = array_to_string(In_b2, 8);
   puts(Out); free(Out);
   Out = array_to_string(In_c, 8);
   puts(Out); free(Out);
   Out = array_to_string(In_c2, 8);
   puts(Out); free(Out);
}

void coloracao_de_caracteres_checadores_unicode(void)
{
	StringColorido positivo = colori_string("\u2714", Verde);
	StringColorido negativo = colori_string("\u2718", Vermelho);
	char texto[UCHAR_MAX];

	sprintf(texto, "Avaliação positiva: %s\nAvaliação negativa: %s\n",
		positivo, negativo);
	puts(texto);
}

void listagem_generica_de_checagens(void) {
   struct Checagem obj_a = {"Armario cinza", true};
   struct Checagem obj_b = {"Mala de camisas", false};
   struct Checagem obj_c = {"Bolas", false};

   imprime_checagem(&obj_a, 40);
   imprime_checagem(&obj_b, 40);
   imprime_checagem(&obj_c, 40);

   struct ListaDeChecagem lista = nova_ldc();

   adiciona_ldc(&lista, &obj_a);
   adiciona_ldc(&lista, &obj_b);
   adiciona_ldc(&lista, &obj_c);

   visualiza_ldc(&lista);
}

int main(void) 
{
   setlocale(LC_CTYPE, "en_US.UTF-8");

   executa_testes_a(
     false, 6,
         primeiro_exemplo_da_funcao_de_listagem, true,
         checando_um_caractere_multibyte, false,
         impressao_de_varios_tipos_de_array, true,
         impressao_de_array_gigante_de_strings, true,
         impressao_de_outros_tipos_de_inteiros, true,
         impressao_de_strings_e_caracteres_unicode, true
   );

   executa_testes_b(
     true, 3,
         Unit(aplicacao_randomica_de_cores_em_pequenas_strings, true),
			Unit(coloracao_de_caracteres_checadores_unicode, true),
         Unit(listagem_generica_de_checagens, true)
   );

   executa_testes_b(
     false, 3,
         Unit(transformacao_de_array_int_em_string_via_pipes, true),
         Unit(conversao_de_arrays_genericas_em_string_por_pipes, true),
         Unit(stringficacao_usando_macro_geral, true)
   );
}
#endif
