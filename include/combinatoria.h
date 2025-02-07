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

 struct Permutas {
   // Array que vai se permutada, e seu comprimento.
   generico_t array; 
   int length;

   // O tanto de bytes do tipo que é esta array.
   int size;

   // Lista de permutações, e seu total.
   uint8_t** list; 
   size_t total;
 };

 struct Combinacoes {
   /* Itens a serem combinados e quanto deles há. Também a informação de 
    * quando de cada itens, serão tomados de tuplas k-k de 'n', ou seja,
    * o tamanho da combinação(que também é uma array). E por último -- e o
    * mais importante -- como é algo genérico, deve se passar o tamanho
    * do tipo da array, seu tanto em bytes, prá ver o que se está 
    * trabalhando(porque vamos precisar realizar copias de bytes). */
   generico_t items;
   int n, k, size;

   /*   Lista de todas combinações(que são arrays), e de quantos resultados
    * estamos falando.
    *   Note que é uma lista de arrays de bytes, se quiser outra, apenas 
    * aplica o 'casting' de ponteiros na array que se quer trabalhar. */
   uint8_t** results;
   size_t total;
 };

 struct Arranjos {
 /* Basicamente o mesmo que combinações. */
   generico_t items; 
   int length, k, size;
   uint8_t** results;
   size_t total;
 };

 /* Função que computa o fatorial de um inteiro positivo até o valor trinta
  * e três. Um inteiro superior a este limite overflow a capacidade do
  * inteiro que computa seu total(um valor muito grande). */
 size_t fatorial(int n);
 /* Dado uma tupla com todos dados relevantes de uma array, tipo: ela mesma;
  * seu comprimento; o tanto de bytes do seu tipo; uma lista de arrays do
  * mesmo tipo, que embarque todas possíveis resultados, e tal quantia. Na
  * verdade, se o último não for fornecido, tal função pode advinhar tal
  * e alocar uma de bytes, o 'casting' é por você no final. */
 struct Permutas gera_permutacoes
   (Generico seq, int n, int sz);
 struct Combinacoes gera_combinacoes
   (Generico itens, int n, int k, int sz);
 struct Arranjos gera_arranjos
   (Generico itens, int n, int k, int sz);

#endif
