
// própria biblioteca.
#include "../../teste.h"
#include "../tabela_dispersao.h"

// biblioteca do C:
#include <assert.h>
#include <stdio.h>

char* chaves[] = {
   "apple", "potatoes",
   "pineapple",  "strawberry", 
   "banana", "carrot"
};
int* valores[] = { 4, -3, 8, 1, -5 };

void apenas_um_teste_basico() {
   HASH_TABLE m = cria_ht();
   assert(vazia_ht(m));
   
   for (int k = 0; k < 5; k++)
      insere_ht(m, chaves[k], valores[k]);

   assert(tamanho_ht(m) == 5);

   insere_ht(m, chaves[5], valores[5]);
   assert(tamanho_ht(m) == 6);

   visualizacao_debug_ht(m);

   puts("\nApós remoção da banana:");
   deleta_ht(m, "banana");
   assert(tamanho_ht(m) == 5);
   visualizacao_debug_ht(m);

   char entradas[5][30] = {
      "strawberry", "pineapple",
      "potatoes", "carrot", "apple"
   };

   for(int p = 0; p < 5; p++) {
      printf("chave '%s' ... ", entradas[p]);
      assert(contem_ht(m, entradas[p]));
      puts("pertence!");
   }

   // atualizando uma chave que existe.
   insere_ht(m, "banana", 0);
   assert(contem_ht(m, "banana"));
   visualizacao_debug_ht(m);
   atualiza_ht(m, "banana", 99);
   visualizacao_debug_ht(m);

   destroi_ht(m);
}

void contagem_de_instancias() {
   puts("teste-unitário iniciado com sucesso.");
   HASH_TABLE m = cria_ht();
   insere_ht(m, "apple", 4);
   insere_ht(m, "potatoes", -3);
   insere_ht(m, "pineapple", 8);
   insere_ht(m, "strawberry", 1);
   insere_ht(m, "banana", -5);
   insere_ht(m, "carrot", 13);

   uint8_t t = total_de_instancias_ht();
   assert(t == 1);
   printf("instâncias contabilizadas: %d\n", t);

   HASH_TABLE m1 = cria_ht();
   insere_ht(m1, "apple", 4);
   insere_ht(m1, "potatoes", -3);
   insere_ht(m1, "pineapple", 8);
   insere_ht(m1, "strawberry", 1);

   t = total_de_instancias_ht();
   assert(t == 2);
   printf("instâncias contabilizadas: %d\n", t);

   destroi_ht(m);
   destroi_ht(m1);
}

void desenvolvendo_iterador() {
   puts("criando um iterador...");
   HASH_TABLE m = cria_ht();

   insere_ht(m, "apple", 4);
   insere_ht(m, "potatoes", -3);
   insere_ht(m, "pineapple", 8);
   insere_ht(m, "strawberry", 1);
   insere_ht(m, "banana", -5);
   insere_ht(m, "carrot", 13);
   insere_ht(m, "orange", 19);
   insere_ht(m, "flower", -2);
   insere_ht(m, "blueberry", 13);

   IterHT iterador = iter_ht(m);
   puts("instância 'IterHT' criada com sucesso.");
   Dado item;
   puts("começando a iteração...");
   for (int k = 1; k <= tamanho_ht(m) + 6; k++) {
      item = next_ht(iterador);
      printf("%s\n", dado_para_string(item));
      // não retorna mais itens válidos.
      if (k > tamanho_ht(m))
         assert(item == NULL);
   }

   destroi_iter_ht(iterador);
   destroi_ht(m);
}

void visualizacao_da_tabela_modo_debug() {
   HASH_TABLE m = cria_ht();

   insere_ht(m, "apple", 4);
   insere_ht(m, "potatoes", -3);
   insere_ht(m, "pineapple", 8);
   insere_ht(m, "strawberry", 1);
   insere_ht(m, "banana", -5);
   insere_ht(m, "carrot", 13);
   insere_ht(m, "orange", 19);
   insere_ht(m, "flower", -2);
   insere_ht(m, "blueberry", 13);

   visualizacao_debug_ht(m);
   printf("mesmo? %s\n", ht_to_str(m));

   destroi_ht(m);
}

#include <stdlib.h>

void remocao_sistematica_da_tabela() {
   char* chaves[] = {
      "banana", "morango", "uva", 
      "maçã", "pêra", "laranja",
      "melância", "cereja", "manga"
   };
   int64_t valores[] = {-3, 8, 1, 10, 2, -5, -19, 7, 4};
   HashTable t = cria_ht();

   for (int k = 0; k < 9; k++)
      insere_ht(t, chaves[k], valores[k]);
   assert(!vazia_ht(t));

   while (!vazia_ht(t)) {

      int indice = rand() % 9;
      CHAVE e = chaves[indice];
      bool resultado_operacao = deleta_ht(t, e);
      if (!resultado_operacao)
         printf("remoção de '%s' negada.\n", e);
      else
         puts(ht_to_str(t));
   }
   assert(vazia_ht(t));
}

void operacao_de_clonagem(void) {
   HashTable T = cria_ht();

   for (int k = 0; k < 5; k++)
      insere_ht(T, chaves[k], valores[k]);
   visualizacao_debug_ht(T);

   HashTable clone_t = clona_ht(T);
   assert (tamanho_ht(clone_t) == tamanho_ht(T));
   printf("original: %s\nclone: %s\n", ht_to_str(T), ht_to_str(clone_t));
}


// execução dos testes:
int main(int argc, char** argv) {
   executa_testes(
      4, contagem_de_instancias, true,
      desenvolvendo_iterador, true,
      remocao_sistematica_da_tabela, true,
      visualizacao_da_tabela_modo_debug, true
   );

   // tais testes conflitam com o teste de clonagem abaixo.
   executa_teste_interruptor(
      "uma instância básica e algumas operações",
      apenas_um_teste_basico, true
   );

   executa_teste_interruptor(
      "testando método de clonagem",
      operacao_de_clonagem, true
   );

   return EXIT_SUCCESS;
}
