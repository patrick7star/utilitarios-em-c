
#ifndef __TEMPO_H__
#define __TEMPO_H__
#include <stdbool.h>
#include <stdint.h>

 typedef enum tipo_de_tempo { 
   // Tempo normal:
   Hora, Minuto, Segundo, 
   // Abaixo de um segundo:
   Miliseg, Microseg, Nanoseg 

 } TempoTipo, TEMPO_TIPO;

 /* Apelidos da estruturas -- por mais que os métodos só exaltem apenas um,
  * os demais também são válidos; todos seus métodos e funções relacionadas
  * com a estrutura.
  *   Todos respectivos apelidos para o tipo de dado também são postos
  * abaixo. Os dois primeiros "incorporam" o ponteiro para você não se 
  * preocupar com isso, já o último é preciso específicar-lô. */
 typedef struct cronometro 
   CRONOMETRO, *Cronometro, 
   cronometro_t;
 typedef struct contador_regressivo 
   *Temporizador, *Timer, 
   TIMER, temporizador_t;

 /* executa uma pausa no programa dado a grandeza do tempo, e o total de 
  * unidades daquela grandeza. */
 extern void breve_pausa(TEMPO_TIPO tipo, uint16_t qtd); 

 /* Operações de ambas estruturas de dados acima. */

 // construtor e destruturo do 'cronômetro'.
 extern Cronometro cria_cronometro(); 
 void destroi_cronometro(Cronometro); 

 // operações sobre o cronômetro:
 extern double marca(Cronometro c); 
 /* Computa a variação deste o último registro marcado, se houve algum, se
  * não, então computa o tempo decorrido desde o ínicio da contagem.*/
 extern double variacao(Cronometro c); 

 // visualização de registros feitos no 'cronômetro'.
 extern void visualiza_marcos(Cronometro c); 
 /* Retorna a formatação string do cronômetro, sendo o nome do tipo de 
  * dado, o tempo decorrido até então, e o total de marcações até agora. */
 char* cronometro_to_str(Cronometro); 


 // construtor e destrutor do 'temporizador'.
 extern Temporizador cria_temporizador (TEMPO_TIPO, uint16_t); 
 extern void destroi_temporizador (Temporizador); 

 // total de instância de 'Temporizadores' na memória.
 extern uint8_t instancias_temporizador();

 // verificação de o Temporizador terminou sua contagem-regressiva.
 extern bool esgotado (Temporizador);
 extern float percentual (Temporizador);

 // ainda em fase de testes:
 char* timer_to_str (Temporizador);
 void recomecar (Temporizador);

#endif //TEMPO_H
