/* O modo de criar paralelismo para o suite executor será trabalhado aqui.
 * Mesmo motivo de sempre, mais organização e claridade, do que o amontoado
 * do outro arquivo; no fim, apenas anexar lá(include diretriz) nele. */
#include <threads.h>
#include <string.h>
#include <time.h>

// Tuplas com entradas e saídas das seguintes threads-funções abaixo:
struct tupla_arg_ct { TesteRelatorio* out; TesteSet* in; };
struct tupla_arg_jt { thrd_t in; int* out; };


static int procedimento_jt(void* arg_ptr) {
   struct tupla_arg_jt* arg = arg_ptr;
   int exitcode;

   thrd_t id = arg->in;
   // Agora tenta o join.
   *(arg->out) = thrd_join(id, &exitcode);

   #ifdef _NOVO_SUITE
   printf("exit code: %d\n", exitcode);
   #endif

   thrd_exit(exitcode); 
}

bool join_timeout(thrd_t id, int miliseg) {
   // Supõe um sucesso desde o ínicio.
   int resultado = thrd_success;
   struct tupla_arg_jt arg = { id,  &resultado };
   // Convertendo nanosegundos para milisegundos...
   size_t ns = miliseg * 1000000;
   const struct timespec LIMITE = {0, ns};

   // Limite de tempo é 1min.
   if (miliseg > 60000) {
      perror("Só é possível um tempo de até 1min.");
      abort();
   }

   /* Choca thread, logo em seguida aguarda do demandado, se nada mudou
    * apenas encerra, e retorna o que aconteceu. */
   int criacao = thrd_create(&id, procedimento_jt, &arg); 

   // Exibindo o erro e interrompendo...
   if (criacao != thrd_success) {
      perror(strerror(errno));
      abort();
   } else 
      thrd_sleep(&LIMITE, NULL);

   if (resultado == thrd_success)
      return true;
   else
      return false;
}

static int procedimento_ct(void* arg) {
   struct tupla_arg_ct* ARG = arg;
   TesteSet a = *(ARG->in);
   TesteRelatorio b = executa_unico_teste(a);

   // memcpy(ARG->out, &b, sizeof(TesteRelatorio));
   *(ARG->out) = b;

   return EXIT_SUCCESS;
}

thrd_t* dispara_todos_testes(TesteSet* tests, TesteRelatorio* reports, 
  int total)
{
   thrd_t* IDs = malloc(total * sizeof(thrd_t));

   #ifdef _NOVO_SUITE
   printf("total: %d\n", total);
   #endif

   for (int j = 0; j < total; j++) {
      struct tupla_arg_ct arg = {
         .out = &reports[j], 
         .in = &tests[j]
      };

      int result = thrd_create(&IDs[j], procedimento_ct, &arg);
      #ifdef _NOVO_SUITE
      puts("Funcionou com esta thread.");
      #endif

      if (result != thrd_error) {
         perror(strerror(errno));
         abort();
      }
   }  
   return IDs;
}


#ifdef _UT_TESTE
/* == == == == == == == == == == == == == == == == == == == == == == == ==
 *                         Testes Unitários
 *                      e Alguns Experimentos 
 *                         a Serem Realizados
 * == == == == == == == == == == == == == == == == == == == == == == == = */
static void visualizacao_sintetizada_do_tr(TesteRelatorio* e) {
   printf("['%s', %d, %zu, %s, %0.4lf]\n",
      e->nome, e->codigo, e->conteudo.length, 
      bool_to_str(!e->falhou), e->decorrido
   );
}

void trabalhando_no_paralelismo(void) {
   puts("Primeiro experimento em paralelismo de testes começando...");

   TesteSet ta = Unit(string_alternada, true);
   TesteSet tb = Unit(executa_algo_mas_crash, true);
   TesteSet tc = Unit(carregando_ate_cem, true);

   TesteRelatorio report_a, report_b, report_c;
   thrd_t id_a, id_b, id_c; 

   struct tupla_arg_ct A = {.out=&report_a, .in=&ta};
   thrd_create(&id_a, procedimento_ct, &A);

   struct tupla_arg_ct B = {.out=&report_b, .in=&tb};
   thrd_create(&id_b, procedimento_ct, &B);

   struct tupla_arg_ct C = {.out=&report_c, .in=&tc};
   thrd_create(&id_c, procedimento_ct, &C);

   puts("Finalização começando...");
   thrd_join(id_a, NULL);
   printf("\tTeste '%s' finalizado.\n", report_a.nome);
   thrd_join(id_b, NULL);
   printf("\tTeste '%s' finalizado.\n", report_b.nome);
   thrd_join(id_c, NULL);
   printf("\tTeste '%s' finalizado.\n", report_c.nome);

   visualizacao_sintetizada_do_tr(&report_a);
   visualizacao_sintetizada_do_tr(&report_b);
   visualizacao_sintetizada_do_tr(&report_c);
}

void usando_arrays_para_compactar_o_codigo_acima(void) {
   puts(
      "Parece que está tento algum problema em eu aplicar o código acima"
      " usando de arrays e for-loops, irei aplicar o mesmo aqui abaixo,"
      " agora que consegui rotar sem problema o código acima."
   );

   TesteSet testes[3] = {

      Unit(string_alternada, true),
      Unit(executa_algo_mas_crash, true),
      Unit(carregando_ate_cem, true)
   };
   TesteRelatorio reports[3];
   thrd_t IDs[3];
   struct tupla_arg_ct args[3];

   for (int p = 0; p < 3; p++) {
      args[p] = (struct tupla_arg_ct){.out=&reports[p], .in=&testes[p]};
      thrd_create(&IDs[p], procedimento_ct, &args[p]);
   }

   puts("\nFinalização começando...");
   for (int p = 0; p < 3; p++) {
      thrd_join(IDs[p], NULL);
      printf("\tTeste '%s' finalizado.\n", reports[p].nome);
      putchar('\t');
      visualizacao_sintetizada_do_tr(&reports[p]);
      putchar('\n');
   }
}

void verificando_se_a_thread_nao_bagunca_o_output(void) {
   TesteSet testes[5] = {
      Unit(string_alternada, true),
      Unit(executa_algo_mas_crash, true),
      Unit(carregando_ate_cem, true),
      Unit(capitaliza_palavras, true),
      Unit(simples_mensagem_do_fortune, true)
   };
   TesteRelatorio reports[5];
   thrd_t IDs[5];
   struct tupla_arg_ct args[5];

   for (int p = 0; p < 5; p++) {
      args[p] = (struct tupla_arg_ct){.out=&reports[p], .in=&testes[p]};
      thrd_create(&IDs[p], procedimento_ct, &args[p]);
   }

   puts("\nFinalização começando...");
   for (int p = 0; p < 5; p++) {
      thrd_join(IDs[p], NULL);
      printf("\tTeste '%s' finalizado.\n", reports[p].nome);
      printf("\tEste é o output: %s\n%s\n%s\n\n", ASPAS, reports[p].conteudo.bytes, ASPAS);
   }
}

int msg_loop(void* arg) {
   int count = 0;
   do {
      printf("\t[thread]This is the message(%d)\n", count++);
      sleep(1);
   } while (count < 8);

   return EXIT_SUCCESS;
}

int msg_loop_alfa(void* arg) {
   int count = 0;
   do {
      printf("\t\t[second thread]A pretty yellow tweety bird(%d)\n", 
            19 - count++);
      sleep(1);
   } while (count < 19);

   return EXIT_SUCCESS;
}

void visualizacao_do_join_temporario(void) {
   thrd_t id; int count = 0;
   int result = thrd_create(&id, msg_loop, NULL);

   if (result == thrd_success)
      puts("Thread iniciada com sucesso.");

   do {
      bool terminado = join_timeout(id, 500);

      if (terminado)
         break;
      printf("[principal] Tentativa nª.: %dª\n", count);
   } while(count++ < 100);
}

void manuseio_de_duas_threads(void) {
   thrd_t IDs[2]; 
   int tentativas = 0, juncoes = 0;

   thrd_create(&IDs[0], msg_loop, NULL);
   thrd_create(&IDs[1], msg_loop_alfa, NULL);

   printf("IDs: [%ld], [%ld]\n\n", IDs[0], IDs[1]);

   do {
      int p = (tentativas + 1) % 2;
      bool terminado = join_timeout(IDs[p], 500);
      tentativas++;

      if (terminado)
         juncoes++;

      printf("\nTentativas= %d\tJunções=%d\n", tentativas, juncoes);
   } while(juncoes < 2);
}

void rodando_um_testeset_via_join_timeout(void) {
   TesteSet teste[1] =  { Unit(carregando_ate_cem, true) };
   TesteRelatorio report[1];

   thrd_t* pool = dispara_todos_testes(teste, report, 1);
   while (join_timeout(pool[0], 2000));
   visualizacao_sintetizada_do_tr(&report[0]);
}

#endif
