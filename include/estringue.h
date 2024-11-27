
#ifndef ESTRINGUE_H
#define ESTRINGUE_H
#include <wchar.h>
#include <stdbool.h>

 // Tupla contendo array de strings e a quantia disto.
 typedef struct { char** lista; size_t total; } ListaStrings;
 typedef char* char_t, CHAR, Char;

 /* Uma representação de string já codificada como Unicode. */
 typedef struct string_dinamica { 
    // A array com caractéres Unicode.
    wchar_t* caracteres; 

    // Quantidade de caractéres adicionado.
    size_t quantia; 

    // A capacidade atual da array.
    size_t total; 

 } string_t, STRING, STR, *String, *Str;

 // retorna todas strings representando palavras(entre espaços).
 ListaStrings palavras(char*); 
 // retorna tupla com strings entre quebra-de-linhas.
 ListaStrings split_linhas(char*);


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

/* == == == == == == == == == == == == == == == == == == == == == == == == 
 *                      Funções Auxiliares para
 *                         Strings Puras em C
 * == == == == == == == == == == == == == == == == == == == == == == === */
 char* maiuscula_ascii        (char* In);
 char* minuscula_ascii        (char* In);
 char* capitaliza_ascii       (char* In);
 char* extrai_letras_ascii    (char* In);
 char* extrai_digitos_ascii   (char* In);
 char* alterna_case_ascii     (char* In);
 /* Dado uma estringue(s) reparte ela em várias partes, isso à partir do
  * padrão(p) dado. 
  *
  * Nota:a lista de retorno são cópias.*/
 ListaStrings reparte_ascii   (char* s, char* p);

 /* Pega um caractére ASCII qualquer, e forma uma estringue com ele repetido
  * 'qtd' vezes. */
 char* repete_caractere_ascii (char p, int qtd);
 /* Troca um trecho(a) na estringue pela sub-estringue(b). */
 char* substitui_ascii        (char* s, char* a, char* b);
 /* Retorna uma string concatenando todas strings passadas.*/
 char* concatena_strings      (int, ...);

#endif
