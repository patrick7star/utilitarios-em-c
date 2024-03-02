
typedef struct nodulo {
   // referência de qualquer tipo ao dado.
   void* dado;

   // referência ao próximo nódulo.
   struct nodulo* seta;

} nodulo_t;

nodulo_t* cria_nodulo(void* dado) {
   nodulo_t* instancia = malloc(sizeof(nodulo_t));
   
   if (instancia != NULL) {
      instancia->dado = dado;
      instancia->seta = NULL;
   }
   return instancia;
}

bool destroi_nodulo(nodulo_t* x) {
   if (x == NULL) return false;
   // tirando referência ao próximo e ao dado também.
   x->seta = NULL;
   x->dado = NULL;
   // liberando ...
   free(x);
   return true;
}

typedef struct {
   // quantificando quantos elementos ela realmente tem.
   size_t quantidade;

   // o ínicio da lista ligada.
   nodulo_t* topo;

} PilhaLigada;

PilhaLigada* cria_pl() {
   PilhaLigada* instancia = malloc(sizeof(PilhaLigada));

   if (instancia != NULL) {
      instancia->quantidade = 0;
      instancia->topo = NULL;
   }
   return instancia;
}

bool coloca_pl(PilhaLigada* s, void* dado) {
   // apenas aceita valores válidos.
   if (s == NULL || dado == NULL) return false;

   // inserindo no começo da lista(que é o topo da lista).
   nodulo_t* novo_item = cria_nodulo(dado);
   novo_item->seta = s->topo;
   s->topo = novo_item;
   // contabilizando inserção ...
   s->quantidade += 1;

   // confirma inserção.
   return true;
}

size_t tamanho_pl(PilhaLigada* s) { return s->quantidade; }
bool vazia_pl(PilhaLigada* s) { return tamanho_pl(s) == 0; }

void visualiza_pl(PilhaLigada* s) {
   if (vazia_pl(s)) { puts("pilha-ligada: []"); return; }

   nodulo_t* cursor = s->topo;

   printf("pilha-ligada: [");
   while (cursor != NULL) {
      printf("%d, ", *(int*)cursor->dado);
      cursor = cursor->seta;
   }
   puts("\b\b]");
}

void* retira_pl(PilhaLigada* s) {
   // abandona toda operação, não é possível continuar.
   if (vazia_pl(s)) { return NULL; }
   
   // referência o começo, a remoção em sí.
   nodulo_t* remocao = s->topo;
   // o começo passa então a referênciar o item depois do primeiro.
   s->topo = s->topo->seta;
   s->quantidade -= 1;
   // pegando dado antes de ele ser destruído, se for requisitado.
   void* dado = remocao->dado;


   // confirma remoção.
   destroi_nodulo(remocao);
   return dado;
}

void* topo_pl(PilhaLigada* s) {
   // abandona toda operação, não é possível continuar.
   if (vazia_pl(s)) { return NULL; }

   // pega o dado no primeiro item da lista.
   return s->topo->dado;
}

void visualiza_pilha_string(PilhaLigada* s) {
   if (vazia_pl(s)) { puts("pilha-ligada: []"); return; }

   nodulo_t* cursor = s->topo;

   printf("pilha-ligada: [");
   while (cursor != NULL) {
      printf("'%s', ", (char*)cursor->dado);
      cursor = cursor->seta;
   }
   puts("\b\b]");
}

int main(int total, char* args[], char* vars[]) {
   // permitindo impressões de caractéres 'latin1'.
   setlocale(LC_CTYPE, "pt_BR.UTF-8");

   PilhaLigada* stack = cria_pl();
   int array[] = {38, 1, -15};
   int* ptr;

   coloca_pl(stack, &array[1]);
   coloca_pl(stack, &array[0]);
   assert (coloca_pl(stack, &array[2]));

   visualiza_pl(stack);

   ptr = retira_pl(stack);
   printf("último valor: %d\n", *ptr);
   ptr = retira_pl(stack);
   printf("último valor: %d\n", *ptr);

   visualiza_pl(stack);

   int* qual_o_topo = topo_pl(stack);
   printf("topo da pilha: %d\n", *qual_o_topo);

   assert (tamanho_pl(stack) == 1);

   PilhaLigada* stack_ii = cria_pl();

   coloca_pl(stack_ii, "wealth and taste");
   visualiza_pilha_string(stack_ii);
   coloca_pl(stack_ii, "sympathy");
   visualiza_pilha_string(stack_ii);
   coloca_pl(stack_ii, "Anastasia screams in vain");
   visualiza_pilha_string(stack_ii);
   coloca_pl(stack_ii, "He said so tshirts");
   visualiza_pilha_string(stack_ii);

   assert (tamanho_pl(stack_ii) == 4);

   PilhaLigada* stack_iii = cria_pl();
   struct estranho { char letra; uint8_t unidades; };
   struct estranho d1 = {'F', 3};
   char d2 = 'M';
   uint32_t d3 = 9999;

   coloca_pl(stack_iii, &d1);
   coloca_pl(stack_iii, &d2);
   coloca_pl(stack_iii, &d3);
   assert (tamanho_pl(stack_iii) == 3);

   char* r3;
   struct estranho* r2;
   char letra = r2->letra;
   uint8_t units = r2->unidades;
   uint32_t* r1;

   r2 = retira_pl(stack_iii);
   assert (tamanho_pl(stack_iii) == 2);
   r3 = retira_pl(stack_iii);
   assert (tamanho_pl(stack_iii) == 1);
   r1 = retira_pl(stack_iii);
   assert (vazia_pl(stack_iii));

   printf("{%c, %i}, %u, %c\n", letra, units, *r1, *r3);

   // fim do programa.
   return EXIT_SUCCESS;
}
