
/*
 * Códigos para gerar valores aletórios com todos os tipos primitivos, e
 * alguns derivados importantes como: espeficicamente, o alfabeto; palavras
 * dos dicionários em inglês e português; também, realiza embalharamento de
 * qualquer array.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifndef ALEATORIO_H
#define ALEATORIO_H

 typedef unsigned char char8_t;
 /* cospe uma letra aleatória do alfabeto, seja ela minúscula, ou 
  * maiúscula. */
 extern char8_t alfabeto_aleatorio(); 

 /* qualquer caractére da tabela ASCII, pode ser selecionado randomicamente,  * com exceção dos códigos de controles. 
  */
 extern char ascii_char_aleatorio();

 // seleciona randomicamente um valor lógico(verdadeiro ou falso)
 extern bool logico(); 

 // embalhara uma 'array de bytes', dado ela e seu tamanho.
 extern void embaralha(uint8_t*, size_t); 

 /* seleção de um inteiro positivo, dado os limites. */
 extern size_t inteiro_positivo(size_t, size_t); 

#endif 
