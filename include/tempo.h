
#include <stdbool.h>
#include <threads.h>
#include <stdint.h>

#ifndef TEMPO_H
#define TEMPO_H
 typedef enum tipo_de_tempo { 
    Segundo, Minuto, 
    Hora, Miliseg, 
    Microseg, Nanoseg 
 }TempoTipo, TEMPO_TIPO;
 
 typedef struct cronometro *CRONOMETRO, *Cronometro, cronometro_t;

 /* executa uma pausa no programa dado a grandeza do tempo, e o total de 
  * unidades daquela grandeza. */
 extern void breve_pausa(TEMPO_TIPO tipo, uint16_t qtd); 

 /* Operações de ambas estruturas de dados acima. */

 // construtor e destruturo do 'cronômetro'.
 extern CRONOMETRO cria_cronometro(); 
 extern void destroi_cronometro(CRONOMETRO c, bool info); 

 // operações sobre o cronômetro:
 extern double marca(CRONOMETRO c); 
 extern double variacao(CRONOMETRO c); 

 // visualização de registros feitos no 'cronômetro'.
 extern void visualiza_marcos(CRONOMETRO c); 
 /* retorna a formatação string do cronômetro, sendo o nome do 
  * tipo de dado, o tempo decorrido até então, e o total de 
  * marcações até agora. */
 extern char* cronometro_to_str(Cronometro c); 


 /* Apelidos da estruturas -- por mais que os métodos só exaltem apenas um,
  * os demais também são válidos; todos seus métodos e funções relacionadas
  * com a estrutura.
  *   Todos respectivos apelidos para o tipo de dado também são postos
  * abaixo. Os dois primeiros "incorporam" o ponteiro para você não se 
  * preocupar com isso, já o último é preciso específicar-lô. */
 typedef struct contador_regressivo 
  *Temporizador, TIMER,
  temporizador_t;

 // construtor e destrutor do 'temporizador'.
 extern Temporizador cria_temporizador(TEMPO_TIPO, uint16_t); 
 extern void destroi_temporizador(Temporizador); 

 // total de instância de 'Temporizadores' na memória.
 extern uint8_t instancias_temporizador();

 // verificação de o Temporizador terminou sua contagem-regressiva.
 extern bool esgotado (Temporizador);
 extern float percentual (Temporizador);

 // ainda em fase de testes:
 extern char* temporizador_to_str (Temporizador);
 extern void recomecar (Temporizador);

#endif //TEMPO_H
