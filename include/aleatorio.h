/* Começando novamente a produção deste módulo. Eu não sei o porquê, porém
 * o último tem um problema sério com as declarações de funções neste 
 * arquivo. */

#ifndef ALEATORIO_H
#define ALEATORIO_H
#include <stddef.h>
#include <stdbool.h>
#include "definicoes.h"

 /* Sorteia um inteiro positivo entre os limites dados(incluído o valor
  * superior). */
 extern size_t inteiro_positivo (size_t , size_t);

 // Sorteia uma letra do alfabeto, seja ela minúscula ou maiúscula.
 unsigned char alfabeto_aleatorio (void);

 // Sorteia um valor lógico de maneira randômica.
 extern bool logico (void);

 // Sorteia um caractére(imprível) da tabela ASCII.
 char ascii_char_aleatorio (void);

 /* Sorteia uma palavra randômica do dicionário, este geralmente nativo do
  * sistema operacional. Se não houver um dicionário(usa como padrão o do 
  * Linux), e você tem algum dicionário com palavras(uma palavra por linha),
  * você pode adiciona-lo definindo a variável DICIONARIO_UTILITARIOS como 
  * o tal caminho para o arquivo. 
  */
 char* palavra_aleatoria (void);
  
 /* Embaralha arrays de qualquer tipo, claro, dado seu tamanho e tamanho
  * em bytes do tipo. */
 void embaralha_array(generico_t seq, int n, int sz);

#endif // ALEATORIO_H
