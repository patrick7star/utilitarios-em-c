
// própria biblioteca.
#include "../deque_ligada.h"
#include "../../teste.h"

// biblioteca do C:
#include <assert.h>
#include <stdio.h>


void funcao_de_pertencimento() {
   DequeLigada l = cria_dl();
   assert(vazia_dl(l));

   insere_traseira_dl(l, cria_dado('m'));

   insere_traseira_dl(l, cria_dado('a'));
   insere_traseira_dl(l, cria_dado('b'));
   insere_traseira_dl(l, cria_dado('c'));

   // não contém tal.
   assert(!contem_dl(l, cria_dado('z')));


   insere_comeco_dl(l, cria_dado('x'));
   insere_comeco_dl(l, cria_dado('y'));

   // contém tais dados.
   assert(contem_dl(l, cria_dado('y')));
   assert(contem_dl(l, cria_dado('x')));

   // ainda não inserido.
   assert(!contem_dl(l, cria_dado('e')));

   insere_traseira_dl(l, cria_dado('d'));
   insere_traseira_dl(l, cria_dado('e'));

   // 'e' já está inserido, portanto, contido na lista.
   assert(contem_dl(l, cria_dado('e')));

   // ainda para conter tal 'w' e 'z' ...
   assert(!contem_dl(l, cria_dado('z')));
   assert(!contem_dl(l, cria_dado('w')));

   insere_comeco_dl(l, cria_dado('w'));
   insere_comeco_dl(l, cria_dado('z'));

   // ainda para conter tal 'w' e 'z' ...
   assert(contem_dl(l, cria_dado('z')));
   assert(contem_dl(l, cria_dado('w')));

   imprime_dl(l);

   // validações com remoções agora ...

   Dado r = remove_comeco_dl(l);
   printf("removido: %s\n", dado_para_string(r));
   // não tem mais o 'z'.
   assert(!contem_dl(l, cria_dado('z')));

   imprime_dl(l);

   // visualiza_dado(remove_traseira_dl(l));
   r = remove_traseira_dl(l);
   printf("removido: %s\n", dado_para_string(r));
   // não tem mais o 'e'.
   //assert(!contem_dl(l, cria_dado('e')));

   imprime_dl(l);
   destroi_dl(l);
}

void insercao_simples() {
   DequeLigada l = cria_dl();
   assert(vazia_dl(l));
   imprime_dl(l);

   insere_traseira_dl(l, cria_dado('m'));
   assert(tamanho_dl(l) == 1);
   insere_traseira_dl(l, cria_dado('a'));
   assert(tamanho_dl(l) == 2);
   imprime_dl(l);

   insere_comeco_dl(l, cria_dado('x'));
   insere_traseira_dl(l, cria_dado('b'));
   insere_traseira_dl(l, cria_dado('c'));
   insere_comeco_dl(l, cria_dado('y'));

   destroi_dl(l);
}

void remocoes_simples() {
   DequeLigada l = cria_dl();
   assert(vazia_dl(l));
   imprime_dl(l);

   insere_traseira_dl(l, cria_dado('m'));
   insere_traseira_dl(l, cria_dado('a'));
   insere_comeco_dl(l, cria_dado('x'));
   insere_comeco_dl(l, cria_dado('y'));

   imprime_dl(l);

   Dado r = remove_traseira_dl(l);
   printf("dado removido: '%s'\n", dado_para_string(r));

   imprime_dl(l);

   destroi_dl(l);
}

void remocao_de_dado_especifico() {
   DequeLigada l = cria_dl();

   insere_traseira_dl(l, cria_dado('f'));
   insere_traseira_dl(l, cria_dado('a'));
   insere_comeco_dl(l, cria_dado('d'));
   insere_traseira_dl(l, cria_dado('b'));
   insere_traseira_dl(l, cria_dado('c'));
   insere_comeco_dl(l, cria_dado('e'));
   assert(tamanho_dl(l) == 6);
   imprime_dl(l);

   assert(remove_dl(l, cria_dado('f')));
   imprime_dl(l);

   assert(remove_dl(l, cria_dado('c')));
   assert(tamanho_dl(l) == 4);
   imprime_dl(l);

   assert(remove_dl(l, cria_dado('e')));
   assert(remove_dl(l, cria_dado('a')));
   assert(tamanho_dl(l) == 2);
   imprime_dl(l);

   destroi_dl(l);
}

#include "../../aleatorio.h"
#include <time.h>
#include <stdlib.h>

void insercao_randomica(DequeLigada l, unsigned short q) {
   while (q > 0) {
      unsigned char letra = alfabeto_aleatorio();
      insere_traseira_dl(l, cria_dado(letra));
      q--;
   }
}

void instanciando_uma_array_de_deques() {
   const uint32_t max = 15;
   ARRAY_DL array = cria_array_dl(max);

   srand(time(NULL));
   for(int p = 0; p < max; p++) {
      assert(vazia_dl(array[p]));
      int t = 5 + rand() % (20 - 5);
      insercao_randomica(array[p], t);
   }

   // visualizando o feito.
   visualizacao_array_dl(array, 15);
   destroi_array_dl(array, max);
}

void iteracao_simples() {
   DequeLigada l = cria_dl();

   insere_traseira_dl(l, cria_dado('m'));
   insere_traseira_dl(l, cria_dado('a'));
   insere_comeco_dl(l, cria_dado('y'));
   insere_comeco_dl(l, cria_dado('x'));
   insere_traseira_dl(l, cria_dado('b'));
   insere_traseira_dl(l, cria_dado('c'));

   imprime_dl(l);

   IterDL iterador = iter_dl(l);

   Dado d = next_dl(iterador);
   while (d != NULL) {
      printf("dado: '%s'\n", dado_para_string(d));
      d = next_dl(iterador);
   }

   // próximo chamadas serão inválidas.
   printf("dado: '%s'\n", dado_para_string(d));
   d = next_dl(iterador);
   assert ( next_dl(iterador) == NULL );

   destroi_dl(l);
}

void iteracao_array_de_deques() {
   const uint32_t max = 8;
   ARRAY_DL array = cria_array_dl(max);

   srand(time(NULL));
   for(int p = 0; p < max; p++) {
      assert(vazia_dl(array[p]));
      int t = 5 + rand() % (20 - 5);
      insercao_randomica(array[p], t);
   }

   // visualizando o feito.
   IterDequeLigada iterador;
   for(int k = 0; k < max; k++) {
      imprime_dl(array[k]); 
      IterDL iterador = iter_dl(array[k]);

      do {
         Dado item = next_dl(iterador);
         printf(
            "\b\b\t%s\t(%lu)\n", 
            dado_para_string(item),
            iter_tamanho_dl(iterador)
         );
      } while (iter_tamanho_dl(iterador) > 0);

      destroi_iter_dl(iterador);
   }

   destroi_array_dl(array, max);
}

void clonagem_de_uma_dequeligada(){
   srand(time(NULL));
   DequeLigada l = cria_dl();
   insercao_randomica(l, rand() % 20 + 7);
   imprime_dl(l);
   DequeLigada clone_de_l = clona_dl(l);
   imprime_dl(clone_de_l);
   assert (tamanho_dl(l) == tamanho_dl(clone_de_l));
   destroi_dl(l);
   destroi_dl(clone_de_l);


   puts("terminado com sucesso!");
}

void clonagem_de_arrays_de_deques() {
   const uint32_t max = 6;
   ARRAY_DL array = cria_array_dl(max);

   srand(time(NULL));
   for(int p = 0; p < max; p++) {
      assert(vazia_dl(array[p]));
      int t = 5 + rand() % (20 - 5);
      insercao_randomica(array[p], t);
   }

   puts("orginal:");
   visualizacao_array_dl(array, max);

   // fazendo clonagem, e destruindo original, já que, ele já
   // foi impresso.
   ARRAY_DL copia = clona_array_dl(array, max);
   destroi_array_dl(array, max);

   puts("clone:");
   visualizacao_array_dl(copia, max);

   destroi_array_dl(copia, max);
}


// execução dos testes:
void main() {
   executa_teste(
      "operação se contém tal dado",
      funcao_de_pertencimento
   );
   executa_teste(
      "inserções em ambas pontas",
      insercao_simples
   );
   executa_teste_interruptor(
      "cuidando das remoções ...",
      remocoes_simples, true
   );
   executa_teste_interruptor(
      "remoção de dado específico",
      remocao_de_dado_especifico, true
   );

   executa_teste_interruptor(
      "criação e destruição de array de deques-ligadas",
      instanciando_uma_array_de_deques, true
   );

   executa_teste_interruptor(
      "tentativa de instanciar e usar o iterador",
      iteracao_simples, true
   );

   executa_teste_interruptor(
      "iteração de uma deque-ligada, na array de deques",
      iteracao_array_de_deques, true
   );

   executa_testes(
      2, clonagem_de_uma_dequeligada, true,
      clonagem_de_arrays_de_deques, true
   );
}
