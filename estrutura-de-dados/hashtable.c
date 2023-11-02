
/* Uma tabela de dispersão, também conhecidas com hashtable, com chaves 
 * sendo caractéres, e valores sendo um inteiro. Está aqui em específico
 * não aceita colisões. Como a chave é um byte, apenas cabem aí 256 símbolos,
 * portanto, o mesmo valor de chaves.
 */

#include <stdint.h>
#include <stdio.h>

// quantas instâncias de tabela-hash existem.
uint64_t hash_instancias = 0;

#include "hashtable/dado.h"

struct tabela_de_dispersao {
   // array com listas para acrescimo de entradas.
   Dado* locais;
   // total de elementos.
   uint64_t total;
   // total de vagões disponíveis.
   uint32_t capacidade;
};

// total de bytes do tipo de dado acima.
#define MAPA_SIZE sizeof(struct tabela_de_dispersao)

// todos nomes possíveis para estrutura acima.
typedef struct tabela_de_dispersao* MAPA;

#include <stdlib.h>
// alocação e liberação da tabela.
MAPA cria_ht() {
   MAPA novo = (MAPA)malloc(MAPA_SIZE);
   puts("allocation was succesuful.");

   // tem que ter sido alocado.
   if (novo != NULL) {
      // nada contabilizado ainda.
      novo->total = 0; 
      // lista(vázia) para cada uma dos vagoes.
      printf("creating all 52 'spot' ...");
      novo->locais = cria_array_dado(52);
      puts("done");
      novo->capacidade = 52;
      // contabiliza nova instância.
      hash_instancias++;
   }
   puts("hashtable created was a successful.");
   // retorna instância, válida ou não.
   return novo;
}

void destroi_ht(MAPA m) {
   destroi_array_dado(m->locais, m->capacidade);
   free(m);
}

#include <stdbool.h>

bool instancia_valida(MAPA m)
   { return m != NULL; }

bool vazia_ht(MAPA m)
   { return m->total == 0; }

uint64_t tamanho_ht(MAPA m)
   { return m->total; }

uint32_t funcao_hash(MAPA m, CHAVE ch) {
   return (uint32_t)ch % m->capacidade;
}

bool insere_ht(MAPA m, CHAVE ch, VALOR v) {
   uint32_t indice = funcao_hash(m, ch);
   Dado branco = cria_dado_branco();

   if (!dados_iguais(m->locais[indice], branco))
      return false;
   else {
      m->locais[indice]->chave = ch;
      m->locais[indice]->dado = v;
      m->total++;
      return true;
   }
}

bool remove_ht(MAPA m, CHAVE ch) {
   // já que não é possível remover nada do vázio.
   if (vazia_ht(m))
      return false;
   
   uint32_t indice = funcao_hash(m, ch);
   Dado branco = cria_dado_branco();

   // se achar uma posição em branca, apenas tal chave não existe.
   if (dados_iguais(branco, m->locais[indice])) 
      return false;
   else {
      /* caso contrário, preenche a posição com tal chave-valor, com um dado
       * em branco. */
      m->locais[indice] = branco;
      // contabiliza remoção.
      m->total--;
      return true;
   }
}

bool contem_ht(MAPA m, CHAVE ch) {
   uint32_t indice = funcao_hash(m, ch);
   Dado branco = cria_dado_branco();
   /* se caí em um dado-branco, então a chave não existe, caso contrário, 
    * valor lógico também contrário. */
   return !dados_iguais(branco, m->locais[indice]);
}

bool atualiza_ht(MAPA m, CHAVE ch, VALOR v) {
   /* Só pode atualizar se houver algo. Caso tenha, a atualização será 
    * uma operação composta: remove antigo valor, e insere um novo. */
   if (contem_ht(m, ch)) {
      remove_ht(m, ch);
      insere_ht(m, ch, v);
      return true;
   }
   // chave acima não existe, portanto operação não realizada.
   return false;
}

void imprime_ht(MAPA m) {
   Dado branco = cria_dado_branco();
   
   // começo da lista.
   printf("Mapa {");
   // iterando todos elementos, mas imprime só os válidos.
   for (uint32_t p = 0; p < m->capacidade; p++) {
      Dado item = m->locais[p];
      // só imprime o item se ele não for "branco".
      if (!dados_iguais(branco, item))
         { printf("%s, ", dado_para_string(item)); }
   }
   // seu fim.
   puts("\b\b}");
}

#include <assert.h>

void main() {
   MAPA m = cria_ht();
   assert(vazia_ht(m));
   puts("it is empty so far now.");
   assert(insere_ht(m, 'a', -3));
   assert(insere_ht(m, 'M', 11));
   assert(insere_ht(m, 'f', 50));
   assert(tamanho_ht(m) == 3);
   imprime_ht(m);
   assert(!remove_ht(m, 'G'));
   assert(tamanho_ht(m) == 3);
   assert(remove_ht(m, 'f'));
   assert(tamanho_ht(m) == 2);
   imprime_ht(m);
   insere_ht(m, 0xA8, 15);
   imprime_ht(m);
   puts("all linked-list there are ready.");
   destroi_ht(m);
   puts("same instance was also destroyed.");
}