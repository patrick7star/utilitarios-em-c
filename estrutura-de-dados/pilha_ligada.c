

/* Implementação de uma pilha LIFO, porém usando como engrenagem
 * interna, uma deque-ligada ao invés da deque-estática. Portanto,
 * não há redimensionamentos em tempo de execução, entretanto,
 * indexação é um problema, mas para a funcionalidade da pilha em sí,
 * fica insignificante.
 *
 * Como ela é apenas um embrulho para deque-ligada, qualquer nova
 * funcionalidade que está adequirir, ou se houver melhoria de
 * desempenho, por extensão, ela também receberár/ou implementará tal.
 */

#include "deque_ligada.h"

struct pilha_ligada {
   // lista que realmente é operada, como também armazena dados.
   DequeLigada lista;

   /* qualquer coisa há mais que deseja-se colocar, já que isso é 
    * só um template para futuras cópias. Mas o grosso, e tudo necessário
    * para apenas a estrutura de pilha, já está dado acima.
    */
};

// nomes para o tipo de dado.
typedef struct pilha_ligada* PL;
typedef PL PILHA_LIGADA;

// tamanho do tipo de dado.
#define PL_SIZE sizeof(struct pilha_ligada)

#include <stdlib.h>
#include <stdio.h>

PILHA_LIGADA cria_pl() {
   PL pilha = (PL)malloc(PL_SIZE);

   // só cria dados internos, se e somente se, a alocação foi válida.
   if (pilha != NULL) {
      puts("'pilha' alocada com sucesso.");
      pilha->lista = cria_dl(); 

      /* se a alocação da lista(deque) interna não foi válida, logo
       * esta também não será. */
      if (pilha->lista == NULL)
         return NULL;

      puts("'lista' interna alocada com sucesso.");
   }

   return pilha;
}

void destroi_pl(PILHA_LIGADA p) {
   // apenas se há algo.
   if (p != NULL) {
      puts("libera a 'lista' interna dela.");
      destroi_dl(p->lista);
      puts("então libera a 'pilha' em sí.");
      free(p);
      return;
   }

   puts("estrutura 'pilha' está inválida!");
}

/* Todas demais operações, são um conjunto que a deque-ligada também
 * realiza, assim, aqui serão passada apenas nomes com chamadas 
 * internas para tais. Algumas, por se tratar de algum diferente, terão
 * que ser reescritas(sobrecarga), mas ainda não muda muita coisa.
 */

bool vazia_pl(PILHA_LIGADA p)
   { return vazia_dl(p->lista); }

uint64_t quantidade_pl(PILHA_LIGADA p)
   { return tamanho_dl(p->lista); }

/* Pela natureza da pilha, inserção e remoção, serão feitas apenas numa
 * ponta, pode ser qualquer uma, a deque-ligada faz ambas em tempo
 * constante, o que importa para a estrutura LIFO, é que sejá feita
 * em apenas uma ponta.
 */
bool coloca_pl(PILHA_LIGADA p, Dado a)
   { return insere_comeco_dl(p->lista, a); }

Dado retira_pl(PILHA_LIGADA p)
   { return remove_comeco_dl(p->lista); }

// verificação do elemento no topo, novamente na mesma ponta operada.
Dado topo_pl(PILHA_LIGADA p)
   { return frente_dl(p->lista); }

#include <string.h>

// formatação da estrutura para string, dado um limite de tamanho dela.
char* pl_to_str(PILHA_LIGADA p) {
   if (vazia_pl(p)) 
      return "Pilha Ligada: []" ;

   // todas variáveis que serão usadas abaixo.
   char formatacao[1000];
   char* resultado = formatacao;
   IterDL iterador = iter_dl(p->lista);

   // formatando o topo inicialmente.
   Dado item = next_dl(iterador);
   sprintf(
      formatacao,
      "Pilha Ligada: (%s) [", 
      dado_para_string(item)
   );

   // próximo a imprimir.
   item = next_dl(iterador);
   // informa se o loop abaixo foi iniciado ou não.
   bool loop_iniciado = false;

   // abre colchete de itens dela.
   while (item != NULL) {
      // concatenando string com 'dado' formatado, e a vírgula separadora.
      strcat(resultado, dado_para_string(item));
      strcat(resultado, ", ");
      item = next_dl(iterador);
      loop_iniciado = true;
   }

   // delimita itens com fecha-colchetes.
   if (loop_iniciado)
      // neste caso, retirar espaço e vírgula extra adicionado.
      strcat(resultado, "\b\b]");
   else
      // se não foi iterado, não é possível retirar.
      strcat(resultado, "]");

   // liberando o iterador já consumido.
   destroi_iter_dl(iterador);
   // retornando a formatação da Pilha.
   return resultado;
}

// reescrevendo função de impressão.
void visualiza_pl(PILHA_LIGADA p) 
   { printf("%s\n", pl_to_str(p)); }


/* Iterador da 'pilha ligada', começa da esquerda para direita, ou seja,
 * do 'topo' da 'pilha'. Muito disso, é apenas a renomeação da lista
 * interna dela, que já executa este tipo de função.
 */
typedef IterDL IterPilhaLigada;

IterPilhaLigada cria_iter_pl(PILHA_LIGADA p)
   { return cria_iter_dl(p->lista); }

void destroi_iter_pl(IterPilhaLigada iterador)
   { destroi_iter_dl(iterador); }

Dado next_pl(IterPilhaLigada iterador) 
   { return next_dl(iterador); }

uint64_t contagem_iter_pl(IterPilhaLigada iterador) 
   { return iter_tamanho_dl(iterador); }

