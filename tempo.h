
#include <stdbool.h>
#include <threads.h>
#include <stdint.h>

#ifndef TEMPO_H
#define TEMPO_H
   enum tipo_de_tempo { 
      Segundo, Minuto, 
      Hora, Miliseg, 
      Microseg, Nanoseg 
   };
   typedef enum tipo_de_tempo TEMPO_TIPO, TempoTipo;
   
   // total de registros que é possível fazer.
   #define TOTAL_REGISTROS 100

   struct cronometro;   
   typedef struct cronometro* CRONOMETRO;
   typedef CRONOMETRO Cronometro;

   /* executa uma pausa no programa dado a grandeza do tempo,
    * e o total de unidades daquela grandeza.
    */
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

   struct contador_regressivo;
   typedef struct contador_regressivo* TEMPORIZADOR;

   // construtor e destrutor do 'temporizador'.
   extern TEMPORIZADOR cria_temporizador(TEMPO_TIPO  tipo, uint16_t n); 
   extern void destroi_temporizador(TEMPORIZADOR t); 

   // total de instância de 'Temporizadores' na memória.
   extern uint8_t instancias_temporizador();

   // verificação de o Temporizador terminou sua contagem-regressiva.
   extern bool esgotado(TEMPORIZADOR t);
   extern float percentual(TEMPORIZADOR t);

   // retorna a formatação string do cronômetro, sendo o nome do 
   // tipo de dado, o tempo decorrido até então, e o total de 
   // marcações até agora.
   extern char* cronometro_to_str(Cronometro c); 

#endif //TEMPO_H
