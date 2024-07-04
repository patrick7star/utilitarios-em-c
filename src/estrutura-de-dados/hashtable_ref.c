
/*   Tipos mais genéricos, então caso necessite mudar-los para outro 
 * programa, apenas troque aqui.
 * 
 *   HashTables com chaves genérica são basicamente impossível, não poderia
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

// biblioteca padrões em C:
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <assert.h>
// Declaração das estruturas, funções e métodos, abaixo:
#include "hashtable_ref.h"
// Apenas incluindo para os testes:
#if defined(_ATUALIZA_HD) || defined(_DELETA_HT) || defined (_OBTEM_HT)
#include "teste.h"
#endif

// todos apelidos dados:
typedef void* generico_t;
typedef struct nodulo_do_hash nodulo_t; 

// todas constantes:
#define INVALIDA NULL
// Eu preciso transformar isso table[key] = value, nisso: insere_ht(table, key, value), ou atualiza_ht(table, key, value)

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 *                Trecho do 'nódulo', embrulho que
 *                   transporta os 'itens'
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */
struct nodulo_do_hash { 
   // valores genéricos tanto da chave como do valor:
   generico_t chave; 
   generico_t valor;

   // referência para próximo item.
   nodulo_t* seta;
};

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

static void destroi_toda_lista_ligada (nodulo_t* lista) {
   if (lista != NULL) {
      // obtendo primeiro item, antes que ele possa ser "perdido".
      nodulo_t* remocao = lista;
      lista = lista->seta;

      // resetando referências internas...
      remocao->chave = NULL;
      remocao->valor = NULL;

      free (remocao);
      // indo para remoção do outro nódulo ...
      destroi_toda_lista_ligada (lista);
   }
}

#ifdef _ALLOW_DEAD_CODE
static bool destroi_nodulo (nodulo_t* item) {
   if (item != INVALIDA) {
      // apenas some com as referências primeiramente...
      item->chave = NULL;
      destroi_toda_lista_ligada (item->seta);
      free (item);
      // confirma destruição.
      return true;
   } 
   return false;
}
#endif

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
   size_t (*__hash__)(generico_t, size_t);
   bool (*__iguais__) (generico_t, generico_t);
   /* É possível passar depois uma 'função hash e de igualdade', assim 
    * como outros métodos. */
   bool __eq__confirmada;
   bool __hash__confirmada;
};

bool adiciona_metodos ( HashTable m, Hash hash, Eq eq) {
   if (m == INVALIDA)
      return false;

   if (m->__hash__confirmada || m->__eq__confirmada) {
      perror ("não é possível aplicar isso, pois já foram anteriormente.");
      abort();
   }
   
   // coloca ambos métodos e marca como positivo paras operações.
   if (hash != INVALIDA) {
      m->__hash__ = hash;
      m->__hash__confirmada = true;
   }
   if (eq != INVALIDA) {
      m->__iguais__ = eq;
      m->__eq__confirmada = true;
   }
   return m->__hash__confirmada && m->__eq__confirmada;
}

HashTable cria_com_capacidade_ht(size_t capacidade, Hash f, Eq g){
   HashTable mapa = malloc(sizeof(hashtable_t));
   size_t Q = capacidade;
   const size_t SIZE_REF = sizeof (nodulo_t*);

   if (mapa != NULL) {
      #ifdef _CRIACAO_HT
      puts ("'tabela' alocada com sucesso.");
      #endif 

      // alocando a array de listas e registrando seu tamanho...
      mapa->locais = malloc (Q * SIZE_REF);
      for (size_t k = 1; k <= Q; k++)
         mapa->locais[k - 1] = NULL;
      mapa->capacidade = Q;

      // sem elementos inicialmente, por motivos óbvios.
      mapa->quantidade = 0;

      // referênciando funções que farão o cálculo hash internamente.
      if (f != NULL) {
         mapa->__hash__ = f;
         // informando a instância tem tais métodos essenciais...
         mapa->__hash__confirmada = true;
      } else
         mapa->__hash__confirmada = false;

      if (g != NULL) {
         mapa->__iguais__ = g;
         mapa->__eq__confirmada = true;
      } else
         mapa->__eq__confirmada = false;

      #ifdef _CRIACAO_HT
      assert (f != INVALIDA && g != INVALIDA);
      puts ("ambas funções passadas são válidas.");
      #endif 
   }
   return mapa;
}

HashTable cria_ht (Hash f, Eq g) 
// Criação vázia da 'hashtable', com uma capacidade inicial.
   { return cria_com_capacidade_ht (20, f, g); }

HashTable cria_branco_ht (void) 
/* Cria a mesma instância que acima, porém sem as instâncias necessárias
 * nele a priori. Elas são necessárias para fazer várias operações, 
 * porém para apenas instânciar, não. */
   { return cria_com_capacidade_ht (30, NULL, NULL); }

bool destroi_ht(HashTable m) { 
   if (m == INVALIDA) return false; 

   size_t Q = m->capacidade, i = 1;
   for (; i < Q; i++) {
      nodulo_t* lista = m->locais[i - 1];
      destroi_toda_lista_ligada (lista);
   }
   // 8463
   free (m);

   #ifdef _DESTRUICAO_HT 
   printf ("todas %lu lista internas foram destruídas.\n", i);
   puts ("a 'tabela' foi desalocada com sucesso.");
   #endif
   // confirma que tudo está liberado.
   return true;
}

// O resultado da função abaixo:
typedef struct { bool contido; size_t posicao; nodulo_t* item; } result_t;

static result_t verifica_lista (
  // lista ligada que será pecorrida e comparada.
  nodulo_t* lista, 
  // referência que será comparada com equivalente na lista.
  generico_t chave, 
  // função que verifica igualdade entre dois 'genéricos' acimas.
  bool (*funcao_eq)(generico_t, generico_t)
) {
   /* Vários algoritmos abaixo, necessitam uma hora varrer a lista ligada
    * interna, obtem o item específicos(compararação) e tal. Tal função 
    * auxiliar terceira para toda cadeia abaixo, tal rotina. Retorna-se 
    * então se há tal, a posição na lista-encadeada, e o item pertecente,
    * mesmo que as funções abaixo não precisem, todas, de todos estes 
    * componentes.
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

      if (funcao_eq (key, chave))
         // verifica se achou um item correspondente..
         return (result_t){ true, indice, atual };

      // indo para próximo item...
      atual = atual->seta;
   } while (atual != NULL);

   // se chegou até aqui, e não encontrou nada, então é isso aí, nada!
   return NEGACAO_PADRAO;
}

bool insere_ht(HashTable m, generico_t ch, generico_t v) {
/* computa posição na array baseada na chave e capacidade. A função
 * hash, por desenho da estrutura, já vem embutida. 
 */
   size_t posicao = m->__hash__ (ch, m->capacidade);
   nodulo_t* entrada = m->locais[posicao];

   if (entrada == INVALIDA) {
      /* se estiver vázia, facilita muito, apenas endereça espaço para uma
       * nova 'entrada' com valor e chave. */
      m->locais[posicao] = cria_nodulo (ch, v);
      // contabiliza inserção.
      m->quantidade += 1;

      #ifdef _INSERCAO_HT
      puts (
         "container vázio, portanto o item a ser inserído será "
         "o primeiro da lista, ou a própria lista"
      );
      #endif

      // confirma inserção como feita, se chegar até aqui.
      return true;
   } else {
      /* Se não for inválida, percorrer lista até o final, enquanto que 
       * compara com cada item, para ver se tem alguma chave já registrada.
       */
      bool (*funcao)(generico_t, generico_t) = m->__iguais__;
      nodulo_t* nova = cria_nodulo (ch, v);
      result_t resultado = verifica_lista (m->locais[posicao], ch, funcao);
      if (resultado.contido)
         return false;
      else {
         nova->seta = m->locais[posicao];
         m->locais[posicao] = nova;
         // contabiliza inserção.
         m->quantidade += 1;
         return true;
      }
   }
}

bool contem_ht(HashTable m,  generico_t ch) { 
/* verifica se tal chave está contida na tabela de dispersão. Chama 
 * função que da uma varrida na lista interna procurando pelo primeiro
 * item correspondente, manda o resultado e muito mais, porém apenas
 * estamos interessado na condição de "pertencimento" retornada. */

   /* computa posição na array baseada na chave e capacidade. A função
    * hash, por desenho da estrutura, já vem embutida. 
    */
   size_t posicao = m->__hash__ (ch, m->capacidade);
   nodulo_t* list = m->locais[posicao];
   // bool (*funcao)(generico_t, generico_t) = m->__iguais__;

   #ifdef _CONTIDO_HT
   if (list == INVALIDA) 
      puts ("container indexado está vázio.");
   #endif

   result_t resultado = verifica_lista (list, ch, m->__iguais__);
   return resultado.contido;
}

bool atualiza_ht(HashTable m,  generico_t ch,  generico_t nv) { 
/* acha uma chave e troca o valor dela, o retorno é bem sucedido se 
 * a atualização foi definitivamente feita, e falho caso o contrário,
 * que seria se por exemplo, se não houvesse a chave demanda para
 * atualização. */
   size_t posicao = m->__hash__ (ch, m->capacidade);

   #ifdef _ATUALIZA_HD
   printf ("hash: %lu\n", posicao);
   #endif

   result_t outcome = verifica_lista (m->locais[posicao], ch, m->__iguais__);
   nodulo_t* item = outcome.item;
   if (item != NULL)
      item->valor = nv;

   #ifdef _ATUALIZA_HD
   printf (
      "resultados: (%s, %luª, LISTA)\n",
      bool_to_str (outcome.contido),
      outcome.posicao
   );
   #endif

   return outcome.contido; 
}

bool deleta_ht(HashTable m, generico_t ch) { 
/* A operação de remoção é apenas usar o método de atualização, para
 * inserir uma chave-em-branco no lugar da 'chave' dada, o resto é 
 * apenas realinhar das 'entradas' na array, tipo chaves-em-branco
 * no fim dela, 'entradas' não vázia no começo, não importando a ordem.
 */
   size_t posicao = m->__hash__ (ch, m->capacidade);
   result_t outcome = verifica_lista (
      m->locais[posicao], 
      ch, m->__iguais__
   );
   nodulo_t* item = outcome.item;
   bool a_chave_existe = outcome.contido;

   // apenas faz a remoção, se e somente se, houver tal 'chave'.
   if (a_chave_existe) {
      nodulo_t *atual = m->locais[posicao];

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
         m->locais[posicao] = atual->seta;

      // descontabiliza item removido.
      m->quantidade -= 1;
      /* libera item(sucetível a erro), possivelmente comentado pois parte
       * causa muitos problemas se não feito de maneira certa. */
      // destroi_nodulo (atual);
   }
   return a_chave_existe;
}

bool vazia_ht(HashTable m) 
   /* Operações, principalmente de encapsulamento, extremamente triviais
    * mais necessárias para esconder, futurumente quando este arquivo
    * fazer parte da 'lib' acessar tais valores da estrutura. */
   { return m->quantidade == 0; }

size_t tamanho_ht(HashTable m) 
// Entrega o contador de itens iterno da estrutura.
   { return m->quantidade; }

generico_t obtem_ht(HashTable m,  generico_t ch) {
/* Basicamente, a função que verifca é está aqui, porém retorna muito
 * mais que o atual nódulo com 'data', aqui só pegamos o importante
 * e o retornamos. */
   size_t posicao = m->__hash__ (ch, m->capacidade);
   result_t outcome = verifica_lista (
      // lista encadeada na determinada posição.
      m->locais[posicao], 
      ch, m->__iguais__
   );

   #ifdef _OBTEM_HT
   printf (
      "posição na array: %lu\nestá contido? %s\n", 
      posicao, bool_to_str (outcome.contido)
   );
   // printf ("valor = '%s'.\n", (char*)outcome.item->valor);
   if (outcome.item != NULL)
      puts ("parece que o itém não é inválido.");
   else
      puts ("item é inválido!");
   #endif

   if (outcome.contido)
      return outcome.item->valor;
   return NULL;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 *                      Iterador: funções e métodos
 *                         relacionados a tal
 *
 *   Esta parte é totalmente referente ao 'iterador' da estrutura, é uma 
 * parte dela, porém para melhor organização e visualização futura ficará
 * este separador entre eles. Assim fica de fácil localização, e os métodos
 * de cada um não serão confundidos.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */
struct iteracao_da_hashtable {
   // contador de itens iterados.
   size_t contagem;
   // posição atual na array.
   size_t cursor;

   // referência ao atual item da 'tabela' referenciado.
   nodulo_t* atual;

   // garantidor de que a 'tabela' não foi alterada.
   size_t inicialmente;
   HashTable tabela;
};

// cédula em branco para indicar termino da iteração ou invalidação.
const IterOutputHT NULO_HT = (IterOutputHT) {NULL, NULL};

IterHT cria_iter_ht (HashTable m) {
   IterHT instancia;

   instancia.contagem = 0;
   instancia.cursor = 0;
   instancia.atual = m->locais[0];
   instancia.inicialmente = tamanho_ht (m);
   // referência ao próprio mapa, dicio, table,... como quiser chamar.
   instancia.tabela = m;

   return instancia;
}

static bool iterador_valido (IteradorHT iter) {
   /* Para realizar qualquer uma das operações abaixo, é necessário
    * que a instância seja válida, ou seja, ainda existe, ou tem o mesmo
    * tamanho que na criação da instância. */
   size_t T = tamanho_ht(iter->tabela);
   bool referencia_existe = (iter->tabela != NULL);
   return (iter->inicialmente == T && referencia_existe);
}

size_t tamanho_iter_ht (IteradorHT iter) {
   /* O restante de iterações é calculado na seguinte forma:
    * total de itens na "lista" menos os já iterados. */
   if (iterador_valido (iter))
      return iter->inicialmente - iter->contagem;

   // se chegar até aqui é erro na certa.
   perror ("não é possível determinar o tamanho de um iterador inválido!");
   abort();
}

IterOutputHT next_ht (IteradorHT iter) {
   /* Casos que não retornar qualquer valor válido. */
   if (!iterador_valido(iter))
      return NULO_HT;
   else if (tamanho_iter_ht (iter) == 0)
      return NULO_HT;

   nodulo_t* atual = iter->atual;
   if (atual != INVALIDA) {
      // colhendo dados necessários ...
      generico_t vl = atual->valor;
      generico_t ch = atual->chave;

      // movendo pela lista ...
      iter->atual = iter->atual->seta;
      // contabiliza iteração.
      iter->contagem++;

      // retorna item "cholido".
      return (IterOutputHT){.key = ch, .value=vl };
   } else  {
      iter->cursor += 1;
      /* primeiro 'nódulo' da lista abaixo. */
      iter->atual = iter->tabela->locais[iter->cursor];
      // chama a função recursivamente novamente...
      return next_ht (iter);
   }
   // se chegar até aqui é o fim da iteração, se houve alguma. Neste caso
   // será retornado uma tupla com endereço inválidos.
   return NULO_HT;
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
#ifdef _UT_HASHTABLE
// bibliotecas:
#include "teste.h"
#include <assert.h>
#include <locale.h>
#include "dados_testes.h"
// Constante que comfirma igualdade entre strings.
#define STR_IGUAIS 0

void varias_entradas_genericas_diferentes (void) {
   puts ("criando simples instância de entry ...");

   // chaves das entradas de todos tipos:
   wchar_t* string_chave = L"minha_chave";
   uint32_t inteiro_chave = 12;
   unsigned char char_chave = 'F';
   float decimal_chave = 2.73f;
   // valores da entrada:
   float decimal_valor = 3.14159;
   bool logico_valor = false;
   wchar_t* string_valor = L"feminino";
   int32_t inteiro_valor = -1000000;

   nodulo_t e = { string_chave, &decimal_valor };
   nodulo_t a = { &inteiro_chave, &logico_valor };
   nodulo_t b = { &char_chave, string_valor };
   nodulo_t c = { &decimal_chave, &inteiro_valor };

   puts ("imprimindo para confirmar:");
   printf (
      "#===>%12ls: %f\n\n", 
      (wchar_t*)e.chave, 
      *((float*)e.valor)
   );
   printf (
      "#===>%3u: %s\n\n", 
      *((uint32_t*)a.chave), 
      bool_to_str(*((bool*)a.valor))
   );
   printf (
      "#===>%2c: %ls\n\n", 
      *((unsigned char*)b.chave), 
      (wchar_t*)b.valor
   );

   printf (
      "#===>%9f: %i\n\n",
      *((float*)c.chave),
      *((int32_t*)c.valor)
   );
}

size_t hash_string (generico_t key, size_t cP) { 
   wchar_t* k = key;
   size_t T = wcslen (key);
   size_t code = (uint8_t)k[T / 2] * (uint8_t)k[0] * (uint8_t)k[T - 1];
    size_t mA = (size_t)k;
   return (T * code * mA) % cP; 
   // return (T * code) % cP; 
} 

bool iguais_string (generico_t a, generico_t b) { 
   return wcscmp ((wchar_t*)a, (wchar_t*)b) == STR_IGUAIS; 
}

void alocao_e_desacalocao_simples_instancia (void) {
   HashTable mapa = cria_ht (hash_string, iguais_string);
   destroi_ht (mapa);
}

void visualiza_interna (HashTable m) {
   puts ("\nHashTable visualização interna:");
   size_t C = m->capacidade;

   for (size_t i = 1; i <= C; i++) {
      nodulo_t* lista = m->locais[i - 1];
      if (lista == INVALIDA)
         printf ("\t---\n");
      else {
         nodulo_t* atual = lista;
         printf ("\t<");
         do {
            float* vl = atual->valor;
            wchar_t* key = atual->chave;
            printf ("'%ls': %0.3f, ", key, *vl);
            atual = atual->seta;
         } while (atual != NULL);
         puts ("\b\b>");
      }
   }
}

struct chave_valor { wchar_t* key; float value; };

struct chave_valor entradas[] = {
   {L"laranja", 1.59}, {L"uva", 3.85},
   {L"pêssego", 5.99}, {L"melância", 10.15},
   {L"banana", 4.31}, {L"abacate", 0.80},
   {L"maçã", 2.50}
};

void aplicacao_de_simples_insercoes (void) {
   HashTable mapa = cria_ht (hash_string, iguais_string);

   assert (vazia_ht (mapa));
   for (size_t i = 1; i <= 7; i++) {
      float* ptr_value = &entradas[i - 1].value;
      wchar_t* ptr_key = entradas[i - 1].key;
      assert (insere_ht (mapa, ptr_key, ptr_value));
   }
   assert (tamanho_ht (mapa) == 7);
   visualiza_interna (mapa);

   destroi_ht (mapa);
}

void verifica_operacao_de_pertencimento (void) {
   HashTable mapa = cria_ht (hash_string, iguais_string);

   for (size_t i = 1; i <= 7; i++) {
      float* ptr_value = &entradas[i - 1].value;
      wchar_t* ptr_key = entradas[i - 1].key;
      assert (insere_ht (mapa, ptr_key, ptr_value));
   }
   visualiza_interna (mapa);

   uint8_t indices[] = { 5, 1, 4, 6, 3, 2 };

   for (size_t i = 1; i <= 7; i++) {
      size_t p = indices[i - 1];
      wchar_t* ptr_key = entradas[i - 1].key;
      printf ("atual chave '%ls' ...", ptr_key);
      assert (contem_ht (mapa, ptr_key));
      puts ("tem.");
   }

   destroi_ht (mapa);
}

void ascii_code_de_wide_strings (void) {
   wchar_t string[] = L"ármario";
   for (size_t i = 1; i <= 7; i++) 
      printf ("'%lc' -- %u\n", string[i - 1], (uint32_t)*(string + i - 1));
}

void simples_atualizacoes_de_alguns_valores (void) {
   HashTable mapa = cria_ht (hash_string, iguais_string);

   for (size_t i = 1; i <= 7; i++) {
      float* ptr_value = &entradas[i - 1].value;
      wchar_t* ptr_key = entradas[i - 1].key;
      assert (insere_ht (mapa, ptr_key, ptr_value));
   }
   size_t total = tamanho_ht (mapa);
   visualiza_interna (mapa);

   puts ("dobrando os preços...");
   for (size_t i = 1; i <= 7; i++) {
      size_t p = i - 1;
      struct chave_valor E = entradas[p];
      wchar_t* key = E.key;
      float* value = malloc (sizeof (float));
      *value = 2 * E.value;

      assert (atualiza_ht (mapa, key, value));
      puts ("atualização ocorreu corretamente!");
   }
   visualiza_interna (mapa);

   // a quantia interna não mudou.
   assert (tamanho_ht (mapa) == total);
   destroi_ht (mapa);
}

void visualiza_mapa_wchar_e_float (HashTable m) {
   size_t cP = m->capacidade;

   printf ("{..");
   for (size_t p = 1; p <= cP; p++) {
      nodulo_t* lista = m->locais[p - 1];

      while (lista != NULL) {
         wchar_t* k = lista->chave;
         float* v = lista->valor;

         printf ("%ls: %2.1f, ", k, *v);

         lista = lista->seta;
      }
   }
   puts ("\b\b}");
}

void algumas_remocoes_feitas (void) {
   HashTable mapa = cria_ht (hash_string, iguais_string);

   for (size_t i = 1; i <= 7; i++) {
      float* ptr_value = &entradas[i - 1].value;
      wchar_t* ptr_key = entradas[i - 1].key;
      assert (insere_ht (mapa, ptr_key, ptr_value));
   }
   puts ("\n .. .. .....antes .. .. .. ......");
   visualiza_interna (mapa);

   size_t antes = tamanho_ht (mapa);
   wchar_t* key = entradas[3].key;
   printf ("removendo a chave: '%ls'\n", key);
   assert (deleta_ht (mapa, key));
   assert (antes > tamanho_ht(mapa));
   visualiza_interna (mapa);

   /* Observação: Está com dificuldade de remover itens na primeira
    * casa da lista encadeada. */
   puts ("\n .. .. ... depois .. .. .. ......");
   antes = tamanho_ht (mapa);
   key = entradas[6].key;
   printf ("removendo a chave: '%ls'\n", key);
   assert (deleta_ht (mapa, key));
   assert (antes > tamanho_ht(mapa));
   visualiza_interna (mapa);

   antes = tamanho_ht (mapa);
   key = entradas[4].key;
   assert (deleta_ht (mapa, key));
   visualiza_mapa_wchar_e_float (mapa);
   key = entradas[1].key;
   assert (deleta_ht (mapa, key));
   visualiza_mapa_wchar_e_float (mapa);
   key = entradas[2].key;
   assert (deleta_ht (mapa, key));
   visualiza_mapa_wchar_e_float (mapa);
   assert (antes == 3 + tamanho_ht(mapa));

   destroi_ht (mapa);
}

size_t hash_int (generico_t dt, size_t cp) {
   uint16_t* ptr = dt;
   uint16_t chave = *ptr;
   // este não leva em conta o endereço virtual de memória do argumento.
   return  chave * (chave - chave / 2) % cp;
}

bool int_eq (generico_t a, generico_t b) 
   { return *((uint16_t*)a) == *((uint16_t*)b); }

void visualizacao_mapa_u16_e_str (HashTable m) {
   size_t cP = m->capacidade;

   printf ("{..");
   for (size_t p = 1; p <= cP; p++) {
      nodulo_t* lista = m->locais[p - 1];

      while (lista != NULL) {
         uint16_t* k = lista->chave;
         char* v = lista->valor;

         printf ("%u: '%s', ", *k, v);
         lista = lista->seta;
      }
   }
   puts ("\b\b}");
}

void operacoes_negadas (void) {
   uint16_t* amostras = (uint16_t*)valores_padronizados_i;
   // inserer, resgatar, e remover até não poder mais...
   HashTable M = cria_ht(hash_int, int_eq);
   assert (vazia_ht(M));

   for (size_t p = 1; p <= 8; p++) {
      uint16_t* key = (uint16_t*)&amostras[p - 1]; 
      char* value = (char*)legumes[p - 1];
      // insere_ht (M, &amostras[p - 1], legumes[p - 1]);
      insere_ht (M, key, value);
   }

   visualizacao_mapa_u16_e_str (M);
   assert (tamanho_ht(M) == 8);

   puts ("novo lotes de inserções, com mesmas chaves negados:");
   for (size_t p = 1; p <= 8; p++) {
      char* value = (char*)frutas[p - 1];
      uint16_t* key =  &amostras[p - 1];
      insere_ht (M, key, value);
   }
   assert (tamanho_ht(M) == 8);

   puts ("agora, apesar de negações, inserindo algumas...");
   size_t negacoes = 0;
   for (size_t p = 5; p <= 13; p++) {
      uint16_t* key =  amostras + p;
      char* vl = (char*)frutas[p - 5];
      // bool foi_inserido = insere_ht (M, &amostras[p], frutas[p - 5]);
      bool foi_inserido = insere_ht (M, key, vl);
      if (!foi_inserido)
         negacoes++;
   }
   printf ("houves %lu negações de inserção.\n",negacoes);
   printf ("há agora %lu items.\n", tamanho_ht (M));
   visualizacao_mapa_u16_e_str (M);

    
   // tentando obter chaves inexistentes ...
   uint16_t chaves_inexistentes[] = {
      918, 9,  380,  88, 832, 
      111, 22, 44, 100, 56, 3
   };

   puts ("\ntestando método 'obter(get)' valores de algumas 'chaves'.");
   for (size_t p = 1; p <= 10; p++) {
      size_t q = p - 1;
      uint16_t* key = &chaves_inexistentes[q];
      char* vl = obtem_ht (M, key);

      if (vl != NULL)
         printf ("chave %u existe, e tem valor '%s'.\n", *key, vl);
      else 
         printf ("chave %u não existe!\n", *key);
   }

   // tentando deletar todas chaves, e muito mais.
   negacoes = 0;
   puts ("\nlimpando a 'tabela'...");
   for (size_t p = 36; p >= 1; p--) {
      uint16_t* key = &amostras[p - 1];
      bool removido = deleta_ht (M, key);

      if (removido)
         visualizacao_mapa_u16_e_str (M);
      else {
         printf ("não foi possível remover %u.\n", *key);
         negacoes++;
      }
   }
   printf ("%lu foram negadas.\n", negacoes);

   destroi_ht (M);
}

void metodo_get_verificacao_basica (void) {
   uint16_t* amostras = (uint16_t*)valores_padronizados_i;
   // inserer, resgatar, e remover até não poder mais...
   HashTable M = cria_ht(hash_int, int_eq);
   assert (vazia_ht(M));

   for (size_t p = 1; p <= 8; p++) {
      uint16_t* key = (uint16_t*)&amostras[p - 1]; 
      char* value = (char*)legumes[p - 1];
      insere_ht (M, key, value);
      // insere_ht (M, &amostras[p - 1], legumes[p - 1]);
   }

   visualizacao_mapa_u16_e_str (M);
   assert (tamanho_ht(M) == 8);

   uint16_t chave = 1;
   char* valor_i = obtem_ht (M, &chave);
   chave = 9;
   char* valor_ii = obtem_ht (M, &chave);

   printf ("valores pegos: '%s' e '%s'\n", valor_i, valor_ii);

   // tentando acessar valores inválidos ...
   chave = 37;
   assert (obtem_ht (M, &chave) == NULL);
   chave = 43;
   assert (obtem_ht (M, &chave) == NULL);
   char* valor_iii; 
   chave = 99;
   valor_iii = obtem_ht (M, &chave);
   assert ( valor_iii != NULL);
   printf ("último pego(key=%u): '%s'\n", chave, valor_iii);
   chave = 98;
   assert (obtem_ht (M, &chave) == NULL);
   puts ("chaves inválidas produziram um valor 'null'.");

   destroi_ht (M);
}

// ---...---...---...---... Testes dos iteradores ---...---...---...---...
void print_item_ht_u16_e_str (IterOutputHT x) {
   printf (" ==> %u: %s\n", *((uint16_t*)x.key), (char*)x.value);
}

void print_inner_u16_e_str (HashTable m) {
   puts ("\nHashTable visualização interna:");
   size_t C = m->capacidade;

   for (size_t i = 1; i <= C; i++) {
      nodulo_t* lista = m->locais[i - 1];
      if (lista == INVALIDA)
         printf ("\t---\n");
      else {
         nodulo_t* atual = lista;
         printf ("\t<");
         do {
            char* vl = atual->valor;
            uint16_t* key = atual->chave;
            printf ("%u: '%s', ", *key, vl);
            atual = atual->seta;
         } while (atual != NULL);
         puts ("\b\b>");
      }
   }
}

void uso_simples_da_iteracao (void) {
   HashTable M = cria_ht(hash_int, int_eq);
   uint16_t* amostras = (uint16_t*)valores_padronizados_i;

   for (size_t p = 1; p <= 9; p++) {
      // insere_ht (M, &amostras[p - 1], legumes[p - 1]);
      uint16_t* key = (uint16_t*)&amostras[p - 1]; 
      char* value = (char*)legumes[p - 1];
      insere_ht (M, key, value);
   }
   for (size_t p = 9; p <= 19; p++) {
      // insere_ht (M, &amostras[p - 1], boys_names[p - 9 - 1]);
      uint16_t* key = (uint16_t*)&amostras[p - 1]; 
      char* value = (char*)boys_names[p - 9 - 1];
      insere_ht (M, key, value);
   }
   visualizacao_mapa_u16_e_str (M);
   print_inner_u16_e_str (M);

   IterHT I = cria_iter_ht (M);
   printf ("contagem em %lu ...\n", tamanho_iter_ht(&I));

   for (size_t count = tamanho_iter_ht(&I); count > 0; count--) {
      IterOutputHT i = next_ht (&I);
      assert (i.key != NULL && i.value != NULL);
      print_item_ht_u16_e_str (i);
      printf ("contagem em %lu ...\n", tamanho_iter_ht(&I));
   }

   printf ("tetando iterar mesmo esgotado ...");
   IterOutputHT i = next_ht (&I);
   assert (i.key == NULL && i.value == NULL);
   i = next_ht (&I);
   assert (i.key == NULL && i.value == NULL);
   i = next_ht (&I);
   assert (i.key == NULL && i.value == NULL);
   puts ("não funcionou!");
   destroi_ht (M);
}

void tentando_iterador_mapa_vazio (void) {
   HashTable M = cria_ht(hash_int, int_eq);
   IterHT I = cria_iter_ht (M);

   IterOutputHT i = next_ht (&I);
   printf ("contagem em %lu ...\n", tamanho_iter_ht(&I));
   assert (i.key == NULL && i.value == NULL);

   i = next_ht (&I);
   printf ("contagem em %lu ...\n", tamanho_iter_ht(&I));
   assert (i.key == NULL && i.value == NULL);

   i = next_ht (&I);
   printf ("contagem em %lu ...\n", tamanho_iter_ht(&I));
   assert (i.key == NULL && i.value == NULL);

   puts ("não funcionou com nenhuma!");
   destroi_ht (M);
}

typedef struct trechos { const char* inicio; uint8_t tamanho; } Trecho;

#define STRINGFY(...) #__VA_ARGS__

void filtra(const char* todos_argumentos) {
   Trecho lista[5];
   char SEP = ',';
   char* ptr = (char*)todos_argumentos;
   uint16_t contador = 0;

   puts("visualizando ...");

   __loop__ {
      char* ptr_v = strchr(ptr, SEP);

      // Abandona o laço se nenhum pointeiro for encontrado.
      if (ptr_v == NULL) {
         break; 
      }

      printf("%s\n", ptr);
      size_t X = ptr_v - ptr;
      lista[contador] = (Trecho){.inicio = ptr_v, .tamanho = X };
      ptr += (X + 1);
      contador++;
   } 
   // size_t X = strlen(todos_argumentos) - ptr;
   size_t X = strlen(todos_argumentos) - strlen(ptr);
   lista[contador++] = (Trecho){.inicio = ptr, .tamanho = X};

   puts("Iterando trechos: ...");
   for (size_t p = 1; p <= 5; p++) 
   {
      printf("%lu. '%s' (%u)\n", 
            p, lista[p - 1].inicio, lista[p - 1].tamanho);
   }
}

void macro_de_insercao_especifico(void) 
{
   filtra(STRINGFY(
      "dado": 5, "cestos": 80, "copos": 500,
      "telas": 17, "latas": 158,
   ));
}

void main(void) {
   setlocale (LC_CTYPE, "en_US.UTF-8");
   executa_testes (
      10, varias_entradas_genericas_diferentes, true,
         alocao_e_desacalocao_simples_instancia, true,
         aplicacao_de_simples_insercoes, true,
         verifica_operacao_de_pertencimento, true,
         // verificação de features do C.
         ascii_code_de_wide_strings, true,
         simples_atualizacoes_de_alguns_valores, true,
         algumas_remocoes_feitas, true,
         operacoes_negadas, true, 
         metodo_get_verificacao_basica, true,
         // Testando instânciação via macros:
         macro_de_insercao_especifico, false
   );

   // testes apenas do iteradores.
   executa_testes (
      2, uso_simples_da_iteracao, false,
      tentando_iterador_mapa_vazio, false
   );
}
#endif

