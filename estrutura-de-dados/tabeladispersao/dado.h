
#include <stdint.h>
#include <stdbool.h>

#ifndef DADO_H
#define DADO_H
   typedef char* CHAVE;
   typedef int64_t VALOR;
   struct dado { CHAVE chave; VALOR dado; };

   // para dados genéricos.
   typedef struct dado* Dado;
   typedef Dado* ArrayDado;

   // criação e liberação do 'Dado'.
   extern Dado cria_dado(CHAVE ch, VALOR v);
   extern void destroi_dado(Dado d); 
   extern Dado clona_dado(Dado d);
   extern Dado cria_dado_branco();

   /* cria uma array, inicialmente 'null'
    * do tipo de dado acima. */
   extern Dado* cria_array_dado(uint64_t n); 
   extern void destroi_array_dado(Dado* a, uint64_t q);

   // verifica se dois dados são iguais.
   extern bool dados_iguais(Dado a, Dado b);

   // pode imprimir o dado.
   void visualiza_dado(Dado d); 
   extern char* dado_para_string(Dado d);
   extern char* dado_to_str(Dado d);

   // obtém os valores do dado, modo indireto de acessa-lôs.
   extern CHAVE get_chave_dado(Dado d);
   extern VALOR get_valor_dado(Dado d);
#endif // DADO_H
