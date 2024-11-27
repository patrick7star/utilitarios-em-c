
#ifndef __CONJUNTO_REF_H__
#define __CONJUNTO_REF_H__
#include "definicoes.h"

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
 bool deleta_set   (Conjunto, generico_t);
 bool limpa_set    (Conjunto);

 // Operações que validam, ou retorna informação da estrutura(conjunto):
 bool pertence_set  (Conjunto, generico_t);
 bool vazia_set     (Conjunto);
 size_t tamanho_set (Conjunto);
 void imprime_set   (Conjunto, ToString);
 
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
 typedef struct iterador_do_conjunto IterSet, *IteradorRefSet;
 // typedef struct saida_da_iteracao_do_set IterOutputSet;
 typedef struct saida_da_iteracao_do_set { generico_t item; } IterOutputSet;
 // Constante que representa nenhum item iterado.
 const IterOutputSet NULO_SET = { NULL};

 // Métodos de criação, mudança e atual estado:
 IterSet cria_iter_set (Conjunto);
 /* Faz a clonagem do iterador dado, inclusive seu estado(consumido ou
  * não), no mesmo estágio do que foi usado.*/
 IterSet clona_iter_set(IteradorRefSet);
 // Total de itens ainda a consumir.
 size_t contagem_iter_set (IteradorRefSet);
 IterOutputSet next_set (IteradorRefSet);
 // Verifica se todos itens do 'conjunto' foram consumido via iterador.
 bool consumido_iter_set (IteradorRefSet);

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
 Set    new_set           (Hash _hash_, Eq _igual_);
 void   drop_set          (Conjunto);
 void   drop_inner_set    (Conjunto, Drop);
 bool   add_methods_set   (Conjunto, Hash, Eq);
 // Segunda parte:
 bool   add_set           (Conjunto, generico_t);
 bool   remove_set        (Conjunto, generico_t);
 // Terceira parte:
 bool   contains_set      (Conjunto, generico_t);
 bool   empty_set         (Conjunto);
 size_t length_set        (Conjunto);
 void   print_set         (Conjunto, ToString);
 // Quarta parte: 
 Set    union_set         (Conjunto a, Conjunto b);
 Set    intersection_set  (Conjunto a, Conjunto b);
 Set    difference_set    (Conjunto a, Conjunto b);

#endif
