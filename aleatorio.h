
/*
 * Códigos para gerar valores aletórios
 * com todos os tipos primitivos.
 */

#include <stdbool.h>
#include <stdint.h>

#ifndef ALEATORIO_H
#define ALEATORIO_H
   /* cospe uma letra aleatória do alfabeto,
    * seja ela minúscula, ou maiúscula. */
   extern unsigned char alfabeto_aleatorio(); 

   /* qualquer caractére da tabela ASCII, pode 
    * ser selecionado randomicamente, com exceção
    * dos códigos de controles. */
   extern unsigned char ascii_char_aleatorio();

   // seleciona randomicamente um valor lógico(verdadeiro ou falso)
   extern bool logico(); 

   /* seleção de um inteiro positivo de 64-bits,
    * dado os limites. */
   // ainda não está pronto!
   extern uint64_t inteiro_positivo(uint64_t a, uint64_t b); 
#endif 
