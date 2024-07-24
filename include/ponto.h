/*   O tipo de dado ponto serve como um referencial em várias aplicações que
 * exigem tais abstrações. Aqui esta originalmente para planos, porém, com
 * um campo a ser usado em três dimensões, apesar de todas atuais operações
 * trabalharem apenas em duas.
 *   Como pode ser visto seus método apenas copiam o valor da instância
 * passado, nada de referência, justamente por ser uma estrutura muito 
 * enxuta só com dados primitivos, porém não muitos para ser custo cria
 * aos bucados instâncias dela.
 *   Tanto a array de pontos como o modo de cria-la, ajudam a trabalhar com
 * vários 'pontos' de uma vez. Também haverá operações para este tipo
 * agrupado de dado.*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef PONTO_H
#define PONTO_H
 /* Representação das coordenadas 'y' e 'x', nesta ordem. */
 typedef struct coordenada_bidimensional { uint16_t y; uint16_t x; }
   Ponto, PONTO, ponto_t;
 // uma array do tipo Ponto, seu alocador e destruídor. 
 typedef Ponto* ArrayPonto;

 /* cria e destrói o 'Ponto'. */
 Ponto cria_ponto (uint16_t y, uint16_t x);
 ArrayPonto cria_array_ponto (size_t qtd, ...);

 // Verifica se o ponto A é menor ou igual ao ponto B.
 bool eq_ponto (Ponto, Ponto); 
 bool ne_ponto (Ponto, Ponto);

 /* Todas tipos de visualização do 'Ponto'. */
 void imprime_ponto (Ponto);
 void imprime_ponto_debug (Ponto);
 char* ponto_to_str (Ponto);

 // A menor distância entre os dois pontos.
 uint16_t distancia_ponto(Ponto, Ponto); 

 /* Retorna todos pontos que formam um retângulo, dado dois pontos.
  * Tais pontos são organizados do canto superior esquerdo com
  * enumeração no sentido anti-horário à partir deste ponto até
  * o inferior esquerdo. */
 ArrayPonto retangulo_vertices(Ponto p, Ponto q); 
 void imprime_array_ponto (ArrayPonto, size_t); 

#endif  // PONTO(ADT)

