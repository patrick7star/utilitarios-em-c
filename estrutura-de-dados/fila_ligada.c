
#include "deque_ligada.h"

struct fila_ligada {
   // lista que realmente é operada, como também armazena dados.
   DequeLigada lista;

   /* qualquer coisa há mais que deseja-se colocar, já que isso é 
    * só um template para futuras cópias. Mas o grosso, e tudo necessário
    * para apenas a estrutura de fila, já está dado acima.
    */
};

// nomes para o tipo de dado.
typedef struct fila_ligada* FilaLigada;
typedef FilaLigada FL;

// tamanho do tipo de dado.
#define FL_SIZE sizeof(struct fila_ligada)

#include <stdlib.h>
#include <stdio.h>

FilaLigada cria_fl() {
   FL fila = (FilaLigada)malloc(FL_SIZE);

   // só cria dados internos, se e somente se, a alocação foi válida.
   if (fila != NULL) {
      puts("'fila ligada' alocada com sucesso.");
      fila->lista = cria_dl(); 

      /* se a alocação da lista(deque) interna não foi válida, logo
       * esta também não será. */
      if (fila->lista == NULL)
         return NULL;

      puts("'lista' interna alocada com sucesso.");
   }

   return fila;
}

void destroi_fl(FilaLigada f) {
   // apenas se há algo.
   if (f != NULL) {
      puts("libera a 'lista' interna dela.");
      destroi_dl(f->lista);
      puts("então libera a 'fila ligada' em sí.");
      free(f);
      return;
   }

   puts("estrutura 'fila ligadada' está inválida!");
}

/* Todas demais operações, são um conjunto que a deque-ligada também
 * realiza, assim, aqui serão passada apenas nomes com chamadas 
 * internas para tais. Algumas, por se tratar de algum diferente, terão
 * que ser reescritas(sobrecarga), mas ainda não muda muita coisa.
 */

bool vazia_fl(FilaLigada f)
   { return vazia_dl(f->lista); }

uint64_t quantidade_fl(FilaLigada f)
   { return tamanho_dl(f->lista); }

/* Operações de remoção e inserção, um FIFO no caso da fila, aqui elas
 * são chamadas 'dequeue' e 'enqueue' respectivamente.
 * Aqui, ambas pontas da 'deque ligada' serão operadas, natureza 
 * da 'fila'.
 */
bool enqueue_fl(FilaLigada f, Dado a)
   { return insere_traseira_dl(f->lista, a); }

Dado dequeue_fl(FilaLigada f)
   { return remove_comeco_dl(f->lista); }

// espiada de itens nas ponta da 'fila ligada'.
Dado frente_fl(FilaLigada f)
   { return frente_dl(f->lista); }

Dado traseira_fl(FilaLigada f)
   { return traseira_dl(f->lista); }


#include <string.h>

// formatação da estrutura para string, dado um limite de tamanho dela.
char* fl_to_str(FilaLigada f) {
   if (vazia_fl(f)) 
      return "Fila Ligada: []" ;

   // todas variáveis que serão usadas abaixo.
   char formatacao[1000];
   char* resultado = formatacao;
   IterDL iterador = iter_dl(f->lista);
   uint64_t total = quantidade_fl(f);

   for(uint64_t p = 1; p <= total; p++) {
      Dado item = next_dl(iterador);
      char* item_str = dado_para_string(item);

      // se for o primeiro item da iteração.
      if (p == 1) {
         // alterar formatação se houver apenas um item nela.
         if (total > 2) {
            sprintf(
               formatacao,
               "Fila Ligada: (%s) [", 
               item_str
            );
         } else if (total == 2) {
            // parecido com a alternativa de único elemento, porém
            // trabalha com última iteração conhecida.
            sprintf(
               formatacao,
               "Fila Ligada: [%s, ", 
               item_str
            );
         } else {
            // escreve já para um único elemento.
            sprintf(
               formatacao,
               "Fila Ligada: [%s]", 
               item_str
            );
         }
      } else if (p == total) {
         char item_traseiro[30];
         // não houve quase iteração.
         if (p == 1 && p == total)
            strcat(resultado, "]");
         else {
            // fechamento igual ao um só elemento para dois itens.
            if (total == 2)
               sprintf(item_traseiro, "%s]", item_str);
            else
               /* retirando espaço e vírgulo extra, e ressaltando 
                * traseira. */
               sprintf(item_traseiro, "\b\b] (%s)", item_str);
            strcat(resultado, item_traseiro);
         }   
      } else {
         // formatação do 'dado' mais separador(vírgula).
         strcat(resultado, item_str);
         strcat(resultado, ", ");
      }
   }

   // liberando o iterador já consumido.
   destroi_iter_dl(iterador);
   // retornando a formatação da Pilha.
   return resultado;
}

// reescrevendo função de impressão.
void visualiza_fl(FilaLigada f) 
   { printf("%s\n", fl_to_str(f)); }
