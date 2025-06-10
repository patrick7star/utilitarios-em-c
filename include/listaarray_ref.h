/*       
 *        Simples implementação genérica da 'Array List'.
 *
 *   Se ficar muito boa se juntará as estruturas de dados tipo(iii) no
 * projeto utilitários, pois é uma array dinâmica que serve incrivelmente
 * para qualquer projeto que precisa armazenar um grande número de objetos,
 * seja eles qualquer que tipo.
 */

#ifndef ARRAYLISTA_H
#define ARRAYLISTA_H
#include <stdbool.h>
#include <stdarg.h>
#include "definicoes.h"

 // todos os apelidos desta estrutura fornecidos:
 typedef struct array_lista *ArrayLista, *Vetor, vetor_t;
 
 /* Criação e destruição de tal lista. Você pode criar-la com capacidade, ou
  * ou seja, um buffer do tamanho específico; assim como um valor automático
  * definido já na definição da estrutura. As desalocações têm de dois tipos
  * sem a destruição do valor interno; e com a destruição, passando seu 
  * destrutor. */
 Vetor  cria_al                (void);
 Vetor  cria_com_capacidade_al (size_t); 
 Vetor  cria_de_al             (int, ...);
 bool   destroi_al             (ArrayLista); 
 bool   destroi_interno_al     (Vetor, Drop);
 void   destroi_todas_al       (int, ...); 

 /* Operações que mudam a estrutura interna da lista. */
 Generico  remove_al        (Vetor); 
 Generico  remove_indice_al (Vetor, size_t);
 Generico  indexa_al        (Vetor, size_t); 
 bool      insere_al        (Vetor, generico_t); 
 /* NOTA: O retorno de todas operações de remoção é NULL, caso estejam sem 
  *       itens. O caso de indexação de índices já removidos ou inexistente,
  *       também revolvem no mesmo tipo de retorno. */

 /* O método 'vácuo' retorna o tamanho espaço vácuo, ou seja, a capacidade 
  * de itens que podem ser adicionados sem que a amortização seja 
  * acionada. Os demais métodos tem nomes 'tamanho' e 'vázio' estão com nomes
  * bem explicitos quanto suas funções, atual comprimento da lista e se está
  * vázia ou não, respectivamente. */
 bool    vazia_al   (ArrayLista);
 size_t  tamanho_al (ArrayLista);
 size_t  vacuo_al   (ArrayLista);

 /* Converte pequenas estruturas em strings. Há uma limitação de com quantos
  * itens tal operação é permitida. Se você deseja fazer isso com centenas 
  * de milhares, milhões ou bilhões(ou mais), é preciso ativar o macro que 
  * permite tal operação, pois por padrão é desativado. */
 char* to_string_al     (ArrayLista L, ToString fn);
 void imprime_lista_al  (Vetor l, ToString f);

/* === === === === === === === === === === === === === === === === === ===
 *                     Iterador da e seus métodos
 * === === === === === === === === === === === === === === === === === ==*/
 typedef struct iterador_da_lista_ligada_al 
   IterAL, *IteradorRefAL, *IterRefAL;
 /* Saída que o iterador produz. É apenas um único campo na estrutura, 
  * nomeado 'item', que entrega a referência que a estrutura retém. */
 typedef struct saida_da_iteracao_da_al { generico_t item; } 
   IterOutputAL;

 IterAL        cria_iter_al      (ArrayLista); 
 IterOutputAL  next_al           (IterRefAL); 
 IterAL        clona_iter_al     (IterRefAL);
 size_t        contagem_iter_al  (IterRefAL);
 bool          consumido_iter_al (IterRefAL);

 /* Observações:
  *   A clonagem do iterador, parte do seu estado(consumido ou não), no 
  * mesmo estágio que está em uso no momento desta operação.
  *   O método 'contagem', conta os itens que faltam a consumir, portanto 
  * ele varia de acordo com a iteração.
  */

#endif 
