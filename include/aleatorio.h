/* Começando novamente a produção deste módulo. Eu não sei o porquê, porém
 * o último tem um problema sério com as declarações de funções neste 
 * arquivo. */

#include <stddef.h>
#include <stdbool.h>

#ifndef _ALEATORIO_NOVO_H
#define _ALEATORIO_NOVO_H

 /* Sorteia um inteiro positivo entre os limites dados(incluído o valor
  * superior). */
 size_t inteiro_positivo (size_t , size_t);

 // Sorteia uma letra do alfabeto, seja ela minúscula ou maiúscula.
 unsigned char alfabeto_aleatorio (void);

 // Sorteia um valor lógico de maneira randômica.
 bool logico (void);

 // Sorteia um caractére(imprível) da tabela ASCII.
 char ascii_char_aleatorio (void);

 #ifdef __linux__
 /* Sorteia uma palavra randômica do dicionário, este geralmente nativo do
  * sistema operacional. */
 char* palavra_aleatoria(void);
 #endif

#endif // _ALEATORIO_NOVO_H
