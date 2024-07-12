
// Declaração de todas tipos de dados e seus métodos abaixo:
#include "pilhaligada_ref.h"
// Biblioteca padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
// Apenas para os testes:
#ifdef _DESTROI_PL
#include <assert.h>
#include <unistd.h>
#endif

/* === === === === === === === === === === === === === === === === === ==
 *                      Nódulo da lista-ligada
 *
 * Abstração da lista-ligada que forma a pilha, e porta a referência ao 
 * dado original.
 * === === === === === === === === === === === === === === === === === ==*/
typedef struct nodulo {
   // referência de qualquer tipo ao dado.
   generico_t dado;

   // referência ao próximo nódulo.
   struct nodulo* seta;

} *Node;

// Tamanho já computado aqui, e devidamente nomeado.
#define NODULO_SZ sizeof (struct nodulo)

Node cria_nodulo(generico_t dado) {
   Node instancia = malloc (NODULO_SZ);
   
   if (instancia != NULL) {
      instancia->dado = dado;
      instancia->seta = NULL;
   }
   return instancia;
}

bool destroi_nodulo(Node x) {
   if (x == NULL) return false;
   /* O dado, que é uma referência, não pode ser desalocado aqui, pois não
    * sabe-se como ele é formado; pode ser que seja até algo estático. Aqui
    * apenas desconectamos tal referência. */
   x->dado = NULL;
   x->seta = NULL;
   // liberando ...
   free(x);
   x = NULL;
   return true;
}

bool destroi_nodulo_e_dado(Node x, Drop descontrutor) {
   if (x == NULL) return false;
   /* O dado, que é uma referência, não pode ser desalocado aqui, pois não
    * sabe-se como ele é formado; pode ser que seja até algo estático. Aqui
    * apenas desconectamos tal referência. */
   x->dado = NULL;
   x->seta = NULL;
   // liberando ...
   free(x);
   x = NULL;
   return true;
}
// === === === === === === === === === === === === === === === === === ===

struct pilha_ligada_abstracao {
   // quantificando quantos elementos ela realmente tem.
   size_t quantidade;

   // o ínicio da lista-ligada.
   Node topo;
};

PilhaLigada cria_pl (void) {
   PilhaLigada instancia = malloc(sizeof(PilhaLigada));

   if (instancia != NULL) {
      instancia->quantidade = 0;
      instancia->topo = NULL;
   }
   return instancia;
}

bool coloca_pl(PilhaLigada s, void* dado) {
   // apenas aceita valores válidos.
   if (s == NULL || dado == NULL) return false;

   // inserindo no começo da lista(que é o topo da lista).
   Node novo_item = cria_nodulo(dado);
   novo_item->seta = s->topo;
   s->topo = novo_item;
   // contabilizando inserção ...
   s->quantidade += 1;

   // confirma inserção.
   return true;
}

size_t tamanho_pl(PilhaLigada s) 
   { return s->quantidade; }

bool vazia_pl(PilhaLigada s) 
   { return tamanho_pl(s) == 0; }

void visualiza_pl(PilhaLigada s) {
   if (vazia_pl(s)) { puts("pilha-ligada: []"); return; }

   Node cursor = s->topo;

   printf("pilha-ligada: [");
   while (cursor != NULL) {
      printf("%d, ", *(int*)cursor->dado);
      cursor = cursor->seta;
   }
   puts("\b\b]");
}

generico_t retira_pl(PilhaLigada s) {
   // abandona toda operação, não é possível continuar.
   if (vazia_pl(s)) { return NULL; }

   Node item_removido = s->topo;
   generico_t dado_extraido = item_removido->dado;
   // achando novo item no topo...
   s->topo = s->topo->seta;
   destroi_nodulo (item_removido);
   // descontabilizando o item removido.
   s->quantidade -= 1;
   
   return dado_extraido;
}

void* topo_pl(PilhaLigada s) {
   // abandona toda operação, não é possível continuar.
   if (vazia_pl(s)) { return NULL; }

   // pega o dado no primeiro item da lista.
   return s->topo->dado;
}

void visualiza_pilha_string(PilhaLigada s) {
   if (vazia_pl(s)) { puts("pilha-ligada: []"); return; }

   Node cursor = s->topo;

   printf("pilha-ligada: [");
   while (cursor != NULL) {
      printf("'%s', ", (char*)cursor->dado);
      cursor = cursor->seta;
   }
   puts("\b\b]");
}

bool destroi_pl (PilhaLigada s) {
/* se a pilha passada for uma referência inválida(nula), confirmar não
 * desalocação de tal estrutura da memória. */
   if (s == NULL) return false;

   #ifdef _DESTROI_PL
   size_t contagem = 0;
   #endif 

   while (!vazia_pl (s)) {
      retira_pl (s);

      #ifdef _DESTROI_PL
      contagem++;
      #endif 
   }

   #ifdef _DESTROI_PL
   printf ("foram removidos %lu itens.\n", contagem);
   #endif

   free (s);
   // tudo ocorreu conforme, apenas confirma desalocação.
   return true;
}

bool destroi_interno_pl (PilhaLigada s, Drop del) {
/* Mesmo fim que o método de desalocação acima, porém aqui também libera
 * o dado passado junto, ou seja, provavelmente tal pilha contém um 
 * monte de dado na heap, sendo tal no máximo uma array de algum tipo,
 * digo de fácil desalocação. */

/* se a pilha passada for uma referência inválida(nula), confirmar não
 * desalocação de tal estrutura da memória. */
   if (s == NULL) return false;

   #ifdef _DESTROI_PL
   size_t parada = tamanho_pl (s) / 2;
   size_t cortes = 0;
   #endif

   while (!vazia_pl (s)) {
      generico_t dado = retira_pl(s);
      // free (dado);
      del(dado);
      dado = NULL;

      #ifdef _DESTROI_PL
      assert (dado == NULL);
      size_t quantia = tamanho_pl (s);
      bool chegou = quantia < parada;
      if (chegou) {
         printf (
            "%03luº. memória cortada à metade!\n"
            "qtd. de itens=%8lu\n", 
            ++cortes, quantia
         );
         parada = quantia / 2;
         sleep (1);
      }
      #endif
   }
   free (s);
   // tudo ocorreu conforme, apenas confirma desalocação.
   return true;
}

/* === === === === === === === === === === === === === === === === === ==
 *                      Iterador e seus métodos
 *
 * Inicialmente, será apenas uma cópia do iterador da estrutura conjunto.
 * Depois começam os retoques para atual estrutura(uma lista-ligada). Aqui,
 * nem todos campos são importantes, então serão removidos, já que são 
 * relevantes apenas a estrutura hashtable(tipo o campo índice). O algoritmo
 * iteração, o mais importante, é mesmo mais simples que o para a estrutura
 * de 'tabela de dispersão', no mais, pilha-ligada é apenas uma simples 
 * lista-ligada que adiciona e remove em uma ponta.
 * === === === === === === === === === === === === === === === === === ==*/
// Novamente um simples campo, que armazena o dado.
struct saida_da_iteracao_da_pl { generico_t item; };

struct iterador_da_pilha_pl {
   // Instância do pilha-ligada que se itera.
    PilhaLigada instancia;

   // Total de itens da estrutura dada, e a contagem de iterações feitas.
   size_t inicial; size_t contagem;

   // Referência ao próximo nó à iterar.
   Node cursor;
};
// Único item é nulo.
const IterOutputPL NULO_PL = { NULL };

IterPL cria_iter_pl(PilhaLigada a) {
   struct iterador_da_pilha_pl iter; 

   // Quantos itens há inicialmente nela(para constante verificação).
   iter.inicial = tamanho_pl(a);
   iter.cursor = a->topo;
   // Começa, obviamente, com zero itens iterados.
   iter.contagem = 0;
   // A referência da instância que se está iterando...
   iter.instancia = a;

   return iter;
}

size_t contagem_iter_set (IteradorRefPL iter) {
   // Verificando se o iterador é válido.
   bool iterador_e_valido = {
      iter != NULL 
      && !consumido_iter_pl(iter)
   };

   if (iterador_e_valido)
      return (iter->inicial - iter->contagem);

   // se chegar até aqui é erro na certa.
   perror ("não é possível determinar o tamanho de um iterador inválido!");
   abort();
}

IterOutputPL next_pl (IteradorRefPL iter) {
// A iteração partirá do topo da pilha até o último nódulo dela.
   if (consumido_iter_pl(iter))
      return NULO_PL;

   generico_t dado_no_nodulo = iter->cursor->dado;
   // Indo para frente(à partir do topo).
   iter->cursor = iter->cursor->seta;
   // Contabilizando iterações realizadas.
   iter->contagem += 1;
   
   return (IterOutputPL){ .item=dado_no_nodulo };
}

bool consumido_iter_pl(IteradorRefPL iter) 
   { return iter->contagem == iter->inicial; }

IterPL clona_iter_pl(IteradorRefPL iter) {
   IterPL novo;
   // Copiando informações, em seus atuais estados:
   novo.instancia = iter->instancia;
   novo.contagem = iter->contagem;
   novo.inicial = tamanho_pl(iter->instancia);
   novo.cursor = iter->cursor;

   return novo;
}
// === === === === === === === === === === === === === === === === === ===

char* stack_to_str_pl(PilhaLigada s, ToString f) {
   if (vazia_pl(s))
      return "";
   else if (tamanho_pl(s) > UINT16_MAX / 3) {
      const char* msg_erro = {
      "Não é possível transformar em "
      "string tal quantia"
      };
      perror(msg_erro); abort();
   }

   // Pilha temporária.
   IterPL i = cria_iter_pl(s);
   IterPL j = clona_iter_pl(&i);
   size_t comprimento = 0;
   char* resultado_fmt, *dado_fmt;

   // Medindo string com maior comprimento.
   do {
      IterOutputPL a = next_pl(&i);
      generico_t r = a.item;
      // Formata o dado.
      char* string_fmt = f(r);

      comprimento += strlen(string_fmt) + 2;
      free(string_fmt);
   } while (!consumido_iter_pl(&i)); 

   // Ajustando o comprimento para o cabeçalho, e os delimitadores:
   comprimento += 50;
   // Alocando e limpando a string.
   resultado_fmt = malloc (comprimento * sizeof(char));
   // Adicioção inicial.
   IterOutputPL b = next_pl(&j);
   dado_fmt = f(b.item);
   sprintf(resultado_fmt, "Pilha(%lu): [%s]-[", tamanho_pl(s), dado_fmt);
   free(dado_fmt);

   // Criação da string, então concatenação.
   do {
      IterOutputPL a = next_pl(&j);
      generico_t r = a.item;
      // Formata o dado.
      dado_fmt = f(r);
      // Anexa espaço, separador e dado "stringuifado" no resultado.
      strcat(resultado_fmt, dado_fmt);
      strcat(resultado_fmt, ", ");
      // Libera string gerada momentaneamente.
      free(dado_fmt);
   } while (!consumido_iter_pl(&j)); 

   // Finalizando a lista em forma de string, então, retornando...
   if (vazia_pl(s))
      strcat(resultado_fmt, "]");
   else
      strcat(resultado_fmt, "\b\b]");
   return resultado_fmt;
}

void imprime_pl(PilhaLigada s, ToString f) {
   char* formatacao_da_pilha = stack_to_str_pl(s, f);
   printf("%s\n", formatacao_da_pilha);
   // Então libera a string gerada.
   free(formatacao_da_pilha);
}

void extende_pl(PilhaLigada s, PilhaLigada p) {
/* Sendo a pilha do primeiro argumento a "principal", ele pega a segunda,
 * extrai todos seus itens, então adiciona(empilha) na primeira. A abstração
 * aqui é: você tem duas pilhas, não importa o tamanho, você pega uma pilha, * digo a sua base, e coloca em cima de outra, o topo desta segunda deve 
 * ser agora o topo da pilha extendida, a outra pilha se torna vázia ou 
 * inexistente. Apesar de retirar todos seus itens para extensão, não 
 * destroi(desaloca) ela.
 */
   PilhaLigada aux = cria_pl();

   /* Colocando numa pilha auxiliar, pois realocar está pilha auxiliar na 
    * principal fará que a ordem não fique invertida, portanto o topo 
    * da secundária será também o topo da principal, formando assim uma
    * extensão de pilhas. */
   while (!vazia_pl(p)) {
      generico_t e = retira_pl(p);
      assert (coloca_pl(aux, e));
   }

   while (!vazia_pl(aux)) {
      generico_t e = retira_pl(aux);
      assert (coloca_pl(s, e));
   }
   destroi_pl(aux);
}

/* === === === === === === === === === === === === === === === === === ==
 *                Tradução de funções e métodos acimas 
 *
 * Nem todas as funções tem uma tradução, inicialmente pelo menos, já que
 * futuramente serão adicionadas. Todas elas serão apenas um embrulha com
 * a chamada da função equivalente em português. O sufixo também continua,
 * já que a pilha ligada não é a única que terá tal tipo de tradução. Alguns
 * métodos e tipos de dados não estão aqui, porque eles já estão de certo
 * modo em inglês, já outros, tenho que ainda decidir um nome.
 * === === === === === === === === === === === === === === === === === ==*/
PilhaLigada new_pl (void) { return cria_pl(); }

bool delete_pl (PilhaLigada s) { return destroi_pl(s); }

bool push_pl(PilhaLigada s, generico_t e) { return coloca_pl(s, e); }

generico_t pop_pl(PilhaLigada s) { return retira_pl(s); }

size_t len_pl (PilhaLigada s) { return tamanho_pl(s); }

bool empty_pl (PilhaLigada s) { return vazia_pl(s); }

void* top_pl (PilhaLigada s) { return topo_pl(s); }

void print_pl (PilhaLigada s, ToString f) { imprime_pl(s, f);  }

/* === === === === === === === === === === === === === === === === === ==
 *                      Testes Unitários 
 * === === === === === === === === === === === === === === === === === ==*/
#ifdef _UT_PILHA_LIGADA
#include <locale.h>
#include <assert.h>
#include <tgmath.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
// Própria biblioteca:
#include "barra_de_progresso.h"
#include "teste.h"
#include "dados_testes.h"

void pilha_com_i32s (void) {
   PilhaLigada stack = cria_pl();
   int array[] = {38, 1, -15};
   int* ptr;

   coloca_pl(stack, &array[1]);
   coloca_pl(stack, &array[0]);
   assert (coloca_pl(stack, &array[2]));

   visualiza_pl(stack);

   ptr = retira_pl(stack);
   printf("último valor: %d\n", *ptr);
   ptr = retira_pl(stack);
   printf("último valor: %d\n", *ptr);

   visualiza_pl(stack);

   int* qual_o_topo = topo_pl(stack);
   printf("topo da pilha: %d\n", *qual_o_topo);

   assert (tamanho_pl(stack) == 1);
   assert (destroi_pl (stack));
}

void pilha_de_strings (void) {
   PilhaLigada stack = cria_pl();

   coloca_pl(stack, "wealth and taste");
   visualiza_pilha_string(stack);
   coloca_pl(stack, "sympathy");
   visualiza_pilha_string(stack);
   coloca_pl(stack, "Anastasia screams in vain");
   visualiza_pilha_string(stack);
   coloca_pl(stack, "He said so tshirts");
   visualiza_pilha_string(stack);

   assert (tamanho_pl(stack) == 4);
   assert (destroi_pl (stack));
}

size_t computa_tamanho_da_pilha (PilhaLigada s) {
   size_t T = sizeof (SIZE_MAX);
   Node atual = s->topo;

   do {
      // T += sizeof (nodulo_t); 
      T += NODULO_SZ;
      atual = atual->seta;
   } while (atual != NULL);
   return T;
}

size_t unidades_por_megabytes (uint16_t nMB, size_t size) 
   { return nMB * pow (2, 20) / size; }

bool deleta_u16(generico_t dt) { free(dt); return true; }

void verificando_vazamento_de_memoria (void) {
   // inteiros de 4 bytes(int) de 50MiB deles.
   size_t total_de_unidades = unidades_por_megabytes(50, sizeof (int));
   PT barra = novo_bpt (total_de_unidades, 40);
   PilhaLigada stack = cria_pl();
   int* X;

   // adiciona na array valores randômicos de 0 até 30.
   srand ((size_t)&cria_pl);
   puts ("carregando os 50 MiB ...");

   for (size_t i = 1; i <= total_de_unidades; i++) {
      X = calloc (1, sizeof(int));
      *X = rand() % (30 + 1); 

      coloca_pl (stack, X);
      visualiza_e_atualiza_bpt (barra, i);
   }
   puts ("destruído, mas ainda na memória?");
   // uma liberação logo em seguida.
   destroi_interno_pl (stack, deleta_u16);
   puts ("destruição completa, a memória foi liberada?");
   sleep (10);
   puts ("programa finalizado.");
}

void estruturas_tamanhos (void) {
   Node no = cria_nodulo ("nada");
   size_t void_ptr_bytes = sizeof (void*);
   size_t pl_bytes = sizeof (pilha_ligada_t);
   PilhaLigada stack = cria_pl();
   size_t instancia_bytes = sizeof *stack;

   printf(
      "Método sizeof estrutura(%lu) e sua instâncias(%lu)\n",
      pl_bytes, instancia_bytes
   );
   printf ("um ponteiro 'void*' ocupa %lu bytes.\n", void_ptr_bytes);
   // printf ("o 'nódulo' ocupa %lu bytes.\n", sizeof (nodulo_t));
   printf ("o 'nódulo' ocupa %lu bytes.\n", NODULO_SZ);
   printf ("instância de 'nódulo' ocupa %lu bytes.\n", sizeof no);
   printf ("uma 'pilha ligada' ocupa %lu bytes.\n", instancia_bytes);
   assert (coloca_pl (stack, "nada"));
   assert (coloca_pl (stack, "dado"));
   assert (coloca_pl (stack, "casa"));
   printf (
      "instância vázia de 'pilha ligada'... %lu bytes.\n"
      "... mesma instância, com alguns elementos ... %lu bytes\n",
      instancia_bytes, sizeof (stack)
   );
   assert (pl_bytes == instancia_bytes);
   printf (
      "instância total é %lu bytes.\n", 
      computa_tamanho_da_pilha(stack)
   );
   destroi_pl (stack);
}

void verificando_vazamento_de_memoria_i (void) {
   PilhaLigada stack = cria_pl();
   // size_t Q = unidades_por_megabytes (300, sizeof (nodulo_t));
   size_t Q = unidades_por_megabytes (300, NODULO_SZ);
   PT barra = novo_bpt (Q, 40);
   // char* mesmo_dado = calloc (50, sizeof (char));
   // strcpy (mesmo_dado,"uma string muito grande, bem grande mesmo.");
   // um bocado de zeros:
   int* mesmo_dado = calloc (Q, sizeof (int));
   puts ("array de 150MB:");
   for (size_t k = 1; k <= Q; k++) 
      mesmo_dado[k - 1] = 8;

   // adiciona na array valores randômicos de 0 até 30.
   srand ((size_t)&cria_pl);

   for (size_t i = 1; i <= Q; i++) {
      // referênciando o mesmo dado milhões de vezes.
      coloca_pl (stack, mesmo_dado);
      visualiza_e_atualiza_bpt (barra, i);
   }
   puts ("fazendo todos items na pilha 'dangling points' ...");
   sleep (15);
   free (mesmo_dado);
   puts ("quanto fica a memória?");
   sleep (10);
   puts ("removendo item por item...");
   destroi_pl (stack);
}

char* conststr_to_str(generico_t dt) {
   char* s = dt;
   char* saida = malloc((strlen(s) + 1) * sizeof(char));
   strcpy(saida, dt);
   return saida;
}

void amostra_simples_de_todos_seus_metodos(void) {
   PilhaLigada stack = cria_pl();

   printf("Está vázia? %s\n", bool_to_str(vazia_pl(stack)));
   printf("Total de itens: %lu\n", tamanho_pl(stack));
   for (size_t k = 0; k < FRUTAS; k++) {
      char* e = (char*)frutas[k];
      printf("\tAdicionando agora '%s'...\n", e);
      assert (coloca_pl(stack, e));
   }
   printf("Total de itens: %lu\n", tamanho_pl(stack));
   printf("Está vázia? %s\n", bool_to_str(vazia_pl(stack)));
   char* item_no_topo = topo_pl(stack);
   printf("No topo: '%s'\n", item_no_topo);

   char* pilha_str = stack_to_str_pl(stack, conststr_to_str);
   printf("\nSua formatação string:\n\t%s\n", pilha_str);
   free(pilha_str);
   puts("Visualização dela:");
   imprime_pl(stack, conststr_to_str);

   puts("\nTestando método extender, e também métodos renomeados em inglês.");
   PilhaLigada stack_i = cria_pl();
   for (size_t k = 0; k < OBJETOS; k++) {
      char* e = (char*)objetos[k];
      printf("\tAdicionando agora '%s'...\n", e);
      assert (push_pl(stack_i, e));
   }
   printf("Total de itens: %lu\n", len_pl(stack));
   printf("Total de itens: %lu\n", len_pl(stack_i));
   print_pl(stack_i, conststr_to_str);
   extende_pl(stack, stack_i);
   assert (empty_pl(stack_i));
   delete_pl(stack_i);
   print_pl(stack, conststr_to_str);

   destroi_pl(stack);
}

int main(int total, char* args[], char* vars[]) {
   // permitindo impressões de caractéres 'latin1'.
   setlocale(LC_CTYPE, "pt_BR.UTF-8");

   executa_testes(
      6, pilha_com_i32s, true,
         pilha_de_strings, true,
         amostra_simples_de_todos_seus_metodos, true,
         // Desativada pois consome bastante CPU e memória:
         verificando_vazamento_de_memoria, false,
         verificando_vazamento_de_memoria_i, false,
         // Verificando algumas coisas da linguagem:
         estruturas_tamanhos, false
   );
   // fim do programa.
   return EXIT_SUCCESS;
}
#endif
