
// própria biblioteca.
#include "../fila_ligada.h"
#include "../dado.h"
#include "../../teste.h"
#include "../../aleatorio.h"

// biblioteca do C:
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

// construção de testes a serem executados.
void criacao_destruicao_e_metodos_basicos() {
   FilaLigada f = cria_fl();

   assert(vazia_fl(f));
   visualiza_fl(f);

   enqueue_fl(f, cria_dado('n'));
   assert(dados_iguais(frente_fl(f), traseira_fl(f)));
   visualiza_fl(f);

   enqueue_fl(f, cria_dado('y'));
   printf("frente: '%s'\n", dado_para_string(frente_fl(f)));
   printf("traseira: '%s'\n", dado_para_string(traseira_fl(f)));
   enqueue_fl(f, cria_dado('B'));
   enqueue_fl(f, cria_dado('i'));
   enqueue_fl(f, cria_dado('a'));
   assert(quantidade_fl(f) == (1 + 4));
   visualiza_fl(f);

   // remoção até esvaziar completamente.
   puts("\nremovendo sucessivamente.");
   do {
      uint64_t q = quantidade_fl(f);
      printf("dado removido '%s'\n", dado_para_string(dequeue_fl(f)));
      assert((q - 1) == quantidade_fl(f));
      visualiza_fl(f);
   } while (!vazia_fl(f));

   destroi_fl(f);
}

// execução dos testes:
void main() {
   executa_teste_interruptor(
      "criação, inserção e remoção, e outros métodos",
      criacao_destruicao_e_metodos_basicos, true
   );
}
