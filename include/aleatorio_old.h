
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

 // typedef unsigned char char8_t;
 /* cospe uma letra aleatória do alfabeto, seja ela minúscula, ou 
  * maiúscula. */
 // extern char8_t alfabeto_aleatorio (void); 
 unsigned char alfabeto_aleatorio (void);

 /* qualquer caractére da tabela ASCII, pode ser selecionado randomicamente,  * com exceção dos códigos de controles. 
  */
 char ascii_char_aleatorio (void);

 // seleciona randomicamente um valor lógico(verdadeiro ou falso)
 bool logico (void); 

 // embalhara uma 'array de bytes', dado ela e seu tamanho.
 void embaralha (uint8_t*, size_t); 

 /* seleção de um inteiro positivo, dado os limites. */
 // size_t inteiro_positivo (size_t, size_t); 
 size_t inteiro_positivo (size_t, size_t); 

 #ifdef __linux__
 char* palavra_aleatoria(void);
 #endif

#endif 
