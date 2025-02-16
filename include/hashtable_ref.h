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


#ifndef HASHTABLE_H_REF
#define HASHTABLE_H_REF
// Tipo de dado genérico trabalhado e template de funções:
#include "definicoes.h"

 // Todos apelidos dados para a 'tabela de dispersão':
 typedef struct tabela_de_dispersao
   // Formas estáticas(para pegarem o objeto, precisam de ponteiros):
   hashtable_t,  HASHTABLE,
   // Ponteiros, ambas formas são comumente escritas:
   *Hashtable, *HashTable, *HT;

 /* Criação e alocação de várias formas possíveis. Alocação pode ser feita
  * com um buffer inicial, com um buffer definido de forma automática, e
  * com buffer automático, porém, os métodos de 'hashing' e 'igualdade', 
  * podem ser delegados para uma inserção posterior. Já a desalocação há duas
  * possibilidades: um que simplesmente desaloca toda estrutura de mapa, e
  * a outra também desaloca os dados internos, se devidamente passados os 
  * descontrutores tanto da chave quando to valor. 
  */
 HashTable  cria_com_capacidade_ht  (size_t, Hash, Eq);
 HashTable  cria_ht                 (Hash, Eq);
 HashTable  cria_branco_ht          (void);
 bool       destroi_ht              (HashTable);
 bool       destroi_interno_ht      (HashTable, Drop k, Drop v);

 /* Necessário para o último método de criação, que não passa as condições
  * necessárias para que a maioria dos métodos abaixo, mudem ou não a
  * 'tabela', sejam realizados com sucesso. Sem eles, qualquer operação
  * abaixo pode de retornar como não confirmado tal operação, até parar o
  * programa por completo. */
 bool adiciona_metodos (HashTable, Hash, Eq);

 /* Operações que mexem com o iterno da 'tabela', mudam ela para mais ou
  * menos, até igual, mas mudam. */
 bool insere_ht   (HashTable m, generico_t ch, generico_t vl);
 bool atualiza_ht (HashTable m,  generico_t ch,  generico_t nv);
 bool deleta_ht   (HashTable m, generico_t ch);

 /* Obtem informações sobre ela, seja seu tamanho, estado, se tem ou não
  * o item, até o 'item' em sí. */
 bool        contem_ht   (HashTable, generico_t ch);
 generico_t  obtem_ht    (HashTable m,  generico_t ch);
 bool        vazia_ht    (HashTable);
 size_t      tamanho_ht  (HashTable);

 /* Métodos para visualização da tabela, seja o resultado uma string, ou
  * na saída principal. */
 // char* hashtable_to_str(HashTable, ToString);
 void imprime_ht (HashTable, ToString fk, ToString gv);

 /* Método clássico de iteração, gera arrays e colocas todos suas 'chaves'
  * ou 'valores' nesta, sendo seu tamanho o mesmo que a quantia de itens
  * da tabela.
  *
  * Nota: ainda estão instaveis, portanto o uso vem com possíveis clashes
  * no programa. */
 generico_t*  valores_ht (HashTable);
 generico_t*  chaves_ht  (HashTable);
/* === === === === === === === === === === === === === === === === === ==
 *                      Iteradores e seus
 *                         Métodos
 * === === === === === === === === === === === === === === === === === ==*/
 typedef struct Iteracao_da_Hashtable_Ref *IterHT, *IteradorHT;

 typedef struct Saida_da_Iteracao_da_Hashtable_Ref
   { generico_t key; generico_t value; } IterOutputHT;

 // cédula em branco para indicar termino da iteração ou invalidação.
 extern const IterOutputHT NULO_HT;

 IteradorHT    cria_iter_ht      (HashTable);
 IteradorHT    clona_iter_ht     (IterHT);
 void          destroi_iter_ht   (IterHT);
 size_t        contagem_iter_ht  (IterHT);
 IterOutputHT  next_ht           (IterHT);
 bool          consumido_iter_ht (IterHT);

/* === === === === === === === === === === === === === === === === === ==
 *                      Renomeação de vários
 *                métodos e funções para os termos
 *                  mais conhecidos em inglês
 *
 * Observação: Estão espaçado e agrupadas de acordo com as originais.
 * === === === === === === === === === === === === === === === === === ==*/
 HT      new_with_capacity_ht (size_t, Hash, Eq);
 HT      new_ht               (Hash, Eq);
 HT      default_ht           (void);
 bool    delete_ht            (HT);
 bool    add_ht               (HT, GNRC key, GNRC vl);
 bool    update_ht            (HT,  GNRC key,  GNRC nvl);
 bool    remove_ht            (HT, GNRC key);
 bool    contains_ht          (HT, GNRC key);
 GNRC    get_ht               (HT, GNRC key);
 bool    empty_ht             (HT);
 size_t  len_ht               (HT);
 void    print_ht             (HT, ToString fk, ToString gv);

#endif
