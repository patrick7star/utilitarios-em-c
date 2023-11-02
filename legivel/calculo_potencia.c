

/* calcula uma pontência, porém retorna ela como um grande inteiro --
 * o maior tipo que existe, já que na falta de um deste na biblioteca
 * padrão do C, é preciso criar um. Não computa para expoentes negativos,
 * por motivos óbvio do tipo de retorno.
 */

#include <stdint.h>

static uint64_t potencia(uint64_t b, uint8_t e) {
   uint64_t produto = 1;

   for (uint8_t k = 1; k <= e; k++)   
      produto *= b;

   return produto;
}
