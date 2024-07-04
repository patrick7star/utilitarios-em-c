

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
#include <stddef.h>
#include <stdbool.h>

#ifndef HASHTABLE_I_H
#define HASHTABLE_I_H
 // todos apelidos dados:
 typedef struct tabela_de_dispersao 
   // Formas estáticas(para pegarem o objeto, precisam de ponteiros):
   hashtable_t,  HASHTABLE,
   // Ponteiros, ambas formas são comumente escritas:
   *Hashtable, *HashTable; 
 // Nome do tipo genérico:
 typedef void* generico_t;
 // Método essencial de cálculo do índice na tabela.
 typedef size_t (*Hash)(generico_t, size_t);
 // Método essencial para comparação de itens inseridos nela.
 typedef bool (*Eq)(generico_t, generico_t);
 // Método essencial para desalocar memória de objeto genérico.
 typedef bool (*Drop)(generico_t);

 // criação e destruição da instância da estrutura de dados.
 HashTable cria_com_capacidade_ht (size_t, Hash, Eq);
 HashTable cria_ht (Hash, Eq);
 /* Criado de tabela, que postega a adição(importantíssima) dos métodos
  * de hash e igualdade entre os tipos internos de dados. Você tem a 
  * instância do tipo de objeto, porém não pode fazer operações nele. 
  * Para usar tal, tem que usar também a função 'adiciona_metodos'. */
 HashTable cria_branco_ht (void); 
 bool destroi_ht (HashTable); 
 /* Também desaloca o tipo de dados internos, se for passado também o 
  * desalocador de tal tipo abstrato que foi alocada na tabela. */
 bool destroi_interno_ht(HashTable);
 
 /* Necessário para o último método de criação, que não passa as condições
  * necessárias para que a maioria dos métodos abaixo, mudem ou não a 
  * 'tabela', sejam realizados com sucesso. Sem eles, qualquer operação
  * abaixo pode de retornar como não confirmado tal operação, até parar o
  * programa por completo. */
 bool adiciona_metodos (HashTable, Hash, Eq);

 /* Operações que mexem com o iterno da 'tabela', mudam ela para mais ou
  * menos, até igual, mas mudam. */
 bool insere_ht (HashTable m, generico_t ch, generico_t vl); 
 bool atualiza_ht (HashTable m,  generico_t ch,  generico_t nv); 
 bool deleta_ht (HashTable m, generico_t ch); 

 /* Obtem informações sobre ela, seja seu tamanho, estado, se tem ou não
  * o item, até o 'item' em sí. */
 bool contem_ht (HashTable, generico_t ch); 
 generico_t obtem_ht (HashTable m,  generico_t ch); 
 bool vazia_ht (HashTable); 
 size_t tamanho_ht (HashTable);

 /* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
  *                     Iterador da Hashtable
  *                        e seus métodos
  * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- - */
 typedef struct iteracao_da_hashtable IterHT, *IteradorHT;
 /* Toda vez que iterador a estrutra acima, retorna o tipo de valor
  * abaixo. Uma tupla com o endereço da 'chave', e outro ponteiro com
  * o 'valor' da respectiva 'entrada' da tabela. */
 typedef struct iterator_ouptut_hs { generico_t key; generico_t value; } 
   // Possíveis nomes para a tupla:
   IterOutputHT, IterSaidaHT; 

 /* Cédula em branco para indicar termino da iteração ou invalidação. O 
  * mesmo que (NULL, NULL).
  */
 const IterOutputHT NULO_HT;

 IterHT cria_iter_ht (HashTable);
 // Total de itens, ainda não consumidos.
 size_t tamanho_iter_ht (IteradorHT); 
 // Retorna próximo item do iterador.
 IterOutputHT next_ht (IteradorHT); 
#endif
