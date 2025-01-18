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
enum InteiroPositivo {i8, u8, i16, u16, i32, u32, i64, u64};

static void imprime_array_tipos_de_int(void* array, int length, 
  enum InteiroPositivo tipo_de_int)
{
   assert(length >= 0 && array != NULL); 
   /* Números grandes de mais são totalmente desnecessário para o que isso
    * foi feito. */
   assert(length <= LIMITE);

   uint8_t* ptr_u8; int8_t* ptr_i8;
   uint16_t* ptr_u16; int16_t* ptr_i16;
   uint32_t* ptr_u32; int32_t* ptr_i32;
   uint64_t* ptr_u64; int64_t* ptr_i64;

   putchar('[');
   for (int p = 0; p < length; p++)
   {
      switch(tipo_de_int)
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
         default:
            perror("Tipo não compátivel!");
            abort();
      }
   }

   if (length == 0)
      puts("]");
   else
      puts("\b\b]");
}

void imprime_array_i8(int8_t* array, int length)
   { imprime_array_tipos_de_int(array, length, i8); }
   
void imprime_array_u8(uint8_t* array, int length)
   { imprime_array_tipos_de_int(array, length, u8); }

void imprime_array_i16(int16_t* array, int length)
   { imprime_array_tipos_de_int(array, length, i16); }

void imprime_array_u16(uint16_t* array, int length)
   { imprime_array_tipos_de_int(array, length, u16); }

void imprime_array_u32(uint32_t* array, int length)
   { imprime_array_tipos_de_int(array, length, u32); }

void imprime_array_i64(int64_t* array, int length)
   { imprime_array_tipos_de_int(array, length, i64); }

void imprime_array_u64(uint64_t* array, int length)
   { imprime_array_tipos_de_int(array, length, u64); }


/* === === === === === === === === === === === === === === === === === ===+
 * .......................................................................&
 * ........................Testes Unitários...............................&
 * .......................................................................&
 * === === === === === === === === === === === === === === === === === ===*/
#ifdef __unit_tests__
#include "dados_testes.h"
#include "teste.h"
#include <limits.h>
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

int main(void) 
{
   setlocale(LC_CTYPE, "en_US.UTF-8");

   executa_testes_a(
     true, 6,
         primeiro_exemplo_da_funcao_de_listagem, true,
         checando_um_caractere_multibyte, false,
         impressao_de_varios_tipos_de_array, true,
         impressao_de_array_gigante_de_strings, true,
         impressao_de_outros_tipos_de_inteiros, true,
         impressao_de_strings_e_caracteres_unicode, true
   );
}
#endif
