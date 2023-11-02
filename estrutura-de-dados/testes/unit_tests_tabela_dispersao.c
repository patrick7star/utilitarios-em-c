
// própria biblioteca.
#include "../../teste.h"
#include "../tabela_dispersao.h"

// biblioteca do C:
#include <assert.h>
#include <stdio.h>

void teste_basico_inicial_com_cria_e_algumas_operacoes() {
   HASH_TABLE m = cria_ht();
   assert(vazia_ht(m));

   insere_ht(m, "apple", 4);
   insere_ht(m, "potatoes", -3);
   insere_ht(m, "pineapple", 8);
   insere_ht(m, "strawberry", 1);
   insere_ht(m, "banana", -5);
   assert(tamanho_ht(m) == 5);

   insere_ht(m, "carrot", 13);
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

void clonagem_da_tabela() {
   char* chaves[] = {
      "banana", "morango", "uva", 
      "maçã", "pêra", "laranja",
      "melância", "cereja", "manga"
   };
   int64_t valores[] = {-3, 8, 1, 10, 2, -5, -19, 7, 4};
   HashTable t = cria_ht();

   for (int k = 0; k < 9; k++)
      insere_ht(t, chaves[k], valores[k]);

   HashTable T = clona_ht(t);
   printf(
      "original: %s\nclone: %s\n", 
      ht_to_str(t), ht_to_str(T)
   );

   // destruindo para mostrar indepedência.
   destroi_ht(t);
   puts(ht_to_str(T));
   // agora assim destruindo ambas...
   destroi_ht(T);
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


// execução dos testes:
void main(int argc, char** argv) {
   executa_teste_interruptor(
      "teste antigo básico com criação/destruição, e algumas operações",
      teste_basico_inicial_com_cria_e_algumas_operacoes, true
   );
   
   executa_teste_interruptor(
      "contabilização das instâncias desta estrutura de dados",
      contagem_de_instancias, true
   );

   executa_teste_interruptor(
      "ainda desenvolvendo e testando o iterador",
      desenvolvendo_iterador, true
   );

   executa_teste_interruptor(
      "testando uma visualização de debug da hashtable",
      visualizacao_da_tabela_modo_debug, true
   );

   executa_teste_interruptor(
      "clonagem da HashTable",
      clonagem_da_tabela, true
   );
   executa_testes(1, remocao_sistematica_da_tabela, true);
}
