// Biblioteca padrão do C:
#include <stdarg.h>
#include <errno.h>
// Bibliotecas do sistema POSIX:
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

// A maioria da 'libc' codica tais valores como sucesso/e fracrasso.
// Modo de localizar-se na array de pipes anônimos.
// const size_t IN = 0, OUT = 1;
enum vias_dos_pipes { IN = 0, OUT };
// Para delimitar visualização de textos formatados.
const char* ASPAS = "\"\"\"";

// Tuplas retornadas ou referênciadas nas funções abaixas:
typedef struct { char* nome; int saida; } RetornoTst;
// A configuração de um testes unitário:
typedef struct { char* nome; FnTst funcao; bool confirmacao; } TesteSet;
// String com determinado com seu comprimento dado:
typedef struct { char* bytes; size_t length; } String;
/* Saída da função que cuida de executar a rotina passada como argumento. */
typedef struct saidas_da_rotina {
   // String dinâmica e seu comprimento.
   String content;

   // Código de saída da função.
   int code; 

   // Tempo medido durante toda sua execução.
   double elapsed;
} SR;
/* Info mais completa que a acima. Além de processar os dados da acima, ela
 * engloba todo relatório de um teste-unitário. */
typedef struct { 
   // Nome da função trabalhada.
   char* nome; 

   // Código de um erro controlado.
   int codigo;

   // Output gerado pela execução do programa.
   String conteudo;

   // Se a execução do teste quebrou em algum momento.
   bool falhou;

   // Tempo que durou toda esta execução.
   double decorrido;

} TesteRelatorio, TR;


static char* substitui_separadores_do_nome(const char* nome_original) {
/*   Pega o nome da função e substitui seus separadores do identificador por
 * devidos espaços brancos. */
   const char* nome = nome_original;
   size_t t = strlen(nome) + 1; 
   char* corrigido = malloc(t * sizeof(char));

   for (size_t i = 1; i <= t; i++) {
      char _char = nome[i - 1];
      if (_char == '_') 
         corrigido[i - 1] = ' '; 
      else 
         corrigido[i - 1] = _char;
   }
   corrigido[-1] = '\0';
   return corrigido;
} 

static TesteSet pack(char* name, FnTst function, bool execution) { 
   #ifdef _NOVO_SUITE
   printf(
      "\tnome: '%s'\n\tendereço: %p\n\tconfirmação? %s\n\n", 
      name, function, bool_to_str(execution)
   );
   #endif

   return (TesteSet) {
      substitui_separadores_do_nome(name), 
      function, execution
   }; 
}

// Macro que stringfy nome da função:
#define TOKEN_TO_STR(FUNCAO) #FUNCAO
// Nome que empacota principais dados dos testes:
#define Unit(FUNCAO, CONFIRMACAO) \
   pack(TOKEN_TO_STR(FUNCAO), FUNCAO, CONFIRMACAO)

static void desenha_seperador() {
   putchar('\n');
   for (size_t i = 1; i <= 60; i++)
      putchar('-');
   putchar('\n');
}

static int executa_teste_int(const char* nome, FnTst funcao, uint8_t indice){ 
/* Recebe um novo tipo de assinatura de função, uma que retorna o 'exit
 * code' se necessitado, e tem nova formatação do cabeçalho. */ 
   size_t comprimento = strlen(nome);
   char mensagem_padrao[comprimento + 10];
   char* referencia = mensagem_padrao;

   // Adiciona enumeração ao nome.
   sprintf(referencia, "[%uº] %s", indice, nome);
   /* Imprime cabeçalho do teste entre barras, e limitado pela dimensão
    * do terminal. */
   nome_do_teste(mensagem_padrao); 
   // executa função passada.
   int resultado = funcao();
   // espaço para outro possível teste, ou só o 'shell'.
   puts("");
   return resultado;
}

void testes_unitarios(const uint8_t total, ...) {
/*   Esta função difere da 'executa_testes', pois ao invés de ter um nome
 * desconhecido do testes, usa a clásula da função, devidamente processada
 * (com os espaços removidos). */
   va_list args;
   va_start(args, total);
   uint8_t habilitados = 0;
   RetornoTst resultados[total];
   uint8_t nao_passaram = 0;
   TesteSet T;

   #ifdef _POSIX_C_SOURCE 
   // Utilitário está apenas disponível para sistemas Unix's.
   Cronometro medicao = cria_cronometro();
   #endif

   #ifdef _NOVO_SUITE
   printf("Total demandado: %u\n", total);
   #endif

   /* Filtrando cada testes posto, e evaluando seus argumentos...*/
   for (uint8_t i = 1; i <= total; i++) 
   {
      T = va_arg(args, TesteSet);   
      // Apenas executa os que foram autorizados.
      if (T.confirmacao) {
         #ifdef _NOVO_SUITE
         printf("[%p] '%s' ... sucedido.\n", T.funcao, T.nome);
         #endif

         RetornoTst R;
         size_t k = habilitados + 1;

         R.nome = T.nome;
         R.saida = executa_teste_int(T.nome, T.funcao, k);
         resultados[habilitados] = R;

         // Contabilizando falhas.
         if (R.saida != EXIT_SUCCESS)
            nao_passaram++;
         // contabilizando os que estão ativados.
         habilitados++;

         #ifdef _POSIX_C_SOURCE 
         marca(medicao);
         #endif
      } 
      #ifndef _NOVO_SUITE
      // Liberando string alocada dinamicamente...
      free(T.nome);
      #else
      #endif
   }
   va_end(args);

   #ifdef _NOVO_SUITE
   puts("\nSaídas:");
   for (size_t p = 1; p <= habilitados; p++) {
      char* name = resultados[p - 1].nome;
      int __exit = resultados[p - 1].saida;
      printf("\t%luª função|%s|: %d\n", p, name, __exit);
      free(resultados[p - 1].nome);
   }
   #endif

   // informando tempo total da execução.
   double tempo_total = marca(medicao);
   // Informação final da série de testes:
   desenha_seperador();
   printf(
      "Levaram %0.4lfs; estados dos testes: %u on | %u off; "
      "total de testes: %u; passaram: %u\n", 
      tempo_total, habilitados, total - habilitados, 
      habilitados, habilitados - nao_passaram
   );
}


static void handle(int s) {
   // close(fileno(stdout));
   // stdout = fdopen(STDOUT_FILENO, "w");
   _exit(EXIT_FAILURE);
}

static SR executa_e_captura_saida(FnTst rotina) {
   pid_t subprocesso;
   int tubo[2], code, lido;
   Cronometro T;
   SR report;
   const int N = 1000;
   char buffer[N];

   pipe(tubo);
   // Duplicando este processo.
   subprocesso = fork();
   // Disparando cronômetro...
   T = cria_cronometro();

   if (subprocesso == Okay) {
      // Fechando pipe de entrada, já que é irrelevante nesta abstração.
      close(tubo[IN]);
      
      // Redirecionando standard output para saída do processo.
      int old_stdout = dup(STDOUT_FILENO);
      int result = dup2(tubo[OUT], STDOUT_FILENO);
      close(tubo[OUT]);

      if (result == Failed) perror(strerror(errno));

      // Retornando o 'standard output'.
      signal(SIGABRT, handle);
      // Executando rotina dada.
      int sinal = rotina();

      result = dup2(old_stdout, STDOUT_FILENO);
      close(old_stdout);         // duvidosa inserção!

      if (result == Failed) perror(strerror(errno));

      // Não é preciso nesta thread.
      destroi_cronometro(T);
      // Saindo após finalizar a execução deste bloco.
      exit(sinal);
   }
   // Fechando via de saída, porque é inútil.
   close(tubo[OUT]);

   // Aguardando finalização do processo.
   wait(&code);

   // Começar a medir o tempo daqui, também começa formação do relatório.
   report.elapsed = marca(T);
   report.code = code;

   // Lendo saído do programa lançado em paralelo...
   lido = read(tubo[IN], buffer, N);
   close(tubo[IN]);

   // Coloca(copia) string para o relatório...
   report.content.length = lido;
   report.content.bytes = malloc(lido * sizeof(char));
   memcpy(report.content.bytes, buffer, lido);

   return report;
}

static SR executa_sem_captura(FnTst rotina) {
   pid_t subprocesso;
   int code;
   Cronometro T;
   SR report;

   // Duplicando este processo.
   subprocesso = fork();
   // Disparando cronômetro...
   T = cria_cronometro();

   if (subprocesso == Okay) {
      // Em caso de interrupção, apenas abandona com "sinal de falha".
      signal(SIGABRT, handle);
      // Executando rotina dada.
      int sinal = rotina();

      // Não é preciso nesta thread.
      destroi_cronometro(T);
      // Saindo após finalizar a execução deste bloco.
      exit(sinal);
   }
   // Aguardando finalização do processo.
   wait(&code);

   // Começar a medir o tempo daqui, também começa formação do relatório.
   report.elapsed = marca(T);
   report.code = code;

   // A captura não foi feita, então "zerar" tal variável.
   report.content.length = 0;
   report.content.bytes = NULL;

   return report;
}

static TesteRelatorio executa_unico_teste(TesteSet a) {
/* Basicamente reprocessamento de alguns dados, e simples cópia de outros.
 * Quando executado o teste aqui, usando de 'wrapper' que pega output,
 * 'código de saída' e 'tempo', este não apenas copias tais dados, como
 * reprocessa e incrementa-os. */
   TesteRelatorio report;

   /* Executa a rotina, e também detalhes importantes... */
   SR result_a = executa_e_captura_saida(a.funcao);

   // Colocando todas informações do relatório no tipo de dado adequado.
   report.nome = a.nome;
   report.decorrido = result_a.elapsed;
   report.codigo = result_a.code;
   // Ainda a trabalhar tal mudança de valor...
   report.falhou = (result_a.code != Okay);
   report.conteudo = result_a.content;

   return report;
}

static void visualiza_teste_set(TesteSet* a) {
   bool logico = a->confirmacao;
   const char* status = bool_to_str(logico);

   printf("==> %s(%s)\n", a->nome, status);
}

static void resumo_do_relatorio_tr(char* nome, bool falhou, int max_recuo, bool estado) 
{
/* Modo melhor formatado para exibição dos testes dados. Passado o relatório
 * e o comprimento da maior string; este último é importante que ele 
 * comprienda o comprimento do maior teste, qualquer coisa menor que isso
 * causará erro de formatação, por isso o programa aqui irá parar se 
 * detectar um valor inferior, em tempo de compilação. */
   char resultado[100];
   const int MARGEM = 11;
   int max = MARGEM + max_recuo + 2 + 1;
   char separador[max];
   int comprimento = strlen(nome);

   if (!estado) {
   // Em caso de o teste não for executado.
      strcpy(resultado, "skip");
   } else {
      if (falhou)
         strcpy(resultado, "fail");
      else
         strcpy(resultado, "ok");
   }
   // Limpando array primeiramente...
   memset(separador, '\0', max);
   // Colocando separador.
   memset(separador, '.', MARGEM + max - comprimento);

   printf("  o teste '%s'", nome);
   // printf("%s::%s", __FILE__, nome);
   printf("%s%s\n", separador, resultado);
}

static void compilado_final_tr(TR* relatorios, int habilitados, 
  int total_de_testes) 
{
   // Acumulador de tempo decorrido de cada teste ativo.
   double tempo_total = 0.0;
   // Testes que não executaram corretamente.
   int falharam = 0;
   int qtd_de_testes = habilitados;

   /* Computando tempo total levado, que é a soma de todos. Também contando
    * a quantidade de testes que passaram, assim, consequentemente se 
    * calcula o total que falharam.  */
   for (int i = 1; i <= qtd_de_testes; i++)
   {
      TR a = relatorios[i - 1];
      tempo_total += a.decorrido;
      
      // Contabilizando testes que fracassaram.
      if (a.falhou) falharam++;
   }
   int passaram = habilitados - falharam;
   int desabilitados = total_de_testes - habilitados;

   desenha_seperador();
   printf(
      "Levaram %0.4lfs; estados dos testes: %u on | %u off; "
      "total de testes: %u; passaram: %u\n", 
      tempo_total, habilitados, desabilitados, habilitados, passaram
   );
}

static TesteRelatorio executa_unico_teste_sem_captura(TesteSet a, 
  uint8_t indice) 
{
/* Retorna relatório como nome, código de saída, tempo decorrido da 
 * função e etc; valores de um relatório, entretanto, este aqui não
 * captura a saída, que é mostrada no console. */
   size_t comprimento = strlen(a.nome);
   char mensagem_padrao[comprimento + 10];
   char* referencia = mensagem_padrao;

   // Adiciona enumeração ao nome.
   sprintf(referencia, "[%uº] %s", indice, a.nome);
   nome_do_teste(referencia); 

   /* Executa a rotina, e também detalhes importantes... */
   SR result = executa_sem_captura(a.funcao);

   puts("");
   return (TesteRelatorio){ 
      .nome = a.nome,
      .decorrido = result.elapsed, 
      .conteudo = result.content,
      // Copia ponteiro, que parece ter um tempo de vida igual a duração
      // do programa(estático).
      .codigo = result.code,
      .falhou = (result.code != Okay)
   };
}

void testes_unitarios_a(bool mostrar_saida, bool paralelizacao, 
  bool desativa_todos, const uint8_t total, ...) 
{
/* O mesmo que acima, porém permite desligar o output dos testes na 
 * saída padrão, assim como também permite a paralelização dos testes
 * unitários. */
   va_list args;
   va_start(args, total);
   TesteSet testes[total];
   int maior_str = 0;

   #ifdef _NOVO_SUITE
   printf("Total demandado: %u\n", total);
   #endif

   // O número de argumentos é inválido.
   if (total == 0) {
      perror(
         "O total de argumentos tem quer ser dado, e ser também "
         "correspondente com o real número de argumentos. Mais irá "
         "quebrar o programa; já menos, não irá executar alguns testes; e "
         "zero nunca é possível, subindo esta mensagem."
      );
      abort();
   }

   // Coletando todos testes passados...
   for (uint8_t i = 1; i <= total; i++) 
   {
      // Apenas filtrando testes passados...
       testes [i - 1] = va_arg(args, TesteSet);   

       int length = strlen(testes[i - 1].nome);
       // Computando também a função com maior nome.
       if (maior_str < length)
         maior_str = length;
   }
   va_end(args);

   #ifdef _NOVO_SUITE
   puts("Verificando que testes estão 'on' e 'off':");
   for (int i = 1; i <= total; i++) 
       visualiza_teste_set(&testes[i - 1]);
   #endif

   /* Se o 'suite' foi desativado, então o programa termina aqui. */
   if (desativa_todos) {
      puts("Todos testes abaixos, apesar do seu status, foram desativados:");
      for (int i = 1; i <= total; i++) {
         TesteSet teste = testes[i - 1];
         const char* str_on_off; 
         char* nome = teste.nome;

         if (teste.confirmacao)
            str_on_off = "on";
         else
            str_on_off = "off";

         printf("\t- '%s'\t\t[%s]\n", nome, str_on_off);
      }

      putchar('\n');
      // Abandonando o suite...
      return;
   }

   puts("\nAs seguintes funções estão sendo executados e analisadas:\n");
   // Armazém para todos relatórios gerados.
   TesteRelatorio reports[total];
   // Onde o próximo relatório será encaixado na array, e também onde
   // contador de quantia de testes habilitados.
   int habilitados = 0;

   // Gerando os relatórios(executando o teste configurado)...
   for (int i = 0; i < total; i++) 
   {
      TesteSet a = testes[i];
      bool teste_ativo = a.confirmacao;
      bool teste_fracassou;
      char* nome_da_funcao = a.nome;

      if (teste_ativo) {
         reports[habilitados] = executa_unico_teste(a);
         // Estado da execução do teste.
         teste_fracassou = reports[habilitados].falhou;
         // Contabiliza testes habilitados.
         habilitados++;
      } else
         /* Apesar de marcar como "fracasso", indica mais sobre a
          * disponibilidade do teste do que seu resultado. */
         teste_fracassou = false;

      // Breve resumo dos dados do testes aplicado.
      resumo_do_relatorio_tr(
         nome_da_funcao, teste_fracassou, 
         maior_str, teste_ativo
      );
   }
   /* Mostra um resumo geral de todos testes, tempo total(soma de todas),
    * o que realmente executaram, o que executaram e passaram/falharam. */
   compilado_final_tr(reports, habilitados, total);
}

/* == == == == == == == == == == == == == == == == == == == == == == == ==
 *                         Testes Unitários
 *
 *   Fazendo os testes unitários aqui por um motivo de organização. Se fizer
 * junto com os demais no arquivo 'teste.c', não ajuda a organizar a ordem
 * feita e de onde eles vieram.
 * == == == == == == == == == == == == == == == == == == == == == == == = */
#ifdef _UT_TESTE
#include "dados_testes.h"
#include <unistd.h>
#include <errno.h>

/* Funções para realizar muitos testes abaixos:
 *    - capitaliza_palavras
 *    - string_alternada
 *    - carregando_ate_cem
 *    - simples_mensagem_do_fortune
 *    - executa_algo_mas_crash
 */
#include "amostras.c"

/* Alguns pontos importados do arquivo aqui:
 *
 *    Testes:
 *       - trabalhando_no_paralelismo 
 *       - usando_arrays_para_compactar_o_codigo_acima
 *       - verificando_se_a_thread_nao_bagunca_o_output
 *
 *    Funções:
 */
#include "paralelismo.c"

void novo_suite_de_testes_unitarios(void) {
   puts("Chamada no escopo para ver se funciona, e como fica: ");

   testes_unitarios (
      3, Unit(capitaliza_palavras, false),
         Unit(string_alternada, true),
         Unit(carregando_ate_cem, true)
   );
}

void resultado_de_todos_testes_desativados(void) {
   // Retirando debug output.
   #undef _NOVO_SUITE
   puts("Chamada no escopo para ver se funciona, e como fica: ");

   TestesUnitarios (
      3, Unit(capitaliza_palavras, false),
         Unit(string_alternada, false),
         Unit(carregando_ate_cem, false)
   );
   // Colocando-o de volta.
   #define _NOVO_SUITE true
}

void funcao_que_captura_sinal_e_saida(void) {
   FnTst f = string_alternada;
   SR x = executa_e_captura_saida(f);

   printf(
      "Exit Code: %d\nTempo: %0.4lfs\nSaída da função(%lu): %s\n%s %s", 
      x.code, x.elapsed, x.content.length, ASPAS, x.content.bytes, ASPAS
   );
}

static void visualiza_teste_relatorio(TesteRelatorio* r) {
   char* bool_str = bool_to_str(r->falhou);
   assert (r->conteudo.bytes != NULL);
   printf(
      "\nVisualizando o relatório:\n\tnome da função: '%s'\n\t"
      "código de saída: %d\n\ttempo de execução: %0.4lfs\n\t"
      "falhou? %s\n\tsaída(%lu): %s\n%s\n\t%s\n",
      r->nome, r->codigo, r->decorrido, bool_str,
      r->conteudo.length, ASPAS, r->conteudo.bytes, ASPAS
   );
}

void executa_um_teste_unitario_e_coleta_informacoes(void) {
   puts("\n\nNovo testador é só a versão acima, porém completa:");
   TesteSet t1 = Unit(simples_mensagem_do_fortune, true);
   TesteSet t2 = Unit(string_alternada, true);
   TesteSet t3 = Unit(carregando_ate_cem, true);

   TesteRelatorio r1 = executa_unico_teste(t1);
   puts("Passou a primeira execução.");
   TesteRelatorio r2 = executa_unico_teste(t2);
   puts("Passou a segunda execução.");
   TesteRelatorio r3 = executa_unico_teste(t3);
   puts("Passou a terceira execução.");

   visualiza_teste_relatorio(&r1);
   visualiza_teste_relatorio(&r2);
   visualiza_teste_relatorio(&r3);
}

void verificando_saida_do_pipe_sem_multiprocessing(void) {
   int tubos_rw[2];
   pipe(tubos_rw);
   const int IN = 0, OUT = 1;
   FILE* new_output = fdopen(tubos_rw[OUT], "wt");
   FILE* old_output = stdout;

   stdout = new_output;
   for (size_t i = 1; i <= 10; i++) {
      printf("Saída contagem ...(%02lu)\n", i);
   }

   fclose(new_output);
   stdout = old_output;

   puts("Todo o conteúdo foi empurado via pipe, aguarde...");
   sleep(2);

   const size_t N = 1000;
   char buffer[N];
   int lido = read(tubos_rw[IN], buffer, N);
   printf("Total de bytes lido: %d\n", lido);
   assert (lido < N);
   puts(buffer);
}

void resultado_apos_aborto_do_processo(void) {
   TesteSet t = Unit(executa_algo_mas_crash, true);
   TR r = executa_unico_teste(t);
   visualiza_teste_relatorio(&r);
}

void abrindo_np_como_um_file(void) {
   FILE* stream_np = fopen("tubo", "w+");
   char buffer[100];

   fputs("Can't stop to shit with you dick\n", stream_np);
   // Nota: 'fgets' apenas funciona, porque há uma quebra-de-linha.
   fgets(buffer, 100, stream_np);
   // Nota: não é possível usar fread, sem fim-de-arquivo, por ser um NP.
   // fread(buffer, sizeof(char), 100, stream_np);
   fclose(stream_np);

   printf("\nIsso foi o que foi lido:\n\t\"%s\"\n", buffer);
}

int listagem_de_veiculos_exemplo(void) {
   for (int p = 1; p <= VEICULOS; p++) 
      printf("\t%dª. %s\n", p, veiculos[p - 1]);
   return EXIT_FAILURE;
}

void captura_de_output_via_pipe(void) {
   pid_t subprocesso;
   int tubo[2];

   pipe(tubo);
   // Duplicando este processo.
   subprocesso = fork();

   if (subprocesso == 0) {
      // Fechando pipe de entrada, já que é irrelevante nesta abstração.
      close(tubo[0]);
      
      // Redirecionando standard output para saída do processo.
      int old_stdout = dup(STDOUT_FILENO);
      int result = dup2(tubo[1], STDOUT_FILENO);
      close(tubo[1]);

      if (result == -1)
         perror(strerror(errno));

      result = listagem_de_veiculos_exemplo();

      // Retornando o 'standard output'.
      result = dup2(old_stdout, STDOUT_FILENO);

      if (result == -1)
         perror(strerror(errno));

      // Saindo após finalizar a execução deste bloco.
      exit(result);
   }
   // Fechando via de saída, porque é inútil.
   close(tubo[1]);
   // Aguardando finalização do processo.
   int code;
   wait(&code);

   const int N = 1000;
   char buffer[N];
   int lido = read(tubo[0], buffer, N);
   close(tubo[0]);

   printf("Conteudo(%d bytes | %d):\n%s\n", lido, code, buffer);
}

void visualiza_dado_sr(SR* obj) {
   puts("\nVisualização do tipo SR:");
   printf("\t\b\b\bexit code: %d\n", obj->code);
   printf("\t\b\b\bdecorrido: %0.4lfseg\n",obj->elapsed);
   printf(
      "\t\b\b\bsaída(%zu bytes): %s\n%s\t%s\n", 
      obj->content.length, ASPAS, obj->content.bytes, ASPAS
   );
}

void novo_tipo_de_captura_criado_via_pipes() {
   SR r = executa_e_captura_saida(string_alternada);
   visualiza_dado_sr(&r);

   SR ra = executa_e_captura_saida(capitaliza_palavras);
   visualiza_dado_sr(&ra);

   SR rb = executa_e_captura_saida(executa_algo_mas_crash);
   visualiza_dado_sr(&rb);
}

static void simples_nota_sobre_tr(TR* report) {
   char resultado[30];

   printf("  o teste '%s'", report->nome);

   if (report->falhou)
      strcpy(resultado, "fail");
   else
      strcpy(resultado, "ok");

   printf("\t\t\t...%s\n", resultado);
}

void novo_relator_de_execucao_de_teste() {
   puts("\n\nNovo testador é só a versão acima, porém completa:");
   TesteSet t1 = Unit(string_alternada, true);
   TesteSet t3 = Unit(simples_mensagem_do_fortune, true);
   TesteSet t2 = Unit(carregando_ate_cem, true);
   TesteSet t4 = Unit(executa_algo_mas_crash, true);
   TesteSet t5 = Unit(capitaliza_palavras, true);


   TesteRelatorio r1 = executa_unico_teste(t1);
   simples_nota_sobre_tr(&r1);

   TesteRelatorio r2 = executa_unico_teste(t2);
   simples_nota_sobre_tr(&r2);

   TesteRelatorio r3 = executa_unico_teste(t3);
   simples_nota_sobre_tr(&r3);

   TesteRelatorio r4 = executa_unico_teste(t4);
   simples_nota_sobre_tr(&r4);

   TesteRelatorio r5 = executa_unico_teste(t5);
   simples_nota_sobre_tr(&r5);

   visualiza_teste_relatorio(&r1);
   visualiza_teste_relatorio(&r2);
   visualiza_teste_relatorio(&r3);
   visualiza_teste_relatorio(&r4);
   visualiza_teste_relatorio(&r5);
   puts("\nStandard output chegou ileso!\n");
}

void gerador_de_relatorio_sem_captura(void) {
   puts("\nTeste de um executor que não utiliza de forques e nada, sem falar que não captura o resultado.");
   TesteSet t1 = Unit(string_alternada, true);
   TesteSet t3 = Unit(simples_mensagem_do_fortune, true);
   TesteSet t2 = Unit(carregando_ate_cem, true);
   TesteSet t4 = Unit(executa_algo_mas_crash, true);
   TesteSet t5 = Unit(capitaliza_palavras, true);


   puts("Geração do relatório por meio de execução:");
   TesteRelatorio r1 = executa_unico_teste_sem_captura(t1, 10);
   TesteRelatorio r2 = executa_unico_teste_sem_captura(t2, 11);
   TesteRelatorio r3 = executa_unico_teste_sem_captura(t3, 12);
   TesteRelatorio r4 = executa_unico_teste_sem_captura(t4, 13);
   TesteRelatorio r5 = executa_unico_teste_sem_captura(t5, 14);

   // Notas de todas execuções:
   simples_nota_sobre_tr(&r1);
   simples_nota_sobre_tr(&r2);
   simples_nota_sobre_tr(&r3);
   simples_nota_sobre_tr(&r4);
   simples_nota_sobre_tr(&r5);
}

void primeiro_simples_teste_final_do_suite(void) {
   testes_unitarios_a(
      true,  true, true, 5,
      Unit(string_alternada, true),
      Unit(simples_mensagem_do_fortune, true),
      Unit(carregando_ate_cem, true),
      Unit(executa_algo_mas_crash, false),
      Unit(capitaliza_palavras, true)
   );
}
#endif
