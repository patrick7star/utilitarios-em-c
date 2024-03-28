
/* Aqui ficam todos testes unitários, então não sobrecarrega a busca no 
 * módulo original por funcionamento interno das operações e métodos
 * lá posto. Este arquivo deve apenas ser incluído no arquivo de 
 * implementação 
 */

#include <assert.h>
void testa_entrada_tipo() {
   Entrada e = cria_entrada("string", 37);
   printf("%s:%d\n", e->chave, e->valor);
   assert(destroi_entrada(e));
}

void igualdade_de_entradas() {
   Entrada e = cria_entrada("string", 37);
   Entrada a = cria_entrada("maça", 37);
   Entrada x = cria_entrada("uva", 8);
   Entrada t = cria_entrada("string", 5);
   assert(!eq_entrada(a, e));
   assert(!eq_entrada(a, x));
   assert(eq_entrada(e, t));
   puts("todas passaram no teste de igualdade!");
   destroi_entrada(e);
   destroi_entrada(a);
   destroi_entrada(x);
   destroi_entrada(t);
}

void alocacao_e_destruicao_array_entrada() {
   ArrayEntrada a = cria_array_entrada(7);
   assert(destroi_array_entrada(a, 7));
}

void alocacao_e_destruicao_matriz_entrada() {
   ArrayEntrada matriz[10];

   for (size_t q = 1; q <= 10; q++)
      matriz[q - 1] = cria_array_entrada(5);
   for (size_t q = 1; q <= 10; q++)
      destroi_array_entrada(matriz[q - 1], 5);
   puts("chegou até aqui com sucesso.");
}

// visualizar interno da 'hashtable'.
static void info_basica_ht(HashTable d) {
   printf(
      "capacidade: (%ld x %ld)\n\rn. itens: %ld\n", 
      d->capacidade, d->n_colunas, d->quantidade
   );
}

static void visualizacao_interna_ht(HashTable d) {
   info_basica_ht(d);

   size_t capacidade = d->capacidade;
   size_t Lx = d->n_colunas;
   puts("HashTableDebug: [..");
   for (size_t i = 0; i < capacidade; i++) {
      printf("  ");
      for (size_t p = 0; p < Lx; p++)
         printf("%s, ", entrada_to_str(d->locais[i][p]));
      puts(""); // quebra-de-linha.
   }
   puts("..]");
}

void criacao_e_destruicao_hashtable() {
   HashTable m = cria_ht();
   info_basica_ht(m);
   assert(destroi_ht(m));
}

void insercao_na_tabela() {
   HashTable m = cria_ht();
   info_basica_ht(m);
   assert(insere_ht(m, "maça", 15));
   assert(insere_ht(m, "uva", 15));
   assert(insere_ht(m, "erva", 4));
   // irá falhar inserção.
   assert(!insere_ht(m, "maça", 3));
   assert(!insere_ht(m, "uva", 15));
   assert(tamanho_ht(m) == 3);
   assert(insere_ht(m, "banana", 47));
   visualizacao_interna_ht(m);
   assert(destroi_ht(m));
}

void insercao_multipla_na_tabela() {
   HashTable m = cria_ht();
   insere_multiplas_ht(
      m, 4, "orange", 12, "grape", 11,
      "strawberry", 7, "papaya", 3
   );
   assert(destroi_ht(m));
}

void operacao_pertence() {
   HashTable m = cria_ht();
   info_basica_ht(m);
   assert(insere_ht(m, "maça", 15));
   assert(insere_ht(m, "uva", 15));
   assert(insere_ht(m, "erva", 4));
   assert(insere_ht(m, "banana", 47));
   assert(insere_ht(m, "cereja", 21));
   assert(insere_ht(m, "pêra", 16));
   assert(insere_ht(m, "goiába", 8));
   assert(insere_ht(m, "laranja", 9));
   assert(insere_ht(m, "mamão", 2));
   visualizacao_interna_ht(m);

   typedef struct chaves_de_entradas {
      char* chave;
      bool resultado;
   };
   // amostra para teste:
   struct chaves_de_entradas entradas[] = {
      {"banana", true},
      {"morango", false},
      {"mamão", true },
      {"abacaxi", false},
      {"melância", false}
   };

   for (size_t i = 0; i < 5; i++) {
      char* ch = entradas[i].chave;
      bool* rst = &entradas[i].resultado;
      if (*rst) {
         assert(contem_ht(m, ch));
         printf("'%s' pertence? %d\n", ch, *rst);
      } else {
         assert(!contem_ht(m, ch));
         printf("'%s' pertence? %d\n", ch, *rst);
      }
   }
   assert(destroi_ht(m));
}

void atualiza_de_entrada() {
   HashTable m = cria_ht();
   puts("tabela inicialmente com as seguintes entradas:");
   assert(insere_ht(m, "maça", 15));
   assert(insere_ht(m, "uva", 15));
   assert(insere_ht(m, "erva", 4));
   assert(insere_ht(m, "banana", 47));
   assert(insere_ht(m, "cereja", 21));
   assert(insere_ht(m, "pêra", 16));
   assert(insere_ht(m, "goiába", 8));
   assert(insere_ht(m, "laranja", 9));
   assert(insere_ht(m, "mamão", 2));
   visualizacao_interna_ht(m);
   assert(tamanho_ht(m) == 9);

   puts("interno da tabela após atualização de algumas células");
   assert(atualiza_ht(m, "goiába", 16));
   assert(atualiza_ht(m, "banana", 47-10));
   assert(atualiza_ht(m, "maça", 15 + 1));
   assert(!atualiza_ht(m, "melância", 10));
   assert(tamanho_ht(m) == 9);
   visualizacao_interna_ht(m);
   assert(destroi_ht(m));
}

void visualizacao_basica_ht(HashTable m) {
   size_t capacidade = m->capacidade;
   size_t colunas = m->n_colunas;

   printf("HashTable: {");
   // caso não tenha nada.
   if (vazia_ht(m)) {
      puts("}");
      return;
   }

   for (size_t y = 0; y < capacidade; y++) {
      for (size_t x = 0; x < colunas; x++) {
         Entrada atual = m->locais[y][x];
         if (!chaves_batem(atual, CHAVE_VAZIA))
            printf("%s: %d, ", atual->chave, atual->valor);
      }
   }
   puts("\b\b}");
}

void operacao_de_remocao() {
   HashTable m = cria_ht();
   puts("tabela inicialmente com as seguintes entradas:");
   assert(insere_ht(m, "maçã", 15));
   assert(insere_ht(m, "uva", 15));
   assert(insere_ht(m, "erva", 4));
   assert(insere_ht(m, "cereja", 21));
   assert(insere_ht(m, "pêra", 16));
   assert(insere_ht(m, "goiába", 8));
   assert(insere_ht(m, "laranja", 9));
   assert(insere_ht(m, "mamão", 2));
   assert(insere_ht(m, "melância", 11));
   assert(insere_ht(m, "banAna", 47));
   visualizacao_interna_ht(m);
   printf("qtd. restante: %lu\n", tamanho_ht(m));
   visualizacao_basica_ht(m);
   assert(!vazia_ht(m));

   typedef struct chaves_de_entradas {
      char* chave;
      bool resultado;
   } Par;

   Par entrada_e_saida[] = { 
      {"melância", true}, {"erva", true}, {"maçã", true},
      {"mamão", true}, {"uva", true}, {"cereja", true}, {"erva", false},
      {"laranja", true}, {"mamão", false}, {"pêra", true}, 
      {"goiába", true}, {"banana", false}, {"banAna", true}
   };
   for (size_t i = 0; i < 13; i++) { 
      bool *output = &entrada_e_saida[i].resultado;
      char* key = entrada_e_saida[i].chave;
      if (deleta_ht(m, key))
         assert(*output);
      else
         assert(!(*output));
   }
   puts("resultado agora após sucedidas remoções:");
   visualizacao_interna_ht(m);
   visualizacao_basica_ht(m);
   printf("qtd. restante: %lu\n", tamanho_ht(m));
   assert(vazia_ht(m));
   assert(destroi_ht(m));
}

void operacao_de_obtem_de_valor() {
   HashTable m = cria_ht();
   assert(insere_ht(m, "maçã", 15));
   assert(insere_ht(m, "cereja", 21));
   assert(insere_ht(m, "pêra", 16));
   assert(insere_ht(m, "laranja", 9));
   assert(insere_ht(m, "mamão", 2));
   assert(insere_ht(m, "banAna", 47));
   assert(!vazia_ht(m));
   puts("inserção feita!");

   const Valor NULO = UINT8_MAX;
   typedef struct chaves_de_entradas {
      char* chave;
      Valor valor;
   } Par_ES;

   Par_ES entrada_e_saida[] = { 
      {"melância", NULO}, {"erva", NULO}, {"maçã", 15},
      {"uva", NULO}, {"cereja", 21}, {"laranja", 9}, 
      {"mamão", 2}, {"pêra", 16}, {"goiába", NULO}, 
      {"banana", NULO}, {"banAna", 47}
   };

   Valor* vl = obtem_ht(m, "cereja");
   assert(vl != NULL);
   printf("cereja: %d\n", *vl);
   vl = obtem_ht(m, "banana");
   assert(vl == NULL);
   // Valor saida[11];
   for (size_t p = 0; p < 11; p++) {
      char* key = entrada_e_saida[p].chave;
      Valor* a = obtem_ht(m, key);
      if (a != NULL)
         printf("'%s': %d\n", key, *a);
      else
         printf("chave '%s' não existe.\n", key);
   }
   visualizacao_basica_ht(m);

   assert(destroi_ht(m));
}

