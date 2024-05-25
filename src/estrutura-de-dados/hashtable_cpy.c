
/*   Isso é um teste simples para dá versatilidade para futuros 
 * projetos, onde a única coisa necessária para fazer esta 'hashtable', 
 * disponível para vários tipos é mexer no começo do arquivo(seus tipos 
 * de chave-e-valor). Claro, ainda em utilitarios é bem inferior ao 
 * já desenvolvido, este usando muitos módulos de outros projetos. A 
 * necessidade deste é que, a outra tabela depedende da manutenção e
 * modificação de vários demais arquivos.
 *   Também estando uma nova modadlidade de 'debug', onde a info pode
 * ser selecionada baseado no tipo testado.
 */

/* tipo de dado necessário para este módulo, caso queira trocar
 * os tipos de dados(chave-valor) mexa nele. */
#include "hashtable/dado.c"

#define QUANTIA_INICIAL 50
// Para fora da capacidade, o valor máximo possível é o mesmo que -1.
#define NAO_ENCONTRADO ULLONG_MAX

typedef struct tabela_de_dispersao {
   // arrays(matriz) para abrigar 'colisões'.
   ArrayEntrada locais[QUANTIA_INICIAL];
   // contabilização de itens(ultra necessário nesta estranha estrutura).
   size_t quantidade;
   /* capacidade para um aumento das array com colisões. Também
    * necessário no cálculo da 'função hash'. */
   size_t capacidade;
   size_t n_colunas;
} *HashTable, hashtable_t;

// criação vázia da 'hashtable'.
HashTable cria_ht() {
   size_t T = sizeof(struct tabela_de_dispersao);
   HashTable mapa = malloc(T);

   if (mapa != NULL) {
      // dez colunas na matriz.
      const size_t n = 10;
      for (size_t m = 0; m < QUANTIA_INICIAL; m++)
         mapa->locais[m] = cria_array_entrada(n);
      mapa->n_colunas = n;
      mapa->capacidade = QUANTIA_INICIAL;
      // sem elementos inicialmente, por motivos óbvios.
      mapa->quantidade = 0;
   }
   return mapa;
}

hashtable_t* cria_com_capacidade_ht(size_t q) {
   hashtable_t* mapa = malloc(sizeof(hashtable_t));

   if (mapa != NULL) {
      // dez colunas na matriz.
      const size_t n = 10;
      for (size_t m = 0; m < QUANTIA_INICIAL; m++)
         mapa->locais[m] = cria_array_entrada(n);
      mapa->n_colunas = n;
      mapa->capacidade = q;
      // sem elementos inicialmente, por motivos óbvios.
      mapa->quantidade = 0;
   }
   return mapa;
}

bool destroi_ht(hashtable_t* m) {
   // se o 'dado' passado for válido:
   if (m != NULL) {
      for (size_t t = 0; t < m->capacidade; t++)
         destroi_array_entrada(m->locais[t], m->n_colunas);
      free(m);
      #ifdef DEBUG_HashTable
      size_t total = m->capacidade * m->n_colunas;
      printf("todas %ld 'entradas' foram destruídas.\n", total);
      #endif
      // confirmando resposta.
      return true;
   }
   #ifdef UT_HashTable
   puts("falha ao desalocar 'hashtable'.");
   #endif
   // confirmando falha.
   return false;
}

#include <limits.h>
#include <string.h>
/* Este tipo de cálculo é específicos para os tipos chave-valor em
 * string(chave) e inteiro(valor). */
static size_t funcao_hash(HashTable m, Chave ch, Valor v) {
   size_t meio = strlen(ch) / 2;
   size_t ascii_code = ch[meio];
   return (ascii_code * strlen(ch)) % m->capacidade;
}

static bool chaves_batem(Entrada x, Chave ch) 
   { return !strcmp(ch, x->chave); }

#include <assert.h>

bool insere_ht(HashTable m, Chave ch, Valor v) {
   size_t indice = funcao_hash(m, ch, v);
   // limites de matriz de 'entradas'.
   size_t Lx = m->n_colunas;

   // falha caso na array não seja encontrado.
   if (indice == NAO_ENCONTRADO) {
      #ifdef DEBUG_InsercaoHashTable
      printf("índice encontrado é %lu.\n", indice);
      #endif
      return false;
   }

   Entrada novo_item; size_t t;
   novo_item = cria_entrada(ch, v);
   /* verificando existência e espaço vázio(coluna disponível). Se 
    * achar já uma chave existente, então não insere nada e retorna
    * o resultado de fracasso da inserção, caso contrário, marca 
    * a coluna(quebra o laço), então insere na posição ocorrida.*/
   for (t = 0; t <= (Lx - 1); t++) {
      Entrada atual = m->locais[indice][t];
      if (chaves_batem(atual, CHAVE_VAZIA)) {
         #ifdef DEBUG_InsercaoHashTable
         printf("linha: %ld, coluna:%ld.\n", indice, t);
         #endif
         break;
      }

      if (chaves_batem(novo_item, ch)) {
         #ifdef DEBUG_InsercaoHashTable
         printf("já existe tal chave '%s' na tabela.\n", ch);
         #endif
         return false;
      }
   }
   /* Atualiza a célula nesta posição, destrói a 'entrada' em branco,
    * contabiliza e confirma a inserção. 
    */
   m->locais[indice][t] = novo_item;
   m->quantidade++;
   return true;
}

#include <stdarg.h>
/* realiza a inserção de múltiplas entradas de uma vez só. Os argumentos
 * consistem em primeiro a 'chave', depois o seu 'valor', não quebre
 * está ordem, caso contrário não funcionará. 
 * --- --- AINDA EM DESENVOLVIMENTO --- ---*/
bool insere_multiplas_ht(HashTable mapa, uint8_t qtd, ...) {
   va_list argumentos;
   // verificação de todas inserções.
   bool resultado = true;


   // começando iteração.
   va_start(argumentos, qtd);
   for (uint8_t p = 0; p < (qtd - 1) * 2; p++) {
      Chave ch = va_arg(argumentos, char*);
      Valor vl = va_arg(argumentos, int);
      #ifdef DEBUG_MultiplaInsercaoHT
      printf( "chave: %s\tvalor: %d\n", ch, vl);
      #endif
      // resultado da inserção
      resultado = resultado && insere_ht(mapa, ch, vl);
   }
   va_end(argumentos);
   return resultado;
}

/* computado o índice na array, varre ela se encontrar a chave informa
 * que há o elemento, caso contrário diz que não está aqui. */
bool contem_ht(HashTable m, Chave ch) {
   size_t indice = funcao_hash(m, ch, 8);
   size_t Lx = m->n_colunas;

   for (size_t t = 0; t <= (Lx - 1); t++) {
      Entrada atual = m->locais[indice][t];
      if (chaves_batem(atual, CHAVE_VAZIA)) {
         #ifdef DEBUG_ContemHashTable
         puts("não há chave alguma.");
         #endif
         return false;
         break;
      }

      if (chaves_batem(atual, ch)) {
         #ifdef DEBUG_ContemHashTable
         printf("já na linha %ld, coluna %ld\n", indice, t);
         #endif
         return true;
      }
   }
   // nunca chega até aqui, porém "se chegar", evalua a função em 
   // falsidade, já que não foi encontrado qualquer chave dada.
   return false;
}

static bool atualiza_entrada(HashTable m, Chave ch, 
  Chave nova_ch, Valor vl)
{
   size_t indice = funcao_hash(m, ch, vl);
   size_t Lx = m->n_colunas, p;

   if (indice == NAO_ENCONTRADO) {
      #ifdef DEBUG_AtualizaHashTable
      printf("índice encontrado é %lu.\n", indice);
      #endif
      return false;
   }

   Entrada atual, novo_item; 
   novo_item = cria_entrada(ch, vl);
   // vazia = cria_entrada_branca();

   // iterando sobre o índice da array computado.
   for (p = 0; p <= (Lx - 1); p++) {
      atual = m->locais[indice][p];
      /* Os codicionais são invertido comparados com o algoritmo de 
       * inserção, onde quase toda base deste código é copiado, ele
       * sinaliza fracasso se achar uma chave-em-branco, ou seja,
       * se não encontrar o 'valor' a alterar. E insere quando acha
       * a chave, que seria atualizar o valor(mesma chave, porém 
       * valor diferente).*/
      if (chaves_batem(atual, CHAVE_VAZIA)) {
         #ifdef DEBUG_AtualizaHashTable
         printf("linha: %ld, coluna:%ld.\n", indice, p);
         #endif
         return false;
      }

      if (chaves_batem(novo_item, ch)) {
         #ifdef DEBUG_AtualizaHashTable
         printf("já existe tal chave '%s' na tabela.\n", ch);
         #endif
         break;
      }
   }
   Entrada antiga = m->locais[indice][p]; 
   // se livrando das 'entradas' auxiliares.
   destroi_entrada(antiga); destroi_entrada(novo_item);
   m->locais[indice][p] = cria_entrada(nova_ch, vl);
   return true;
}

/* acha uma chave e troca o valor dela, o retorno é bem sucedido se 
 * a atualização foi definitivamente feita, e falho caso o contrário,
 * que seria se por exemplo, se não houvesse a chave demanda para
 * atualização. */
bool atualiza_ht(HashTable m, Chave ch, Valor v) {
   return atualiza_entrada(m, ch, ch, v);
}

/* A operação de remoção é apenas usar o método de atualização, para
 * inserir uma chave-em-branco no lugar da 'chave' dada, o resto é 
 * apenas realinhar das 'entradas' na array, tipo chaves-em-branco
 * no fim dela, 'entradas' não vázia no começo, não importando a ordem.
 */
bool deleta_ht(HashTable m, Chave ch) {
   // captura o valor da atualização, se foi bem sucedida ou não.
   bool resultado = atualiza_entrada( m, ch, CHAVE_VAZIA, VALOR_VAZIO);
   // aplica operação de reordenação da array.
   // descontabiliza item adicionado se o resultado for confirmado.
   if (resultado) m->quantidade -= 1;
   return resultado;
}

/* Operações, principalmente de encapsulamento, extremamente triviais
 * mais necessárias para esconder, futurumente quando este arquivo
 * fazer parte da 'lib' acessar tais valores da estrutura. */
bool vazia_ht(HashTable m)
   { return m->quantidade == 0; }

size_t tamanho_ht(HashTable m)
   { return m->quantidade; }

/* Obtém o endereço do valor da dada 'chave' na tabela, se a tal não 
 * pertencer a 'tabela', então simplesmente retorna 'null'. Talvez
 * o nome 'obtem' não seja adequado, no futuro pode ser alterado ou
 * ter outros métodos que 'embrulham' este.
 */
Valor* obtem_ht(HashTable m, Chave ch) {
   size_t indice = funcao_hash(m, ch, VALOR_VAZIO);
   size_t Lx = m->n_colunas;

   for (size_t t = 0; t <= (Lx - 1); t++) {
      Entrada atual = m->locais[indice][t];
      if (chaves_batem(atual, CHAVE_VAZIA)) {
         #ifdef DEBUG_GetHashTable
         puts("não há chave alguma.");
         #endif
         return NULL;
      }

      if (chaves_batem(atual, ch)) {
         #ifdef DEBUG_GetHashTable
         printf("já na linha %ld, coluna %ld\n", indice, t);
         #endif
         return &atual->valor;
      }
   }
   /* se chegar até aqui, algo que não vai, retornará NULL, pois tudo
    * foi verificado. */
   return NULL;
}

/* Testando todos métodos, funções, e dados abstratos acima. Deixando
 * bem referênciado esta parte, pois fica fácil descartar -- além de 
 * ser necessário se os tipos forem trocados, do contrário o programa
 * não compila; se copiado para vários projetos. Caso também esta parte
 * futuramente for colocada num subdiretório, e os tipos serem trocados
 * apenas comentar tal declaração pré-processada para não incluir o que
 * pode conflitar.
 */
#ifdef _UT_HASHTABLE
// apenas inclue com testes tais "módulos".
#include "hashtable/testes_unitarios.c"
#include "teste.h"

void main() {
   executa_testes(
      10, testa_entrada_tipo, true,
      igualdade_de_entradas, true,
      alocacao_e_destruicao_array_entrada, true,
      alocacao_e_destruicao_matriz_entrada, true,
      criacao_e_destruicao_hashtable, false,
      operacao_pertence, false,
      atualiza_de_entrada, false,
      operacao_de_remocao, true,
      // [AINDA EM DESENVOLVIMENTO]
      insercao_na_tabela, false,
      operacao_de_obtem_de_valor, true
   );
   return;
}
#endif

