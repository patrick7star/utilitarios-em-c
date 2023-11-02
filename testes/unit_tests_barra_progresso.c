
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "../barra_de_progresso.h"
#include "../teste.h"
//#include "../barra_de_progresso.h"


void progresso_simples() {
   const uint64_t limite = 30;
   ProgressoSimples p = novo_bps(limite, 0);
   for (uint64_t k = 0; k <= limite; k++) { 
      atualiza_bps(p, k);
      visualiza_bps(p); 
      sleep(1); 
   }
   assert(p->esgotado);
}

void progresso_simples_varios_tamanhos() {
   const uint64_t limite = 30;

   ProgressoSimples pI = novo_bps(limite, 4);
   for (uint64_t k = 0; k <= limite; k++) { 
      atualiza_bps(pI, k);
      visualiza_bps(pI); 
      sleep(1); 
   }
   assert(pI->esgotado);

   ProgressoSimples pII = novo_bps(limite, 10);
   for (uint64_t k = 0; k <= limite; k++) { 
      atualiza_bps(pII, k);
      visualiza_bps(pII); 
      sleep(1); 
   }
   assert(pII->esgotado);

   ProgressoSimples pIII = novo_bps(limite, 20);
   for (uint64_t k = 0; k <= limite; k++) { 
      atualiza_bps(pIII, k);
      visualiza_bps(pIII); 
      sleep(1); 
   }
   assert(pIII->esgotado);
}

void progresso_temporal() {
   const uint64_t limite = 30;
   PT p = novo_bpt(limite, 32);

   for (uint64_t k = 0; k <= limite; k++) { 
      atualiza_bpt(p, k);
      visualiza_bpt(p); 
      sleep(1); 
   }
}

// execução dos testes:
int main() {
   executa_teste(
      "um progresso simples",
      progresso_simples
   );
   executa_teste(
      "PP onde barras tem variados tamanhos",
      progresso_simples_varios_tamanhos
   );
   executa_teste(
      "progresso temporal simples", 
      progresso_temporal
   );
   return 0;
}
