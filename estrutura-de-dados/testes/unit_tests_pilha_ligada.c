
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../../teste.h"
#include "../pilha_ligada.h"
#include "../dado.h"
#include "../../aleatorio.h"

void instancia_basica() {
   PILHA_LIGADA p = cria_pl();
   destroi_pl(p);
}

void algumas_operacoes_simples() {
   PILHA_LIGADA p = cria_pl();
   visualiza_pl(p);
   assert(vazia_pl(p));

   coloca_pl(p, cria_dado('M'));
   visualiza_pl(p);
   assert(quantidade_pl(p) == 1);

   coloca_pl(p, cria_dado('a'));
   assert(dados_iguais(cria_dado('a'), topo_pl(p)));
   coloca_pl(p, cria_dado('J'));
   assert(quantidade_pl(p) == 3);
   visualiza_pl(p);
   // imprime_dl(p->lista);

   coloca_pl(p, cria_dado('m'));
   coloca_pl(p, cria_dado('k'));
   coloca_pl(p, cria_dado('Y'));
   coloca_pl(p, cria_dado('p'));
   assert(quantidade_pl(p) == 7);
   visualiza_pl(p);

   // topo funciona.
   assert(!dados_iguais(cria_dado('L'), topo_pl(p)));
   assert(dados_iguais(cria_dado('p'), topo_pl(p)));


   while (!vazia_pl(p)) {
      printf(
         "dado removido '%s'\n", 
         dado_para_string(retira_pl(p))
      );
      visualiza_pl(p);
   }

   destroi_pl(p);
}

void impressao_e_formatacao_string() {
   char entradas[20] = "aMjlKyApqR";
   PILHA_LIGADA p = cria_pl();

   for (int i = 0; i< strlen(entradas); i++) {
      char _char = entradas[i];
      printf("'%c' >>> %s\n", _char, pl_to_str(p));
      Dado data = cria_dado(_char);
      coloca_pl(p, data);
      assert(dados_iguais(data, topo_pl(p)));
   }
   visualiza_pl(p);

   while (!vazia_pl(p)) {
      printf(
         "dado removido '%s'\n", 
         dado_para_string(retira_pl(p))
      );
      visualiza_pl(p);
   }

   destroi_pl(p);
}

void iteracao_da_pilha() {
   PILHA_LIGADA P = cria_pl();

   for (int p = 0; p < 15; p++) {
      unsigned char _char = alfabeto_aleatorio();
      coloca_pl(P, cria_dado(_char));
   }
   visualiza_pl(P);

   IterPL iter = cria_iter_pl(P);
   Dado atual = next_pl(iter);

   while (atual != NULL) {
      printf(
         "%s(%lu) -- ", 
         dado_para_string(atual),
         contagem_iter_pl(iter)
      );
      atual = next_pl(iter);
   }
   printf("\b\b\b\n");

   visualiza_pl(P);
   destroi_iter_pl(iter);
   destroi_pl(P);
}

void main() {
   executa_testes(
      4, instancia_basica, true,
      algumas_operacoes_simples, true,
      impressao_e_formatacao_string, true,
      iteracao_da_pilha, true
   );
}
