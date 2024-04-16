



#ifndef ESTRINGUE_H
#define ESTRINGUE_H

 // Tupla contendo array de strings e a quantia disto.
 typedef struct { char** lista; size_t total; } OutcomePalavras;
 typedef OutcomePalavras OutcomeStrings;

 // retorna todas strings representando palavras(entre espaços).
 OutcomePalavras palavras(char*); 
 // retorna tupla com strings entre quebra-de-linhas.
 OutcomeStrings split_linhas(char*);

 /* Retorna uma string concatenando todas strings passadas.*/
 char* concatena_strings(uint8_t, ...);

#endif
