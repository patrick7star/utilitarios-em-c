
/* Tal estrutura e suas operações foram jogadas aqui para não tumuar o
 * arquivo principal com muito código, assim melhorar a legibilidade.
 */

#include "tupla.c"

/* Este código não precisa de bibliotecas para funcionar pois será incluída
 * no código principal via diretivas de compilação. */

struct contador_regressivo {
   // tempo que ele durará.
   TEMPO_TIPO ordem;
   uint16_t unidades;

   // contador de reusos desta instância.
   uint8_t reutilizacoes;

   // informa se a contagem acabou.
   bool esgotado;

   // percentual decorrido na contagem.
   uint8_t percentual;

   // referências para alguns valores aqui que mudam em "parelo".
   TuplaThread referencias;

   /* função responsável pela alteração desta estrutura que diz se 
    * o temporizador chegou ao seu fim.*/
   thrd_t mudador_status;
};

// tamanho(em bytes) e nome curto para codificação:
typedef struct contador_regressivo *TEMPORIZADOR;
# define TIMER_SIZE sizeof(struct contador_regressivo)

/* contabiliza as várias instâncias que foram criadas deste tipo.*/
static uint8_t instancias_timer = 0;

#include "computa_percentil.c"

static thrd_start_t altera_status(TuplaThread* args) {
   /*   Muda o valor da variável lógica da estrutura para 'verdadeira', 
    * depois de certo período de tempo.
    *   Ao invés de uma chamada de pausa, faz cem delas, assim pode-se
    * acompanhar o progresso percentual do 'temporizador'. Por ser
    * apenas 100 chamadas, não é esperado que isso prejudique o
    * desempenho,e consequemente funcionamento da estrutura.
    */
   Duracao T = centesima_unidade((*args).grandeza, (*args).unidades);

   #ifdef _DEBUG_ALTERA_STATUS
   printf("\ntipo: '%s'\tunidades: %lu\n", tempotipo_str(T.t), T.n);
   #endif

   for (int k = 100; k > 0; k--) {
      *args->porcentagem += 1;
      breve_pausa(T.t, T.n);
   }

   *args->estado = true;
   #ifdef _DEBUG_ALTERA_STATUS
   puts("tempo esgotado, 'estado' foi alterado.");
   #endif
   return thrd_success;
}

TEMPORIZADOR cria_temporizador(TEMPO_TIPO  tipo, uint16_t n) {
   /* Por usar concorrência que disputam o mesmos dados, serão permitidos
    * apenas uma instância deste tipo de dado por vez. 
    */
   TEMPORIZADOR timer = malloc(TIMER_SIZE);

   // se a alocação falhar, então interromper o programa.
   if (timer == NULL) {
      perror(
         "[error 1]:devido a não alocação do temporizador, não será "
         "possível o programa está sendo interrompindo!"
      );
      abort();
   }

   timer->reutilizacoes = 0;
   // arranjando tempo demandado...
   timer->ordem = tipo;
   timer->unidades = n;
   timer->esgotado = false;
   timer->percentual = 0;

   // referênciando os dois últimos atributos.
   timer->referencias.porcentagem = &timer->percentual;
   timer->referencias.estado = &timer->esgotado;
   timer->referencias.unidades = n;
   timer->referencias.grandeza = tipo;

   // criando thread até antes da criação da instância.
   int resultado = thrd_create(
      &timer->mudador_status,
      (thrd_start_t)altera_status, 
      &timer->referencias
   );

   /* Para o programa se não foi criada de acordo com o esperado. Se
    * foi bem sucedida, então apenas mostra uma mensagem que a contagem
    * regressiva foi acionada. */
   switch(resultado) {
      case thrd_success:
         puts("contagem-regressiva acionada com sucesso.");
         break;
      default:
         perror("algum erro desconhecido, por enquanto.");
         abort();
   }
   
   // contabilizando mais uma instâncias de 'temporizador'.
   instancias_timer++;
   // retorna a nova instância criada.
   return timer;
}

void destroi_temporizador(TEMPORIZADOR t, bool info) {
   // (na verdade não sei o porquê disso aqui, porém deixar inicialmente para não arranjar problema.)
   // quando destruído, o status esgotado se vai.
   // t->esgotado = false;

   #ifdef _DEBUG_DESTROI_TEMPORIZADOR
      printf("a liberação do 'temporizador'... ");
   #endif

   free(t);
   // se passou no passo acima, então foi liberado com sucesso.
   // agora é sobre descontabiliza a instâncias liberada.
   instancias_timer--;

   #ifdef _DEBUG_DESTROI_TEMPORIZADOR
      puts("foi concluída.");
   #endif
}

uint8_t instancias_temporizador()
   { return instancias_timer; }

bool esgotado(TEMPORIZADOR t)
   { return t->esgotado; }

float percentual(TEMPORIZADOR t) 
   { return (float)t->percentual / 100.0; }

