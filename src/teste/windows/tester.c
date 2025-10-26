// Módulos do próprio projeto:
#include "../macros.h"
// Biblioteca padrão do C:
#include <stddef.h>
// API do Windows.
// #include <libloaderapi.h>
// #include <sysinfoapi.h>
#include <windows.h>

/* === === === === === === === === === === === === === === === === === ===+
 *                      Dados Globais Referentes as
 *                   Implementações Abaixo
 * === === === === === === === === === === === === === === === === === ===*/
/* contabilizando testes sem nomes/ ou mensagens passadas. */
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
   char barra[largura];
   char rotulo[DIM.colunas];

   // Fazendo separador e formatando seu título ...
   memset(barra, '\0', DIM.colunas);
   memset(barra, '-', DIM.colunas - 1);
   sprintf(rotulo, "%s::[%d]", nome, CONTAGEM_DE_TESTES);
   memmove(barra + MARGEM, rotulo, strlen(rotulo));

   printf("\n%s\n\n", barra);
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

// ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~
static bool captura_testes_definidos
  (struct TesteConfig* lista, const int TOTAL, va_list argumentos)
{
   if (TOTAL == 0)
      return false;

   struct TesteConfig aux;

   for (int n = 1; n <= TOTAL; n++) {
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
   struct TesteConfig testes[total];
   va_list args;
   char* nome_fn = NULL;

   va_start(args, total);
   captura_testes_definidos(testes, total, args);
   va_end(args);

   for (int q = 1; q <= total; q++) {
      nome_fn = (char*)testes[q - 1].nome;
      cabecalho_do_teste(nome_fn);
      testes[q - 1].rotina();
      CONTAGEM_DE_TESTES++;
   }
   imprime_separador(60);
}

#ifdef __unit_tests__
 #ifdef _WIN32
/* === === === === === === === === === === === === === === === === === ===+
 *                         Testes Unitários
 * === === === === === === === === === === === === === === === === === ===*/
#include "../amostras.h"

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

TESTE prototipo_da_funcao_executa_testes
  (bool execucao_do_suite, int total, ...)
{
   struct TesteConfig testes[total];
   va_list args;

   va_start(args, total);
   captura_testes_definidos(testes, total, args);

   for (int i = 1; i <= total; i++)
      { putchar('\n'); debug_teste_config(&testes[i - 1]); putchar('\n'); }
   va_end(args);

   for (int i = 1; i <= total; i++)
      cabecalho_do_teste((char*)testes[i - 1].nome);
}

TESTE captura_de_todos_argumentos(void) {
   prototipo_da_funcao_executa_testes (
     true, 2,
         Unit(percorrendo_string, false),
         Unit(stringficacao_de_valores_primitivos, true)
   );
}
 #endif
#endif

