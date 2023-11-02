
#include <stdint.h>
#include <stdbool.h>
#include "tabeladispersao/dado.h"
#include "tabela_dispersao.h"

#ifndef CONJUNTO_H
#define CONJUNTO_H
   typedef HashTable Conjunto;
   typedef Conjunto Set;

   // criação e liberação de Conjuntos.
   extern Conjunto cria_set(); 
   extern Conjunto clona_set(Conjunto);
   extern void destroi_set(Conjunto s); 

   // 'conjuntos' não recebem chaves, mas sim, elementos.
   typedef CHAVE Elemento;
   /* insere em ambas pontas o 'dado' passado, se 
    * ele é válido ou não fica a cargo do programador.
    * Confirma se a operação foi bem sucedida. */
   extern bool insere_set(Conjunto s, Elemento e);
   extern bool remove_set(Conjunto s, Elemento e);

   /* Visualização da 'Lista', formato final
    * algo como "display", e a versão do debug
    * que mostra até os espaços vagos da 
    * abstração. */
   extern char* set_to_str(Conjunto s); 
   extern void visualiza_set(Conjunto s); 

   /* Operações de verificação da estrutura, como seu tamanho, se
    * está vázia ou não, e se um dado Elemento pertence a ela. */
   extern bool pertence_set(Conjunto s, Elemento e);
   extern bool vazio_set(Conjunto s);
   extern uint64_t tamanho_set(Conjunto s);

   // total de instâncias deste tipo de dado.
   extern uint64_t total_de_instancias_set();

   // Iterador de dados, e seus métodos..
   typedef IterHT IterSet;

   extern IterSet iter_set(Conjunto);
   extern Elemento next_set(IterSet i); 
   extern void destroi_iter_set(IterSet i);

   // operações próprias da estrutura matemática conjunto.
   extern Conjunto uniao(Conjunto a, Conjunto b);

   // verifica se os conjuntos são iguais.
   extern bool eq_set(Conjunto a, Conjunto b);

#endif // CONJUNTO_H
