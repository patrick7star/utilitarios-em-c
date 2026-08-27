struct Iteracao_da_Hashtable_Ref {
   // contador de itens iterados.
   size_t contagem;
   // posição atual na array.
   size_t indice;

   // referência ao atual item da 'tabela' referenciado.
   nodulo_t* cursor;

   // garantidor de que a 'tabela' não foi alterada.
   size_t inicialmente;
   HashTable instancia;

};

// O valor da iteração, quando não é mais possível consumir, será este.
const IterOutputHT NULO_HT = {NULL, NULL};

IterHT cria_iter_ht (HashTable m) {
   const int sz_iter = sizeof(struct Iteracao_da_Hashtable_Ref);
   nodulo_t* no = m->locais[0];
   IterHT self = malloc (sz_iter);

   if (self != NULL) {
      self->contagem = 0;
      self->indice = 0;
      self->cursor = no;
      self->inicialmente = tamanho_ht (m);
      // referência ao próprio mapa, dicio, table,... como quiser chamar.
      self->instancia = m;
   }
   return self;
}

static bool iterador_valido (IterHT iter) {
/* Para realizar qualquer uma das operações abaixo, é necessário
 * que a instância seja válida, ou seja, ainda existe, ou tem o mesmo
 * tamanho que na criação da instância. */
   size_t T = tamanho_ht(iter->instancia);
   bool referencia_existe = (iter->instancia != NULL);
   return (iter->inicialmente == T && referencia_existe);
}

size_t contagem_iter_ht (IterHT iter) {
/* O restante de iterações é calculado na seguinte forma: total de itens 
 * na "lista" menos os já iterados. */
   if (iterador_valido (iter))
      return iter->inicialmente - iter->contagem;

   // se chegar até aqui é erro na certa.
   perror ("não é possível determinar o tamanho de um iterador inválido!");
   abort();
}

IterOutputHT next_ht (IterHT iter) {
/* O algoritmo que pega o próximo item da iteração. Seu modo de funcionar
 * é o seguinte; vai iterando o cada posição na array de listas-ligadas,
 * se não houver nada nela(sem lista), ele pula para o próximo índice 
 * nela, se houver um nó, seguir a lista ligada. Cada iteração com um
 * item válido(nó) é contabilizado. */
   if (!iterador_valido(iter))
      return NULO_HT;
   else if (contagem_iter_ht(iter) == 0)
      return NULO_HT;

   nodulo_t* atual = iter->cursor;
   if (atual != INVALIDA) {
      // colhendo dados necessários ...
      generico_t vl = atual->valor;
      generico_t ch = atual->chave;

      // movendo pela lista ...
      iter->cursor = iter->cursor->seta;
      // contabiliza iteração.
      iter->contagem++;

      // retorna item "cholido".
      return (IterOutputHT){.key = ch, .value=vl };
   } else  {
      iter->indice += 1;
      /* primeiro 'nódulo' da lista abaixo. */
      iter->cursor = iter->instancia->locais[iter->indice];
      // chama a função recursivamente novamente...
      return next_ht (iter);
   }
}

bool consumido_iter_ht(IterHT iter) 
// Diz se o iterador se esgotou(contagem atingiu valor inicial).
   { return iter->contagem == iter->inicialmente; }

IterHT clona_iter_ht(IterHT iter) {
/* Clona o iterador passado, à partir do estágio que está. A alteração 
 * deste novo clone, ou do original, não alteram a iteração de cada, más 
 * sim, a mudança da estrutura original, que não permite ambos realizar 
 * mais iterações. */ 
   IterHT novo = cria_iter_ht(iter->instancia);

   // Copiando informações:
   if (novo != NULL) {
      novo->instancia = iter->instancia;
      novo->contagem = iter->contagem;
      novo->inicialmente = tamanho_ht(iter->instancia);
      // Camos internos para iteração(até mais importante que os acimas):
      novo->cursor = iter->cursor;
      novo->indice = iter->indice;
   }
   return novo;
}

void destroi_iter_ht(IterHT iter) {
   iter->cursor = NULL;
   iter->instancia = NULL;
   free(iter);
}

/*   Os testes de itaradores foram retirados do submódulo principal que 
 * é destinado a todos os demais testes unitários em base de um grande 
 * experimento. Funcionou, porém depois de alguns retoques. Estes módulos 
 * não são nada mais do que anexos do arquivo principal, portanto a ordem
 * que são retirados e colocados no principal importa. Aqui os problemas 
 * foram resolvidos com algumas declarações das funções compartilhadas. */
#ifdef __unit_tests__
#include "teste.h"
#include "dados-testes.h"
#include "memoria.h"
#include "primitivos.h"
#include "aleatorio.h"

static void visualizacao_mapa_u16_e_str (HashTable); 

static void print_item_ht_u16_e_str (IterOutputHT x) {
   printf (" ==> %u: %s\n", *((uint16_t*)x.key), (char*)x.value);
}

static void print_inner_u16_e_str (HashTable m) {
   puts ("\nHashTable visualização interna:");
   size_t C = m->capacidade;

   for (size_t i = 1; i <= C; i++) {
      nodulo_t* lista = m->locais[i - 1];
      if (lista == INVALIDA)
         printf ("\t---\n");
      else {
         nodulo_t* atual = lista;
         printf ("\t<");
         do {
            char* vl = atual->valor;
            uint16_t* key = atual->chave;
            printf ("%u: '%s', ", *key, vl);
            atual = atual->seta;
         } while (atual != NULL);
         puts ("\b\b>");
      }
   }
}

static void alimenta_mapa_com_frutas_e_precos_aleatorios(HashTable InOut)
{
   HashTable mapa = InOut;
   int inteiro, n;
   float decimal;
   char* chave = NULL;
   
   for (n = 0; n < FRUITS; n++)
   {
      inteiro = inteiro_positivo(1, 1e4);
      decimal = (float)inteiro / 1000.0;
      chave = (char*)fruits[n];

      add_ht(mapa, chave, box_float(decimal));
   }
}

TESTE transporte_de_hashtable_para_array(void)
{
   HashTable mapa = NULL; 
   IterOutputHT * array = NULL;
   const int size = sizeof(IterOutputHT);
   int quantia, n = 0;
   IterHT iter = NULL;
   char* chave; float* valor;

   mapa = new_ht(hash_string, eq_string); 

   alimenta_mapa_com_frutas_e_precos_aleatorios(mapa);
   print_ht(mapa, debug_string, debug_f32);

   iter = new_iter_ht(mapa);
   quantia = len_ht(mapa);
   array = malloc(quantia * size);

   while (!consumido_iter_ht(iter))
      array[n++] = next_ht(iter);

   printf("n: %d | quantia: %d\n", n, quantia);
   assert(n == quantia);
   drop_iter_ht(iter);

   printf("\nVisualizando conteúdo da array[%d]...\n", quantia);

   for (n = 0; n < quantia; n++)
   {
      chave = (char*)array[n].key;
      valor = (float*)array[n].value;
      printf("\t\b\b\b%s ===> U$ %2.2f\n", chave, *valor);
   }

   drop_i_ht(mapa, NULL, free_box);
}

TEST uso_simples_da_iteracao (void)
{
   HashTable M = cria_ht(hash_u16, eq_u16);
   uint16_t* amostras = (uint16_t*)valores_padronizados_i;

   for (size_t p = 1; p <= 9; p++) 
   {
      uint16_t* key = (uint16_t*)&amostras[p - 1]; 
      char* value = (char*)legumes[p - 1];
      insere_ht (M, key, value);
   }
   for (size_t p = 9; p <= 19; p++) 
   {
      uint16_t* key = (uint16_t*)&amostras[p - 1]; 
      char* value = (char*)boys_names[p - 9 - 1];
      insere_ht (M, key, value);
   }
   visualizacao_mapa_u16_e_str (M);
   print_inner_u16_e_str (M);

   IterHT I = cria_iter_ht (M);
   printf ("contagem em %lu ...\n", contagem_iter_ht(I));

   for (size_t count = contagem_iter_ht(I); count > 0; count--) 
   {
      IterOutputHT i = next_ht (I);
      assert (i.key != NULL && i.value != NULL);
      print_item_ht_u16_e_str (i);
      printf ("contagem em %lu ...\n", contagem_iter_ht(I));
   }

   printf ("tetando iterar mesmo esgotado ...");
   IterOutputHT i = next_ht (I);
   assert (i.key == NULL && i.value == NULL);
   i = next_ht (I);
   assert (i.key == NULL && i.value == NULL);
   i = next_ht (I);
   assert (i.key == NULL && i.value == NULL);
   puts ("não funcionou!");
   destroi_ht (M);
}

TESTE tentando_iterador_mapa_vazio (void) {
   HashTable M = cria_ht(hash_u16, eq_u16);
   IterHT I = cria_iter_ht (M);

   IterOutputHT i = next_ht (I);
   printf ("contagem em %lu ...\n", contagem_iter_ht(I));
   assert (i.key == NULL && i.value == NULL);

   i = next_ht (I);
   printf ("contagem em %lu ...\n", contagem_iter_ht(I));
   assert (i.key == NULL && i.value == NULL);

   i = next_ht (I);
   printf ("contagem em %lu ...\n", contagem_iter_ht(I));
   assert (i.key == NULL && i.value == NULL);

   puts ("não funcionou com nenhuma!");
   destroi_iter_ht(I);
   destroi_ht (M);
}
#endif
