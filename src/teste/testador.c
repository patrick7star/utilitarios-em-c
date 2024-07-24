
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

// Modo de localizar-se na array de pipes anônimos.
const size_t IN = 0, OUT = 1;
// Para delimitar visualização de textos formatados.
const char* ASPAS = "\"\"\"";

// Tuplas retornadas ou referênciadas nas funções abaixas:
typedef struct { char* nome; int saida; } RetornoTst;
typedef struct { char* nome; FnTst funcao; bool confirmacao; } TesteSet;
typedef struct { char* bytes; size_t length; } String;

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

typedef struct saidas_da_rotina {
   // String dinâmica e seu comprimento.
   String content;

   // Código de saída da função.
   int code; 

   // Tempo medido durante toda sua execução.
   double elapsed;
} SR;

static SR executa_e_captura_saida(FnTst rotina) {
/* Obs.: não é uma função multithread, então cuidado! */
   const size_t TAMANHO_BUFFER = 10000;
   size_t lido, size;
   char buffer[TAMANHO_BUFFER];
   char nome_do_stream[100];
   FILE* novo_stream;
   FILE* antigo_stream = stdout;
   SR result;
   Cronometro M;

   // Formando nome do arquivo usado com meio.
   sprintf(nome_do_stream, "conteudo.%u.txt", getpid());
   novo_stream = fopen(nome_do_stream, "w+r");
   // Desligando output padrão na tela, e ligando-o numa array.
   stdout = novo_stream;

   // Registrando código de saída, e medindo o tempo de execução.
   M = cria_cronometro();
   result.code = rotina();
   result.elapsed = marca(M);
   destroi_cronometro(M);

   // Lendo dados escritos no arquivo temporário...
   rewind(novo_stream);
   lido = fread(buffer, sizeof(char), TAMANHO_BUFFER, novo_stream);
   size = lido * sizeof(char);
   result.content.length = lido + 1;

   // Copiando para string, então fecha o stream.
   result.content.bytes = malloc(size);
   strcpy(result.content.bytes, buffer);

   // Fecha stream temporário, e remove arquivo relacionado.
   fclose(novo_stream);
   remove(nome_do_stream);
   // Retornando stream padrão da tela.
   stdout = antigo_stream;

   return result;
}

/*   O mesmo que o acima, porém usando named pipes, ao invés de arquivos 
 *   temporários.
 * Obs.: não é uma função multithread, então cuidado! */
static SR executa_e_captura_saida_i(FnTst rotina) {
   char nome_do_stream[100];
   FILE* antigo_stream = stdout;
   SR result;
   Cronometro M;

   // Formando nome do arquivo usado com meio.
   sprintf(nome_do_stream, "conteudo.%u", getpid());
   mkfifo(nome_do_stream, umask(0777));
   chmod(nome_do_stream, umask(0));
   // Desligando output padrão na tela, e ligando-o numa array.
   int saida = open(nome_do_stream, O_WRONLY);
   char* msg = "Any shit data in ...";
   write(saida, msg, strlen(msg) + 1);
   stdout = fdopen(saida, "w");

   // Registrando código de saída, e medindo o tempo de execução.
   M = cria_cronometro();
   result.code = rotina();
   result.elapsed = marca(M);
   destroi_cronometro(M);

   // Lendo dados escritos no arquivo temporário...
   char buffer[UINT16_MAX];
   int entrada = open(nome_do_stream, O_RDONLY);
   int lido = read(entrada, buffer, UINT16_MAX);
   assert (lido < UINT16_MAX);
   result.content.length = lido + 1;

   // Copiando para string, então fecha o stream.
   size_t size = result.content.length * sizeof(char);
   result.content.bytes = malloc(size);
   strcpy(result.content.bytes, buffer);

   // Fechando todos pipes e streams ...
   close(entrada);
   fclose(stdout);
   stdout = antigo_stream;
   close(saida);

   return result;
}

static TesteRelatorio executa_unico_teste(TesteSet a) {
/* Executa teste e colhe dados. No caso, a saída que tal programa gera;
 * o retorno final do teste(exit code); se o programa quebrou; e por fim,
 * o tempo que levou(em segundos). */
   int tubos_rw[2];
   TesteRelatorio report;
   pid_t subprocesso;
   size_t size = sizeof(TesteRelatorio);

   pipe(tubos_rw);
   subprocesso = fork();

   if (subprocesso == 0) {
      // Dentro do subprocesso(filho)...

      // Colocando todas informações do relatório no tipo de dado adequado.
      report.nome = a.nome;
      // SR result_a = executa_e_captura_saida(a.funcao);
      SR result_a = executa_e_captura_saida(a.funcao);
      report.decorrido = result_a.elapsed;
      report.conteudo = result_a.content; ;
      report.codigo = result_a.code;
      // Ainda a trabalhar tal mudança de valor...
      report.falhou = false;

      // Transferindo dados para o processo pai...
      write(tubos_rw[OUT], &report, size);
      // Transferindo string dinâmica, por que está na heap
      // deste processo apenas, somente threads compartilham memŕoria
      size_t bloco = sizeof(char) * report.conteudo.length;
      write(tubos_rw[OUT], report.conteudo.bytes, bloco);

      exit(EXIT_SUCCESS);
   } else {
      // Ainda no processo principal(pai)...
      read(tubos_rw[IN], &report, size);

      char buffer[10000]; 
      size_t t = report.conteudo.length;
      size_t bloco = sizeof(char) * t;

      // Recebendo cópia da string dinâmica no outro processo.
      read(tubos_rw[IN], buffer, t);
      // Copiando a string para a estrutura.
      report.conteudo.bytes = malloc (bloco);
      strcpy(report.conteudo.bytes, buffer);
   }
   return report;
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

void testes_unitarios_i(bool mostrar_saida, bool paralelizacao, 
  bool desativa_suite, const uint8_t total, ...) 
{
/* O mesmo que acima, porém permite desligar o output dos testes na 
 * saída padrão, assim como também permite a paralelização dos testes
 * unitários. */
   va_list args;
   // Contabilização de testes que são para executar.
   // uint8_t habilitados = 0;
   // Array com espaço para todos testes habilitados.
   // RetornoTst resultados[total];
   // Contador dos testes que não passaram, retorna algum erro previsível.
   // uint8_t nao_passaram = 0;
   TesteSet colecao[total];
   // Utilitário está apenas disponível para sistemas Unix's.
   // Cronometro medidor;

   #ifdef _NOVO_SUITE_COMPLETO
   printf(
      "Comandos argumentos:\n\ttotal: %u\n\tmostrar saída: %s\n\t"
      "paralelização: %s\n\tdesativar suíte: %s\n\n",
      total, bool_to_str(mostrar_saida), 
      bool_to_str(paralelizacao),
      bool_to_str(desativa_suite)
   );
   puts("\nColhendo testes, e listagem de suas configurações:");
   #endif
   va_start(args, total);

   for (uint8_t i = 1; i <= total; i++) {
      colecao[i - 1] = va_arg(args, TesteSet);   

      uint8_t p = i - 1;
      FnTst endereco_fn = colecao[p].funcao;
      char* nome_fn = colecao[p].nome;
      bool ativado_fn = colecao[p].confirmacao;
      const char* okay_str = bool_to_str(ativado_fn);

      #ifdef _NOVO_SUITE_COMPLETO
      printf(
         "\t[%p] '%s', acionada? %s.\n", 
         endereco_fn, nome_fn, okay_str
      );
      #endif
   }
   va_end(args); 

   // Começando a contabilizar a execução de cada teste...
   // medidor = cria_cronometro();
   // destroi_cronometro(medidor);
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
#include "barra_de_progresso.h"
#include <unistd.h>
#include <errno.h>

int capitaliza_palavras(void) {
   puts("Capitalizando tais palavras ...");

   for (size_t i = 1; i <= FRUTAS; i++) {
      char* fruta = (char*)frutas[i - 1];
      size_t size = strlen(fruta) + 1;
      char copia[size];

      strcpy(copia, fruta);
      copia[0] = (char)toupper(copia[0]);
      printf("\t%s ===> %s\n", fruta, copia);
   } 
   return EXIT_SUCCESS;
}

int string_alternada(void) {
   puts("Colocando caractéres de forma alternada ...");
   for (size_t i = 1; i <= GIRLS_NAMES; i++) {
      char* nome = (char*)girls_names[i - 1];
      size_t size = strlen(nome) + 1;
      char copia[size];

      strcpy(copia, nome);

      for (size_t i = 0; i < strlen(copia); i++) {
         if (i % 2 == 0) 
            copia[i] = toupper(copia[i]);
         else
            copia[i] = tolower(copia[i]);
      }
      printf("\t%s ===> %s\n", nome, copia);
   } 
   return EXIT_SUCCESS;
}

// Função falha sempre(não chega a retornar o código(exit code) de sucesso.
int carregando_ate_cem(void) {
   PS bps = novo_bps(100, 50);
   size_t contagem = 1;
   // 70 milisegundos ...
   struct timespec pausa = { 0, 70000000 };

   do {
      visualiza_bps(bps);
      atualiza_bps(bps, contagem++);
      nanosleep(&pausa, NULL);

      if (contagem == 70)
         return EXIT_FAILURE;
   } while (contagem <= 100);

   return EXIT_SUCCESS;
}

void novo_suite_de_testes_unitarios(void) {
   puts("Chamada no escopo para ver se funciona, e como fica: ");

   TestesUnitarios (
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

void executa_um_teste_unitario_e_coleta_informacoes(void) {
   puts("\n\nNovo testador é só a versão acima, porém completa:");
   TesteSet t1 = Unit(capitaliza_palavras, false);
   TesteSet t2 = Unit(string_alternada, false);
   TesteSet t3 = Unit(carregando_ate_cem, true);

   testes_unitarios_i (false, true, false, 3, t1, t2, t3);

   // TesteRelatorio r1 = executa_unico_teste(t1);
   TesteRelatorio r2 = executa_unico_teste(t2);
   TesteRelatorio r3 = executa_unico_teste(t3);

   // visualiza_teste_relatorio(&r1);
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

void simple_teste_de_named_pipe(void) {
   const char* NOME = "named_pipe_teste";
   const int OK = 0;
   const int FAIL = -1;

   remove(NOME);
   mkfifo(NOME, umask(0001));
}

void captura_de_saida_via_named_pipe(void) {
   puts("O teste é um testes de capturar a saída padrão via 'named pipe'.");
   const char* NOME = "via_de_transferencia";
   const int OK = 0;
   const int FAIL = -1;
   mode_t atributo = umask(0664);
   int result;

   result = mkfifo(NOME, umask(0));
   if (result == OK)
      printf("[%s]!\n", strerror(errno));
   else
      puts("Named Pipe criado com sucesso.");

   result = chmod(NOME, umask(770));
   if (result == FAIL)
      printf("[%s]!\n", strerror(errno));
   else
      puts("Permissão alterada com sucesso.");

   /*
   int saida = open(NOME, O_WRONLY);
   FILE* novo_stream_de_saida = fdopen(saida, "w");
   FILE* stream_padrao = stdout;
   puts("Arquivo do 'named pipe' foi aberto com sucesso.");
   */
   /*
   printf("Mudando a saída padrão ...");
   fflush(stdout);
   stdout = novo_stream_de_saida;
   puts("feito");*/

   freopen(NOME, "wt", stdout);
   puts("Testando saídas ...");

   for (int i = 1; i <= 15; i++) 
      printf("\t%dª mensagem.\n", i);
}
#endif
