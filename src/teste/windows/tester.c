// Módulos do próprio projeto:
#include "macros.h"
#include "terminal.h"
// Biblioteca padrão do C:
#include <stddef.h>
// API do Windows.
#include <Windows.h>

/* Contabilizando testes sem nomes/ ou mensagens passadas. */
static uint16_t CONTAGEM_DE_TESTES = 0;

/* === === === === === === === === === === === === === === === === === ===+
 *                   Funções auxiliares, e Funções
 *                Concretas em sí, Declaradas no Cabeçalho
 * === === === === === === === === === === === === === === === === === ===*/
static void cabecalho_do_teste(char *nome)
{
/* Imprime o separador entre os testes executados. Contém o nome dado
 * no canto esquerdo deste rótulo. */
   const int MARGEM = 5;
   struct TerminalSize DIM = obtem_dimensao();
   int largura = DIM.colunas - MARGEM;
   /* Nota: tal coisa é importante, já que o Windows C Compiler(MSVC), não
    * suporta tal feature. Logo é preciso alocar memória dinâmica manualmente
    * usando o 'memory allocation(malloc)'.
    */
   const int sz = sizeof(char);
   char* barra = malloc((uint64_t)largura * sz);
   char* rotulo = malloc((uint64_t)DIM.colunas * sz);
   uint16_t indice = CONTAGEM_DE_TESTES + 1;

   // Fazendo separador e formatando seu título ...
   memset(barra, '\0', DIM.colunas);
   memset(barra, '-', DIM.colunas - 1);
   sprintf(rotulo, " [%uº]::%s ", indice, nome);
   memmove(barra + MARGEM, rotulo, strlen(rotulo));

   printf("\n%s\n\n", barra);
   // Liberação não automática na plataforma Windows(sem VLA).
   free(barra);
}

static void imprime_separador(int n)
{
/* Imprime uma barreira, usando do caractére dash(-), que separa os outputs
 * dos testes, e o resultado final com algunas informações relevantes sobre
 * todo suíte de testes. */
   putchar('\n');
   for (int k = 0; k <= n; k++)
      { putchar('-'); }
   // Quebra-de-linha ...
   putchar('\n');
}

static void contabiliza_execucao(void)
   { CONTAGEM_DE_TESTES += 1; }

static int conta_valores_true(struct TesteConfig* list, const int n)
{
   int contagem = 0;

   for (int m = 1; m <= n; m++)
      if (list[m - 1].ativado)
         contagem++;
   return contagem;      
}

static void mostra_status_geral
  (struct TesteConfig* lista, const int n, double tempo)
{
   int habilitados = conta_valores_true(lista, n);
   int desabilitados = abs(n - habilitados);

   printf(
      "Testes definidos: %d on | %d off; Levou %0.3lfseg\n\n", 
      habilitados, desabilitados, tempo
   );
}

static double decorrido_seg(SYSTEMTIME a, SYSTEMTIME b)
{  
   double inicio, fim;

   inicio = (double) (
      a.wDay * DIA + a.wMonth * MES + a.wYear * ANO + 
      a.wHour * HORA + a.wMinute* MINUTO + a.wSecond + 
      (double)a.wMilliseconds / 1.0e3
   );
   fim = (double) (
      b.wDay * DIA + b.wMonth * MES + b.wYear * ANO + 
      b.wHour * HORA + b.wMinute* MINUTO + b.wSecond + 
      (double)b.wMilliseconds / 1.0e3
   );

   return fim - inicio;
}
// ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~
static bool captura_testes_definidos
  (struct TesteConfig* lista, const int TOTAL, va_list argumentos)
{
   struct TesteConfig aux;

   if (TOTAL == 0)
      return false;

   for (int n = 1; n <= TOTAL; n++) 
   {
      aux = va_arg(argumentos, struct TesteConfig);
      lista[n - 1] = aux;
   }
   return true;
}

CROSSLIB void executa_testes(bool execucao_do_suite, int total, ...)
{
/* Esta função acrescenta em relação a versão 'a', que agora pega também o
 * nome da função para diferênciar, e tem lista de desabilitação. O quê? Como
 * assim? Uma lista de inteiros positivos que se você quiser desabilitar
 * tal teste, mesmo que ele esteja habilitado, tal lista tem procedência.
 * Para fazer isso, apenas coloque o número referênte ao teste, numa ordem
 * top-down. Se você não quiser usar tal feature, apenas coloque 'NULL' ou
 * uma constante especial que indique isso. Números negativos, maiores que
 * a quantia total de testes, serão desconsiderados. O teste também é
 * definido de forma diferente dos demais. Nãa basta agora apenas passar
 * a 'function poitner' dele e seu status, Isso tem que ser passado dentro
 * de uma tupla definida pelo macro 'unit'. Isso é o que permite a captura
 * do nome da função em sí. */
   va_list args;
   char* nome_fn = NULL;
   /* Como no MSVC não tem Variable-Length-Arrays(VLAs), um modo de alocar
    * memória dinâmica apenas declarando o comprimento da array numa variável
    * isso em tempo de compilação. O negócio é usar 'malloc' mesmo. Lembre-se
    * e é preciso liberar posteriormente. */
   const int N = (int)total;
   /* Só por garantia o dobro do que é preciso. Quase nada com o que
    * geralmente é alocado. No máximo, algumas dezenas de testes. Centenas
    * de milhares, está descartado. */
   struct TesteConfig testes[2 * N];
   SYSTEMTIME a, b;

   va_start(args, total);
   captura_testes_definidos(testes, N, args);
   va_end(args);
   // Começando a medida.
   GetSystemTime(&a);

   if (execucao_do_suite)
   {
      for (int q = 1; q <= N; q++)
      {
         nome_fn = (char*)testes[q - 1].nome;
         cabecalho_do_teste(nome_fn);

         if (testes[q - 1].ativado)
            testes[q - 1].rotina();
         else
            puts("\tO teste está DESABILITADO.");
         contabiliza_execucao();
      }
   }
   // Termina a medida de tempo.
   GetSystemTime(&b);
   imprime_separador(60);
   mostra_status_geral(testes, total, decorrido_seg(a, b));
}

#ifdef __unit_tests__
/* === === === === === === === === === === === === === === === === === ===+
 *                         Testes Unitários
 * === === === === === === === === === === === === === === === === === ===*/
#include "../amostras.h"
#include "legivel.h"

TESTE debug_teste_config(struct TesteConfig* obj) {
   const char* const SEP = "\t\b\b";
   bool ativado = (*obj).ativado;
   void* address_fn = (void*)(*obj).rotina;

   printf(
      "TesteConfig [0x%p]\n%snome: '%s'\n%srotina: 0x%p\n%sativado: %s\n",
      (void*)obj, SEP, (*obj).nome, SEP, address_fn, SEP,
      bool_to_str(ativado)
   );
}

// Testes que não quebram para testar com o suite.
TESTE capitaliza_palavras_void(void)
   { capitaliza_palavras(); }

TESTE string_alternada_void(void)
   { string_alternada(); }

TESTE funcao_que_executa_multiplos_testes(void) {
   puts(
      "\n\nOs testes aqui testados, realmente estão usando a função"
      " 'executa_testes', que é a função final, e não um protótipo dela."
      "Para usar tal função, é preciso que seu primeiro argumento seja, se "
      "é para usar o 'suíte', isto é, todos as funções declaradas no "
      "escopo da chamada; assim também quantas são no total. Ambos "
      "argumentos são separados por vírgula. Para declara uma função, esta "
      "tem que ser do tipo 'TESTE'[void(*)(void)], você precisa apenas "
      "escrever o seu nome, sem string sem nada, e seu estado, se deseja "
      "executar tal,... tudo isso dentro da função-macro 'Unit', terminado "
      "separa com vírgula, tanto da declaração inicial ativação e quantia "
      "como da das outras declarações de testes unitários."
   );

   executa_testes(
     true, 4,
         Unit(percorrendo_string, true),
         Unit(stringficacao_de_valores_primitivos, false),
         Unit(capitaliza_palavras_void, true),
         Unit(string_alternada_void, true)
   );
}

void debug_systemtime(SYSTEMTIME x)
{
   printf(
      "%u/%02u/%u - %02u:%02u:%02u %ums - dia(%u)\n", 
      x.wDay, x.wMonth, x.wYear, 
      x.wHour, x.wMinute, x.wSecond,
      x.wMilliseconds, x.wDayOfWeek
   );
}

TESTE medicao_de_tempo(void) {
   DWORD a, b;
   SYSTEMTIME x, y;

   GetSystemTime(&x);
   a = GetTickCount();
   Sleep(2);
   b = GetTickCount();
   GetSystemTime(&y);

   debug_systemtime(x);
   debug_systemtime(y);

   printf("Decorrido(GetTickCount): %lums\n", b - a);
   printf("Decorrido(GetSystemTime): %lfms\n", decorrido_seg(x, y));
}

TESTE configuracao_de_testes(void) {
   struct TesteConfig input[] = {
      Unit(percorrendo_string, false),
      Unit(stringficacao_de_valores_primitivos, false),
      Unit(capitaliza_palavras_void, true),
      Unit(string_alternada_void, true)
   };
   const int N = sizeof(input) / sizeof(struct TesteConfig);

   for (int n = 0; n < N; n++)
      debug_teste_config(&input[n]);
}
#endif

