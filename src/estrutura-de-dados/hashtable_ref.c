
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

// Declaração das estruturas, funções e métodos, abaixo:
#include "hashtable_ref.h"
// biblioteca padrões em C:
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <assert.h>

// todas constantes:
#define INVALIDA NULL

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 *                Trecho do 'nódulo', embrulho que
 *                   transporta os 'itens'
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --*/
#include "hashtable/nodulo.c"

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
   size_t (*__hash__)(generico_t, size_t);
   bool (*__iguais__) (generico_t, generico_t);
   /* É possível passar depois uma 'função hash e de igualdade', assim 
    * como outros métodos. */
   bool __eq__confirmada;
   bool __hash__confirmada;
};

// O resultado da função abaixo:
typedef struct { bool contido; size_t posicao; nodulo_t* item; } result_t;

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

HashTable cria_com_capacidade_ht(size_t capacidade, Hash f, Eq g)
{
/* Também serve de método genéricos para demais construturoes abaixo. Estes
 * que recebem bem menos parâmetros, alguns até nenhum. 
 */
   const size_t size = sizeof(struct tabela_de_dispersao);
   HashTable mapa = malloc(size);
   size_t Q = capacidade;

   if (mapa != NULL) {
      mapa->locais = array_nodulo(Q);
      mapa->capacidade = Q;
      // Sem elementos inicialmente, por motivos óbvios.
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
   const int CAPACIDADE = (*m).capacidade;
   Node* array = (*m).locais, remocao = NULL;
   int n;

   for (n = 0; n < CAPACIDADE; n++)
   {
      // Remove o primeiro item até acabar.
      if (array[n] != NULL)
      {
         while (array[n] != NULL)
         {
            remocao = array[n];
            array[n] = array[n]->seta;
            // Desaloca apenas o nódulo.
            // free(remocao);
            destroi_nodulo(remocao);
         }
      }
   }
   // free(array); free(m);
   destroi_array_nodulo(array); free(m);
   return true;
}

bool destroi_interno_ht(HashTable m, Drop fk, Drop gv) 
{
   const int CAPACIDADE = (*m).capacidade;
   Node* array = (*m).locais, remocao = NULL;
   int n;

   for (n = 0; n < CAPACIDADE; n++)
   {
      // Remove o primeiro item até acabar.
      if (array[n] != NULL)
      {
         while (array[n] != NULL)
         {
            remocao = array[n];
            array[n] = array[n]->seta;

            /* Desaloca o 'nódulo' e a 'chave' e 'valor', com as funções que
             * foram fornecidas. A ordem tem que ser obviamente, chave ou 
             * valor, e a aí o nódulo, que os contém. Caso tenha sido passado
             * argumentos 'null', então quer dizer que tal desalocador
             * não está ativado. */
            if (fk != NULL)
               fk(remocao->chave);
            if (gv != NULL)
               gv(remocao->valor);
            // free(remocao); Trocado pelo método próprio.
            destroi_nodulo(remocao);
         }
      }
   }
   // free(array); free(m);
   destroi_array_nodulo(array); free(m);
   return true;
   return false;
}

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

   result_t resultado = verifica_lista (list, ch, m->__iguais__);
   return resultado.contido;
}

bool atualiza_ht(HashTable m,  generico_t ch,  generico_t nv) { 
/* acha uma chave e troca o valor dela, o retorno é bem sucedido se 
 * a atualização foi definitivamente feita, e falho caso o contrário,
 * que seria se por exemplo, se não houvesse a chave demanda para
 * atualização. */
   size_t posicao = m->__hash__ (ch, m->capacidade);

   result_t outcome = verifica_lista (m->locais[posicao], ch, m->__iguais__);
   nodulo_t* item = outcome.item;
   if (item != NULL)
      item->valor = nv;

   return outcome.contido; 
}

bool deleta_ht(HashTable m, generico_t ch) { 
/* A operação de remoção é apenas usar o método de atualização, para
 * inserir uma chave-em-branco no lugar da 'chave' dada, o resto é 
 * apenas realinhar das 'entradas' na array, tipo chaves-em-branco
 * no fim dela, 'entradas' não vázia no começo, não importando a ordem.
 */
   size_t capacity = (*m).capacidade;
   size_t posicao = m->__hash__ (ch, capacity);
   result_t outcome = verifica_lista (
      (*m).locais[posicao], 
      ch, (*m).__iguais__
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
 #include "hashtable/iteracao.c"

void imprime_ht(HashTable m, ToString fk, ToString gv) {
/* Usa o iterador para pegar cada entrada, transforma a chave e o valor em
 * respectivas strings -- passados seus transformadores como argumento; 
 * então imprime cada entrada, formatada, depois coloca um separador entre
 * elas. No fim, apenas "fecha" tais formatações. A variável de iteração
 * serve para mostrar o melhor fechamento, já que se usa caractéres 
 * especias de backspace, e não quer se comer qualquer caractére imprimido.
 */
   IterHT iter = cria_iter_ht(m); 
   bool iterado_alguma_vez = false;
   size_t Q = tamanho_ht(m);
   const size_t LIMITE = UINT16_MAX / 4;

   if (Q > LIMITE) {
   /* Ocupar recursos da máquina por muito tempo, processando um bocado
    * de string. Tudo isso para um output totalmente inútil de visualizar
    * para qualquer tipo de impressão, por isso o limite. Fututamente tal
    * poderá ser desativado na compilação. */
      perror(
         "toma muito recursor imprimir tamanha instância"
         " desta estrutura."
      );
      // Desaloca iterador, já que não vai continuar.
      destroi_iter_ht(iter);
      return;
   }

   printf("HashTable(%zu): {", Q);
   while (!consumido_iter_ht(iter)) {
      IterOutputHT S = next_ht(iter); 
      char* key_str = fk(S.key);
      char* value_str = gv(S.value);

      printf("%s: %s, ", key_str, value_str);
      free(key_str); free(value_str);

      if (!iterado_alguma_vez)
         iterado_alguma_vez = true;
   }
   if (iterado_alguma_vez)
      puts("\b\b}");
   else
      puts("}");
   destroi_iter_ht(iter);
}

GenT clona_ht(HashTable m)
{
   Hash f = (*m).__hash__; 
   Eq g = (*m).__iguais__;
   HashTable output = cria_ht(f, g);
   IteradorHT iter = cria_iter_ht(m);
   GenT key = NULL, value = NULL;
   IterOutputHT result;

   while (!consumido_iter_ht(iter))
   {
      result  = next_ht(iter);
      key     = result.key;
      value   = result.value;

      if (!add_ht(output, key, value))
         { perror("Não foi possível clonar!"); abort(); }
   }
   return output;
}

struct ArrayHT hashtable_to_array(HashTable input)
{
/*   O algoritmo é bem simples. Calcula a quantidade de itens no mapa, então
 * cria uma array dinâmica neste comprimento, cria um iterador, então vai
 * iterando cada item e copiando a saída de cada iteração na array.
 * 
 * NOTA: o algoritmo deixa bem claro que ele não consome ou libera o
 *       mapa depois desta operação, apenas faz copia das chaves e 
 *       valores e coloca numa array. 
 */
   // Apelido local para ajudar na codificação.
   typedef IterOutputHT Type;

   HashTable mapa = input; 
   // Array que será gerada.
   const int size = sizeof(Type);
   int quantia, n = 0;
   IterHT iter = NULL;
   struct ArrayHT output;

   iter = new_iter_ht(mapa);
   quantia = len_ht(mapa);
   output.array = (Type*)malloc(quantia * size);

   while (!consumido_iter_ht(iter))
      output.array[n++] = next_ht(iter);
   output.length = n;

   drop_iter_ht(iter);
   return output;
}


/* === === === === === === === === === === === === === === === === === ==
 *                      Renomeação de vários 
 *                métodos e funções para os termos
 *                  mais conhecidos em inglês
 *
 *  Apenas o encapsulamento das antigas com nomes novos, sendo que seus 
 * blocos de instruções apenas fazem chamadas das originais, retornando
 * o mesmo valor.
 * === === === === === === === === === === === === === === === === === ==*/
 #include "hashtable/ingles.c"

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 *                      Testes Unitários 
 *
 * Testando todos métodos, funções, e dados abstratos acima. Deixando bem 
 * referênciado esta parte, pois fica fácil descartar -- além de ser 
 * necessário se os tipos forem trocados, do contrário o programa não 
 * compila; se copiado para vários projetos. Caso também esta parte 
 * futuramente for colocada num subdiretório, e os tipos serem trocados
 * apenas comentar tal declaração pré-processada para não incluir o que
 * pode conflitar.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --*/
#ifdef __unit_tests__
#include "hashtable/testes.c"

// ---...---...---...---... Testes dos iteradores ---...---...---...---...
TESTE uso_simples_da_iteracao (void); 
TESTE tentando_iterador_mapa_vazio (void);
TESTE transporte_de_hashtable_para_array(void); 


void main(void) {
   setlocale (LC_CTYPE, "en_US.UTF-8");

   executa_testes_b(
      true, 1, 
        Unit(metodo_de_clonagem, true)
   );
   executa_testes_a (
      false, 9, 
          varias_entradas_genericas_diferentes, true,
          alocao_e_desacalocao_simples_instancia, true,
          aplicacao_de_simples_insercoes, true,
          verifica_operacao_de_pertencimento, true,
          metodo_get_verificacao_basica, true,
          operacoes_negadas, true, 
          algumas_remocoes_feitas, true,
          simples_atualizacoes_de_alguns_valores, true,
          // verificação de features do C.
          ascii_code_de_wide_strings, true
   );

   // testes apenas do iteradores.
   executa_testes_b (
      true, 3, 
         Unit(uso_simples_da_iteracao, true),
         Unit(tentando_iterador_mapa_vazio, true),
         Unit(transporte_de_hashtable_para_array, true)
   );
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
   printf ("contagem em %lu ...\n", contagem_iter_ht(I));

   for (size_t count = contagem_iter_ht(I); count > 0; count--) {
      IterOutputHT i = next_ht (I);
      assert (i.key != NULL && i.value != NULL);
      print_item_ht_u16_e_str (i);
      printf ("contagem em %lu ...\n", contagem_iter_ht(I));
   }

   printf ("tetando iterar mesmo esgotado ...");
   IterOutputHT i = next_ht (I);
   assert (i.key == NULL && i.value == NULL);
   i = next_ht (I);
   assert (i.key == NULL && i.value == NULL);
   i = next_ht (I);
   assert (i.key == NULL && i.value == NULL);
   puts ("não funcionou!");
   destroi_ht (M);
}

void tentando_iterador_mapa_vazio (void) {
   HashTable M = cria_ht(hash_int, int_eq);
   IterHT I = cria_iter_ht (M);

   IterOutputHT i = next_ht (I);
   printf ("contagem em %lu ...\n", contagem_iter_ht(I));
   assert (i.key == NULL && i.value == NULL);

   i = next_ht (I);
   printf ("contagem em %lu ...\n", contagem_iter_ht(I));
   assert (i.key == NULL && i.value == NULL);

   i = next_ht (I);
   printf ("contagem em %lu ...\n", contagem_iter_ht(I));
   assert (i.key == NULL && i.value == NULL);

   puts ("não funcionou com nenhuma!");
   destroi_iter_ht(I);
   destroi_ht (M);
}

#endif

