#include <stddef.h>
#include <libloaderapi.h>
#include <sysinfoapi.h>
#include "macros.h"

/* === === === === === === === === === === === === === === === === === ===+
 *                      Dados Globais Referentes as
 *                   Implementações Abaixo
 * === === === === === === === === === === === === === === === === === ===*/
/* contabilizando testes sem nomes/ ou mensagens passadas. */
static uint8_t testes_contagem = 0;
/* Contabiliza instâncias do 'executa_testes_a', isso para isolar do outro
 * tipo de 'executador de teste' descontinuado. */
static uint16_t contagem_de_testes = 0;
/* Endereço de memória da função a executar; e sua definação,... se é para
 * executa-la?! */
struct TesteConfig { Fn rotina; bool ativado; };

/* === === === === === === === === === === === === === === === === === ===+
 *                   Funções auxiliares, e Funções
 *                Concretas em sí, Declaradas no Cabeçalho
 * === === === === === === === === === === === === === === === === === ===*/
static void nome_do_teste(char *nome) 
{
/* Imprime o separador entre os testes executados. Contém o nome dado 
 * no canto esquerdo deste rótulo. */
   int C = strlen(nome);
   // Dimensao dim = dimensao_terminal();
   struct TerminalSize dim = obtem_dimensao(); 
   int largura = dim.colunas - 3;
   char barra[largura];

   // Fazendo separador...
   memset(barra, '\0', largura);
   memset(barra, '-', largura - 1);

   // margens do texto.
   barra[5] = ' ';
   barra[5 + C + 1] = ' ';

   // Colocando título no separador...
   for (int k = 0; k < C; k++) {
      if (k < C && nome[k] != '\0')
         barra[k + 6] = nome[k]; 
   }

   printf("\n\n%s\n\n", barra);
}

static void executa_teste(char* nome, void (*funcao)()) {
/* Executa o teste, dado a função que o faz; também é preciso do nome que 
 * ele tem, pode ser o nome da função, ou não. */
   // imprime o nome do programa.
   if (strlen(nome) == 0) { 
      testes_contagem += 1;
      char mensagem_padrao[100];
      char* referencia = mensagem_padrao;
      sprintf(
         referencia, 
         "%dº teste anônimo", 
         testes_contagem
      );
      nome_do_teste(mensagem_padrao); 
   } else 
      { nome_do_teste(nome); }
   // executa função passada.
   funcao();
   // espaço para outro possível teste, ou só o 'shell'.
   puts("");
}

void executa_tst(char* descricacao, Fn call, bool acionado) 
{
/* O mesmo que o 'executa_teste', porém este pode ser facilmente desativado
 * se for preciso, apenas mudando o parâmetro. */
   // não executa nada se for falso.
   if (strlen(descricacao) == 0)
   { 
      testes_contagem += 1;
      char mensagem_padrao[100];
      char* referencia = mensagem_padrao;
      sprintf(
         referencia, 
         "%dº teste anônimo", 
         testes_contagem
      );
      nome_do_teste(mensagem_padrao); 
         
   } else 
      nome_do_teste(descricacao);

   if (acionado)
      // quebra-de-linha no final.
      { call(); puts(""); }
   else
      puts("DESATIVADO TEMPORIAMENTE!");
}

void executa_testes(int total, ...) {
   Cronometro medicao = cria_cronometro();
   va_list args;
   size_t habilitados = 0;

   // como também conta o valor lógico se é para executa-lá no momento.
   va_start(args, total);

   for (uint8_t t = 1; t <= 2 * total; t+=2) {
      // primeiro pega a função.
      Fn funcao = va_arg(args, Fn);
      // a confirmação se é para executa-lá, ou não.
      bool e_para_executar = va_arg(args, int);

      if (e_para_executar) {
         executa_teste("", funcao);
         marca(medicao);
         habilitados++;
      }
   }
   va_end(args);

   // Para não compilar no Windows, assim mantém compatibilidade
   // Informando tempo total da execução.
   double tempo_total = marca(medicao);

   if (tempo_total <= 0)
      puts("\nexecução \"instântanea\".");
   else 
      printf(
         "\ntodos testem levaram %s\n", 
         tempo_legivel(tempo_total)
      );
   printf ("há %zu testes desativados.\n", total - habilitados);
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

void executa_testes_a(bool execucao_do_suite, int total, ...) 
{
/* O mesmo que acima, entretanto, este tem um "interruptor" que desliga
 * todo o "suíte" de teste, indepedente de alguns foram configurados para 
 * executar, ou não. */
   int habilitados = 0, quantidade=total;
	SYSTEMTIME inicio, final;
   struct TesteConfig testes[quantidade];

   va_list args;
   // como também conta o valor lógico se é para executa-lá no momento.
   va_start(args, total);

   // Capturando testes e sua configuração...
   for (int t = 1; t <= quantidade; t++) 
   {
      testes[t - 1].rotina = va_arg(args, Fn);
      testes[t - 1].ativado = va_arg(args, int);

      // Contabiliza os que foram dados como ativados.
      if (testes[t - 1].ativado)
         habilitados++;
   }
   va_end(args);

	double decorrido = 0.0;
   /* Executando os testes, pelo menos o que estão configurados para tal. */
   // Começando a medição das execuções...
   GetSystemTime(&inicio);
   for (int t = 0; t < quantidade; t++) 
   {
      if (testes[t].ativado && execucao_do_suite)
         executa_teste_a(SEM_MENSAGEM, testes[t].rotina); 
   }
	GetSystemTime(&final);
	decorrido = diffsystemtime(inicio, final);

   int desabilitados = quantidade - habilitados;
   const char* suite_msg;

   if (execucao_do_suite)
      suite_msg = "execuções finalizadas"; 
   else
      suite_msg = "nada executado";

   imprime_separador(57);
   printf (
      "Testes: %d on | %d off; Levou: %0.4lfseg; Final: '%s'\n\n", 
      habilitados, desabilitados, decorrido, suite_msg
   );
}

// ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~
void executa_testes_b
  (bool execucao_do_suite, const int lista_off[], int total, ...)
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
}

#ifdef _UT_TESTE
/* === === === === === === === === === === === === === === === === === ===+
 *                         Testes Unitários
 * === === === === === === === === === === === === === === === === === ===*/
#include "amostras.h"

void primeira_versao_alternativa_de_executa_testes(void)
{
   puts(
      "Testando a função 'executa_testes_a', configuração básica, "
      "com um monte de testes(quase todos eles), porém ainda ativada"
   );
   executa_testes_a(
      true, 5, 
      capitaliza_palavras, true,
      string_alternada, true,
      carregando_ate_cem, false,
      simples_mensagem_do_fortune, true,
      executa_algo_mas_crash, false
   );

   puts(
      "\nCom apenas três testes, um bem \"perigoso\", porque ele "
      "interrompe, entretanto, todo 'suíte' está desativado(off)."
    );
   executa_testes_a(
      false, 3, 
      capitaliza_palavras, true,
      simples_mensagem_do_fortune, true,
      executa_algo_mas_crash, true
   );
   puts(
      "\nVerificando decorrer do tempo é realmente cronômetrado..."
    );
   executa_testes_a(
      true, 4, 
      simples_mensagem_do_fortune, false,
      carregando_ate_cem, true,
      executa_algo_mas_crash, false,
      conta_ate_esgotar_valor, true
   );
}

void teste_interruptor_renomeado(void)
{
   puts(
      "A função que executava tal testes simples foi renomeada, agora "
      "ele é só chamada de 'executa_teste', recebe os mesmos parâmetros "
      "que o antigo protótipo. Renomei na cara duro, pois, raramente, "
      "utilizo tal função, logo a quebra-de-compatibilidade com algum "
      "código antigo que ainda o uso é improvável."
   );
   executa_tst(
      "Itera uma Simples String",
      percorrendo_string, true
   ); 
   executa_tst(
      "Transformação de Strings em Minúscula",
      transforma_toda_string_em_minuscula, true
   ); 

   executa_tst(SEM_MENSAGEM, percorrendo_string, false);
}
#endif

