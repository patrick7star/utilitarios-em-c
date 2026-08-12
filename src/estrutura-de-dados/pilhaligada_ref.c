// Declaração de todas tipos de dados e seus métodos abaixo:
#include "pilhaligada_ref.h"
// Biblioteca padrão do C:
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
// Apenas para os testes:

/* === === === === === === === === === === === === === === === === === ==
 *                      Nódulo da lista-ligada
 *
 * Abstração da lista-ligada que forma a pilha, e porta a referência ao
 * dado original.
 * === === === === === === === === === === === === === === === === === ==*/
 #include "pilha-ligada/nodulo.c"

/* === === === === === === === === === === === === === === === === === ==
 *                      Declaração e Implementação
 *                            da Estrutura
 * === === === === === === === === === === === === === === === === === ==*/
struct pilha_ligada_abstracao {
   // quantificando quantos elementos ela realmente tem.
   size_t quantidade;

   // o ínicio da lista-ligada.
   Node topo;
};

PilhaLigada cria_pl (void) {
   size_t sz = sizeof (struct pilha_ligada_abstracao);
   // PilhaLigada instancia = malloc(sizeof(PilhaLigada));
   PilhaLigada instancia = malloc(sz);

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

size_t comprimento_pl(PilhaLigada s)
   { return s->quantidade; }

bool vazia_pl(PilhaLigada s)
   { return s->quantidade == 0; }

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

   while (!vazia_pl (s))
      retira_pl (s);

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

   while (!vazia_pl (s)) {
      generico_t dado = retira_pl(s);
      del(dado);
      dado = NULL;
   }
   free (s);
   // tudo ocorreu conforme, apenas confirma desalocação.
   return true;
}

size_t tamanho_pl(PilhaLigada s, size_t size_dt) {
   size_t a = sizeof(pilha_ligada_t) + sizeof(PilhaLigada);
   size_t b = sizeof(struct nodulo) + sizeof(Node);
   size_t c = size_dt + sizeof(generico_t);
   size_t n = comprimento_pl(s);

   return a + (b + c) * n;
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
 #include "pilha-ligada/iteracao.c"

char* stack_to_str_pl(PilhaLigada s, ToString f)
{
   size_t char_sz = sizeof(char);
   size_t t = comprimento_pl(s);
   char* resultado_fmt = NULL, 
       * dado_fmt = NULL, 
       * vazio_fmt = NULL;
   const int MAX_BUFFER = 25;

   if (vazia_pl(s)) {
      vazio_fmt = malloc(MAX_BUFFER * char_sz);

      sprintf(vazio_fmt, "Pilha(%lu): []-[]", t);
      return vazio_fmt;
   } else if (comprimento_pl(s) > UINT16_MAX / 3) {
      const char* msg_erro = {
      "Não é possível transformar em "
      "string tal quantia"
      };
      perror(msg_erro); abort();
   }

   // Pilha temporária.
   IterPL i = cria_iter_pl(s);
   IterPL j = clona_iter_pl(i);
   size_t comprimento = 0;

   // Medindo string com maior comprimento.
   do {
      IterOutputPL a = next_pl(i);
      generico_t r = a.item;
      // Formata o dado.
      char* string_fmt = f(r);

      comprimento += strlen(string_fmt) + 2;
      free(string_fmt);
   } while (!consumido_iter_pl(i));

   // Ajustando o comprimento para o cabeçalho, e os delimitadores:
   comprimento += 50;
   // Alocando e limpando a string.
   resultado_fmt = malloc (comprimento * char_sz);
   // Adicioção inicial.
   IterOutputPL b = next_pl(j);
   dado_fmt = f(b.item);
   sprintf(resultado_fmt, "Pilha(%lu): [%s]-[", t, dado_fmt);
   free(dado_fmt);

   // Criação da string, então concatenação.
   while (!consumido_iter_pl(j)) {
      IterOutputPL a = next_pl(j);
      generico_t r = a.item;
      // Formata o dado.
      dado_fmt = f(r);
      // Anexa espaço, separador e dado "stringuifado" no resultado.
      strcat(resultado_fmt, dado_fmt);
      strcat(resultado_fmt, ", ");
      // Libera string gerada momentaneamente.
      free(dado_fmt);
   }

   // Finalizando a lista em forma de string, então, retornando...
   if (vazia_pl(s) || t == 1)
      strcat(resultado_fmt, "]");
   else
      strcat(resultado_fmt, "\b\b]");

   destroi_iter_pl(i);
   destroi_iter_pl(j);
   return resultado_fmt;
}

void imprime_pl(PilhaLigada s, ToString f) {
   char* pilha_fmt;

   pilha_fmt = stack_to_str_pl(s, f);
   printf("%s\n", pilha_fmt);
   fflush(stdout);
   // Então libera a string gerada.
   free(pilha_fmt);
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

Generico to_array_pl(PilhaLigada Input, int size) {
/* Pega todas referências colocadas na pilha, então copia suas referências para uma array.
 * A pilha continua intacta, se for liberada, os elementos perterenceram apenas aos que
 * foram copiados para array. O tamanho da array, como fica a cargo do chamador computar 
 * fora desta função, apenas realiza as cópias. Fica a cargo do chamador liberar a memória
 * alocada(array).
 */
   bool nenhum_item_na_pilha = vazia_pl(Input);

   if (nenhum_item_na_pilha)
   // Com uma pilha vázia não será preciso alocar nada.
      return NULL;

   IterPL i = cria_iter_pl(Input);
   size_t tamanho = size * length_pl(Input);
   uint8_t* output = malloc(tamanho);
   IterOutputPL item;
   uint8_t* pointer = NULL;
   size_t cursor = 0;

   while (!consumido_iter_pl(i)) {
      item = next_pl(i);
      pointer = output + cursor * size;
      memmove(pointer, item.item, size);
      cursor++;
   }
   return output;
}

Generico into_array_pl(PilhaLigada input, int size) 
{
/* Pega a pilha, retira cada elemento, então faz uma cópia na array. Sendo a ponta esquerda
 * o topo da pilha na array. A array conta com alocação de memória, que seria o bastante 
 * para colocar todos itens da pilha, sendo cada um com 'size' do objeto interno. A pilha
 * é limpada durante o processo, e destruida ainda dentro do escopo da função, portanto o
 * objeto passado não existirá mais pós execução. Como é pedido o tamanho, o comprimento da
 * array também fica á cargo do chamador da função. Fica a cargo do chamador liberar a 
 * memória alocada(array).
 */
   size_t quantia = length_pl(input);
   size_t total = quantia * size;
   uint8_t* output = malloc(total);
   size_t cursor = 0; 
   GenT removido = NULL;
   uint8_t* pointer = NULL;

   if (output == NULL)
      // É para supostamente parar a aplicação.
      { perror(strerror(errno)); abort(); }

   while (!empty_pl(input))
   {
      removido = pop_pl(input);
      pointer = output + (cursor * size);
      memmove(pointer, removido, size);
      cursor++;
   }
   destroy_pl(input);
   input = NULL;
   return output;
}

void inverte_pl(PilhaLigada s)
{
/* Não será trabalhado na inversão de uma fila, usando as propriedades do
 * tipo, porque isso aqui é um modo de aproximar o algoritmo de inversão
 * de uma lista-encadeada.
 */
   Node anterior = NULL;
   Node atual    = s->topo;
   Node proximo  = NULL;

   // Itera a lista 'o total de itens' vezes.
   // for (size_t i = 1; i <= N; i++)
   while (atual != NULL)
   {
      proximo     = atual->seta;
      atual->seta = anterior;
      anterior    = atual;
      atual       = proximo;
   }
   // O topo pega o anterior ao fim da lista.
   s->topo = anterior;
}

PilhaLigada clona_pl(PilhaLigada s, Clone f)
{
/* Tendo a função que clona o tipo de dado que a pilha-ligada retém. Vamos
 * duplicar está instância, nos moldes que está seu estado. O algoritmo aqui
 * segue a abstração de pilha, ou seja, removemos cada item, clonamos, e
 * colocamos o original de volta, e o clone na pilha de cópia. Depois
 * revertemos ambas elas pois a iteração de remoção fez ficarem reviradas.
 * Por fim, retorna o clone da original. */

   // Pilha que ficará os itens clonados, e uma pilha temporaria.
   PilhaLigada e = cria_pl(), a = cria_pl();
   size_t qtd = comprimento_pl(s);

   while(qtd-- > 0) {
      generico_t remocao = retira_pl(s);
      generico_t copia = f(remocao);

      // Inserindo cópia de item, e original removido em temporária.
      assert(coloca_pl(e, copia));
      assert(coloca_pl(a, remocao));
   }

   /* Decarrega temporária novamente, porém agora na inicial. Tal iteração
    * para reposição de volta já dispensa o processo de inverter a original
    * novamente. */
   do {
      generico_t remocao = retira_pl(a);
      assert(coloca_pl(s, remocao));

   } while(!vazia_pl(a));

   /* Inverte a copia, porque a iteração de copia e inserção faz ela ficar
    * revirada. Libera a pilha temporária que está vázia. E por último,
    * retorna a clonagem da pilha. */
   inverte_pl(e);
   destroi_pl(a);
   return e;
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
 #include "pilha-ligada/ingles.c"

/* === === === === === === === === === === === === === === === === === ==
 *                      Testes Unitários
 * === === === === === === === === === === === === === === === === === ==*/
 #ifdef __unit_tests__
 #include "pilha-ligada/unitarios.c"
 #endif
