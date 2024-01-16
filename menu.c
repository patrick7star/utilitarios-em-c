

/* Implementação de um simples menu para 'parser' opções dos comandos.*/

typedef enum tipo_valor { 
   Caractere, Decimal, 
   Logico, Inteiro, 
   String
} Tipo;

#include <stdlib.h>

const char* tipo_to_str(Tipo tipagem) {
   switch (tipagem) {
      case Caractere:
         return "char";
      case Logico:
         return "bool";
      case Inteiro:
         return "size_t";
      case String:
         return  "char*";
      case Decimal:
         return "double";
   }
   return NULL;
}

typedef struct menu {
   const char* comando;
   const char atalho;
   const char* valor;
   Tipo tipo;
} Menu;

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Retorna o enum identificando o tipo de argumento dado em string. O
 * algoritmo consiste no seguinte: se houver apenas algarismos nela,
 * então é um inteiro; caso tenha algarismos e no mínimo um ponto/ou 
 * vírgula, na posição correta(entre os números, nuca na ponta) então
 * é um decimal; já se houver apenas um caractére a string, é um 
 * caractére; os valores lógicos serão identificados por qualquer 
 * permutação minúsculas das palavras tru/verdadeiro e false/falso;
 * todos demais padrãos serão acusados devidamente de apenas strings,
 * inclusive strings vázias.
 */
Tipo identifica_valor(const char* s) {
   bool valor_numerico() {
      for (size_t i = 0; i < strlen(s); i++) {
         if (isdigit(s[i]) == false)
            return false;
      }
      // se chegar até aqui, então é verdadeiro.
      return true;
   }

   bool valor_decimal() {
      int indice = -1;
      uint8_t qtd_algs = 0;
      uint8_t t = strlen(s);

      // iterando cada caractére da 'raw string'.
      for (size_t i = 0; i < t; i++) {
         char _char = s[i];
         if (_char == '.')
            indice = i;
         // contando valores numéricos.
         if ((uint8_t)_char >= 0x30 && (uint8_t)_char <= 0x39)
            qtd_algs += 1;
      }
      #ifdef _DEBUG_MENU
      printf("%d\n", indice);
      printf("qtd_algs= %d\n", qtd_algs);
      #endif
      // todas condiçõe impostas tem que ser verdadeiras:
      return (
         // tem que haver algum ponto de separação decimal.
         (indice != -1) &&
         // todos demais caractéres tem que ser numéricos.
         (qtd_algs == (t - 1)) &&
         // o ponto não pode ser o primeiro ou último caractére.
         (indice > 0 && indice < (t - 1))
      );
   }

   bool e_um_caractere() {
      return ( strlen(s) == 1);
   }

   bool valor_booleano() {
      return (
         !strcmp(s, "false") ||
         !strcmp(s, "true") ||
         !strcmp(s, "verdadeiro") ||
         !strcmp(s, "falso") 
      );
   }

   if (valor_decimal())
      return Decimal;
   else if (valor_numerico())
      return Inteiro;
   else if (e_um_caractere())
      return Caractere;
   else if (valor_booleano())
      return Logico;
   else
      return String;
}

#include <tgmath.h>
#include "teste.h"

void extrai_valor(void* ptr, const char* s) {
   Tipo o_tipo_de_variavel = identifica_valor(s);

   // converte uma string para o maior inteiro possível de base dez.
   size_t str_to_int(const char* s) {
      uint8_t to_alg(char _char) {
         uint8_t code = (uint8_t)_char;
         if (code >= 0x30 && code <= 0x39)
            return code - 0x30;
         return UINT8_MAX;
      }
      size_t S = 0, t = strlen(s);

      for (size_t i = 0; i < t; i++) {
         uint8_t alg = to_alg(s[i]);
         S += alg * pow(10, (t - (i + 1)));
      }
      return S;
   }

   switch (o_tipo_de_variavel) {
      case Inteiro:
         *((size_t*)ptr) = str_to_int(s);
         break;
      case Decimal:
         *((double*)ptr) = atof(s);
         break;
      case Caractere:
         *((char*)ptr) = s[0];
         break;
      case String:
         memcpy((char*)ptr, s, strlen(s));
         break;
      case Logico:
         *((bool*)ptr) = str_to_bool(s);
         break;
   }
}



#ifdef _UT_MENU
void identifica_valor_simples_teste() {
   const char* entradas[] = {
      "15.32", "3829123012", "come to my life!",
      "z", "falso", "verdadeiro", "0.003293", "jellos",
      "100", "5", "100000", "true", "false"
   };

   for (size_t i = 0; i < 13; i++) {
      const char* str = entradas[i];
      printf("%s ===> %s\n", str,
      tipo_to_str(identifica_valor(str)));
   }
}

void extrai_alguns_valores() {
   const char* s = "0.0091132";
   double valor_double;
   extrai_valor(&valor_double, s);
   printf("valor_double=%f\n", valor_double);

   char valor_str[30];
   s = "hoje o dia está belo!";
   extrai_valor(valor_str, s);
   printf("valor_str='%s'\n", valor_str);

   char valor_char;
   s = "p";
   extrai_valor(&valor_char, s);
   printf("valor_char= '%c'\n", valor_char);

   size_t valor_inteiro;
   s = "3812";
   extrai_valor(&valor_inteiro, s);
   printf("valor_inteiro= '%lu'\n", valor_inteiro);

   bool valor_logico;
   s = "false";
   extrai_valor(&valor_logico, s);
   printf("valor_logico= %s\n", bool_to_str(valor_logico));
}

#include <assert.h>

void retirando_trechos_da_string() {
   char entrada[] = "roses are red; violets are blue; i see you big pretty eyes... as a huge cloud comes me soon";

}

void isolamento_dos_argumentos_dados() {
   const char* entradas[] = {
      "programa --argumento(II)=valor(I) --argumento(III) --argumento(IV) --argumento(V)=valor(II)",
   };
   // separa_argumentos(entradas[0]);
}


int main(int qtd, char* argumentos[], char* variaveis[]) {
   executa_testes(
      4, identifica_valor_simples_teste, false,
      extrai_alguns_valores, false,
      isolamento_dos_argumentos_dados, true,
      retirando_trechos_da_string, true
   );
   return EXIT_SUCCESS;
}
#endif // MENU
