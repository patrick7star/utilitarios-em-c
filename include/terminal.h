#include <stdint.h>

#ifndef TERMINAL_H
# define TERMINAL_H
 /* A dimensão é apenas uma array com dois inteiros positivos de 8-bits,
  * onde o primeiro valor é a 'altura', e o segundo a 'largura'. */
 typedef uint8_t *Dimensao, dimensao_t, DIMENSAO;

 /* Tupla com dimensão do terminal, onde o primeiro elemento é o nª de 
  * linhas, e o segundo só pode ser as colunas. */
 extern Dimensao dimensao_terminal(); 

  // Destrói o tipo de dado específico.
  extern void destroi_dimensao(Dimensao d);

#endif 
