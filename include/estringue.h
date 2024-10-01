
#ifndef ESTRINGUE_H
#define ESTRINGUE_H
#include <wchar.h>
#include <stdbool.h>

 // Tupla contendo array de strings e a quantia disto.
 typedef struct { char** lista; size_t total; } OutcomePalavras;
 typedef OutcomePalavras OutcomeStrings;
 typedef char* char_t, CHAR, Char;

 /* Uma representação de string já codificada como Unicode. */
 typedef struct string_dinamica { 
    // A array com caractéres Unicode.
    wchar_t* caracteres; 

    // Quantidade de caractéres adicionado.
    size_t quantia; 

    // A capacidade atual da array.
    size_t total; 

 } string_t, STRING, STR, *String;

 // retorna todas strings representando palavras(entre espaços).
 OutcomePalavras palavras(char*); 
 // retorna tupla com strings entre quebra-de-linhas.
 OutcomeStrings split_linhas(char*);

 /* Retorna uma string concatenando todas strings passadas.*/
 char* concatena_strings(uint8_t, ...);

 /* Operações em cima da estrutura String, sendo que elas podem ou não,
  * modificar o interno da estrutura. */
 String cria_com_capacidade_str (size_t);
 String cria_str (void); 
 String from_str (const char* s); 
 bool destroi_str (String);

 void anexa_str (String, char*); 
 void adiciona_str (String, char);
 /* Preenche a string com determinado caractére a quantia de vezes que 
  * você deseja. */
 void preenche_str (String, char, int);

 bool vazia_str(String);
 // O comprimento da string interna.
 size_t comprimento_str(String);
 // Quantos caractéres da 'capacidade' ainda há.
 size_t vacuo_str(String);

#endif
