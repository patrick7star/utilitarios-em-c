/* Muitos das implementações das outras estruturas definidas neste diretório
 * usam muito estas definições. Aqui reescrevo elas, e nas demais, serão
 * simplesmente importadas daqui. Melhora as definições do demais, deixando
 * menos coisas no 'cabeçalho', e cada uma não fica reimplementando as
 * mesmas definições, mesmo que os impactos por causa disso até agora sejam
 * nulos.
 */

#ifndef __DEFINICOES_H__
#define __DEFINICOES_H__
/* Bastante necessária para definição de tipo lógico. Podia já vir no C
 * como padrão, porém não vem. */
#include <stdbool.h>
#include <stddef.h>

 /* O ponteiro do tipo void é algo parecido, mais longe de ser igual --
  * vamos dizer que permite realizar o mesmo -- função de genéricos em
  * várias linguagens.
  */
 typedef void* generico_t, *Generico, *GNRC;

 // Renomeando todos traços referênte aos 'fat pointers' abaixo:
 // Método essencial de cálculo do índice na tabela.
 typedef size_t (*Hash)(generico_t, size_t);
 // Método essencial para comparação de itens inseridos nela.
 typedef bool (*Eq)(generico_t, generico_t);
 // Método essencial para desalocar memória de objeto genérico.
 typedef bool (*Drop)(generico_t);
 // Propriedade de transformar o dado genérico numa string.
 typedef char* (*ToString)(generico_t);
 // Template de função da clonagem(aplica uma 'deepcopy') de um dado.
 typedef generico_t (*Clone)(generico_t);

 /*  A notação acima é bem Javeira e Rustcean, aqui vem como algo mais
  * Pythonico. */
 typedef Drop     __del__;
 typedef ToString __str__;
 typedef Hash     __hash__;
 typedef Eq       __eq__;
 typedef Clone    __clone__;
 /* Comparações de menor, maior, e menor/maior e igual. */
 typedef bool (*__lt__)(generico_t, generico_t);
 typedef bool (*__le__)(generico_t, generico_t);
 typedef bool (*__ge__)(generico_t, generico_t);
 typedef bool (*__gt__)(generico_t, generico_t);

#endif
