/* Mesmo que o temporizador principal, porém compatível com Windows apenas.
 * Todo o mecanismo interno usa coisas do Windows(WinAPI32). Quando for 
 * incluir tal no arquivo principal específique este na devida plataforma. 
 */

#ifndef __TEMPO_H__
#define __TEMPO_H__
#include <stdbool.h>
#include <stdint.h>

 typedef enum tipo_de_tempo { 
   Hora, Minuto, Segundo, 
   Miliseg, Microseg, Nanoseg 

 } TempoTipo, TEMPO_TIPO;

 /* executa uma pausa no programa dado a grandeza do tempo, e o total de 
  * unidades daquela grandeza. */
 extern void breve_pausa(TempoTipo tipo, uint16_t qtd); 

/* === === === === === === === === === === === === === === === === === ==
 *									Cronômetro
 *
 * Passar o cronômetro para aqui, onde fica mais organizado a distinção
 * entre seus métodos, e outras funções do 'módulo tempo', sem falar
 * que facilita a leitura.
 * === === === === === === === === === === === === === === === === === ==*/
 typedef struct cronometro CRONOMETRO, *Cronometro, cronometro_t;

 Cronometro cria_cronometro(void); 
 void destroi_cronometro(Cronometro); 

 // operações sobre o cronômetro:
 double marca(Cronometro c); 
 /* Computa a variação deste o último registro marcado, se houve algum, se
  * não, então computa o tempo decorrido desde o ínicio da contagem.*/
 double variacao(Cronometro c); 

 // visualização de registros feitos no 'cronômetro'.
 void visualiza_marcos(Cronometro c); 
 /* Retorna a formatação string do cronômetro, sendo o nome do tipo de 
  * dado, o tempo decorrido até então, e o total de marcações até agora. */
 char* cronometro_to_str(Cronometro); 

/* === === === === === === === === === === === === === === === === === ==
 *									 Temporizador
 * === === === === === === === === === === === === === === === === === ==*/
 typedef struct contador_regressivo *Temporizador, *Timer, 
   TIMER, temporizador_t;

 Temporizador cria_temporizador (TempoTipo, uint16_t); 
 void destroi_temporizador (Temporizador); 

 // Total de instância de 'Temporizadores' na memória.
 uint8_t instancias_temporizador(void);

 // Verificação de o Temporizador terminou sua contagem-regressiva.
 bool esgotado (Temporizador);
 float percentual (Temporizador);

 // ainda em fase de testes:
 char* timer_to_str (Temporizador);
 void recomecar (Temporizador);

#endif //TEMPO_H
