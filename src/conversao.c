/* Aqui ficam a transformação de vários tipos de inteiros em bytes, em dois
 * principais tipos de ordem: little endian and big endian. Existe a versão
 * padrão também, que respeita a plataforma que ela está sendo chamada. 
 */
// Definições de todos dados e funções abaixo:
#include "conversao.h"
// Biblioteca padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <wchar.h>
#include <stdarg.h>

void print_array(uint8_t* array, int t)
{
   putchar('[');
   for (int i = 1; i <= t; i++)
      printf("%d, ", array[i - 1]);
   printf("\b\b]\n");
}

bool arrays_iguais(uint8_t* a, uint8_t* b, int t) {
/* O algoritmo consiste no seguinte, supõe que todos itens são iguais, caso
 * ache algum valor que não bate, então desconfirma hipotese inicial, caso 
 * contrário confirma. */
   #ifdef __debug__
   print_array(a, t);
   print_array(b, t);
   #endif 

   for (int i = 1; i <= t; i++)
   {
      if (a[i - 1] != b[i - 1])
         return false;
   }
   return true;
}

bool maquina_little_endian(void) {
/* Se o processador tem uma ordem "little endian", o valor de 11, por 
 * exemplo, só pode ter o byte relevante no começo, os outros três sendo 
 * iguais a zero. */
   uint8_t array_do_valor_onze[] = {0xB, 0x0, 0x0, 0x0};
   int valor_arbitrario = 11;
   int* pointer = &valor_arbitrario;
   uint8_t* array_de_bytes = (uint8_t*)pointer;

   return arrays_iguais(array_do_valor_onze, array_de_bytes, 4);
}

bool maquina_big_endian(void) {
/* Tenta verificar se a ordem dos bytes é "little endian", se não confirmar,
 * só pode ser o oposto, "big endian". Aqui não aplicarei a negativa do 
 * código acima, pelo contrário, copiarei ele, e só mudarei a array de 
 * comparação, prá justamente mostrar que tal algoritmo funciona. */
   uint8_t array_do_valor_onze[] = {0x0, 0x0, 0x0, 0xb};
   int valor_arbitrario = 11;
   int* pointer = &valor_arbitrario;
   uint8_t* array_de_bytes = (uint8_t*)pointer;

   return arrays_iguais(array_do_valor_onze, array_de_bytes, 4);

}

void int_to_bytes(int input, uint8_t* output) {
/* Assumindo que tal tem 4 bytes, então este é o tamanho da array trabalhada
 * neste algoritmo. A ordem é a da máquina, a transformação é direta, então
 * antes de utilizar o resultada aqui, apenas verifica, com as funções dadas
 * acima, se ela é little-endian ou big-endian. Os bytes capturados, via 
 * endereço da variável passada, serão copiadas para o buffer também passado
 * por argumento. */
   int size = sizeof(int);
   /* Pega endereço que armazena chuck de bytes do inteiro de 32-bits. */
   int* pointer = &input;
   // Converte o pointeiro(chuck) para bytes...
   uint8_t* bytes = (uint8_t*)pointer;

   // Copia tais bytes para o buffer de saída do escopo.
   memcpy(output, bytes, size);
}

int from_bytes_to_int(uint8_t* input) {
/*   Pega a array de 4 bytes que representa um inteiro, então converte ela 
 * para o respectivo inteiro. A array de bytes tem que representar a mesma
 * ordem da atual máquina(big ou little)-endian, se não o resultado pode 
 * ser indefinido com uma entrada não válida. 
 *
 *   A conversão é o mesmo método * que o algoritmo anterior, porém, ao 
 * inverso. Aqui o pointeiro de bytes serão passados, converteremos para um 
 * pointeiro do determinado tipo desta * função, e no fim, retornamos o 
 * valor que ele forma. */
   int* pointeiro = (int*)input;

   return *pointeiro;
}

/* Parece muito trivial para apenas um byte, entretanto, seria bastante
 * trabalhoso fazer isso toda vez. Sem falar, que por mais que pareça 
 * desenecessário, as vezes, apenas seguindo os demais, este tipo de 
 * raciocínio pode ser aplicado, então é melhor fazer aqui facilitar futuros
 * códigos, sem falar que serve meio como algum tipo de interface. */
void u8_to_bytes(uint8_t input, uint8_t* output)
   { output[0] = input; }

uint8_t from_bytes_to_u8(uint8_t* input)
   { return *input; }

void i8_to_bytes(int8_t input, uint8_t* output)
   { output[0] = (uint8_t)input; }

int8_t from_bytes_to_i8(uint8_t* input)
   { return (int8_t)*input; }

void u16_to_bytes(uint16_t input, uint8_t* output) {
/* Quase o mesmo algoritmo que é aplicado para o inteiro(padrão do * C, que 
 * é com sinal, e de 32-bits). O que muda aqui é a array de output tem que 
 * ter apenas dois bytes, e a entrada é um inteiro positivo de 2 bytes. */
   int size = sizeof(uint16_t);
   /* Pega endereço que armazena chuck de bytes do inteiro de 32-bits. */
   uint16_t* pointer = &input;
   // Converte o pointeiro(chuck de bytes) para equivalente de 8-bits...
   uint8_t* bytes = (uint8_t*)pointer;

   // Copia tais bytes para o buffer de saída do escopo.
   memcpy(output, bytes, size);
}

uint16_t from_bytes_to_u16(uint8_t* input) {
/* Apenas muda os tipos de pointeiros que está se convertendo... */
   uint16_t* pointeiro = (uint16_t*)input;
   return *pointeiro;
}

void i16_to_bytes(int16_t input, uint8_t* output) {
   int size = sizeof(int16_t);
   int16_t* pointer = &input;
   uint8_t* bytes = (uint8_t*)pointer;

   memcpy(output, bytes, size);
}

int16_t from_bytes_to_i16(uint8_t* input) {
   int16_t* pointeiro = (int16_t*)input;
   return *pointeiro;
}

/*   Com estas foram a primeira feita, porém com o nome original, não vale a
 * pena copiar o código, apenas fazer a chamadas internas com os mesmos
 * argumentos. Refazendo nesta parte só por um caso de consistências com 
 * os tipos precedentes. 
 *
 *   Note que 'int32_t' é só um apelido para 'int'. 
 */
void i32_to_bytes(int32_t input, uint8_t* output)
   { int_to_bytes(input, output);}

int32_t from_bytes_to_i32(uint8_t* input)
   { return from_bytes_to_int(input); }

void u32_to_bytes(uint32_t input, uint8_t* output) {
   int size = sizeof(uint32_t);
   uint32_t* pointer = &input;
   uint8_t* bytes = (uint8_t*)pointer;

   memcpy(output, bytes, size);
}

uint32_t from_bytes_to_u32(uint8_t* input) {
   uint32_t* pointeiro = (uint32_t*)input;
   return *pointeiro;
}

void u64_to_bytes(uint64_t input, uint8_t* output) {
   int size = sizeof(uint64_t);
   uint64_t* pointer = &input;
   uint8_t* bytes = (uint8_t*)pointer;

   memcpy(output, bytes, size);
}

uint64_t from_bytes_to_u64(uint8_t* input) {
   uint64_t* pointeiro = (uint64_t*)input;
   return *pointeiro;
}

void i64_to_bytes(int64_t input, uint8_t* output) {
   int size = sizeof(int64_t);
   int64_t* pointer = &input;
   uint8_t* bytes = (uint8_t*)pointer;

   memcpy(output, bytes, size);
}

int64_t from_bytes_to_i64(uint8_t* input) {
   int64_t* pointeiro = (int64_t*)input;
   return *pointeiro;
}

void sizet_to_bytes(size_t input, uint8_t* output) {
   int size = sizeof(size_t);
   size_t* pointer = &input;
   uint8_t* bytes = (uint8_t*)pointer;

   memcpy(output, bytes, size);
}

size_t from_bytes_to_sizet(uint8_t* input) {
   size_t* pointeiro = (size_t*)input;
   return *pointeiro;
}
/* === === === === === === === === === === === === === === === === === ==
 *                      Conversão de Tipos que
 *                   não sejam Inteiros, ou sequer
 *                      Primitivos Apenas
 *
 *   Uma coisa notável é o design de saída se alterou, o compatível com o
 * padrão acima não é muito bom para este tipo de serialização.
 * === === === === === === === === === === === === === === === === === ==*/
void bool_to_bytes(bool input, uint8_t* output)
/* Apenas converte valor lógico num byte, então usa função que armazena
 * bytes para terminar o serviço de serialização. */
   { u8_to_bytes((uint8_t)input, output); }


void double_to_bytes(double input, uint8_t* output) {
/* Converte tal decimal num inteiro que representa o mesmo tamanho de bytes,
 * então usa a serialização deste inteiro para serializar o valor decimal
 * de 64-bits. */
   static_assert(
      sizeof(uint64_t) == sizeof(double),
      "Inteiro errado que é equivalente ao decimal longo"
   );
   double* ptr_f64 = &input;
   uint64_t* pointer = (uint64_t*)ptr_f64; 
   u64_to_bytes(*pointer, output); 
}

double from_bytes_to_double(uint8_t* input)
{ 
/* Mesmo esquema da serialização, converte para o inteiro de mesmo número
 * de bytes, então usa seus bits, estes que são iguais, para uma realizar
 * uma transmutação entres os valores. */
   /* Converte bytes para determinaod inteiro novamente que foi inicialmente
    * convertido. */
   uint64_t valor = from_bytes_to_u64(input);
   /* Entretanto, tal inteiro representa na verdade um decimal de 64-bits,
    * logo pegamos seus mesmos bytes(bits), e reinterpletamos como 
    * tal valor decimal. */
   double* pointeiro = (double*)&valor;
   return *pointeiro;
}

void float_to_bytes(float input, uint8_t* output) {
   static_assert(
      sizeof(int) == sizeof(float),
      "'int' it doesn't seem to be equivalent to real value of 32-bits"
   );
   /* Reiterpletando os bytes do decimal de 32-bits como se fosse um inteiro
    * com o tanto de bytes equivalente.*/
   float* ptr_float = &input;
   int* pointer = (int*)ptr_float; 
   /* Evaluando estes bytes equivalentes, então convertendo, apesar de que
    * não resultará no mesmo valor(sem fração) serializado, isso é de total
    * irrelevância no processo final de deserialização. */
   int_to_bytes(*pointer, output); 
}

float from_bytes_to_float(uint8_t* input)
{ 
/* Mesmo algoritmo do 'double' acima, pois funciona! Apenas é claro, troca
 * os 'casts' pelo tipo e inteiro equivalente que trabalhamos agora. */
   int valor = from_bytes_to_int(input);
   float* pointeiro = (float*)&valor;
   return *pointeiro;
}

void free_bytes(struct Bytes* a)
/* Libera alocação dinâmica da estrutura acima que é o retorno de várias 
 * funções abaixo. */
   { free(a->bytes); }

struct Bytes string_to_bytes(char* input)
{
/*O requerimento principal para isso funcionar é, a array de otutput deve
 * ter, no mínimo, o tamanho da string(sem contar o caractére nulo). */
   size_t L = strlen(input);
   size_t sz = sizeof(char);
   const size_t sz_int = sizeof(size_t);
   struct Bytes result;
   /* Conversão do atual pointeiro para 8-bit inteiro sem sinal. */
   uint8_t* ptr = (uint8_t*)input;

   size_t Q = L + sz_int;
   // Alocando memória necessária...
   result.bytes = malloc(Q * sz);
   // Contabilizando os 8 bytes do tamanho enfurnado nos bytes da string.
   result.total = Q;

   // Serializando tal valor(comprimento da string)...
   uint8_t buffer[sz_int];
   sizet_to_bytes(L, buffer);

   // Copiando primeiro os bytes do tamanho da string...
   memcpy(result.bytes, buffer, sz_int);
   // Então copiando os bytes da string em si.
   memcpy(result.bytes + sz_int, ptr, L);

   return result;
}

char* from_bytes_to_string(struct Bytes* input)
{
   uint8_t* ptr = input->bytes;
   size_t t = from_bytes_to_sizet(ptr);
   char* ptr_str, *copia;
   size_t sz = sizeof(char);

   // Pula oito bytes referente ao 'tamanho' já decodificado.
   ptr += 8;
   ptr_str = (char*)ptr;
   // Alocando memória. O mais um é para colocar o caractére nulo.
   copia = malloc((t + 1) * sz);
   // Agora copiando ele para memória alocada.
   memcpy(copia, ptr_str, t);
   // Fincando o fim da string nela...
   copia[t] = '\0';

   return copia;
}

struct Bytes string_unicode_to_bytes(wchar_t* input)
{
   // Quantidade de bytes do inteiro de máquina, e do caractére Unicode.
   const size_t SzWchar = sizeof(wchar_t);
   const size_t SzInt = sizeof(size_t);
   // Total de caractéres da string, caractére nulo não contabilizado.
   size_t qC = wcslen(input);
   /* Total de bytes que a string Unicode possui, claro, não está contando
    * o caractére nulo. */
   size_t tB = SzWchar * qC; 
   /* Conversão do ponteiro de 'wide character' para ponteiro do inteiro 
    * positivo de 8-bits. */
   uint8_t* ptr = (uint8_t*)input;
   /* Tupla com serializaçao: tanto os bytes, como sua quantidade total. */
   struct Bytes result;
   /* Sequência dos bytes da estringue Unicode, e do comprimento dela. */
   uint8_t bytes_len[SzInt];
   uint8_t bytes_str[tB];

   // Copiando sequências de bytes para seus respectivos containers...
   sizet_to_bytes(qC, bytes_len);
   memcpy(bytes_str, ptr, tB);

   // Alocando e registrando o atual tanto de bytes...
   result.total = tB + SzInt;
   result.bytes = malloc(result.total);
   // Então copiando ambas sequências para a alocada.
   memcpy(result.bytes, bytes_len, SzInt);
   // Compuntando novo ponteiro a iniciar a cópia de bytes...
   ptr = result.bytes + SzInt;
   memcpy(ptr, bytes_str, tB);

   return result;
}

wchar_t* from_bytes_to_string_unicode(struct Bytes* input)
{
   uint8_t* ptr = input->bytes;
   wchar_t* string, *ptrSU;
   /* Quantidade de caractéres da estringue, e o tamanho, em bytes, de um 
    * caractére Unicode. */
   size_t qtd, sz = sizeof(wchar_t);
   // Total de bytes que a estringue contém.
   size_t tB;

   // Decodifica os primeiros 8 bytes e transforma num inteiro positivo.
   qtd = from_bytes_to_sizet(ptr);
   // Pula oito bytes referente ao 'tamanho' já decodificado.
   ptr += sizeof(size_t);
   /* Transforma ponteiro que indica o ínicio da estringue Unicode no 
    * equivalente ponteiro(poniteiro de wchar). */
   ptrSU = (wchar_t*)ptr;
   // Total de bytes que a estringue Unicode conterá, esta contabilizando
   // o caractére nulo.
   tB = (qtd + 1) * sz;
   // Alocando memória. O mais um é para colocar o caractére nulo.
   string = malloc(tB);
   // Agora copiando ele para memória alocada.
   memcpy(string, ptrSU, tB);
   // Fincando o fim da estringue nela...
   string[qtd] = L'\0';

   return string;
}

struct Bytes concatena_sequencias_de_bytes(int quantia, ...)
{
/*   Pega sequência N de bytes, não importa quantos, e quais valores tais
 * representam, e concatenam na ordem que foram postas seus parâmetros.
 * Os argumentos são pares na seguinte forma: sequência de bytes o tamanho
 * de tal. */
   va_list args;
   struct Bytes sequencias[UINT8_MAX];
   struct Bytes resultado;

   if (quantia >= UINT8_MAX)
   /* Não podem passar de mais de três centenas de argumentos. Se chegar
    * nisso um dia, provavelmente o programador está fazendo algo errado. */
      { perror("Excedeu limite de argumentos."); abort(); }

   // Colhendo os argumentos na ordem que foram filtrados, e armazenando-os.
   va_start(args, quantia);

   for (int i = 1; i <= quantia; i++) {
      uint8_t* pointer = va_arg(args, uint8_t*);
      size_t tamanho = va_arg(args, size_t);
      size_t p = i - 1;

      sequencias[p].bytes = pointer;
      sequencias[p].total = tamanho;
      resultado.total += tamanho;
   }
   va_end(args);

   // Aloca total de bytes a concatenar...
   resultado.bytes = malloc(resultado.total);
   uint8_t* pointer_dst = resultado.bytes;

   for (int j = 1; j <= quantia; j++) {
      int p = j - 1;
      size_t Q = sequencias[p].total;
      uint8_t* pointer_src = sequencias[p].bytes;

      /* Copia neste ponto do concatenador acumulador a certa quantia de 
       * bytes da sequência iterada. */
      memcpy(pointer_dst, pointer_src, Q);
      // Move-se para o próximo ponto de alocação...
      pointer_dst += Q;
   }
   return resultado;
}



#if defined(__UT_CONVERSAO__) && defined(__linux__)
/* === === === === === === === === === === === === === === === === === ==
 *                      Testes Unitários
 *
 *   A coisa agora é restringir a execução de testes unitários no sistema
 * que foi feito, porque na hora de criar, e corrigir pequenos erros, não
 * há incômodo de fazer isso multiplataforma. Isso é apenas com o teste,
 * as funções em sí são compatíveis com multiplas plataformas, se não forem
 * estarão marcadas assim no cabeçalho.
 * === === === === === === === === === === === === === === === === === ==*/
#include <locale.h>
#include <time.h>
#include <float.h>
#include <tgmath.h>
#include <sys/time.h>
#include "teste.h"

#define POT pow(10.0, 6)

void verificacao_da_atual_maquina(void) {
   #ifdef _WIN64
   setlocale(LC_CTYPE, "");
   bool resultado = maquina_little_endian();
   const wchar_t* resultstr = bool_to_str(resultado);

   wprintf(L"É está máquina 'little endian'? %ls\n", resultstr);

   resultado = maquina_big_endian();
   resultstr = bool_to_str(resultado);
   wprintf(L"É está máquina 'big endian'? %ls\n", resultstr);
   #elif defined(__linux__)
   if (maquina_big_endian())
      puts("Esta máquina tem uma ordem 'big-endian'.");
   else
      puts("Esta máquina tem uma ordem 'little-endian'.");
   #endif
}

void alguns_testes(void) {
   uint8_t bytes[] = {0xB, 0xE, 0xE, 0xF};
   int convertido = from_bytes_to_int(bytes);
   printf("BEEF ==> %d\n", convertido);

   uint8_t exemplo[] = {0xf, 0x0, 0x0, 0x0};
   convertido = from_bytes_to_int(exemplo);
   assert (convertido == 15);
   printf("[0xf, 0, 0, 0] ==> %d\n", convertido);

   printf("\ntamanho do 'time_t': %zu\n", sizeof(time_t));
   time_t timestamp = time(NULL);
   #ifdef _WIN64
   printf("%lld\n", timestamp);
   // #ifdef __linux__
   #elif defined(__linux__) 
   printf("%ld\n", timestamp);
   #endif
   uint8_t bytes_tt[8];
   sizet_to_bytes(timestamp, bytes_tt);
   print_array(bytes_tt, sizeof(time_t));
   time_t reconvertido_tt = from_bytes_to_sizet(bytes_tt);
   #ifdef _WIN64
   printf("reconvertendo novamente ==> %lld\n", reconvertido_tt);
   #elif defined(__linux__) 
   printf("reconvertendo novamente ==> %ld\n", reconvertido_tt);
   #endif
   assert(timestamp == reconvertido_tt);
}

void experimento_os_macros(void) {
   int16_t n = -5320;
   uint8_t bytes_n[2];
   to_bytes(n, bytes_n);
   print_array(bytes_n, 2);
   printf("reconvertido:%d\n", from_bytes_to_i16(bytes_n));
}

void converte_byte(void) {
   int8_t x = INT8_MIN + 8;
   uint8_t buffer[1];

   printf("x: %d\n", x);
   i8_to_bytes(x, buffer);
   print_array(buffer, 1);
   int8_t y = from_bytes_to_i8(buffer);
   printf("y: %d\n", y);
   assert (x == y);
}

void serializa_e_deserializa_string_ascii(void)
{
   char* s = {
      "Happiness, more less, it seems to change for my liberty "
      "... on my mind!"
   };

   struct Bytes result = string_to_bytes(s);
   printf(
      "Tentando verificar conteudo(%lu):\n %s\n", 
      result.total, result.bytes
   );
   print_array(result.bytes, result.total);

   char* S = from_bytes_to_string(&result);
   printf("Pós reconversão:\n\"%s\"\n", S);

   free_bytes(&result);
}

void serializa_e_deserializa_string_unicode(void)
{
   wchar_t* s = {
      L"Every the moment i need you at most, you keep your believe in a "
      L"magic lost,... you tell me the blue sky fade into gray, you tell "
      L"me your passion gone away,... and i don't need know anymore..."
      L"\n\n\t \U0001f385 \U0001F449 \U0001f454 \t\nAll the muscle titans "
      L"I wish you have principles,... in other side of the world to me"
   };

   struct Bytes result = string_unicode_to_bytes(s);
   print_array(result.bytes, result.total);

   wchar_t* S = from_bytes_to_string_unicode(&result);
   printf("Pós reconversão:\n\"%ls\"\n", S);

   free_bytes(&result);

   const wchar_t* t = {
      L"\"I wish principles\", acho que a tradução literal "
      L"é algo como 'eu espero princípios... de você', não completo, más "
      L"algo do tipo"
   };
   result = string_unicode_to_bytes((wchar_t*)t);
   wchar_t* T = from_bytes_to_string_unicode(&result);
   free_bytes(&result);

   printf("\nConteúdo:\t####\n\n%ls\n####\n", T);
}

// Computa o decorrer em segundos desta estrutura, dado o começo e o fim.
static double decorrido(struct timeval i, struct timeval f)
{
   /* Convertando cada equivalente em segundos. */
   double start = (double)i.tv_sec + (double)i.tv_usec / POT;
   double end = (double)f.tv_sec + (double)f.tv_usec / POT;

   return end - start;
}

void tempo_de_conversao(void) 
{
   const int Okay = 0;
   const wchar_t* amostra = {
      L"\U0001f3b5 \U0001f3b6 Salve à noite, \U0001f3b6 \U0001f3b5 me "
      L"levar com eles, \U0001f3b5 \U0001f3b6 os cuzões vem, para me levar "
      L"emborar, \U0001f3a7 eu queria muito ficar \U0001f3b5 \U0001f3b5"
   }; 
   wchar_t* pointer = (wchar_t*)amostra;
   const double qtd = (double)UINT16_MAX;
   double sT, dT, tT = 0.0;
   struct timeval start, end, inicio, fim;

   printf("Conteúdo trabalhado:\n%ls\n", amostra);
   gettimeofday(&inicio, NULL);

   for (int i = 1; i <= UINT16_MAX; i++)
   {
      // Serialização...
      gettimeofday(&start, NULL);
      struct Bytes a = string_unicode_to_bytes(pointer);
      gettimeofday(&end, NULL);
      sT += decorrido(start, end);

      // Deserialização...
      gettimeofday(&start, NULL);
      wchar_t* b = from_bytes_to_string_unicode(&a);
      gettimeofday(&end, NULL);
      dT += decorrido(start, end);

      assert(wcscmp(amostra, b) == Okay);
      free(b);
      free_bytes(&a);
   }
   gettimeofday(&fim, NULL);
   tT = decorrido(inicio, fim);

   printf(
      "\nCiclos de (S e D)/seg: %0.1lf\nMédia de serialização: %lfseg\n"
      "Média de deserialização: %lfseg\n", qtd / tT, (sT/ qtd), (dT / qtd)
   );
}
void serializacao_de_decimais_64_bits(void) {
   double samples[] = {
      0.0001, 54321.6789, 3.14915, 5.99, 
      999.99, DBL_MIN, DBL_MAX
   };
   int sz = sizeof(double);
   int n = sizeof(samples) / sz;
   uint8_t buffer[sz];

   for (int i = 0; i < n; i++) 
   {
      double A = samples[i];
      printf("Antes da conversão: %lf\n", A);
      double_to_bytes(A, buffer);
      print_array(buffer, sz);
      double a = from_bytes_to_double(buffer);
      printf("Reconvertido: %lf\n\n", a);
   }
}

void serializacao_de_decimais_32_bits(void) {
   float samples[] = {
      0.0009F, 54321.6789F, 3.14915F, 5.0099F, 
      999.99F, FLT_MAX
   };
   int sz = sizeof(float);
   int n = sizeof(samples) / sz;
   uint8_t buffer[sz];

   for (int i = 0; i < n; i++) 
   {
      double A = samples[i];
      printf("Antes da conversão: %f\n", A);
      double_to_bytes(A, buffer);
      print_array(buffer, sz);
      double a = from_bytes_to_double(buffer);
      printf("Reconvertido: %f\n\n", a);
   }
}

int main(void) {
   setlocale(LC_CTYPE, "");

   executa_testes_a(
      true, 9,
      verificacao_da_atual_maquina, true,
      alguns_testes, false,
      experimento_os_macros, true,
      converte_byte, false,
      serializa_e_deserializa_string_ascii, false,
      serializa_e_deserializa_string_unicode, false,
      tempo_de_conversao, false,
      serializacao_de_decimais_64_bits, true,
      serializacao_de_decimais_32_bits, true
   );
}
#endif
