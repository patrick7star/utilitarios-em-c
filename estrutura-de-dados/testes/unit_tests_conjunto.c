
// própria biblioteca.
#include "../../teste.h"
#include "../conjunto.h"

// biblioteca do C:
#include <assert.h>
#include <stdio.h>

char* entradas[] = {
   "banana", "maçã", "morango",
   "abacaxi", "uva", "maracujá",
   "laranja", "pêssego"
};

void criacao_da_instancia(void) {
   Conjunto x = cria_set();
   puts("conjunto criado com sucesso.");
   assert(vazio_set(x));
   puts("está vázio.");
   destroi_set(x);
   puts("o deslocamento foi sucedido.");
}

void insercoes_controladas() {
   Conjunto x = cria_set();
   visualiza_set(x);
   assert(vazio_set(x));

   insere_set(x, "banana");
   insere_set(x, "uva");
   insere_set(x, "morango");
   puts("todas inserções realizadas.");
   assert(!vazio_set(x));
   visualiza_set(x);

   destroi_set(x);
}

void remocao_de_todos_elementos() {
   Conjunto x = cria_set();
   printf("Conjunto(%s)\n", set_to_str(x));
   assert(vazio_set(x));

   for (int k = 0; k < 8; k++)
      insere_set(x, entradas[k]);
   assert(tamanho_set(x) == 8);
   visualiza_set(x);

   for (int k = 0; k < 8; k++) {
      bool removido = remove_set(x, entradas[k]);
      assert(removido);
      printf(
         "%s\tremoveu? %s\n",
         set_to_str(x), 
         bool_to_str(removido)
      );
   }

   visualiza_set(x);
   destroi_set(x);
}

void operacao_de_uniao_de_conjuntos() {
   Conjunto a = cria_set();
   Conjunto b = cria_set(); 
   Conjunto saida = cria_set();

   for (int k = 0; k < 8; k++) {
      Elemento e = entradas[k];
      if (k > 3)
         insere_set(a, e);
      if (k % 2 == 0)
         insere_set(b, e);
   }
   printf("A = %s\nB = %s\n", set_to_str(a), set_to_str(b));

   Conjunto a_uniao_b = uniao(a, b);
   visualiza_set(a_uniao_b);
   assert(eq_set(a_uniao_b, saida));
}

void clonagem_de_conjuntos() {
   Conjunto a = cria_set();

   for (int k = 0; k < 8; k++) {
      if (k % 2 == 0)
         insere_set(a, entradas[k]);
   }
   visualiza_set(a);

   Conjunto clone_de_a = clona_set(a);
   puts("clone gerado com sucesso.");
   visualiza_set(clone_de_a);
}

void iteracao_do_conjunto() {
   Conjunto a = cria_set();

   for (int k = 0; k < 8; k++) {
      if (k % 2 == 0)
         insere_set(a, entradas[k]);
   }
   visualiza_set(a);

   IterSet i = iter_set(a);
   puts("iterador criado com sucesso.");
   for (Elemento e = next_set(i); e != NULL; e = next_set(i)) 
      puts(e); 
   destroi_iter_set(i);
}

// execução dos testes:
void main(int argc, char** argv) {
   executa_testes(
      /* sempre lembrar de atualização o total se novo teste for 
       * adicionando/ou retirado. */
      6, criacao_da_instancia, true,
      insercoes_controladas, true,
      remocao_de_todos_elementos, false,
      operacao_de_uniao_de_conjuntos, true,  
      clonagem_de_conjuntos, true,
      iteracao_do_conjunto, true
   );
}
