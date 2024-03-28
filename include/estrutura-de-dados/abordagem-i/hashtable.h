

/*   Tipos mais genéricos, então caso necessite mudar-los para outro 
 * programa, apenas troque aqui.
 * 
 *   Hashtables com chaves genérica são basicamente impossível, não poderia
 * implementar a hash no interno da estrutura. Basicamente implementar uma
 * para cada tipo primitivo(string, caractere, inteiro, etc...). O valor
 * pode sempre ser genérico, já que ele é irrelevante para o cálculo hash.
 * Digo apenas para tipos primitivos, pois são com os que sempre usei, 
 * nunca utilizei outras estruturas com hash como chave em um mapa, mesmo
 * no Python, onde isso é extramente fácil de se chegar em tal código 
 * macarronico.
 *
 *   Novamente, como é algo genérico, então a parte de visualização fica 
 * irrelevante aqui, apenas o comportamento da estrutura de dados é 
 * importante nestes casos.
 */

// biblioteca padrões em C:
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef HASHTABLE_I_H
#define HASHTABLE_I_H
 // todos apelidos dados:
 typedef void* generico_t;
 typedef struct tabela_de_dispersao hashtable_t, *HashTable, HASHTABLE; 

 // criação e destruição da instância da estrutura de dados.
 hashtable_t* cria_com_capacidade_ht (
  size_t capacidade, 
  size_t (*Fh)(generico_t, size_t),
  bool (*Eq) (generico_t, generico_t)
 ); 
 hashtable_t* cria_ht (
  size_t (*Fh)(generico_t, size_t),
  bool (*Eq) (generico_t, generico_t)
 );
 hashtable_t* cria_branco_ht (void); 
 bool destroi_ht (hashtable_t* m); 
 
 /* Necessário para o último método de criação, que não passa as condições
  * necessárias para que a maioria dos métodos abaixo, mudem ou não a 
  * 'tabela', sejam realizados com sucesso. Sem eles, qualquer operação
  * abaixo pode de retornar como não confirmado tal operação, até parar o
  * programa por completo.
  */
 bool adiciona_metodos (
  hashtable_t* m, 
  size_t (*hash)(generico_t, size_t),
  bool (*eq)(generico_t, generico_t)
 ); 

 /* Operações que mexem com o iterno da 'tabela', mudam ela para mais ou
  * menos, até igual, mas mudam. */
 bool insere_ht (hashtable_t* m, generico_t ch, generico_t v); 
 bool atualiza_ht (hashtable_t* m,  generico_t ch,  generico_t nv); 
 bool deleta_ht (hashtable_t* m, generico_t ch); 

 /* Obtem informações sobre ela, seja seu tamanho, estado, se tem ou não
  * o item, até o 'item' em sí. */
 bool contem_ht (hashtable_t*, generico_t ch); 
 generico_t obtem_ht (hashtable_t* m,  generico_t ch); 
 bool vazia_ht (hashtable_t*); 
 size_t tamanho_ht (hashtable_t*);

#endif // HASHTABLE_H
