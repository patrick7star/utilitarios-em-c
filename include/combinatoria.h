/*   Classe de funções básicas que permitem a geração de permutações e seus
 * derivados.
 *   É para uso genérico, ou sejam, aceitam qualquer tipo, dado é claro,
 * as funções que fazem comparações entre tais tipos.
 */

#ifndef __COMBINATORIA_H__
#define __COMBINATORIA_H__
#include <stdlib.h>
#include <stdint.h>
#include "definicoes.h"

/*   Itens a serem combinados e quanto deles há. Também a informação de 
 * quando de cada itens, serão tomados de tuplas k-k de 'n'(length), ou 
 * seja, o tamanho da combinação(que também é uma array). E por último -- 
 * e o mais importante -- como é algo genérico, deve se passar o tamanho
 * do tipo da array, seu tanto em bytes, prá ver o que se está 
 * trabalhando(porque vamos precisar realizar copias de bytes). 
 *   Agora, depois de alguns ajustes. Tal esturutura é o retorno principal
 * dois três algoritmos. Com arranjos e combinações é perfeito, já que era
 * basicamnete o mesmo, mas com 'permutas', alguns campos ficam subutilizados
 * nada que faça grande diferença. */
 typedef struct resultados_do_tres_grandes_algoritmos 
 {
   generico_t items;
   int length, k, size;

   /*   Lista de todas combinações(que são arrays), e de quantos resultados
    * estamos falando.
    *   Note que é uma lista de arrays de bytes, se quiser outra, apenas 
    * aplica o 'casting' de ponteiros na array que se quer trabalhar. */
   uint8_t** results;
   size_t total;

 } Combinacoes, Arranjos, Permutas, ResultadosCombinatorios;

 /* Função que computa o fatorial de um inteiro positivo até o valor trinta
  * e três. Um inteiro superior a este limite overflow a capacidade do
  * inteiro que computa seu total(um valor muito grande). */
 size_t fatorial(int n);

 /* Dado uma tupla com todos dados relevantes de uma array, tipo: ela mesma;
  * seu comprimento; o tanto de bytes do seu tipo; uma lista de arrays do
  * mesmo tipo, que embarque todas possíveis resultados, e tal quantia. Na
  * verdade, se o último não for fornecido, tal função pode advinhar tal
  * e alocar uma de bytes, o 'casting' é por você no final. */
 Permutas     gera_permutacoes (Generico seq, int n, int sz);
 Combinacoes  gera_combinacoes (Generico itens, int n, int k, int sz);
 Arranjos     gera_arranjos    (Generico itens, int n, int k, int sz);
 
 /* Por ser apenas uma estrutura, meio que uma só função serve para todas 
  * as demais. */
void free_resultados_combinatorios (ResultadosCombinatorios* X, Drop g);

#endif
