/* Simples funcionalidade de pegar a dimensão do terminal/ ou emultador de 
 * terminal atual, então retorna-lô. Outras coisas serão colocados no
 * futuro, por enquanto é só isso.
 */

#ifndef  __TERMINAL_H__
# define __TERMINAL_H__
#include <stdint.h>

 /* Resultado com linhas e colunas representando a dimensão da tela do 
  * console. Tipo de retorno que não é preciso desalocar posteriormente.
  */
 struct TerminalSize { uint8_t linhas; uint8_t colunas; };

 /* Retorna a dimensão, nos devidos campos nomeados, numa tupla. */
 struct TerminalSize obtem_dimensao (void);

 /* NOTA: Sim, a outra função não parece aqui. Tome isso como algo que foi
  *       descontinado. Por quê? Eu ja estava pensando em deleta-la, mas 
  *       muita depedência dela, então na versão do Windows realmente é uma
  *       uma oportunidade. Isso pois, aqui, eu só tenho a opção de não 
  *       fazer-la multi-plataforma, que foi o que fiz.
  */
#endif 
