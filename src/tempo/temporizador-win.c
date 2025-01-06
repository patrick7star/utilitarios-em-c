/* Mesmo que o temporizador principal, porém compatível com Windows apenas.
 * Todo o mecanismo interno usa coisas do Windows(WinAPI32). Quando for 
 * incluir tal no arquivo principal específique este na devida plataforma. 
 */

#include <string.h>
#include "tupla.c"
// API do Windows:
#include <windows.h>
#include <processthreadsapi.h>

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
   // thrd_t mudador_status;
	LPTHREAD_START_ROUTINE mudador_status;
};

// tamanho(em bytes) e nome curto para codificação:
# define TIMER_SIZE sizeof(struct contador_regressivo)

/* contabiliza as várias instâncias que foram criadas deste tipo.*/
static uint8_t instancias_timer = 0;

/* Funções auxiliares que tal módulo porta:
 *       - tempotipo_str
 *       - centesima_unidade
 */
#include "computa_percentil.c"

static DWORD WINAPI altera_status(void* argumentos) {
/*   Muda o valor da variável lógica da estrutura para 'verdadeira', 
 * depois de certo período de tempo.
 *   Ao invés de uma chamada de pausa, faz cem delas, assim pode-se
 * acompanhar o progresso percentual do 'temporizador'. Por ser
 * apenas 100 chamadas, não é esperado que isso prejudique o
 * desempenho,e consequemente funcionamento da estrutura.
 */
	TuplaThread* args = (TT*)argumentos;
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
   // return thrd_success;
	return EXIT_SUCCESS;
}

Temporizador cria_temporizador(TEMPO_TIPO  tipo, uint16_t n) {
/* Por usar concorrência que disputam o mesmos dados, serão permitidos
 * apenas uma instância deste tipo de dado por vez. */
   Temporizador timer = malloc(TIMER_SIZE);

   // se a alocação falhar, então interromper o programa.
   if (timer == NULL) {
      const char* mensagem_erro = {
         "[erro(1)] Devido a não alocação do temporizador, não será "
         "possível o programa está sendo interrompindo!"
      };
      perror(mensagem_erro);
      abort();
   } else if (instancias_timer == UINT8_MAX) {
      /* Lembrete: aumentar tal capacidade de instânciações. Inicialmente
       * foi uma restrição alto imposta, para no mínimo um, por medo de 
       * não abarcar multithread-safety, entretanto, a implementação mudou
       * e, é possível múltiplos, mais ainda não testado até quantos 
       * múltiplas instâncias é possível, e o consumo de recursos para
       * cada uma gerada. */
      perror("[erro(2)] Limite de temporizadores na executação atingido.");
      abort();
   } else if (n >= 1000) {
      const char* mensagem_erro = {
         "[erro(3)] Não é possível criar com tals tanto de unidades, para"
         " tal grandeza de tempo"
      };
      perror(mensagem_erro);
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
	HANDLE tredi = CreateThread
		(NULL, 0, altera_status, &timer->referencias, 0, NULL); 

   /* Para o programa se não foi criada de acordo com o esperado. Se
    * foi bem sucedida, então apenas mostra uma mensagem que a contagem
    * regressiva foi acionada. */
	if (tredi == NULL) {
		#ifdef _Temporizador
		puts("contagem-regressiva acionada com sucesso.");
		#endif
	} else {
		perror("algum erro desconhecido, por enquanto.");
		abort();
	}

   // Contabilizando mais uma instâncias de 'temporizador'.
   instancias_timer++;
   // Retorna a nova instância criada.
   return timer;
}

uint8_t instancias_temporizador()
   { return instancias_timer; }

bool esgotado(Temporizador t)
   { return t->esgotado; }

float percentual(Temporizador t) 
   { return (float)t->percentual / 100.0; }

void recomecar (Temporizador obj) {
/* Ao invés de ter que destruir a instância, e criar uma nova com o mesmo
 * tipo, tal método, permite-o reutilizar tal, resetando todos seus 
 * contadores internos, e começando uma nova thread-contadora para 
 * modifica-lôs. */
   // Incrementa contador de 'reutilizações'.
   obj->reutilizacoes += 1;
   // Desmarca tal como esgotado, e também zera percentual.
   obj->esgotado = false;
   obj->percentual = 0.0;

   /* Criando thread até antes da criação da instância.
    * Disparando, novamente, a thread que muda estado. */
	DWORD id;

	HANDLE tredi = CreateThread
		(NULL, 0, altera_status, &obj->referencias, 0, &id); 

   /* Para o programa se não foi criada de acordo com o esperado. Se
    * foi bem sucedida, então apenas mostra uma mensagem que a contagem
    * regressiva foi acionada. */
	if (tredi == NULL) {
		#ifdef _Temporizador
		puts("contagem-regressiva acionada com sucesso.");
		#endif
	} else {
		perror("algum erro desconhecido, por enquanto.");
		abort();
	}
}

static const char* sufixo_do_tempotipo(TempoTipo tipo) {
//  Retorna sufixo que identifica cada submultiplo da grandeza dada. 
   switch (tipo) {
   case Nanoseg:
      return "ns";
   case Microseg:
      return "\u03bcs";
   case Miliseg:
      return "ms";
   case Segundo:
      return "s";
   case Minuto:
      return "min";
   case Hora:
      return "h";
   }

   perror("'TempoTipo' inválido.");
   abort();
   return NULL;
}

static char* transforma_em_str(uint8_t u, TempoTipo t) {
/* Dada as unidades, e o tipo de grandeza de tempo, formata parte do 
 * output da string formatada abaixo.
 */
   size_t tamanho = 100 * sizeof(char);
   char* string = malloc(tamanho);

   sprintf(string, "%u%s", u,  sufixo_do_tempotipo(t));
   return string;
}

char* timer_to_str(Temporizador obj) {
/* Transforma o 'temporizador' numa string, apresentando alguns dados sobre
 * ele, como: seu tempo inicial, quanto de já decorreu, e se tal já foi
 * esgotado. Não necessiramente nesta ordem. */
   size_t tamanho = 100 * sizeof(char);
   char* buffer = malloc(tamanho);
   char esgotado[10];
   uint8_t p = obj->percentual;
   uint8_t u = obj->unidades;
   TempoTipo t = obj->ordem;
   char* limite_str = transforma_em_str(u, t);

   if (!obj->esgotado)
      strcpy_s(esgotado, 10, "on");
   else
      strcpy_s(esgotado, 10, "off");

   sprintf(buffer, "Timer[%s | %s |%3u%%]", limite_str, esgotado, p);
   // Liberando string formatada para cópia...
   free(limite_str);

   return buffer;
}

void destroi_temporizador(Temporizador t) {
   #ifdef _DEBUG_DESTROI_Temporizador
      printf("a liberação do 'temporizador'... ");
   #endif

   free(t);
   // agora é sobre descontabiliza a instâncias liberada.
   instancias_timer--;

   #ifdef _DEBUG_DESTROI_Temporizador
      puts("foi concluída.");
   #endif
}
