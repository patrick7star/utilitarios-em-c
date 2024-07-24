
temporizador_t cria_temporizador_s(TEMPO_TIPO  tipo, uint16_t n) {
   temporizador_t timer;
   // Nota: talvez o problema seja o escopo local, tal thread é chocada
   // aqui, com endereços(pointeiros) que são gerados em tal escopo, a 
   // thread é retornada, porém os endereços gerados localmente são 
   // apagados, futuramente a thread tenta acessar um pointeiro gerado
   // neste escopo, então o programa é interrompido, porque tal endereço
   // não existe mais(dangling pointer erro).
   timer.ptr = &timer;

   timer.reutilizacoes = 0;
   // arranjando tempo demandado...
   timer.ordem = tipo;
   timer.unidades = n;
   timer.esgotado = false;
   timer.percentual = 0;

   // referênciando os dois últimos atributos.
   timer.referencias.porcentagem = timer.ptr->percentual;
   timer.referencias.estado = timer.ptr->esgotado;
   // Apenas copia os valores que não mudam.
   timer.referencias.unidades = n;
   timer.referencias.grandeza = tipo;

   // criando thread até antes da criação da instância.
   int resultado = thrd_create(
      &timer.ptr->mudador_status,
      (thrd_start_t)altera_status, 
      &timer.ptr->referencias
   );

   /* Para o programa se não foi criada de acordo com o esperado. Se
    * foi bem sucedida, então apenas mostra uma mensagem que a contagem
    * regressiva foi acionada. */
   switch(resultado) {
      case thrd_success:
         #ifdef _Temporizador
         puts("contagem-regressiva acionada com sucesso.");
         #endif
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
#if defined(_UT_TEMPO)
#include <assert.h>

void inicializacao_do_temporizador_estatico(void) {
   TIMER t = cria_temporizador_s(Segundo, 4); 

   // Decorrendo algum tempo ...
   breve_pausa(Segundo, 3);
   puts("Tudo bem após certo tempo decorrido.");
   // Decorrendo tempo que excede o do temporizador ...
   breve_pausa(Segundo, 1);
   breve_pausa(Miliseg, 200);

   puts("Chegou ao fim sem qualquer problema.");
}

void verificando_esgotamento_do_temporizador(void) {
   TIMER t = cria_temporizador_s(Segundo, 4); 
   
   do {
      // char* timer_fmt = timer_to_str(&t);
      // puts(timer_fmt);
      breve_pausa(Miliseg, 500);
   } while (esgotado(&t));

   puts("Chegou ao fim sem qualquer problema.");
}
#endif
