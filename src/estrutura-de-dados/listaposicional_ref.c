/*   Simples implementação genérica da deque-ligada, ou seja uma lista 
 * duplamente encadeada.
 *
 *   Se ficar muito boa se juntará as estruturas de dados tipo(iii) no
 * projeto utilitários, pois é uma array dinâmica que serve incrivelmente
 * para qualquer projeto que precisa armazenar um grande número de objetos,
 * seja eles qualquer que tipo.
 * 
 *   O que importa nestes tipos de estruturas são seu comportamento, várias
 * outras coisas que necessitam de visualização e cópia, são negligênciados
 * nesta versão de estrutura de dados.
 * 
 *   Assim como os tipos de estrutura(II), toda implementação só ficará num
 * único arquivo(este). Este ainda é melhor que o dois, pois como apenas o
 * comportamento é necesssitado, a implementação das estruturas comuns, são
 * bem menores no tipo(III).
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <assert.h>
// Declaração das funções, métodos e estruturas abaixos:

// tipo vázio que pode assumir qualquer endereço.
typedef void* generico_t;
// Assinatura da 'function pointer' que desaloca o objeto que a 'lista'
// guarda dentro dos seus nódulos.
typedef void (*Destrutor)(generico_t);
// Assinatura do 'function pointer' que permite conversão de string do dado.
typedef char* (*ToString)(generico_t);
// Nomeando nódulo para melhor legibilidade.
typedef struct item_nodulo nodulo_t, *Node; 
// Um índice inválido, é o último inteiro que a máquina permite registrar.
#define INDICE_INVALIDO SIZE_MAX
// indicador de objeto, qualquer seja, está inválido.
#define INVALIDO NULL

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * Item essencial, com visibilidade apenas aqui, para construção da
 * estrutura como todo. 
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -- */
struct item_nodulo {
   // dado genérico que, pode ser qualquer coisa.
   generico_t dado;

   // referência aos próximos itens de sua estrutura.
   nodulo_t* esquerda;
   nodulo_t* direita;
};

static nodulo_t* cria_nodulo(generico_t dt, nodulo_t* l, nodulo_t* r)
{
   size_t tamanho = sizeof(nodulo_t);
   nodulo_t* novo_nodulo = malloc (tamanho);

   if (novo_nodulo != INVALIDO) {
      // captura dado transferido.
      novo_nodulo->dado = dt;
      // novos nódulos conecta com respectivas posições passadas.
      novo_nodulo->esquerda = l;
      novo_nodulo->direita = r;
   }
   return novo_nodulo;
}

static void destroi_nodulo(nodulo_t* obj) {
// Não destrói dado interno do nódulo, apenas desaloca o nódulo.
   if (obj != INVALIDO) {
      /* libera dado nele contido. Cuidado! Se houver outra referência 
       * externa ao tal dado interno do nódulo será perdida bem aqui. */
      // free (obj->dado);
      obj->dado = NULL;
      // se livra de referência.
      obj->direita = NULL;
      obj->esquerda = NULL;
      // finalmente libera o nódulo em sí.
      free (obj);
   }
}
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 

typedef struct lista_duplamente_encadeada {
/* Simples estrutura de lista duplamente ligada. A implementação deste 
 * difere um pouco das principais, por usar sentinelas. */
   // sentinelas começo e fim.
   nodulo_t* inicio;
   nodulo_t* final;

   /* Nó que é tipo o cursor, ele está inicialmente no primeiro, porém 
    * pode ser movido, baseado onde ele está, tipo, se ele não está 
    * em uma das pontas, pode se mover tanta para direita como para 
    * esquerda, caso esteja uma, apenas pode ser mover pelo lado livre. 
    * Também tem um marcador númerico, para que seja possível navegar
    * via indexação.*/
   nodulo_t* cursor;
   size_t posicao;

   // quantidade de itens nela no momento.
   size_t quantidade;

} *ListaPosicional;

size_t quantidade_lp(ListaPosicional l) 
   { return l->quantidade; }

bool vazia_lp(ListaPosicional l)
   { return l->quantidade == 0; }

ListaPosicional cria_lp(void) {
// Modo de instanciação nula, portanto não é preciso nenhum argumento.
   size_t size = sizeof(struct lista_duplamente_encadeada);
   ListaPosicional instancia = malloc(size);

   // se foi instanciado corretamente, então completar os campos.
   if (instancia != NULL) {
      /* Criando cada sentinela da lista(fim e começo). Sem nenhum dado, 
       * ou inicial referência nas suas pontas, e mesmo quando houver só 
       * será em uma, a outra permanecerá sempre inutilizada. */ 
      instancia->inicio = cria_nodulo(NULL, NULL, NULL);
      instancia->final = cria_nodulo(NULL, NULL, NULL);

      /* Elas também serão conectadas uma a outra. */
      instancia->inicio->direita = instancia->final;
      instancia->final->esquerda = instancia->inicio;

      // começa sem nenhum item(zero).
      instancia->quantidade = 0;

      /* Também marcando o cursor, e seu indicador. Como não tem itens,
       * apenas referência o "endereço nulo". */
      instancia->cursor = NULL;
      instancia->posicao = 0;
   }
   return instancia;
}

static void insere_entre(generico_t e, Node left, Node right) {
   // criação do novo nódulo já conecta as pontas.
   Node novo = cria_nodulo(e, left, right);

   if (novo != INVALIDO) {
      assert (left != NULL);
      assert (right != NULL);
      // conectando nódulos da ponta ao 'novo meio'.
      left->direita = novo;
      right->esquerda = novo;
   }
}

static generico_t remove_entre(Node e) {
/* Obtém referência do nódulo passado, então inválida campo do nódulo,
 * já que está sendo removido(logo o nódulo restante será destruído), e 
 * tal referência será retornada. 
 */
   generico_t dado_removido = e->dado;
   // obtendo seus vizinhos de ambos lados...
   nodulo_t* node_right = e->direita;
   nodulo_t* node_left = e->esquerda;
   
   // desconectando cada um, e conectando um ao outro...
   node_right->esquerda = node_left;
   node_left->direita = node_right;

   // Destrói o nódulo, sem apagar o objeto que ele contém.
   destroi_nodulo (e);
   // retorna o dado que ele embrulha...
   return dado_removido;
}

bool insere_comeco_lp(ListaPosicional L, generico_t e) {
   // único caso que a operação pode falhar, se a lista não existir.
   if (L == INVALIDO) { return false; }

   insere_entre (e, L->inicio, L->inicio->direita);
   // contabilizando o novo dado.
   L->quantidade += 1;
   /* Inserção no começo, então o cursor é colocado no começo. */
   L->cursor = L->inicio->direita;
   // O marcador também segue o cursor.
   L->posicao = 0;

   return true;
}

bool insere_final_lp(ListaPosicional L, generico_t e) {
   // único caso que a operação pode falhar, se a lista não existir.
   if (L == INVALIDO) { return false; } 

   insere_entre (e, L->final->esquerda, L->final);
   L->quantidade += 1;
   /* Mexe o cursor também para o devido lugar de inserção, assim como o
    * seu marcador. */
   L->cursor = L->final->esquerda;
   /* A indexação segue a padrão em programação, de 0 à (n - 1), sendo 'n'
    * o número de elementos. */
   L->posicao = quantidade_lp(L) - 1;

   return true;
}

bool insere_depois_lp(ListaPosicional l, generico_t e) {
   // único caso que a operação pode falhar, se a lista não existir.
   if (l == INVALIDO) 
      { return false; } 
   else if (vazia_lp(l))
      { return insere_comeco_lp(l, e); }

   Node left = l->cursor;
   Node right = l->cursor->direita;
   insere_entre (e, left, right);
   l->quantidade += 1;
   /* O 'cursor' muda para o item adicionado, e a posição aumenta em um.*/
   l->cursor = left->direita;
   l->posicao += 1;

   return true;
}

bool insere_antes_lp(ListaPosicional l, generico_t e) {
   // único caso que a operação pode falhar, se a lista não existir.
   if (l == INVALIDO) 
      { return false; } 
   else if (vazia_lp(l))
      { return insere_final_lp(l, e); }

   Node left = l->cursor->esquerda;
   Node right = l->cursor;
   insere_entre (e, left, right);
   l->quantidade += 1;
   /* A posição(o marcador) fica estagnada, já o 'cursor' move-se para o
    * item inserido, o que é o mesmo que ficar estagnado, já que a lista
    * aumentou. */
   l->cursor = left->direita;

   return true;
}

generico_t comeco_lp(ListaPosicional l) { 
   // não pode obter um item de uma lista vázia.
   if (vazia_lp (l)) { return NULL; }

   nodulo_t* item = l->inicio->direita; 
   // Ajustando o cursor ...
   l->cursor = item;
   l->posicao = 0;

   return item->dado;
}

generico_t final_lp(ListaPosicional l) {
   // não pode obter um item de uma lista vázia.
   if (vazia_lp (l)) return NULL;

   nodulo_t* item = l->final->esquerda; 
   size_t Q = quantidade_lp(l);
   l->cursor = item;
   /* A indexação segue a padrão em programação, de 0 à (n - 1), sendo 'n'
    * o número de elementos. */
   l->posicao = Q - 1;

   return item->dado;
}

generico_t remove_lp(ListaPosicional l) {
/* Remove item(se houver algum), "debaixo" do cursor, então o cursor aponta
 * para seu sucessor(o da direita, se houver algum, é claro). */
   if (vazia_lp(l)) {
   // só remove lista com alguma coisa...
      perror("Não há itens na lista");
      abort();
   } 
   else if (l->cursor == NULL) {
   // Outras condições: deve ter um 'nó' á direita, e posição maior que 
   // zero.
      perror("Cursor inválido(não apontando item)");
      abort();
   }
   
   /* O algoritmo é o seguinte: salva o atual local, que é o item antecessor
    * deste, remove o item que o cursor aponta, então, liga o cursor 
    * a este lugar salvo, e retorno o dado no nódulo removido. */
   // Node posicao = l->cursor->esquerda;
   Node posicao = l->cursor->direita;
   generico_t removido = remove_entre (l->cursor);
   size_t P = l->posicao, Q = quantidade_lp(l);

   l->cursor = posicao;
   // Corrigindo em distorção, se for um sentinela, então volta...
   if (posicao->dado == NULL && posicao->direita == NULL)
      l->cursor = l->cursor->esquerda;

   #ifdef _REMOCAO_DA_LISTA
   puts("A remoção foi bem sucedida.");
   #endif

   // Recomposição do atual índice, no caso de ele está no fim da lista.
   if (P == (Q - 1) && P > 0)
      l->posicao -= 1;
   l->quantidade -= 1;

   return removido;
}

static bool move_cursor_para_final(ListaPosicional L) {
   if (vazia_lp(L)) return false;

   // Antecessor do nódulo sentinela na extrema-direita.
   L->cursor = L->final->esquerda;
   // Cuidando do índice numérico, contando de zero.
   L->posicao = quantidade_lp(L) - 1;
   return true;
}

static bool move_cursor_para_comeco(ListaPosicional L) {
   if (vazia_lp(L)) return false;

   // Antecessor do nódulo sentinela na extrema-direita.
   L->cursor = L->inicio->direita;
   L->posicao = 0;
   return true;
}

bool direita_lp(ListaPosicional L) {
   size_t Q = quantidade_lp(L);
   size_t p = L->posicao;

   if (vazia_lp(L)) return false;

   // Não pode passar o último item da lista.
   if (p < (Q - 1)) {
      L->cursor = L->cursor->direita;
      L->posicao += 1;
      return true;
   } else
      return false;
}

bool esquerda_lp(ListaPosicional L) {
   Node no = L->cursor->esquerda;
   bool nao_e_sentinela = !(no->dado == NULL && no->esquerda == NULL);
   bool lista_nao_tem_itens = vazia_lp(L);

   if (lista_nao_tem_itens) 
      return false;
   // Não pode passar o último item da lista.
   else if (nao_e_sentinela) {
      L->cursor = L->cursor->esquerda;
      if (L->posicao > 0)
         L->posicao -= 1;
      return true;
   } else
      return false;
}

generico_t remove_comeco_lp(ListaPosicional l) {
   if (move_cursor_para_comeco(l)) 
      return remove_lp(l->cursor);
   return NULL;
}

generico_t remove_final_lp(ListaPosicional l) {
   if (move_cursor_para_final(l)) 
      return  remove_lp(l->cursor);
   return NULL;
}


void visualiza_lista_lp(ListaPosicional l, ToString funcao) {
/* Método genérico que, dada um modo de transformar o tipo de dado que a 
 * lista armazena em string, faz a visualização de pequenas listas. Coloco
 * aqui pequenas, pois centenas de megabytes ou gibytes de impressão de 
 * uma lista é totalmente inútil. */
   Node atual = l->inicio;

   // "Cabeçalho" da lista.
   printf("Lista: {");
   if (vazia_lp(l))
      { putchar('}'); putchar('\n'); }

   do {
      generico_t dt = atual->dado;

      // Apenas imprime se o 'dado' no 'nó' for válido.
      if (dt != NULL) { 
         char* data_str = funcao(dt);

         // Marca o 'cursor'.
         if (atual == l->cursor)
            { printf("|%s|, ", data_str); }
         else
            { printf("%s, ", data_str); }

         // Depois de usado, libera.
         free(data_str);
      }
      atual = atual->direita;
   } while (atual->direita != NULL);
   puts("\b\b}");
}

size_t indice_do_cursor_lp(ListaPosicional l) { 
/* O último valor possível do inteiro positivo de máquina, simplesmente
 * indica que nõa há posição, porque a lista está vázia. */
   if (vazia_lp(l))
      { return INDICE_INVALIDO; }

   return l->posicao; 
}

bool destroi_lp(ListaPosicional l) {
// Destroi lista, e seus nódulos, porém não o dado em cada um deles.
   if (l != INVALIDO) {
      /* Só destroi se houver uma lista. */
      while (!vazia_lp(l)) 
         // Esvaziando a lista ...
         remove_lp (l); 
      // Se livra da referência do 'cursor'.
      l->cursor = NULL;

      // Destrói(desaloca) as sentinelas.
      destroi_nodulo (l->inicio);
      destroi_nodulo (l->final);
      // destrói lista e confirma desalocação.
      free (l);

      // Confirma processo como sucedido, se chegou até aqui.
      return true;
   }
   // Lista inválida, informa que nada foi desalocado.
   return false;
}

bool destroi_interno_lp(ListaPosicional l, Destrutor f) {
/* Dado od desalocador do tipo de dado interno da lista, e sendo tal não
 * estático(não vive toda execução do programa, ou seja, na heap da 
 * memória), portanto, tal método também fica encarregado de liberar tal.*/
   if (l != INVALIDO) {
      // Remove cada nódulo, e libera com a função passada.
      while (!vazia_lp(l)) {
         generico_t retorno = remove_lp(l);
         // Se o dado for válido, então desaloca-o com o descontrutor.
         if (retorno != INVALIDO) f(retorno); 
      }
      // Se livra da referência do 'cursor'.
      l->cursor = NULL;
      // remove sentinelas...
      destroi_nodulo(l->inicio);
      destroi_nodulo(l->final);

      // destrói lista e confirma desalocação.
      free (l);
      return true;
   }
   return false;
}

void destroi_n_lp(int qtd, ...) {
/* Desalocação de múltiplas listas, entretanto, está não libera o dado
 * interno. Na verdade, fazer uma dessa com tal utilidade, necessitaria
 * de vários deconstrutors, pois cada um pode liberar apenas um tipo de 
 * dado -- claro que se todas tivessem o mesmo tipo, seria mais fácil. */
   va_list lista;
   size_t contagem = qtd;

   va_start(lista, qtd);
   do {
      ListaPosicional L = va_arg(lista, ListaPosicional);
      destroi_lp(L);
   } while (contagem-- > 0);

   va_end(lista);
}

#ifdef _UT_LISTA_POSICIONAL
// bibliotecas necessárias:
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <string.h>
// Própria biblioteca:
#include "dados_testes.h"
// #include "aleatorio.h"
#include "teste.h"
//
void desaloca_8bits_obj(generico_t O) { 
   int8_t* ptr = O;
   printf("Objeto '%d' genérico desalacado com sucesso.\n", *ptr); 
}

char* int8t_to_str(generico_t v) {
   char* result_fmt = malloc(10 * sizeof(char));
   int8_t* ptr_valor = v;

   sprintf(result_fmt, "%d", *ptr_valor);
   return result_fmt;
}

void alguns_testes_basicos_de_funcionamento(void) {
   int8_t input[] = {
      -9, -6, -3, 0, 3, 6, 9, -100, 101,
      59, -58, 57, -56
   };

   ListaPosicional L = cria_lp();
   printf("Qtd. de itens: %lu\n", quantidade_lp(L));

   insere_comeco_lp(L, &input[2]);
   insere_final_lp(L, &input[1]);
   insere_comeco_lp(L, &input[6]);
   insere_final_lp(L, &input[0]);
   printf("Qtd. de itens: %lu\n", quantidade_lp(L));
   printf("Cursor em %lu.\n", indice_do_cursor_lp(L));
   visualiza_lista_lp(L, int8t_to_str);

   insere_antes_lp(L, &input[7]);
   visualiza_lista_lp(L, int8t_to_str);
   insere_antes_lp(L, &input[8]);
   visualiza_lista_lp(L, int8t_to_str);
   
   printf("Cursor em %lu.\n", indice_do_cursor_lp(L));
   insere_depois_lp(L, &input[9]);
   visualiza_lista_lp(L, int8t_to_str);
   printf("Cursor em %lu.\n", indice_do_cursor_lp(L));
   insere_depois_lp(L, &input[10]);
   visualiza_lista_lp(L, int8t_to_str);
   printf("Cursor em %lu.\n", indice_do_cursor_lp(L));
   insere_depois_lp(L, &input[11]);
   visualiza_lista_lp(L, int8t_to_str);
   printf("Cursor em %lu.\n", indice_do_cursor_lp(L));
   insere_depois_lp(L, &input[12]);
   visualiza_lista_lp(L, int8t_to_str);
   printf("Cursor em %lu.\n", indice_do_cursor_lp(L));

   generico_t remocao = remove_lp(L);
   printf("Item removido: %d\n", *((int8_t*)remocao));
   visualiza_lista_lp(L, int8t_to_str);
   remocao = remove_lp(L);
   printf("Item removido: %d\n", *((int8_t*)remocao));
   visualiza_lista_lp(L, int8t_to_str);

   puts("\nComeçando desalocação, com destruição interna...");
   destroi_interno_lp(L, desaloca_8bits_obj);
   // destroi_lp(L);
}

char* conststr_to_str(generico_t v) {
   const char* pointer = v;
   size_t tamanho = strlen(pointer) * sizeof(char); 
   char* bufferstr = malloc(tamanho);
   sprintf(bufferstr, "%s", pointer);
   return bufferstr; 
}

static size_t count = 0;
typedef bool (*Metodo)(ListaPosicional, generico_t);

static void insersor_randomico(ListaPosicional l, generico_t dt) {
   Metodo funcoes[] = {
      insere_depois_lp, insere_antes_lp, 
      insere_comeco_lp, insere_antes_lp,
      insere_final_lp, insere_depois_lp,
      insere_depois_lp, insere_antes_lp,
      insere_comeco_lp, insere_depois_lp,
      insere_final_lp, insere_antes_lp
   };
   funcoes[count++ % 12](l, dt);
}

void acompanhando_movimentacao_do_cursor(void) {
   ListaPosicional L = cria_lp();

   assert (vazia_lp(L));
   printf("Cursor em %lu.\n", indice_do_cursor_lp(L));
   printf("Qtd. de itens: %lu\n", quantidade_lp(L));
   
   for (size_t i = 1; i <= VEICULOS; i++) {
      generico_t dt = (generico_t)veiculos[i - 1];
      insersor_randomico(L, dt);
      visualiza_lista_lp(L, conststr_to_str);
      printf("Cursor em %lu.\n\n", indice_do_cursor_lp(L));
   }

   printf("Qtd. de itens: %lu\n", quantidade_lp(L));
   // assert(destroi_lp(L));
}

void movendo_cursor_pela_lista(void) {
   ListaPosicional L = cria_lp();

   for (size_t i = 1; i <= BOYS_NAMES; i++) {
      generico_t dt = (generico_t)boys_names[i - 1];
      insersor_randomico(L, dt);
   }

   assert(move_cursor_para_comeco(L));
   do {
      visualiza_lista_lp(L, conststr_to_str);
      printf("Atual posição na lista: %lu\n", indice_do_cursor_lp(L));
   } while (direita_lp(L));

   assert(!direita_lp(L));
   do {
      visualiza_lista_lp(L, conststr_to_str);
      printf("Atual posição na lista: %lu\n", indice_do_cursor_lp(L));
   } while(esquerda_lp(L));
   assert(!esquerda_lp(L));

   printf("Qtd. de itens: %lu\n", quantidade_lp(L));
   assert(destroi_lp(L));
}

void remocao_de_unico_item_na_lista(void) {
   ListaPosicional L = cria_lp();
   char* removido;
   generico_t item;

   assert (vazia_lp(L));
   printf("Qtd. de itens: %lu\n", quantidade_lp(L));

   puts("\nRemoção de único item:");
   item = objetos[7];
   assert (insere_comeco_lp(L, item));
   removido = (char*)remove_lp(L);
   printf("Remoção: '%s'\n\n", removido);

   puts("Pelo final agora: ");
   item = objetos[4];
   assert (insere_final_lp(L, item));
   removido = (char*)remove_lp(L);
   printf("Remoção: '%s'\n\n", removido);

   assert (vazia_lp(L));
   destroi_lp(L);
}

void remocao_de_tudo(void) {
   ListaPosicional L = cria_lp();
   assert (vazia_lp(L));
   printf("Qtd. de itens: %lu\n", quantidade_lp(L));

   puts("\nRemoção de único item:");
   generico_t item = objetos[7];
   assert (insere_comeco_lp(L, item));
   const char* removido = remove_lp(L);
   printf("Remoção: '%s'\n\n", removido);

   for (size_t i = 1; i <= 4; i++) 
      insersor_randomico(L, (generico_t)veiculos[i - 1]);
   visualiza_lista_lp(L, conststr_to_str);
   printf("Qtd. de itens: %lu\n", quantidade_lp(L));
   printf("Cursor em %lu.\n", indice_do_cursor_lp(L));

   puts("\nRemoção à partir do cursor ...");
   move_cursor_para_final(L);
   for (size_t q = 4; q > 0; q--) {
      printf(
         "Restantes: %lu, índice:%lu\n\n", 
         quantidade_lp(L), indice_do_cursor_lp(L)
      );
      visualiza_lista_lp(L, conststr_to_str);
      char* saida = remove_lp(L);
      printf("Removido: '%s'\n\n", saida);
   }

   assert (vazia_lp(L));
   assert(destroi_lp(L));
}

int main(int total, char* argumentos[]) {
   executa_testes(
      5, alguns_testes_basicos_de_funcionamento, true,
         acompanhando_movimentacao_do_cursor, true,
         remocao_de_unico_item_na_lista, true,
         remocao_de_tudo, true,
         movendo_cursor_pela_lista, true
   );
   return EXIT_SUCCESS;
}
#endif

