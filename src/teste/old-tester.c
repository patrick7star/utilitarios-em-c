#include <stddef.h>
#ifdef __linux__
#include <sys/time.h>
#include <unistd.h>
#elif defined(_WIN64)
#include <libloaderapi.h>
#include <sysinfoapi.h>
#endif

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
   #ifndef _WIN64
   // Informando tempo total da execução.
   double tempo_total = marca(medicao);

   if (tempo_total <= 0)
      puts("\nexecução \"instântanea\".");
   else 
      printf(
         "\ntodos testem levaram %s\n", 
         tempo_legivel(tempo_total)
      );
   #endif
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

#ifdef __linux__
static void nome_do_executavel(char* output, const int N)
{
/* Pega o nome do arquivo que está o teste, então processa ele, tirando 
 * o caminho e o sufixo que identifica o arquivo. */
   char caminho[N];

   memset(caminho, '\0', N);
	#ifdef __linux__
   assert(readlink("/proc/self/exe", caminho, N) != 1);
	#elif defined(_WIN64)
	LPSTR ptr = caminho;
	assert(GetModuleFileNameA(NULL, ptr, N) != 0);	
	#endif

   char* base_caminho = strrchr(caminho, '/') + 1;
   /* Comprimento da string do sufixo, e também da base do arquivo.*/
   int L = strlen(base_caminho);

   // Reseta buffer passado com caractéres 'nulos'.
   memset(output, '\0', N);
   // Escreve a 'base' até o sufixo...
   memcpy(output, base_caminho, L);
}

static void executa_teste_a(char* nome, Fn rotina) 
{
   int T = strlen(nome);
   const int N = 200;
   bool nome_nao_tem_caracteres = (T == 0);

   if (nome_nao_tem_caracteres) 
   { 
      contagem_de_testes++;
      /* Conta os testes executados, mais para colocar ordem nos testes
       * anônimos. Outro suíte, aberto e executado num mesmo arquivo,
       * continua esta contagem. */
      char mensagem_padrao[N];
      // Tira a base do caminho(o nome), e pula o separador de caminhos 
      // também.
      char nome_do_arquivo[N];

      nome_do_executavel(nome_do_arquivo, N);
      sprintf(
         mensagem_padrao, "%s::teste[%uº]", 
         nome_do_arquivo, contagem_de_testes
      );
      nome_do_teste(mensagem_padrao); 
   } else 
      nome_do_teste(nome);

   // Executa função passada.
   rotina();
}
#endif

#ifdef __linux__
static double difftimeval(struct timeval start, struct timeval end)
{
/* Computa um double, representando segundos inteiros e frações deles(até
 * mais ou menos microsegundos), do tipo de dado 'timeval', uma estrutura
 * com mais precisão do que simples 'time_t'. 
 */
   double a = (double)(end.tv_sec - start.tv_sec);
   double b = (double)(end.tv_usec - start.tv_usec);

   return a + b / 1000000.0;
}
#elif defined(_WIN64)
static double diffsystemtime(SYSTEMTIME start, SYSTEMTIME end)
{
	double a = (
		(double)start.wHour * 3600.0 + (double)start.wMinute * 60.0 
		+ (double)start.wSecond + start.wMilliseconds / 1000.0
	);
	double b = (
		(double)end.wHour * 3600.0 + (double)end.wMinute * 60.0 
		+ (double)end.wSecond + end.wMilliseconds / 1000.0
	);

	return b - a;
}
#endif

void executa_testes_a(bool execucao_do_suite, int total, ...) 
{
/* O mesmo que acima, entretanto, este tem um "interruptor" que desliga
 * todo o "suíte" de teste, indepedente de alguns foram configurados para 
 * executar, ou não. */
   int habilitados = 0, quantidade=total;
	#ifdef __linux__
   struct timeval inicio, final;
	#elif defined(_WIN64)
	SYSTEMTIME inicio, final;
	#endif
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
	#ifdef __linux__
   gettimeofday(&inicio, NULL);
   for (int t = 0; t < quantidade; t++) 
   {

      if (testes[t].ativado && execucao_do_suite)
         executa_teste_a(SEM_MENSAGEM, testes[t].rotina); 
   }
   // Depois de todas medições, o 'marco' final para medir o decorrido.
   gettimeofday(&final, NULL);
   decorrido = difftimeval(inicio, final);

	#elif defined(_WIN64)
   GetSystemTime(&inicio);
   for (int t = 0; t < quantidade; t++) 
   {
      if (testes[t].ativado && execucao_do_suite)
         executa_teste_a(SEM_MENSAGEM, testes[t].rotina); 
   }
	GetSystemTime(&final);
	decorrido = diffsystemtime(inicio, final);
	#endif

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

