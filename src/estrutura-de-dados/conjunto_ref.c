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

#include "conjunto_ref.h"
// biblioteca padrões em C:
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <assert.h>

// todas constantes:
#define INVALIDA NULL

typedef struct nodulo_do_hash { 
   // valores genéricos tanto da chave como do valor:
   generico_t chave; 
   /* Item inútil, com seus dias contados, porém para não obter qualquer
    * erro de primeira, será deixado aqui inicialmente, e depois trocado
    * por algo mais leve(um 'byte', por exemplo), já que um ponteiro ocupa
    * oito bytes. 
    */
   generico_t valor;

   // referência para próximo item.
   struct nodulo_do_hash* seta;

} nodulo_t, *Node;

// Apelido específico para array do tipo acima.
typedef nodulo_t** ArrayNodulo;

// O resultado da função que verifica se tal 'lista' tem determinado item:
typedef struct { bool contido; size_t posicao; nodulo_t* item; } result_t;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// Parte que demarca uma parte interna, o 'nódulo' da lista ligada.

static nodulo_t* cria_nodulo (generico_t key, generico_t value) {
   /* Retorna uma instância inválida ou não. Dependendo se a alocação 
    * foi bem sucedidad. */
   nodulo_t* instancia = malloc (sizeof (nodulo_t));

   if (instancia != INVALIDA) {
      instancia->chave = key;
      instancia->valor = value;
      instancia->seta = NULL;
   }
   return instancia;
}

static void destroi_toda_lista_ligada (nodulo_t* lista, bool tentar) {
   if (lista != INVALIDA) {
      // obtendo primeiro item, antes que ele possa ser "perdido".
      nodulo_t* remocao = lista;
      lista = lista->seta;

      // resetando referências internas...
      if (tentar) {
         // Tentando liberar referências passadas.
         free(remocao->chave);
         free(remocao->valor);
      } else {
         remocao->chave = NULL;
         remocao->valor = NULL;
      }

      free (remocao);
      // indo para remoção do outro nódulo ...
      destroi_toda_lista_ligada (lista, tentar);
   }
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

struct tabela_de_dispersao {
   // array de containers dos dados:
   nodulo_t** locais;

   // contabilização de itens(ultra necessário nesta estranha estrutura).
   size_t quantidade;

   // quantia de blocos que tem lista-encadeadas.
   size_t capacidade;

   /* Pointeiro de função para a hash que se aplicará a todos valores. Por
    * isso é importante que tal mapa tenha apenas um tipo de dado por 
    * instância. Uma função que retorne o maior valor inteiro positivo
    * possível. Ela receberá a 'chave' e a capacidade do 'mapa'. Também
    * deve-se inserir a função que compara os dois tipos. Se não houver
    * nenhuma das duas, a estrutura hash para aceitar tal tipo fica 
    * impossível de se fazer. */
   Hash __hash__; Eq __iguais__;
   /* É possível passar depois uma 'função hash e de igualdade', assim 
    * como outros métodos. */
   bool __eq__confirmada;
   bool __hash__confirmada;
};

bool adiciona_metodos_set (Conjunto s, Hash fH, Eq fC) {
/* Permite a adição dos métodos essenciais de overloading, posteriormente.
 * Assim pode-se criar uma instância inicial, sem necessáriamente precisar
 * declarar-lôs antes, porém, só é possível adicionar/buscar/remover se 
 * houver no escopo da instância tais métodos anexados. */
   if (s == INVALIDA)
      return false;

   if (s->__hash__confirmada || s->__eq__confirmada) {
      perror ("não é possível aplicar isso, pois já foram anteriormente.");
      abort();
   }
   
   // coloca ambos métodos e marca como positivo paras operações.
   if (fH != INVALIDA) {
      s->__hash__ = fH;
      s->__hash__confirmada = true;
   }
   if (fC != INVALIDA) {
      s->__iguais__ = fC;
      s->__eq__confirmada = true;
   }
   return s->__hash__confirmada && s->__eq__confirmada;
}

Conjunto cria_com_capacidade_set(size_t capacidade, Hash fH, Eq fC) {
   Conjunto mapa = malloc(sizeof(conjunto_t));
   size_t Q = capacidade;
   const size_t SIZE_REF = sizeof (nodulo_t*);

   if (mapa != NULL) {
      // alocando a array de listas e registrando seu tamanho...
      mapa->locais = malloc (Q * SIZE_REF);
      for (size_t k = 1; k <= Q; k++)
         mapa->locais[k - 1] = NULL;
      mapa->capacidade = Q;

      // sem elementos inicialmente, por motivos óbvios.
      mapa->quantidade = 0;

      // referênciando funções que farão o cálculo hash internamente.
      if (fH != NULL) {
         mapa->__hash__ = fH;
         // informando a instância tem tais métodos essenciais...
         mapa->__hash__confirmada = true;
      } else
         mapa->__hash__confirmada = false;

      if (fC != NULL) {
         mapa->__iguais__ = fC;
         mapa->__eq__confirmada = true;
      } else
         mapa->__eq__confirmada = false;
   }
   return mapa;
}

Conjunto cria_set (Hash _hash_, Eq _igual_)
   // criação vázia da 'hashtable', com uma capacidade inicial.
   { return cria_com_capacidade_set (20, _hash_, _igual_); }

Conjunto cria_branco_set (void) 
/* cria a mesma instância que acima, porém sem as instâncias necessárias
 * nele a priori. Elas são necessárias para fazer várias operações, 
 * porém para apenas instânciar, não. */
   { return cria_com_capacidade_set (30, NULL, NULL); }

bool destroi_set(Conjunto S) { 
   if (S == INVALIDA) return false; 

   size_t Q = S->capacidade, i = 1;
   for (; i < Q; i++) {
      nodulo_t* lista = S->locais[i - 1];
      destroi_toda_lista_ligada (lista, false);
   }
   free (S);
   // confirma que tudo está liberado.
   return true;
}

void destroi_interno_set(Conjunto S, Drop g) { 
   if (S == INVALIDA) return ; 

   size_t Q = S->capacidade, i = 1;
   for (; i < Q; i++) {
      nodulo_t* lista = S->locais[i - 1];
      destroi_toda_lista_ligada (lista, true);
   }
   free (S);
}

static result_t 
verifica_lista (nodulo_t* lista, generico_t chave, Eq compara) {
/* Vários algoritmos abaixo, necessitam uma hora varrer a lista ligada
 * interna, obtem o item específicos(compararação) e tal. Tal função 
 * auxiliar terceira para toda cadeia abaixo, tal rotina. Retorna-se 
 * então se há tal, a posição na lista-encadeada, e o item pertecente,
 * mesmo que as funções abaixo não precisem, todas, de todos estes 
 * componentes.
 *
 *    Entrada: 
 *       - inicio da 'lista encadeada'
 *       - 'chave' do dicionário que se quer verificar
 *       - função que compara dado da referência
 *    Saída:
 *       - uma tupla 'result_t'
 */
   const result_t NEGACAO_PADRAO = { false, SIZE_MAX, NULL };
   size_t indice = 0;

   if (lista == INVALIDA) return NEGACAO_PADRAO;

   nodulo_t* atual = lista;
   // itera toda lista procurando por item correspondente.
   do {
      // pega referência da chave, e marca o atual índice na lista.
      generico_t key = atual->chave;
      indice++;

      if (compara (key, chave))
         // verifica se achou um item correspondente..
         return (result_t){ true, indice, atual };

      // indo para próximo item...
      atual = atual->seta;
   } while (atual != NULL);

   // se chegou até aqui, e não encontrou nada, então é isso aí, nada!
   return NEGACAO_PADRAO;
}

bool adiciona_set(Conjunto s, generico_t e) {
/* Computa posição na array baseada na chave e capacidade. A função
 * hash, por desenho da estrutura, já vem embutida. O retorno "positivo" 
 * do tipo lógico confirma que a operação foi sucedida no fim.
 */
   size_t posicao = s->__hash__ (e, s->capacidade);
   nodulo_t* entrada = s->locais[posicao];

   if (entrada == INVALIDA) {
      /* se estiver vázia, facilita muito, apenas endereça espaço para uma
       * nova 'entrada' com valor e chave. */
      s->locais[posicao] = cria_nodulo (e, NULL);
      // contabiliza inserção.
      s->quantidade += 1;

      // confirma inserção como feita, se chegar até aqui.
      return true;
   } else {
      /* Se não for inválida, percorrer lista até o final, enquanto que 
       * compara com cada item, para ver se tem alguma chave já registrada.
       */
      bool (*funcao)(generico_t, generico_t) = s->__iguais__;
      nodulo_t* nova = cria_nodulo (e, NULL);
      result_t resultado = verifica_lista (s->locais[posicao], e, funcao);
      if (resultado.contido)
         return false;
      else {
         nova->seta = s->locais[posicao];
         s->locais[posicao] = nova;
         // contabiliza inserção.
         s->quantidade += 1;
         return true;
      }
   }
}

bool pertence_set(Conjunto S, generico_t e) { 
/* verifica se tal chave está contida na tabela de dispersão. Chama 
 * função que da uma varrida na lista interna procurando pelo primeiro
 * item correspondente, manda o resultado e muito mais, porém apenas
 * estamos interessado na condição de "pertencimento" retornada. */

   /* computa posição na array baseada na chave e capacidade. A função
    * hash, por desenho da estrutura, já vem embutida. 
    */
   size_t posicao = S->__hash__ (e, S->capacidade);
   nodulo_t* list = S->locais[posicao];
   result_t resultado = verifica_lista (list, e, S->__iguais__);

   return resultado.contido;
}

bool deleta_set(Conjunto S, generico_t e) { 
/* A operação de remoção é apenas usar o método de atualização, para
 * inserir uma chave-em-branco no lugar da 'chave' dada, o resto é 
 * apenas realinhar das 'entradas' na array, tipo chaves-em-branco
 * no fim dela, 'entradas' não vázia no começo, não importando a ordem. */
   size_t posicao = S->__hash__ (e, S->capacidade);
   result_t outcome = verifica_lista(S->locais[posicao], e, S->__iguais__);
   nodulo_t* item = outcome.item;
   bool a_chave_existe = outcome.contido;

   // apenas faz a remoção, se e somente se, houver tal 'chave'.
   if (a_chave_existe) {
      nodulo_t *atual = S->locais[posicao];

      if (outcome.posicao > 1) {
         nodulo_t* antecessor = NULL;
         size_t i = 1;

         // indo até o antecessor deste índice...
         do {
            antecessor = atual;
            atual = atual->seta;
            i++;
         } while (i < outcome.posicao);
         antecessor->seta = item->seta;
      } else 
         /* Se for o primeiro item na lista encadeada, então apenas fazer
          * seu começo apontar prá o que seu único item está apontando.
          */
         S->locais[posicao] = atual->seta;

      // descontabiliza item removido.
      S->quantidade -= 1;
      /* libera item(sucetível a erro), possivelmente comentado pois parte
       * causa muitos problemas se não feito de maneira certa. */
   }
   return a_chave_existe;
}

bool vazia_set(Conjunto S) 
/* Operações, principalmente de encapsulamento, extremamente triviais
 * mais necessárias para esconder, futurumente quando este arquivo
 * fazer parte da 'lib' acessar tais valores da estrutura. */
   { return S->quantidade == 0; }

size_t tamanho_set(Conjunto S) 
   // Total de itens contidos no 'conjunto'.
   { return S->quantidade; }


/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 *                      Iterador: funções e métodos
 *                         relacionados a tal
 *
 *   Basicamente uma cópia do que foi feito na estrutura hashtable. Que é,
 * toda esta implementação, um cópia desta, com algumas modificações, digo
 * principalmente os nomes, o sufixo os diferencia. Algum problema futuro, 
 * consultar tal, pode ter sido uma copia ou alteração aqui que está 
 * causando tal bug. Lá, os algoritmos são muito mais sólidos.
 *   Alguns nomes dos campos das abstrações diferem em nome e significado,
 * no fim, fico com este, pois é mais recente, o modo aqui chamados deve
 * ser mais correto do que a implementação original, que foi feita de forma
 * apressada, assim não houve muito tempo para criar bons nomes.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */
/* A saída da iteração é apenas um ponteiro que guarda endereços de objetos
 * genéricos, neste caso aqui apenas 'chave', porque o valor na estrutura
 * conjunto é inútil. */
// struct saida_da_iteracao_do_set { generico_t item; };

struct iterador_do_conjunto {
/* Necessita de algumas coisas: a referência do conjunto que ele itera; 
 * o total de itens iniciais, já que é necessário que tal não mude na 
 * iteração, guardamos tal valor, para verificar a cada chamada de iteração
 * se não houve alteração; um cursor, assim a iteração é simplesmente 
 * dependente da chamada; também a contagem de itens iterados, assim é 
 * possível estimar quanto falta para consumir todo iterador(acabar). */
   // Instância do conjunto que se itera.
   Conjunto instancia;

   size_t total_inicial;
   // Total de itens iteradores do conjunto.
   size_t contagem;

   // Referência ao próximo nó à iterar.
   Node cursor;
   /* Índice na array de listas-ligadas que forma a hashtable(motor da 
    * estrutura conjunto). */
   size_t indice;
};

// Definindo constante de iteração inválida(ou nula) do iterador:
// const IterOutputSet NULO_SET = { NULL };

IterSet cria_iter_set(Conjunto a) {
/* O iterador é criado estaticamente(na stack), e, apenas funciona enquanto
 * o conjunto que e ele itera existe. */
   struct iterador_do_conjunto iter; 

   // Quantos itens há inicialmente nela(para constante verificação).
   iter.total_inicial = tamanho_set(a);
   // Sempre começa no primeiro índice da array de linked-lists.
   iter.cursor = a->locais[0];
   iter.indice = 0;
   // Começa, obviamente, com zero itens iterados.
   iter.contagem = 0;
   // A referência da instância que se está iterando...
   iter.instancia = a;

   return iter;
}

static bool iterador_valido (IteradorRefSet iter) {
/* Para realizar qualquer uma das operações abaixo, é necessário
 * que a instância seja válida, ou seja, ainda existe, ou tem o mesmo
 * tamanho que na criação da instância. */
   size_t T = tamanho_set(iter->instancia);
   bool referencia_existe = (iter->instancia != NULL);
   return (iter->total_inicial == T && referencia_existe);
}

size_t contagem_iter_set (IteradorRefSet iter) {
/* O restante de iterações é calculado na seguinte forma: total de itens na
 * "lista" menos os já iterados. 
 */
   if (iterador_valido (iter))
      return iter->total_inicial - iter->contagem;

   // se chegar até aqui é erro na certa.
   perror ("não é possível determinar o tamanho de um iterador inválido!");
   abort();
}

IterOutputSet next_set (IteradorRefSet iter) {
/* Retorna "próximo" item do iterador de tal conjunto, à partir de onde 
 * parou na última chamada, se houver algum. Em caso de ter se esgotado,
 * ou simplemente não houver nada a iterar, simplesmente retorna o valor
 * nulo deste tipo de dado. */
   Node atual = iter->cursor;
   generico_t dadoref = NULL;

   /* Casos que não retornar qualquer valor válido. */
   if (!iterador_valido(iter))
      return NULO_SET;
   else if (contagem_iter_set (iter) == 0)
      return NULO_SET;

   if (atual != INVALIDA) {
      // colhendo dados necessários ...
      dadoref = atual->chave;
      // movendo pela lista ...
      iter->cursor = iter->cursor->seta;
      // contabiliza iteração.
      iter->contagem++;

      // retorna item "cholido".
      return (IterOutputSet){ .item = dadoref };
   } else  {
      iter->indice += 1;
      /* primeiro 'nódulo' da lista abaixo. */
      iter->cursor = iter->instancia->locais[iter->indice];
      // chama a função recursivamente novamente...
      return next_set (iter);
   }
   /* Se chegar até aqui é o fim da iteração, se houve alguma. Neste caso
    * será retornado uma tupla com endereço inválidos. */
   return NULO_SET;
}

bool consumido_iter_set(IteradorRefSet iter) 
/* Verifica se o iterador foi todo consumido, ou seja, todos itens do 
 * conjunto foram devidamente iterados. */
   { return iter->contagem == iter->total_inicial; }

IterSet clona_iter_set(IteradorRefSet iter) {
/* Clona o iterador passado, à partir do estágio que está. A alteração 
 * deste novo clone, ou do original, não alteram a iteração de cada, más 
 * sim, a mudança da estrutura original, que não permite ambos realizar 
 * mais iterações. */ 
   IterSet novo;
   // Copiando informações:
   novo.instancia = iter->instancia;
   novo.contagem = iter->contagem;
   novo.total_inicial = tamanho_set(iter->instancia);
   // Camos internos para iteração(até mais importante que os acimas):
   novo.cursor = iter->cursor;
   novo.indice = iter->indice;

   return novo;
}
// === === === === === === === === === === === === === === === === === ===

char* set_to_str(Conjunto S, ToString f) {
/* Converte uma string para string, dado a função que transforma o dado
 * interno dela em string. Há um limite aqui, é uma função que usa muito
 * recurso, então há um limite de itens que se podem imprimir, aqui no 
 * caso está na casa das dezenas de milhares; se for necessário aumentar
 * isso é possível(para o máximo até). Coloco tal limite, porque uso 
 * geralmente isso para exemplos com poucas quantidades, não vejo nenhuma
 * necessidade de aumentar isso. */
   char* resultado_fmt;
   IterSet iterador = cria_iter_set(S);
   IterSet iterador_copia = clona_iter_set(&iterador);
   size_t mC = 0;
   const char* cabecalho = "Conjunto: {";
   const char* fim;
   const char* separador = ", ";
   char* dado_str;
   // Nesta quantia de itens, tal impressão fica totalmente desnecessária
   // na maioria dos casos. Se quiser ativar, apenas comente isso.
   const size_t LIMITE_PROCESSAMENTO = UINT16_MAX / 4;

   // Limite de processamento para tal quantia de itens.
   if (tamanho_set(S) >= LIMITE_PROCESSAMENTO) {
      const char msg_erro[] = {
         "Mais que tal quantia, às vezes é desnecessário"
      };
      perror(msg_erro); abort();
   }

   // Computa dado que gera a mais longa string.
   while (!consumido_iter_set(&iterador)) {
      IterOutputSet i = next_set(&iterador);
      generico_t dado = i.item;
      dado_str = f(dado);
      size_t t = strlen(dado_str);

      if (t > mC)
      // Sempre atualiza para maior comprimento de string.
         mC = t + 1;
      // Após de ter medida a string, libera ela.
      free(dado_str);
   }

   // Alocando string que terár tal formatação:
   size_t total_alocado = (mC + strlen(separador)) * sizeof(char) + 30;
   resultado_fmt = malloc(total_alocado);

   // Branqueando-a com algum conteudo.
   strcpy(resultado_fmt, "");
   // Colocando o cabeçalho primeiramente ...
   strcat(resultado_fmt, cabecalho);

   if (vazia_set(S)) 
      // O fim depedente se foi iterado ou não.
      fim = "}";
   else {
      while (!consumido_iter_set(&iterador_copia)) {
         IterOutputSet i = next_set(&iterador_copia);
         generico_t dado = i.item;
         dado_str = f(dado);
         strcat(resultado_fmt, dado_str);
         strcat(resultado_fmt, separador);
         // Removendo dado em formato string.
         free(dado_str);
      }
      fim = "\b\b}";
   }
   // Enclausurando a formatação da estrutura.
   strcat(resultado_fmt, fim); 
   return resultado_fmt; 
}

void impressao_generica(Conjunto S, ToString funcao) {
/* Dado um conjunto, e o método que converte seu tipo de dado interno numa
 * string, a função faz a impressão de uma lista simples dele. */
   IterSet i = cria_iter_set(S);
   char* dado_str;

   printf("Conjunto(%lu): {", tamanho_set(S));
   if (vazia_set(S))
      { puts("}"); return; }

   while (!consumido_iter_set(&i)) {
      IterOutputSet a = next_set(&i);
      generico_t dt = a.item;

      // Estringuifica, imprime, e libera ciclo.
      dado_str = funcao(dt);
      printf("%s, ", dado_str);
      free(dado_str);
   }
   puts("\b\b}");
}

void imprime_set(Conjunto a, ToString f)
   { return impressao_generica(a, f); }

bool limpa_set(Conjunto a) {
/*   Remove todos itens do conjunto. O algoritmo para realizar isso é 
 * obter a lista(uma array) de itens, indexar uma por uma, e pedir para
 * o método apropriado do conjunto faça isso. Retorno lógico funciona de
 * modo simples, 'falso' se ela estava vázia, ou seja, não era precisa 
 * remover nada; 'verdadeiro' caso contrário. */
   if (vazia_set(a))
      return false;

   IterSet iterador = cria_iter_set(a);
   IterOutputSet lista[tamanho_set(a)];
   size_t t = 0;

   // Listando itens numa array-fixa ...
   while (!consumido_iter_set(&iterador))
      lista[t++] = next_set(&iterador);
   // Zerando para indexação seguinte ...
   t = 0;

   while (!vazia_set(a))
      // Agora aplicando a remoção ...
      deleta_set(a, lista[t++].item);
   
   return true;
}

/* === === === === === === === === === === === === === === === === === ==
 *                      Operações envolvendo 
 *                            Conjuntos
 *
 *   Veja que é necessário tomar precação com estas operações, porque o 
 * resultado é um conjunto novo, porém compartilhando o endereço dos 
 * itens de cada conjunto passado, ou seja, se você liberar algum deles
 * com destruição dos itens, ou o resultado das operações, é possível que 
 * causa problemas de memórias tipo 'dangling pointers'.
 * === === === === === === === === === === === === === === === === === ==*/
Conjunto uniao_set(Conjunto a, Conjunto b)
{
/*   Pega dois conjuntos não vázio, então junta todos itens, distintos e 
 * iguais de cada, sem duplicar o item em sí. O retorno será um conjunto
 * com ambos itens de cada conjunto.
 *   O algoritmo para conseguir tal conjunto resultante é o seguinte: Ambos
 * conjuntos serão iterados, e adicionado ao resultante, que cuida 
 * internamente com itens já adicionados(duplicados). */
   bool ambos_iteradores_esgotados = false;
   /* Funções hashes, e de comparação, de cada uma, são as mesmas, ou 
    * resultam no mesmo valor para inputs semelhantes. */
   Hash hash = a->__hash__; Eq equal = a->__iguais__;
   Conjunto resultado = cria_set(hash, equal);
   IterSet ia = cria_iter_set(a), ib = cria_iter_set(b);
   
   do {
      IterOutputSet x = next_set(&ia);
      IterOutputSet y = next_set(&ib);

      /* Adiciona se, e somente se, a saída de iterador for válida. Assim,
       * evita que um iterador que se esgotou quebre a lógica da inserção.
       */
      if (x.item != NULO_SET.item)
         adiciona_set(resultado, x.item);
      if (y.item != NULO_SET.item)
         adiciona_set(resultado, y.item);

      // Verificando se ambos iteradores se esgotarem de novas itens.
      ambos_iteradores_esgotados = (
         !consumido_iter_set(&ia) ||
         !consumido_iter_set(&ib)
      );

   } while (ambos_iteradores_esgotados);

   #ifdef __debug__
   printf("Total de itens: %lu\n", tamanho_set(resultado));
   #endif

   return resultado;
}

Conjunto intersecao_set(Conjunto a, Conjunto b)
{
/* O algoritmo de interseção é o seguinte: pegue o iterador de algum dos
 * argumentos, na iteração, verifique se tal item já está no outro, se 
 * estiver, então adicione tal item. */
   Hash hash = a->__hash__; Eq equal = a->__iguais__;
   Conjunto result = cria_set(hash, equal);
   IterSet iA = cria_iter_set(a);

   do {
      IterOutputSet e = next_set(&iA);

      if (e.item != NULO_SET.item)
      {
         if (pertence_set(b, e.item))
         // Apenas adiciona tal elemento, na resultante, se o outro 
         // conjunto também tem ele.
            adiciona_set(result, e.item);
      }
   } while (!consumido_iter_set(&iA));

   return result;
}

Conjunto diferenca_set(Conjunto a, Conjunto b)
{
/* Algoritmo da diferença de conjuntos -- observe que aqui a ordem das 
 * operações importam, segue estritamente a definição da operação: todos
 * itens de A, que não estão em B. Basicamente, podemos fazer isso seguindo
 * o algoritmo de interseção, porém adicionar apenas itens que não fazem
 * interção, uma negação neste trecho de tal algoritmo. */
   Hash hash = a->__hash__; Eq equal = a->__iguais__;
   Conjunto result = cria_set(hash, equal);
   IterSet iA = cria_iter_set(a);

   do {
      IterOutputSet e = next_set(&iA);

      if (e.item != NULO_SET.item)
      {
         // O elemento, para inserção, não pode está também contido em B.
         if (!pertence_set(b, e.item))
            adiciona_set(result, e.item);
      }
   } while (!consumido_iter_set(&iA));

   return result;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 *                      Testes Unitários 
 *
 * Testando todos métodos, funções, e dados abstratos acima. Deixando
 * bem referênciado esta parte, pois fica fácil descartar -- além de 
 * ser necessário se os tipos forem trocados, do contrário o programa
 * não compila; se copiado para vários projetos. Caso também esta parte
 * futuramente for colocada num subdiretório, e os tipos serem trocados
 * apenas comentar tal declaração pré-processada para não incluir o que
 * pode conflitar.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --*/
#ifdef _UT_CONJUNTO
#include <assert.h>
#include "teste.h"
#include "tempo.h"
#include "legivel.h"
#include "dados_testes.h"

size_t hash_byte (generico_t key, size_t cp) {
   uint8_t K = *((uint8_t*)key);
   return K % cp;
}

bool eq_byte(generico_t a, generico_t b) {
   return *((uint8_t*)a) == *((uint8_t*)b);
}

char* byte_to_str(generico_t v) {
   char* resultado_fmt = malloc(4 * sizeof(char));
   uint8_t* valor = (uint8_t*)v;
   sprintf(resultado_fmt, "%3u", *valor);
   return resultado_fmt;
}

void print_byte_set(Conjunto S) 
   { impressao_generica(S, byte_to_str); }

void insercoes_randomicas(conjunto_t* s, size_t qtd) {
// As inserções são apenas de bytes(inteiros positivos).
   // Alimentando semente para números randomicos.
   srand((size_t)&print_byte_set);

   for (size_t n = 1; n <= qtd; n++) {
      size_t size = sizeof(uint8_t);
      uint8_t* X = malloc(size);
      //*X = (uint8_t)inteiro_positivo(1, UINT8_MAX);
      *X = (uint8_t)(rand() % (UINT8_MAX) + 1);
      adiciona_set(s, X);
   }
}

static bool free_byte(generico_t x) 
   { free(x); return true; }

void operacoes_basicas_na_estrutura(void) {
   Conjunto S = cria_branco_set();
   adiciona_metodos_set(S, hash_byte, eq_byte);
   uint8_t input[] = { 22, 2, 89, 2, 7, 13, 7, 89, 0};

   puts("Primeiro lote de inserções: ");
   printf("Total de itens: %lu\n", tamanho_set(S));
   adiciona_set(S, &input[0]);
   adiciona_set(S, &input[1]);
   adiciona_set(S, &input[2]);
   printf("Total de itens: %lu\n", tamanho_set(S));
   // visualizacao_interna_set(S);
   assert (pertence_set(S, &input[3]));
   assert (!pertence_set(S, &input[4]));
   adiciona_set(S, &input[5]);
   adiciona_set(S, &input[6]);
   printf("Total de itens: %lu\n", tamanho_set(S));
   assert (pertence_set(S, &input[6]));
   // visualizacao_interna_set(S);

   // Colocando itens da heap, ou seja, é preciso elimina-las manualmente.
   insercoes_randomicas(S, 13);
   adiciona_set(S, &input[1]);
   print_byte_set(S);
   printf("Total de itens: %lu\n", tamanho_set(S));

   puts("Segundo lote de mudanças e verificação na estrutura:");
   size_t inicialmente = tamanho_set(S);
   assert (pertence_set(S, &input[7]));
   // Tentar remover inexistentes!
   assert(!deleta_set(S, &input[8]));
   assert(deleta_set(S, &input[7]));
   assert (!pertence_set(S, &input[7]));
   assert (tamanho_set(S) == (--inicialmente));
   // Tentar remover inexistentes!
   assert(!deleta_set(S, &input[7]));
   print_byte_set(S);

   // Removendo dados estáticos, para que a declaração abaixo funcione.
   for (size_t i = 1; i <= 6; i++)
      deleta_set(S, &input[i - 1]);
   // assert (destroi_set(S));
   destroi_interno_set(S, free_byte);
}

void rejeicao_de_entradas_duplicadas_em_massa(void) {
   Conjunto S = cria_com_capacidade_set(30, hash_byte, eq_byte);
   const size_t TOTAL = 30000;
   size_t rejeicoes = 0;

   print_byte_set(S);
   // Alimentando semente para números randomicos.
   srand((size_t)&print_byte_set);

   for (size_t i = 1; i <= TOTAL; i++) {
      uint8_t* X = malloc(sizeof(uint8_t));
      *X = rand() % UINT8_MAX;

      if (!adiciona_set(S, X)) 
         { rejeicoes++; free(X); }
   }

   printf(
      "total de aprovações|rejeições: %lu|%lu\n", 
      TOTAL - rejeicoes, rejeicoes
   );
   print_byte_set(S);

   assert ((TOTAL - rejeicoes) == tamanho_set(S));
   destroi_interno_set(S, free_byte);
}

size_t total_de_bytes(Conjunto S, size_t tipo_size) {
   size_t Q = tamanho_set(S);
   size_t C = S->capacidade;
   size_t t = tipo_size;
   // Tamanho dos tipos primitivos e abstratos:
   size_t nS = sizeof(nodulo_t);
   size_t npS = sizeof(nodulo_t*);
   size_t spS = sizeof(size_t*);


   /* Contabilizando os bytes manualmente, vamos lá. A estrutura 'nódulo'
    * tem 3 pointeiros; já array que segura a lista ligada tem 
    * 'a capacidade' de pointeiros. A estrutura ao todo, tem estas duas
    * estrutura dinâmicas, e outras estáticas, sendo tais últimas: dois
    * ponteiros, dois valores lógicos, e dois inteiros de 64-bits.
    */
   return sizeof(npS + nS)*C + Q*(nS + npS + t + spS) + (4 * 8 + 2);
   // return 8*C + Q*(3*8 + t) + (4*8 + 2);
}

bool eq_sizet(generico_t a, generico_t b) {
   return *((size_t*)a) == *((size_t*)b);
}

size_t hash_sizet (generico_t key, size_t cp) {
   size_t K = *((size_t*)key);
   return K % cp;
}

Temporizador timer;
bool instanciado = false;
int64_t insercoes_por_seg = 0;
double aumento_percentual = 0.0;
double fator_de_carga = 0.0;

// O endereço de memória da função de hashing como semente.
static size_t SEED = (size_t)&(hash_sizet);

size_t* int_sorteio(size_t a, size_t b) {
   srand(SEED);
   size_t* X = malloc(sizeof(size_t));
   *X = rand() % b + a;
   return X;
}

bool free_usize(generico_t x)
   { free(x); return true; }

size_t media_itens_por_slot(Conjunto S) {
   size_t C = S->capacidade;
   size_t Lc = C / 8, L = Lc;
   Conjunto I = cria_com_capacidade_set(L, hash_sizet, eq_sizet);
   nodulo_t** array = S->locais;
   size_t rol = 0;

   // Metada das listas ligadas serão tirada média de itens:
   do {
      // Sorteia um índice aleatóriamente dos possíveis.
      size_t* i = int_sorteio(0, C - 1);
      nodulo_t* atual = array[*i];

      // Iterar até sortear um índice novo ...
      while (!adiciona_set(I, i))
         i = int_sorteio(0, C - 1);

      while (atual != NULL) 
         { atual = atual->seta; rol++; }
   } while(--Lc > 0);

   #ifdef _MEDIA_DOS_SLOTS
   printf("média computada: %lu\n", rol / L);
   printf("total de índices: %lu\n", tamanho_set(I));
   #endif

   destroi_interno_set(I, free_usize);
   return rol / L;
}

struct pausa_args { TempoTipo type; uint16_t units; };

void info_controlada(Conjunto S, size_t size, size_t* rejeicoes, 
  struct pausa_args A
){
   if (!instanciado) {
      timer = cria_temporizador(A.type, A.units);
      instanciado = true;
   }

   if (esgotado(timer)) {
      // Inserções comparada a última.
      insercoes_por_seg = tamanho_set(S) - insercoes_por_seg;
      double ips = insercoes_por_seg;
      double ts = tamanho_set(S);
      double segs;
      aumento_percentual = (ips / ts) * 100.0;
      // Agora dividindo pelos segundos percorridos.
      switch (A.type) {
      case Segundo:
         insercoes_por_seg /= A.units;
         break;
      case Miliseg:
         // Convertendo para segundos primeiramente...
         segs = 0.001 * (double)A.units;
         insercoes_por_seg /= (int64_t)segs + 1;
         break;
      default:
         perror("Não implementado!");
         abort();
      };
      fator_de_carga = tamanho_set(S) / S->capacidade;

      char* qtd_itens_str = valor_legivel(tamanho_set(S));
      char* size_str = tamanho_legivel(total_de_bytes(S, size));
      char* addsup_per_str = valor_legivel(insercoes_por_seg);

      // Tela de monitoramento ...
      printf(
         "\nTamanho do 'conjunto' atualmente: %s\n"
         "Nº. de itens: %s\nInserções per seg: %s (%0.1lf%%)\n"
         "Fator de carga: %0.1lf\nItens por lista-interna: %lu\n", 
         size_str, qtd_itens_str, addsup_per_str, aumento_percentual,
         fator_de_carga, media_itens_por_slot(S)
      );

      free(qtd_itens_str); free(size_str); free(addsup_per_str);
      destroi_temporizador(timer);
      // Criando novo temporizador ...
      timer = cria_temporizador(A.type, A.units);
   }
}

static void insere_na_array_de_nodulos(ArrayNodulo array, generico_t dado,
  size_t capacidade, Hash posicao, Eq compara) 
{
/* Verifica se o 'container' da lista a ser usado já tem uma lista, se 
 * não tiver, o 'dado' será seu primeiro nódulo, se houver um/alguns, 
 * travessa a 'lista' verificando se já existe um nódulo como o atual 
 * item a ser inserido, se não houver, então adiciona no final dela. */
   size_t indice = posicao(dado, capacidade);
   // nodulo_t* entrada = S->locais[indice];
   nodulo_t* entrada = array[indice];

   if (entrada == INVALIDA) {
      /* se estiver vázia, facilita muito, apenas endereça espaço para uma
       * nova 'entrada' com valor e chave. */
      //S->locais[indice] = cria_nodulo (dado, NULL);
      entrada = cria_nodulo(dado, NULL);
   } else {
      /* Se não for inválida, percorrer lista até o final, enquanto que 
       * compara com cada item, para ver se tem alguma chave já registrada.
       */
      nodulo_t* nova = cria_nodulo (dado, NULL);
      result_t resultado = verifica_lista(entrada, dado, compara);

      if (!resultado.contido) {
         // Não constatar item contido, apenas adiciona no começo.
         nova->seta = array[indice];
         array[indice] = nova;
      }
   }
}

bool dobra_array_interna(Conjunto S) {
/* Quando 'o fator de carga' fica maior que dez, então os containers das
 * 'lista encadeadas', são dobrados. Só que para ficar igual, todos itens
 * nos respectivos índices tem quer ser removidos e reinseridos. Se ao ter
 * chamado tal função, houvesse redimensionamento interno, o retorno lógico
 * confirma tal proposição, do caso contrário, nega. */
   size_t t = S->quantidade;
   size_t C = S->capacidade;
   double fator_de_carga = (double)t / (double)C;

   if (fator_de_carga > 10.0) 
   {
      // Nova array de containers(locais aqui nomeado).
      size_t size = sizeof (nodulo_t*);
      size_t nC = 2 * C;
      nodulo_t** new_array = calloc(nC, size);
      nodulo_t** old_array = S->locais;
      Hash calculo = S->__hash__;
      Eq compara = S->__iguais__;

      // Travessando a array de listas, e copiando o dado interno.
      for (size_t p = 1; p <= C; p++) 
      {
         // Lista no índice p-nésimo.
         nodulo_t* lista = old_array[p - 1];
         // Primeiro 'nó' da lista, uma referência móvel.
         nodulo_t* X = lista;

         // Apenas percorre-a se for possível...
         while (X != NULL) {
            // Copiando chave do atual nódulo, e seguindo para próximo.
            generico_t dado_ref = X->chave;
            X = X->seta;

            insere_na_array_de_nodulos(
               new_array, dado_ref, nC,  
               calculo, compara
            ); 
         }
         destroi_toda_lista_ligada(lista, false);
      }

      /* Antes de tudo, destrói todos nódulos da antiga 'array de lista',
       * inclusive a própria array. O passo seguinte é, atualizar para
       * nova 'capacidade'. Então atribui a estrutura sua nova 'array de
       * listas'. Por fim, confirma que houve redimensionamento. */
      S->locais = new_array;
      S->capacidade = nC;
      return true;
   }
   // Nada aconteceu, então informando isso...
   return false;
}

void monitorando_propriedades_sobre_estresse(void) {
   const size_t TOTAL = 3000000;
   const size_t CP = 3000;
   Conjunto S = cria_com_capacidade_set(CP, hash_sizet, eq_sizet);
   size_t rejeicoes = 0;
   size_t size = sizeof(size_t);

   printf("Tamanho inicial, sem itens: %lu\n", total_de_bytes(S, size));

   for (size_t i = 1; i <= TOTAL; i++) {
      size_t* X = malloc(sizeof(size_t));
      *X = SIZE_MAX - i;

      assert (adiciona_set(S, X));
      struct pausa_args t = {Segundo, 3};
      info_controlada(S, size, &rejeicoes, t);
   }
   destroi_temporizador(timer);
   destroi_interno_set(S, free_usize);
}

void insercoes_stats_havendo_redimensionamento(void) {
   const size_t TOTAL = 9000000;
   const size_t CP = 500;
   Conjunto S = cria_com_capacidade_set(CP, hash_sizet, eq_sizet);
   size_t rejeicoes = 0;
   size_t size = sizeof(size_t);

   printf("Tamanho inicial, sem itens: %lu\n", total_de_bytes(S, size));
   for (size_t i = 1; i <= TOTAL; i++) {
      size_t* X = malloc(sizeof(size_t));
      *X = SIZE_MAX - i;

      assert (adiciona_set(S, X));
      dobra_array_interna(S);
      struct pausa_args t = {Miliseg, 500};
      info_controlada(S, size, &rejeicoes, t);
   }
   destroi_temporizador(timer);
   destroi_interno_set(S, free_usize);
}

void iteracao_simples_para_testar_a_implmentacao(void) {
   Conjunto S = cria_set(hash_byte, eq_byte); 

   for (size_t i = 0; i < VALORES_PADRONIZADOS; i++) {
      uint8_t* X = (uint8_t*)&valores_padronizados[i];
      generico_t Y = X;
      assert (adiciona_set(S, Y));
   }
   puts("Conjunto que será iterador:");
   impressao_generica(S, byte_to_str);
   IterSet I = cria_iter_set(S);
   printf("Itens à consumir: %lu\n", contagem_iter_set(&I));

   puts("\nComeçando a iteração...");
   IterOutputSet resultado;
   uint8_t* dado;

   puts("\n\tPrimeiro, iterações manuais...");
   resultado = next_set(&I);
   dado = resultado.item;
   printf("\t\tItem iterado: '%u'\n", *dado);
   resultado = next_set(&I);
   dado = resultado.item;
   printf("\t\tItem iterado: '%u'\n", *dado);
   resultado = next_set(&I);
   dado = resultado.item;
   printf("\t\tItem iterado: '%u'\n", *dado);
   assert (contagem_iter_set(&I) == 7);

   puts("\n\tEntão, iterações automáticas...");
   do {
      resultado = next_set(&I);
      dado = resultado.item;
      printf(
         "\t\tItem iterado: '%u', \tfaltam %lu itens\n", 
         *dado, contagem_iter_set(&I)
      );
   } while(!consumido_iter_set(&I));

   destroi_set(S);
}

size_t hash_string(generico_t s, size_t cp) {
   char* string = s;
   size_t t = strlen(s);
   size_t code = (size_t)string[t / 2];
   return (t + code) % cp;
} 

bool eq_string(generico_t a, generico_t b) {
   char* str_a = a;
   char* str_b = b;
   return strcmp(str_a, str_b) == 0;
}

char* conststring(generico_t a) {
   char* fonte = a;
   size_t t = strlen(fonte) + 1;
   char* saida = malloc(t * sizeof(char));

   strcpy(saida, fonte);
   return saida;
}

void simples_clonagem_inicial_de_iteradores(void) {
   Conjunto S = cria_set(hash_string, eq_string); 

   for (size_t i = 0; i < OBJETOS; i++) {
      char* s = (char*)objetos[i];
      generico_t K = s;
      assert (adiciona_set(S, K));
   }
   puts("Conjunto que será iterador:");
   impressao_generica(S, conststring);
   IterSet I = cria_iter_set(S);
   printf("Itens à consumir: %lu\n", contagem_iter_set(&I));
   IterSet H = clona_iter_set(&I);

   assert (contagem_iter_set(&I) == contagem_iter_set(&H));
   for (size_t i = 0; i < 4; i++) {
      IterOutputSet X = next_set(&I);
      char* dado = X.item;
      printf("\tIterado: '%s'\n", dado);
   }
   printf(
      "Contagem do original/clone: %lu/%lu\n",
      contagem_iter_set(&I), contagem_iter_set(&H)
   );
   assert (contagem_iter_set(&I) < contagem_iter_set(&H));

   char* dado_clone, *dado_original; size_t q, Q; 
   do {
      q = contagem_iter_set(&I);
      Q = contagem_iter_set(&H);
      IterOutputSet X = next_set(&I), Y = next_set(&H);
      dado_original = X.item;
      dado_clone = Y.item;

      if (!consumido_iter_set(&I) && !consumido_iter_set(&H))
         printf(
            "  Original(%lu) >>> '%s'\t\t\tClone(%lu) >>> '%s'\n", 
            q, dado_original, Q, dado_clone
         );
      else if (!consumido_iter_set(&I)) 
         printf("  Original(%lu) >>> '%s'\n", q, dado_original);
      else if (!consumido_iter_set(&H)) 
         printf("  Clone(%lu) >>> '%s'\n", Q, dado_clone);
   } while (!consumido_iter_set(&H) || !consumido_iter_set(&I));

   destroi_set(S);
}

static char* u16_to_str(generico_t dt) {
   char* fmt = malloc(15 * sizeof(char));
   uint16_t* pointer = dt;
   uint16_t valor = *pointer;

   memset(fmt, '\0', 15);
   sprintf(fmt, "%u", valor);
   return fmt;
}

static bool eq_u16(generico_t a, generico_t b) {
   uint16_t* ptr_a = a;
   uint16_t* ptr_b = b;
   return *ptr_a == *ptr_b;
}

static size_t hash_u16(generico_t a, size_t cp) {
   uint16_t* ptr = a;
   return *ptr % cp;
}

void demonstracao_simples_da_conversao_em_str(void) {
   Conjunto S = cria_set(hash_u16, eq_u16); 

   for (size_t i = 0; i < VALORES_PADRONIZADOS_I; i++) {
      uint16_t* X = (uint16_t*)&valores_padronizados_i[i];
      adiciona_set(S, X);
   }
   puts("Aqui como fica a formatação em string:");
   char* stringset = set_to_str(S, u16_to_str);
   puts(stringset);

   free(stringset);
   destroi_set(S);
}

void teste_primario_de_operacoes_de_conjuntos(void) 
{
   const int N = 10;
   uint16_t input_b[N] = { 1, 3, 6, 9, 12, 15, 18, 21, 24, 27};
   uint16_t input_a[N] = { 1, 2, 4, 6, 8, 12, 14, 16, 18, 20 };
   Conjunto B = cria_set(hash_u16, eq_u16); 
   Conjunto A = cria_set(hash_u16, eq_u16);
   uint16_t* X, *Y;
   ToString f = u16_to_str;

   for (size_t i = 0; i < N; i++) 
   {
      X = &input_b[i];
      Y = &input_a[i];
      adiciona_set(B, X);
      adiciona_set(A, Y);
   }

   puts("Amostras dos conjuntos B e A, respectivamente:");
   imprime_set(B, f);
   imprime_set(A, f);

   Conjunto C = uniao_set(B, A);
   Conjunto D = intersecao_set(A, B);
   Conjunto E = diferenca_set(A, B);

   puts(
      "\nResultados das iterseção, união e diferença, respectivamente, "
      "referentes aos conjuntos A e B:"
   );
   imprime_set(D, f);
   imprime_set(C, f);
   imprime_set(E, f);

   destroi_set(B); destroi_set(A); destroi_set(C); destroi_set(D);
   destroi_set(E);
}

void efetuando_operacoes_com_conjuntos_vazios(void)
{
   uint16_t input_a[] = { 1, 2, 4, 6, 8};
   Conjunto B = cria_set(hash_u16, eq_u16); 
   Conjunto A = cria_set(hash_u16, eq_u16);
   uint16_t *Y;
   ToString f = u16_to_str;
   int sz = sizeof(input_a) / sizeof(uint16_t);

   for (int i = 0; i < sz; i++) {
      Y = &input_a[i];
      adiciona_set(A, Y);
   }

   puts("Amostras dos conjuntos B e A, respectivamente:");
   imprime_set(B, f);
   imprime_set(A, f);

   Conjunto C = uniao_set(B, A);
   Conjunto D = intersecao_set(A, B);
   Conjunto E = diferenca_set(A, B);
   Conjunto F = diferenca_set(B, A);
   
   puts(
      "\n\t\tPrimeiro com um conjunto vázio e outro cheio:"
      "\nResultados das iterseção, união e diferença, respectivamente, "
      "referentes aos conjuntos A e B:"
   );
   imprime_set(D, f);
   imprime_set(C, f);
   imprime_set(E, f);
   imprime_set(F, f);

   destroi_set(C); destroi_set(D); destroi_set(E); destroi_set(F);

   limpa_set(A);
   puts("Amostras dos conjuntos B e A, respectivamente:");
   imprime_set(B, f);
   imprime_set(A, f);

   C = uniao_set(B, A);
   D = intersecao_set(A, B);
   E = diferenca_set(A, B);
   F = diferenca_set(B, A);
   
   puts(
      "\n\t\tSegundo com ambos conjuntos vázios:"
      "\nResultados das iterseção, união e diferença, respectivamente, "
      "referentes aos conjuntos A e B:"
   );
   imprime_set(D, f);
   imprime_set(C, f);
   imprime_set(E, f);
   imprime_set(F, f);

   destroi_set(B); destroi_set(A); destroi_set(C); destroi_set(D);
   destroi_set(E); destroi_set(F);
}

int main(int total, char* args[], char* vars[]) 
{
	executa_testes_a(
   /* Não está indentado para que mostre os testes de forma direta. O 
    * editor usado aqui sempre dobra, subtrechos identados quando abre. */
      false, 7, 
         operacoes_basicas_na_estrutura, true,
         rejeicao_de_entradas_duplicadas_em_massa, true,
         iteracao_simples_para_testar_a_implmentacao, true,
         simples_clonagem_inicial_de_iteradores, true,
         demonstracao_simples_da_conversao_em_str, true,
         // Desativados, pois consomem CPU e tempo:
         monitorando_propriedades_sobre_estresse, false,
         insercoes_stats_havendo_redimensionamento, false
	);

	executa_testes_a(
   /* Teste referentes apenas das implementações de iteradores: */
      false, 2, 
         iteracao_simples_para_testar_a_implmentacao, true,
         simples_clonagem_inicial_de_iteradores, true
	);

   executa_testes_a(
   /* Principalmente as operações de conjuntos em sí. As mais importantes
    * que são implementadas depois que toda estrutura principal foi 
    * construída. */
      true, 2,
         teste_primario_de_operacoes_de_conjuntos, true,
         efetuando_operacoes_com_conjuntos_vazios, true
   );

   return EXIT_SUCCESS;
}
#endif
