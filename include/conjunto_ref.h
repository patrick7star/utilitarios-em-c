/*
 *   Este código foi extraído de implementação de 'hashtable', já que, é
 * uma implementação bem robusta, que já tem uma massa de testes feito
 * sobre, com as principais operações. Como o 'conjunto' em sí é
 * basicamente uma 'tabela de dispersão' interna, isso economiza bastante
 * tempo tentanto "reiventar" a roda. No decorrer só serão feitos alguns
 * ajustes, tipo: descartar operações que não são relevantes na estrutura
 * 'conjunto'(como o método de atualizar); assim como todos trechos de
 * 'debug', simplesmente inútil, porque o condensado de testes feitos com
 * a estrutura, logo deixar aqui é desnecessário, e catastrófico para
 * legibilidade; também, uma mexida massiva como os atuais nomes, estes
 * por motivos óbvios de conflito e consistência do atual código.
 */

#ifndef __CONJUNTO_REF_H__
#define __CONJUNTO_REF_H__
#include "definicoes.h"
#include "primitivos.h"

 // Todos apelidos que a estrutura 'conjunto' assume:
 typedef struct tabela_de_dispersao
     set_t, conjunto_t, *Conjunto, *Set, SET, CONJUNTO;

 // Alocação e desalocação da estrutura:
 Conjunto cria_com_capacidade_set(size_t, Hash, Eq);
 Conjunto cria_branco_set (void);
 Conjunto cria_set (Hash _hash_, Eq _igual_);
 /* Libera o 'conjunto'. Já o segundo ele também libera os 'itens' dele, se
  * o 'desconstrutor' for passado jundo. */
 bool destroi_set (Conjunto);
 void destroi_interno_set (Conjunto, Drop);
 /* Essencial na construção da instância, pois é possível postergar métodos
  * para realizar tais operações sobre a isntância. Está função geralmente
  * é usada quando a função 'cria_branco_set' é chamada, ao invés das demais
  * que cuidam de alocação. */
 bool adiciona_metodos_set (Conjunto, Hash, Eq);

 // Operações que alteram a estrutura interna do 'conjunto':
 bool adiciona_set (Conjunto, generico_t);
 bool remove_set   (Conjunto, generico_t);
 bool limpa_set    (Conjunto);
 /* Está função não é tão obvia quanto parece, ela remove e deleta um elemento
  * aleatório do conjunto, se houver algum é claro. */
 GenT deleta_set   (Conjunto);

 // Operações que validam, ou retorna informação da estrutura(conjunto):
 bool    pertence_set  (Conjunto, generico_t);
 bool    vazia_set     (Conjunto);
 size_t  tamanho_set   (Conjunto);
 void    imprime_set   (Conjunto, ToString);

 /* Operações específicas para Conjuntos. Uma coisa que tem de tomar-se
  * bastante cuidado é, os conjuntos resultantes das funções abaixo carregam
  * consigo items dos argumentos passados, portanto deve-se tomar cuidado
  * na liberação deles. */
 Conjunto uniao_set      (Conjunto a, Conjunto b);
 Conjunto intersecao_set (Conjunto a, Conjunto b);
 Conjunto diferenca_set  (Conjunto a, Conjunto b);

 /* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
  *                     Iterador do Conjunto
  *                        e seus métodos
  * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- - */
 typedef struct iterador_do_conjunto* IterSet;
 typedef struct saida_da_iteracao_do_set { GenT item; } IterOutputSet;

 // Constante que representa nenhum item iterado.
 extern const IterOutputSet NULO_SET;

 // Métodos de criação, mudança e atual estado, além da destruição:
 IterSet cria_iter_set (Conjunto);
 void destroi_iter_set(IterSet);
 /* Faz a clonagem do iterador dado, inclusive seu estado(consumido ou
  * não), no mesmo estágio do que foi usado.*/
 IterSet clona_iter_set(IterSet);
 // Total de itens ainda a consumir.
 size_t contagem_iter_set (IterSet);
 IterOutputSet next_set (IterSet);
 // Verifica se todos itens do 'conjunto' foram consumido via iterador.
 bool consumido_iter_set (IterSet);

/* === === === === === === === === === === === === === === === === === ===
 *                   Métodos e Funções em Inglês
 *
 * Espaçado do seguinte modo:
 *    Tipo de Retorno      Nome do Método       Parâmetros
 *
 * Nota: não é recomendado rearranjar todas declarações abaixo, pois elas
 *       seguem a ordem das funções e métodos declarados acima, por um
 *       motivo de futuras consultas.
 * === === === === === === === === === === === === === === === === === ==*/
 // Primeira parte:
 Set    new_set           (Hash, Eq);
 void   drop_set          (Set);
 void   drop_inner_set    (Set, Drop);
 bool   add_methods_set   (Set, Hash, Eq);
 // Segunda parte:
 bool   add_set           (Set, GenT);
 bool   remove_set        (Set, GenT);
 bool   clear_set         (Set);
 GenT   pop_set           (Set a);
 // Terceira parte:
 bool   contains_set      (Set, GenT);
 bool   empty_set         (Set);
 size_t length_set        (Set);
 void   print_set         (Set, ToString);
 // Quarta parte:
 Set    union_set         (Set a, Set b);
 Set    intersection_set  (Set a, Set b);
 Set    difference_set    (Set a, Set b);

 // Métodos de iteração.
 void drop_iter_set(IterSet);

#endif
