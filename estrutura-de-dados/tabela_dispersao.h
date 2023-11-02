


#include <stdint.h>
#include <stdbool.h>
#include "tabeladispersao/dado.h"

#ifndef TABELADISPERSAO_H
#define TABELADISPERSAO_H
   typedef struct tabela_de_dispersao* HASH_TABLE;
   typedef HASH_TABLE HashTable;
   typedef HASH_TABLE HT;

   extern HashTable cria_ht();
   extern HashTable clona_ht(HashTable m);
   extern void destroi_ht(HASH_TABLE d);

   extern bool vazia_ht(HASH_TABLE d);
   extern uint64_t tamanho_ht(HASH_TABLE d);
   extern bool insere_ht(HASH_TABLE d, CHAVE ch, VALOR v); 
   extern bool deleta_ht(HASH_TABLE d, CHAVE ch); 
   extern bool contem_ht(HASH_TABLE d, CHAVE ch); 
   extern bool atualiza_ht(HASH_TABLE d, CHAVE ch, VALOR vl); 

   /* nunca dá acesso a instância principal, pois alterações podem
    * causar mudanças inesperadas no programa. */
   extern uint8_t total_de_instancias_ht();

   /* iterador para a estrutura de dados. Essencial, pois servirá de 
    * base para impressão de tal estrutura, além de outros que precisam
    * verificar todos dados internos.
    */
   struct iteracao_tabela_de_dispersao; 
   typedef struct iteracao_tabela_de_dispersao* IterHT;

   extern IterHT iter_ht(HashTable d); 
   extern Dado next_ht(IterHT iterador); 
   extern void destroi_iter_ht(IterHT iterador); 

   // visualização da tabela, assim com sua representação string.
   extern void visualizacao_debug_ht(HashTable m); 
   extern char* ht_to_str(HashTable m);

#endif
