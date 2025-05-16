// Declaração de objetos e seus métodos/e funções abaixo:
#include "combinatoria.h"
#include "memoria.h"
// Biblioteca padrão do C:
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <threads.h>
// Biblioteca padrão do Linux(Unix):
#include <sys/time.h>


static void libera_lista_de_resultados(uint8_t** list, size_t n, Drop f)
{
/* Libera lista de pointeiro com um comprimento 'n', do tipo genérico que
 * pode ser liberado pela função 'f'. */
   for (size_t k = 0; k < n; k++)
      free(list[k]);
   free(list);
}

static uint8_t** cria_lista_de_array_de_bytes(size_t n)
/* O resultado é uma array de pointeiros de byte, não incializada. */
{ 
   uint8_t** lista_de_ponteiros;
   const int sz = sizeof(uint8_t*);

   lista_de_ponteiros = malloc(n * sz);
   return lista_de_ponteiros;
}

size_t fatorial(int n)
{
// De modo recursivo retorna o fatorial de n(n!).
   // assert(n >= 1 && n <= 33);

   if (n >= 2)
      return n * fatorial(n - 1);
   else
      return 1;
}

void free_resultados_combinatorios(ResultadosCombinatorios* X, Drop g)
   { libera_lista_de_resultados(X->results, X->total, g); }

/* === === === === === === === === === === === === === === === === === ==
 *                      Geração de Permutações
 * === === === === === === === === === === === === === === === === === ==*/
static void algoritmo_gerador_de_permutas(Permutas* obj, int M)
{
 /*   A legenda dos parâmetors: S é a array que será amplamente modificada
 * durante o processo; já 'm' é o tamanho relativo, indica até que parte
 * desta array estamos alternando as posições; 'n' é o tamanho real da
 * array; 'out' é a lista que "concatena" as permutas; já 'q' é o cursor
 * que ajuda a contabilizar, e a adiciona as permutas geradas. */
   int sz       = obj->size;
   uint8_t* seq = obj->items;
   int t        = obj->length;
   size_t n     = obj->total;

   if (M == 1) {
      obj->results[n] = clona_array(seq, sz, t);
      obj->total = n + 1;

   } else {
      uint8_t* a = NULL, *b = NULL;

      for (int p = 0; p < M; p++)
      {
         algoritmo_gerador_de_permutas(obj, M - 1);

         if (M % 2 == 1)
         {
            a = seq + 0*sz;
            b = seq + (M - 1) * sz;

         } else {
            a = seq + p * sz;
            b = seq + (M - 1) * sz;
         }
         alterna(a, b, sz);
      }
   }
}

Permutas gera_permutacoes(generico_t seq, int n, int sz)
{
/*   Neste caso não é preciso passar uma tupla, apenas a array, seu
 * comprimento, e o tanto de bytes do seu tipo. */
   size_t t = fatorial(n);
   Permutas output;
   const int Q = sizeof(uint8_t*);
   uint8_t** list = (uint8_t**)malloc(t * Q);

   for (size_t p = 0; p < t; p++)
      list[p] = NULL;

   output = (Permutas) {
      .items   = seq,
      .length  = n,
      .size    = sz,
      .total   = 0,
      .results = list,
      // Desnecessários para este tipo de resultado:
      .k = -1,
   };

   algoritmo_gerador_de_permutas(&output, n);
   printf("Foi processado: %zu\n", output.total);

   return output;
}


/* === === === === === === === === === === === === === === === === === ==
 *                      Geração de Arranjos
 *
 *   O arranjo é simplesmente a permuta, porém com menos entradas. Logo,
 * todo escopo para gerar a outra pode ser utilizado com uma simples
 * modificação no argumento utilizado.
 * === === === === === === === === === === === === === === === === === ==*/
static bool contem_sequencia(uint8_t** L, int t, generico_t seq, int n, 
  int sz)
{
/*   Pega uma lista de sequências(list), com tamanho até então de 't', e 
 * verifica então se a array(seq) -- com o mesmo comprimento, do tipo 'sz'
 * (seu tamanho em bytes), já pertence a ela com uma iteração linear. */
   uint8_t* seq_a = (uint8_t*)seq;
   uint8_t* seq_b = NULL;

   /* Itera todas arrays na lista, e verifica uma por uma se a 'seq' passada
    * é igual, retornando se alguma confirmou, porque o final da iteração
    * sem qualquer disparo, confirma que ela não pertence a lista passada. 
    */
   for (int p = 0; p < t; p++)
   {
      seq_b = L[p];

      if (memcmp(seq_a, seq_b, n * sz) == 0)
      // Se acontecer de uma ser igual, confirma que contém tal sequência.
         return true;
   }
   /* Se chegar até aqui, sem confirmação de alguma, então não existe tal
    * sequência na lista. */
   return false;
}

static void algoritmo_gerador_de_arranjos(generico_t seq, int n, int k, 
  uint8_t** list, size_t* t, int m, int sz)
{
/*   O algoritmo aqui é quase o mesmo das permutações, más com algumas 
 * modificações importantes. A primeira, existe mais um parâmetro, aqui 
 * posta propositalmente depois do comprimento real das array(seq). Ele 
 * indica que não é necessário gerar uma permuta com o mesmo comprimento
 * da original, apenas 'k' itens. Este obviamente não é o algoritmo certo
 * para gerar tipo de coisa, porém, serve com um bom quebra-galho. Aqui 
 * delimitamos, e também diferenciamos do algoritmo original numa busca 
 * por se a sequência, de tamanho 'k', gerada já foi posta na lista. Sim,
 * adicionamos uma complexidade O(t*k) no algoritmo original, 't' pela
 * varredura linear, e 'k' pela verradura de comparação entre as arrays,
 * aqui feita com 'memcmp'.
 *
 *   Infelizmente tal algoritmo falha quando a array(seq) tem itens
 * repetidos. Neste caso, não serão gerados todos os arranjos possíveis,
 * porque simplesmente a parte que verifica se contém tal resultado, irá
 * rejeitar novas permutas iguais.
 *
 *   A legenda dos parâmetros é a seguinte: a array(seq) genérica, e seu 
 * comprimento(n); o valor 'k' que é o comprimento que cada nova sequência
 * será gerada; a lista de bytes(list) que guarda seus resultados, e claro,
 * o seu cursor(t) que acompanha tal "concatenação" nela; por último, o
 * cursor da recursão(m[uma propriedade do algoritmo]), o tipo de array(sz),
 * aqui é o número de bytes do tipo dela.
 *
 */
   if (m == 1) {
      uint8_t* copia = clona_array(seq, sz, k);

      if (!contem_sequencia(list, *t, seq, k, sz)) {
      // Adiciona e incrementa a contagem de permutas(sequẽncias).
         list[*t] = copia;
         *t += 1;
      } else
      // Apenas libera caso já existe na lista tal sequência gerada.
         free(copia);

   } else {
      uint8_t* a = NULL, *b = NULL;

      for (int p = 0; p < m; p++)
      {
         algoritmo_gerador_de_arranjos(seq, n, k, list, t, m - 1, sz);

         if (m % 2 == 1)
         {
            a = seq + 0*sz;
            b = seq + (m - 1) * sz;

         } else {
            a = seq + p * sz;
            b = seq + (m - 1) * sz;
         }
         alterna(a, b, sz);
      }
   }
}

static size_t calcula_arranjos(int n, int p)
// O total de arranjos dado o número de itens e quanto tomar de cada.
   { return fatorial(n) / (fatorial(n - p)); }

static bool sem_itens_repetidos(generico_t array, int n, int sz)
{
   uint8_t* ptr = (uint8_t*)array;
   uint8_t* seq_a, *seq_b;

   for (int i = 0; i < (n - 1); i++)
   {
      for (int p = i + 1; p < n; p++)
      {
         /* O 'a' será colocada aqui mesmo, e sim, é fazer uma operação 
          * com o mesmo resultado um monte de vezes, porém como é uma vez
          * realizada, e seu tamanho não é tão grande, a perda de performace
          * não é relevante. 
          */
         seq_a = ptr + i * sz;
         seq_b = ptr + p * sz;

         if (memcmp(seq_a, seq_b, sz) == 0)
            return false;
      }
   }
   return true;
}

static void condicoes_necessarias_sobre_algoritmo_do_arranjo
  (generico_t array, int n, int k, int sz)
{
/* Todas condições necessárias para execução do algoritmo. Se não forem 
 * seguidas corretamente, então o programa provalvemente quebra, e manda
 * uma informação sobre o error. */
   if (k > n) {
      perror("Valor de agrupamento inválido, não pode ser maior");
      abort();
   } else if (k == n) {
      perror("Neste caso é melhor usar a função 'gera_permutacoes'.");
      abort();
   } else if (!sem_itens_repetidos(array, n, sz)) {
      perror("Apenas array com itens distintos permitida.");
      abort();
   }
}

Arranjos gera_arranjos(generico_t array, int n, int k, int sz)
{
 /*   Gera um arranjo, sem repetição, de todos itens na 'array' com 
  * comprimento 'length', estes tomados de 'k' à 'k'.
  *
  * Nota: não é aceito um 'k' maior ou igual a 'n'. O primeiro obviamente é
  *       um erro, porque é inválida com a propriedade matemática; já o
  *       último, porque geraria o mesmo resultado do gerador original de 
  *       permutações, assim fica meio sem sentido usar este. Este segundo
  *       geraria o resultado perfeitamente, porém com uma carga extra de
  *       tempo, portanto não vale a pena. */
  condicoes_necessarias_sobre_algoritmo_do_arranjo(array, n, k, sz);
   
   size_t t = calcula_arranjos(n, k);
   Arranjos output = {
      .items = array,
      .length = n, .k = k, 
      .size = sz,
      .results = cria_lista_de_array_de_bytes(t),
      .total = 0
   };

   algoritmo_gerador_de_arranjos(
      output.items, output.length, output.k, 
      output.results, &output.total, n, sz
   );
   assert(output.total == t);
   return output;
}


/* === === === === === === === === === === === === === === === === === ==
 *                      Geração de Combinações
 * === === === === === === === === === === === === === === === === === ==*/
static size_t computa_combinacoes(int n, int k)
   { return fatorial(n) / (fatorial(n - k) * fatorial(k)); }

static void gerador_de_combinacao_generica(generico_t items, int n, 
  generico_t seq, int* m, uint8_t** list, size_t* p, int i, int k, int sz)
{
   if (*m == k) {
      list[*p] = clona_array(seq, sz, k);
      *p += 1;
      return;
   }

   uint8_t* seq_ptr, *items_ptr;

   for (int a = i; a < n; a++)
   {
      /*   Operação de atribuição, mais para pointeiros 'void': 
       *
       *                   seq[*m] = items[a];
       */
      seq_ptr   = (uint8_t*)seq + *m;
      items_ptr = (uint8_t*)items + a;

      memcpy(seq_ptr, items_ptr, sz);
      *m += 1;

      gerador_de_combinacao_generica(
         // Array e seus comprimentos
         items, n, seq, m, list, p, 
         // Cursores e tamanho em bytes.
         a + 1, k, sz
      );
      *m -= 1;
   }
}

Combinacoes gera_combinacoes(generico_t itens, int n, int k, int sz)
{
/* Encapsulando vários parâmetros de contagem, apenas retorna a lista 
 * de combinações geradas e sua contagem. Também os itens usados, seu 
 * comprimento e tipo de array, entretanto, é só um modo para identificar
 * tal tupla de forma aleatória. */
   generico_t buffer = calloc(k, sz);
   size_t N = computa_combinacoes(n, k);
   uint8_t** L = cria_lista_de_array_de_bytes(N);
   int m = 0; size_t p = 0;
   Combinacoes output; 

   gerador_de_combinacao_generica(itens, n, buffer, &m, L, &p, 0, k, sz);
   output = (Combinacoes) {
      .items = itens,
      .length = n, .k = k, .size = sz,
      .results = L,
      .total = N,
   };
   // É necessário que todas combinações tenham sido geradas.
   assert (output.total == p);
   // Não será mais utilizado, logo libera, e atribui nulo ao ponteiro.
   free(buffer);

   return output;
}

#if defined(__unit_tests__) && defined(__linux__)
/* === === === === === === === === === === === === === === === === === ==
 *                      Testes Unitários
 * === === === === === === === === === === === === === === === === === ==*/
#include <locale.h>
#include <time.h>
#include <assert.h>
#include <float.h>
#include "teste.h"
// Biblioteca externa:
#include "impressao.h"

static bool free_struct(Generico dt)
{ 
   if (dt == NULL)
      return false;
   else 
      { free(dt); return true; }
}

void maximo_potencial_do_fatorial(void)
{
   int inputs[] = {
      0, 1, 2, 3, 4, 5, 6, 10, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
      30, 31, 32, 33, 40, 50, 60, 70
   };
   const int N = sizeof(inputs) / sizeof(int);

   puts(
      "Parece que o limite do fatorial é 21|22, daí em diante o inteiro "
      "começar 'wrapping around' este valor. "
   );
   for (int k = 0; k < N; k++)
      printf("\t\b\b%3d! = %30zu\n", inputs[k], fatorial(inputs[k]));
}

void swap_de_ponteiros_void(void)
{
   char* a = "morango";
   char* b = "abacaxi";
   const int sz = sizeof(char);
   int c[] = {1, 3, 5};

   printf("a: '%s'\nb: '%s'\n", a, b);
   alterna(&a, &b, sz);
   printf("\na: '%s'\nb: '%s'\n", a, b);

   puts("\nAgora alternando array(int):");
   imprime_array(c, 3);
   alterna(&c[1], &c[2], sz);
   imprime_array(c, 3);
   alterna(&c[1], &c[0], sz);
   imprime_array(c, 3);
}

void array_inteira_permutacao(void)
{
   int** L = malloc(6 * sizeof(int*));
   Permutas out = {
      .items = (int[]){1, 5, 10}, .length = 3, 
      .size = sizeof(int), .results = (uint8_t**)L, 0
   };

   puts("Resultado de simples permutação:");
   algoritmo_gerador_de_permutas(&out, 3);
   printf("Contabilização: %zu\n", out.total);

   for (int i = 0; i < out.total; i++)
      imprime_array((int*)out.results[i], 3);
   free_resultados_combinatorios(&out, free_struct);
}

void permuta_de_pequenos_genericos(void)
{
   Permutas inputs[] = {
      {
         .items   = (int[]){1, 2, 3, 4},
         .length  = 4,
         .size    = sizeof(int),
         .results = cria_lista_de_array_de_bytes(fatorial(4)),
         .total   = 0
      },
      {
         .items   = (float[]){0.1234, 3.14159, 2.71, 0.999999},
         .length  = 4,
         .size    = sizeof(float),
         .results = cria_lista_de_array_de_bytes(fatorial(4)),
         .total   = 0
      },
      {
         .items   = (char[]){'b', 'A', 'r', 'C', 'o', '\0'},
         .length  = 5,
         .size    = sizeof(char),
         .results = cria_lista_de_array_de_bytes(fatorial(5)),
         .total   = 0
      },
      {
         .items   = (bool[]){true, false, true},
         .length  = 3,
         .size    = sizeof(bool),
         .results = cria_lista_de_array_de_bytes(fatorial(3)),
         .total   = 0
      },
   };
   const int n = sizeof(inputs) / sizeof(Permutas);

   for (int i = 0; i < n; i++)
      algoritmo_gerador_de_permutas(&inputs[i], inputs[i].length);

   for (int t = 0; t < inputs[0].total; t++)
      imprime_array((int*)inputs[0].results[t], inputs[0].length);

   for (int t = 0; t < inputs[1].total; t++)
      imprime_array((float*)inputs[1].results[t], inputs[1].length);

   for (int t = 0; t < inputs[2].total; t++)
      imprime_array((char*)inputs[2].results[t], inputs[2].length);

   for (int t = 0; t < inputs[3].total; t++)
      imprime_array((bool*)inputs[3].results[t], inputs[3].length);

   printf("Liberando ...");
   for (int i = 0; i < 4; i++)
      free_resultados_combinatorios(&inputs[i], free_struct);
   puts("feito");
}

void clonagem_de_void_arrays(void)
{
   generico_t g;

   int in_a[] = {9, 99, 999, 9999};
   g = in_a;
   generico_t mid_a = clona_array(g, sizeof(int), 4);
   int* out_a = (int*)mid_a;

   imprime_array(out_a, 4);
   free(out_a);

   float in_b[] = {0.99, 9.99, 99.99, 999.99};
   g = in_b;
   generico_t mid_b = clona_array(g, sizeof(float), 4);
   float* out_b = mid_b;

   imprime_array(out_b, 4);
   free(out_b);

   char in_c[] = {'b', 'A', 'r', 'C', 'o'};
   g = in_c;
   generico_t mid_c = clona_array(g, sizeof(char), 5);
   char* out_c = mid_c;

   imprime_array(out_c, 5);
   free(out_c);

   bool in_d[] = {true, false};
   g = in_d;
   generico_t mid_d = clona_array(g, sizeof(bool), 2);
   bool* out_d = mid_d;

   imprime_array(out_d, 2);
   free(out_d);
}

void funcao_principal_de_geracao_de_permutas(void)
{
   char in_a[] = {'g', 'I', 'R', 'o'};
   int in_b[] = {0 , 1};
   float in_c[] = {3.14159, 1.6, 0.4321};
   char* in_d[] = {"dado", "bola", "petéca"};
   Permutas out_a , out_b, out_c, out_d;

   out_a = gera_permutacoes(in_a, 4, sizeof(char));
   out_b = gera_permutacoes(in_b, 2, sizeof(int));
   out_c = gera_permutacoes(in_c, 3, sizeof(float));
   out_d = gera_permutacoes(in_d, 3, sizeof(char*));

   for (int i = 1; i <= out_a.total; i++)
      imprime_array((char*)out_a.results[i - 1], out_a.length);

   for (int i = 1; i <= out_b.total; i++)
      imprime_array((int*)out_b.results[i - 1], out_b.length);

   for (int i = 1; i <= out_c.total; i++)
      imprime_array((float*)out_c.results[i - 1], out_c.length);

   for (int i = 1; i <= out_d.total; i++)
      imprime_array((char**)out_d.results[i - 1], out_d.length);

   free_resultados_combinatorios(&out_a, free_struct);
   free_resultados_combinatorios(&out_b, free_struct);
   free_resultados_combinatorios(&out_c, free_struct);
   free_resultados_combinatorios(&out_d, free_struct);
}

void tamanho_de_alguns_ponteiros_tipos(void)
{
   printf(
      "(uint8_t*): %zu bytes\n(ptrdiff_t*): %zu bytes\n" "(double*): %zu"
      " bytes\n(float*): %zu bytes\n(int16_t*): %zu bytes\n",
      sizeof(uint8_t*), sizeof(ptrdiff_t*), sizeof(double*), sizeof(float*),
      sizeof(int16_t*)
   );
}

static void gerador_de_combinacao_int(int* itens, int n, int* seq, int *m,  
  uint8_t** results, int *t, int i, int k)
{
   const int sz = sizeof(int);

   if (k == *m) {
      int* screenshot = malloc(k * sz);

      memcpy(screenshot, seq, k * sz);
      results[*t] = (uint8_t*)screenshot;
      *t += 1;

      return;
   }

   for (int q = i; q < n; q++)
   {
      seq[*m] = itens[q];
      *m = *m + 1;
      gerador_de_combinacao_int(itens, n, seq, m, results, t, q + 1, k);
      *m -= 1;
   }
}

void geracao_de_combinacoes_prototipo(void)
{
   int t = 0, t1 = 0;
   const int sz = sizeof(int*);
   int array[] = {1, 2, 3, 4};
   int buffer[4];
   size_t Cnk = computa_combinacoes(4, 2);
   uint8_t** list =  (uint8_t**)calloc(Cnk, sz);

   printf("Total de combinações a serem gerada é %zu.\n", Cnk);
   gerador_de_combinacao_int(array, 4, buffer, &t, list, &t1, 0, 2);

   for (int i = 0; i < Cnk; i++)
      imprime_array((int*)list[i], 2);
   libera_lista_de_resultados(list, Cnk, free_struct);
}

void teste_do_gerador_completo_de_combinacoes(void)
{
   char in[] = {'a', 'B', 'c', 'D', 'e'};
   const int sz = sizeof(char);
   const int N = sizeof(in) / sz;
   const int K = 3;

   Combinacoes out = gera_combinacoes(in, N, K, sz);

   printf("Total de combinações geradas: %zu\n", out.total);
   for (size_t p = 0; p < out.total; p++)
      imprime_array((char*)out.results[p], out.k);
   free_resultados_combinatorios(&out, free_struct);

   int8_t in_a[] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};
   Combinacoes out_a = gera_combinacoes(in_a, 9, 7, sizeof(int8_t));

   printf("\nOutro tipo de dado. Total: %zu combinações\n", out_a.total);
   for (size_t p = 0; p < out_a.total; p++)
      imprime_array((int8_t*)out_a.results[p], out_a.k);
   free_resultados_combinatorios(&out_a, free_struct);
}

static bool arrays_i8_iguais(int8_t* a, int8_t* b, int n)
{
   for (int i = 0; i < n; i++)
   {
      if (a[i] != b[i])
         return false;
   }
   return true;
}

static bool verificando_igualdade_entre_suas_saidas(Combinacoes* in)
{
   size_t      eq = 0, ne = 0;
   size_t      n  = in->total;
   uint8_t** list = in->results;

   for (size_t k = 0, q = 0; k < n; k++)
   {
      for (size_t p = k + 1; p < n; p++, q++)
      {
         int8_t* a = (int8_t*)list[k];
         int8_t* b = (int8_t*)list[p];

         if (arrays_i8_iguais(a, b, n)) 
            eq++;
         else
            ne++;
      }

      if (q % 60 == 0)
         printf("Quantia de pares analizada: %zu\n", q);
   }
   printf("\nIguais: %zu\tDiferentes: %zu\n", eq, ne);
   return eq == 0;
}

static void escolhe_algumas_combinacoes_i8(Combinacoes* in)
{
   uint8_t** list = (*in).results;
   size_t t = (*in).total;
   int8_t* array = NULL;
   int k = (*in).k;

   for (int i = 1; i <= 10; i++) {
      int z = (i * 13) % t;
      array = (int8_t*)list[z];
      imprime_array(array, k);
   }
}

void checando_que_combinacoes_nao_sao_iguais(void)
{
   signed char in[] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};
   const int sz = sizeof(signed char);
   const int N = sizeof(in) / sz, K = 3;
   Combinacoes out = gera_combinacoes(in, N, K, sz);

   assert(verificando_igualdade_entre_suas_saidas(&out));
   escolhe_algumas_combinacoes_i8(&out);
   free_resultados_combinatorios(&out, free_struct);
}

void tentando_gerar_anagramas_com_motor_de_permutas(void)
{
   char array[] = {'a', 'b', 'c', 'd'};
   const int sz = sizeof(char), P = 2, pA = 3, pB = 1;
   const int n = sizeof(array) / sz;
   int Anp = calcula_arranjos(n, P);
   uint8_t** list = calloc(fatorial(n), sizeof(uint8_t*));
   size_t t = 0;

   algoritmo_gerador_de_arranjos(array, n, P, list, &t, n, sz);
   printf("\nn: %d\tP: %d\nA(n, p): %d\tt: %zu\n\n", n, P, Anp, t);

   for (int i = 1; i <= t; i++)
   {
      uint8_t* L = list[i - 1];
      assert (L != NULL);
      printf("%2dª). ", i);
      imprime_array((char*)L, P);
   }
   libera_lista_de_resultados(list, fatorial(n), free_struct);

   puts("\nGerando agora para arranjo diferente ...");
   t = 0;
   Anp = calcula_arranjos(n, pA);
   uint8_t** list_a = calloc(Anp, sizeof(uint8_t*));

   algoritmo_gerador_de_arranjos(array, n, pA, list_a, &t, n, sz);
   assert(t == Anp);
   printf("A(%d, %d): %d\n", n, pA, Anp);

   for (int i = 1; i <= t; i++)
   {
      uint8_t* L = list_a[i - 1];
      printf("%2dª). ", i);
      imprime_array((char*)L, pA);
   }
   libera_lista_de_resultados(list_a, Anp, free_struct);

   puts("\nAgora com apenas P=1(o esperado é total igual a 4) ...");
   t = 0;
   Anp = calcula_arranjos(n, pB);
   uint8_t** list_b = calloc(Anp, sizeof(uint8_t*));

   algoritmo_gerador_de_arranjos(array, n, pB, list_b, &t, n, sz);
   assert(t == Anp);
   printf("A(%d, %d): %d\n", n, pB, Anp);

   for (int i = 1; i <= t; i++)
   {
      uint8_t* L = list_b[i - 1];
      printf("%2dª). ", i);
      imprime_array((char*)L, pB);
   }
   libera_lista_de_resultados(list_b, Anp, free_struct);
}

void gerador_de_arranjo_completo(void)
{
   char in[] = "cebo";
   const int sz = sizeof(char);
   Arranjos out = gera_arranjos(in, strlen(in), 3, sz);

   for (size_t p = 0; p < out.total; p++)
      imprime_array((char*)out.results[p], out.k);
   free_resultados_combinatorios(&out, free_struct);

   char in_a[] = "trem";
   Arranjos out_a = gera_arranjos(in_a, strlen(in_a), 2, sz);

   for (size_t p = 0; p < out_a.total; p++)
      imprime_array((char*)out_a.results[p], out_a.k);
   free_resultados_combinatorios(&out_a, free_struct);

   int in_b[] = {8, 7, 1, 3, 4, 5, 2};
   const int szB = sizeof(int);
   const int nB = sizeof(in_b) / szB;
   Arranjos out_b = gera_arranjos(in_b, nB, 2, szB);

   for (size_t p = 0; p < out_b.total; p++)
      imprime_array((int*)out_b.results[p], out_b.k);
   free_resultados_combinatorios(&out_b, free_struct);
}


int main(int qtd, char* args[], char* envs[])
{
   executa_testes_b(
     true, 7,
         // Testes de features totalmente fora do escopo deste módulo:
         Unit(swap_de_ponteiros_void, true),
         Unit(maximo_potencial_do_fatorial, true),
         // [off] a saída de texto é gigante
         Unit(permuta_de_pequenos_genericos, false),
         // [off] Testes rápidos de funções auxiliares bobas:
         Unit(clonagem_de_void_arrays, false),
         // [off] É um teste da linguagem:
         Unit(tamanho_de_alguns_ponteiros_tipos, false),
         // ... continuando ...
         Unit(array_inteira_permutacao, true),
         Unit(funcao_principal_de_geracao_de_permutas, true)
   );

   executa_testes_b(
     true, 3,
         Unit(teste_do_gerador_completo_de_combinacoes, true),
         Unit(checando_que_combinacoes_nao_sao_iguais, true),
         // [off] Teste de algoritmo apenas:
         Unit(geracao_de_combinacoes_prototipo, false)
   );

   executa_testes_b(
     true, 2,
       Unit(tentando_gerar_anagramas_com_motor_de_permutas, true),
       Unit(gerador_de_arranjo_completo, true)
   );
}
#endif
