#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
// Da própria biblioteca:
#include "definicoes.h"
#include "macros.h"
#include "legivel.h"
#include "memoria.h"
#include "combinatoria.h"
#include "filaligada_ref.h"
// Biblioteca padrão do Linux(Unix):
#include <threads.h>
#include <sys/time.h>

// Máximo da array até sua reinicialização.
#define MAX UCHAR_MAX
const int PAD = 5;

/* === === === === === === === === === === === === === === === === === === =
 *                Estrutura de Colhimento de Dados
 * === === === === === === === === === === === === === === === === === === */
struct InformacaoDoProcessamento {
   /* Informacao dos parâmetros que se processa. */
   Generico seq; int length; int size;
   FilaLigada queue;

   // Total de permutas que serão geradas.
   size_t total;

   // Registro de cada quantia ao longo do tempo.
   size_t quantia[MAX];

   // Cada novo registro de tempo feito. 
   struct timeval tempo[MAX];

   // Mémoria total que o algoritmo acumula ao longo do tempo.
   size_t memoria[MAX];

   // Proprietário que decide quando acessar qualquer um dos recursos acima.
   mtx_t dono;

   // Cursor que acompanha a inserção de cada registro.
   int cursor;
};
// Apelido mais curto para estrutura acima.
typedef struct InformacaoDoProcessamento IDP;

// ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~

static size_t delta_quantia(IDP* obj) {
   size_t cursor = obj->cursor;

   if (cursor >= 2) {
      size_t q = cursor;
      size_t qa = obj->quantia[q - 1];
      size_t qb = obj->quantia[q - 2];

      return qa - qb;
   }
   return 0;
}

static double struct_timeval_to_seg(struct timeval e)
   { return (double)e.tv_sec + (double)e.tv_usec / 1.0e6; }

static double delta_tempo_seg(IDP* obj)
{
   size_t cursor = obj->cursor;
   double diferenca = 0.0;

   if (cursor >= 2) { 
      size_t q = cursor;
      struct timeval a, b;
      double ta, tb;

      a = obj->tempo[q - 1];
      b = obj->tempo[q - 2];
      ta = struct_timeval_to_seg(a);
      tb = struct_timeval_to_seg(b);
      diferenca = ta - tb;

   }
   return diferenca;
}

static size_t delta_memoria(IDP* obj)
{
   int cursor = (*obj).cursor;

   if (cursor >= 2) 
   {
      size_t q = cursor;
      size_t ma = obj->memoria[q - 1];
      size_t mb = obj->memoria[q - 2];

      return ma - mb;
   }
   return obj->memoria[0];
}

static size_t memoria_atual(IDP* obj)
{
   // Realizar uma cópia para evitar 'data race'.
   int cursor = obj->cursor;

   if (cursor >= 1 && cursor <= (MAX - 1))
      return obj->memoria[cursor - 1];
   return 0;
}

static double percentual_do_processamento(IDP* obj)
{
   int cursor = (*obj).cursor;

   if (cursor >= 1) 
   {
      size_t q = cursor;
      double a = obj->quantia[q - 1];
      double b = obj->total;

      return 100.0 * (a / b);
   }
   return 0.0;
}

static void zera_todas_arrays_internas(IDP* obj) {
   const int sz_a = sizeof(size_t);
   const int sz_b = sizeof(struct timeval);

   memset(obj->quantia, 0x0, sz_a * MAX);
   memset(obj->memoria, 0x0, sz_a * MAX);
   memset(obj->tempo,   0x0, sz_b * MAX);
}

// ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~
IDP new_idp(Generico seq, int n, int sz, FilaLigada q) {
   IDP obj;
   int result;

   result = mtx_init(&obj.dono, mtx_plain);
   assert(result == thrd_success);
   zera_todas_arrays_internas(&obj);

   obj.total = fatorial(n);
   obj.seq = seq;
   obj.length = n;
   obj.size = sz;
   obj.queue = q;
   obj.cursor = 0;
   return obj;
}

void debug_idp(IDP* obj) {
/* Visualização simples dos dados que estão sendo monitorados.*/
   const char* cabecalho = stringfy(struct InformacaoDoProcessamento); 

   size_t memoria = delta_memoria(obj);
   char* memoria_str = tamanho_legivel(memoria);
   size_t memoria_total = memoria_atual(obj);
   char* memoria_str_i = tamanho_legivel(memoria_total);

   const char* RECUO ="\t\b\b";
   /* Assumindo que, a variação de quantia de um registro para outro é 
    * constante, então o que se faz 8 milisegundos, será feita 125 vezes
    * mais em um segundo inteiro. */
   size_t total = delta_quantia(obj) * 125;
   char* total_str = valor_legivel(total);

   double tempo = delta_tempo_seg(obj);
   char* tempo_str = tempo_legivel(tempo);

   printf(
      "\n%s{\n%sMemória: %s ~%s\n%sQuantidade: %s perm./seg\n%sTempo: %s"
      "\n%sCursor: %d\n%sPercentual: %3.1lf%%\n}\n", 
      cabecalho, RECUO, memoria_str, memoria_str_i, RECUO, total_str, RECUO, 
      tempo_str, RECUO, obj->cursor, RECUO, percentual_do_processamento(obj)
   );
   // Liberação pós impressão da formatação.
   free(memoria_str);
   free(memoria_str_i);
   free(total_str);
   free(tempo_str);
}

static void reseta_arrays_se_necessario(IDP* obj) {
   int cursor = obj->cursor;

   if (cursor >= MAX) {

      for (int k = 0; k < PAD; k++) 
      {
         int p = (cursor - 1) - PAD + k;

         obj->tempo[k] = obj->tempo[p];
         obj->quantia[k] = obj->quantia[p];
         mtx_lock(&obj->dono);
         obj->memoria[k] = obj->memoria[p];
         mtx_unlock(&obj->dono);
      }
      // Coloca ele no ínicio novamente.
      obj->cursor = PAD - 1;
   }
}

void get_idp(IDP* obj) {
/* Realiza registro de novos dados do processamento de permutações. */
   reseta_arrays_se_necessario(obj);

   int p = obj->cursor;
   struct timeval t;
   size_t qp = tamanho_fl(obj->queue);
   int n = obj->length;
   int sz = obj->size;

   gettimeofday(&t, NULL);
   obj->quantia[p] = tamanho_fl(obj->queue);
   obj->tempo[p] = t;
   // Acrescenta a memória da array clonada.
   mtx_lock(&obj->dono);
   obj->memoria[p] += (qp * n * sz);
   mtx_unlock(&obj->dono);
   obj->cursor = p + 1;
}

static int thread_idp(void* args) {
   struct timespec PAUSA = { 0, 8e6 };
   IDP* obj = (IDP*)args;

   __loop__ {
      get_idp(obj);
      thrd_sleep(&PAUSA, NULL);
   }
   return EXIT_SUCCESS;
}

void start_gather_idp(IDP* obj) {
   thrd_t ID;

   switch (thrd_create(&ID, thread_idp, obj))
   {
      case thrd_success:
         puts("Coletamento iniciado com sucesso.");
         break;
      default:
         puts("Outro sinal enviado!");
   }
}

/* === === === === === === === === === === === === === === === === === ==
 *                Algoritmo para Geração de Permutações.
 * === === === === === === === === === === === === === === === === === ==*/

static void adiciona_sobrecarga_de_memoria(IDP* obj, int n, size_t m)
{
   mtx_lock(&obj->dono);
   (*obj).memoria[(*obj).cursor] += n * m; 
   mtx_unlock(&obj->dono);
}

static void algoritmo_gerador_de_permutas_genericas_info
  (Generico seq, int n, int sz, FilaLigada queue, int M, IDP* info)
{
   if (M == 1) {
      /* Acrescentando memória da stack, devida as múltiplas recursões. */
      adiciona_sobrecarga_de_memoria(
         // Lista de parâmetros:
         info, n, 3 * sizeof(int) + sizeof(IDP*) + 
         sizeof(Generico) + sizeof(FilaLigada) +
         // Objetos delcarados intermente:
         sizeof(uint8_t*) * 2 + sizeof(int) + 
         // Funções chamadas internamente:
         sizeof(algoritmo_gerador_de_permutas_genericas_info) +
         sizeof(alterna)
      );
      insere_fl(queue, clona_array(seq, sz, n));

   } else {
      uint8_t* a = NULL, *b = NULL;

      for (int p = 0; p < M; p++)
      {
         algoritmo_gerador_de_permutas_genericas_info
            (seq, n, sz, queue, M - 1, info);

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

/* === === === === === === === === === === === === === === === === === ==
 *                         Testes Unitários
 * === === === === === === === === === === === === === === === === === ==*/
#include <locale.h>
#include <time.h>
#include "teste.h"
// Biblioteca externa:
#include "impressao.h"

int visualizacao_da_array(Generico arg)
{
   struct timespec PAUSA = { 0, 4e8 };
   struct timespec PAUSA_CURTA = { 0, 2e8 };
   IDP* obj = (IDP*)arg;
   int* array = (int*)obj->seq;
   int length = obj->length;

   __loop__ {
      imprime_array(array, length);
      debug_idp(obj);
      thrd_sleep(&PAUSA, &PAUSA_CURTA);
   }
   return EXIT_FAILURE;
}

void monitoramento_do_processamento(IDP* obj)
{
   thrd_t ID;

   switch (thrd_create(&ID, visualizacao_da_array, obj))
   {
      case thrd_success:
         printf("Thread de '%s' iniciada com sucesso.\n", 
               stringfy(visualizacao_da_array)
         );
         break;
      default:
         puts("Outro problema na inicialização da thread!");
   }
}

int main(int qtd, char* args[], char* envs[])
{
   const int sz = sizeof(int);
   int seq[] = { -4, -2, -8, -16, -32, 1, 5, 25, 125, 625 };
   const int n = sizeof(seq) / sz;
   FilaLigada results = cria_fl();
   struct InformacaoDoProcessamento info;

   info = new_idp(seq, n, sz, results);
   start_gather_idp(&info);
   monitoramento_do_processamento(&info);

   algoritmo_gerador_de_permutas_genericas_info
      (seq, n, sz, results, n, &info);
   debug_idp(&info);

   return EXIT_SUCCESS;
}
