// Biblioteca padrão do C:
#include <assert.h>
#include <locale.h>
// Módulos do projeto:
#include "dados-testes.h"
#include "teste.h"
#include "macros.h"
#include "primitivos.h"
#include "memoria.h"
#include "aleatorio.h"

void alimenta_mapa_com_frutas_e_precos_aleatorios(HashTable InOut)
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

TESTE metodo_de_clonagem(void)
{
   HashTable mapa = cria_ht(hash_string, eq_string);
   HashTable copia = NULL;
   char* key = NULL; int8_t* value = NULL;
   const int Y = sizeof(char*);
   int i = 0;

   for (i = 0; i < GIRLS_NAMES; i++)
   {
      key = (char*)girls_names[i];
      add_ht(mapa, key, box_i8(i));
   }

   printf("Total de elementos: %zu\n", len_ht(mapa));
   puts("Original:");
   print_ht(mapa, debug_string, debug_i8);
   copia = clona_ht(mapa);
   puts("Clonado com sucesso.");

   for (i = 0; i < GIRLS_NAMES / 2; i += 2)
   {
      key = (char*)girls_names[i];
      value = obtem_ht(mapa, key);
      *value += 10;
   }

   puts("Clone após modificação da original:");
   print_ht(copia, debug_string, debug_i8);
   drop_ht(mapa);
   drop_i_ht(copia, NULL, free_box);
}

TESTE varias_entradas_genericas_diferentes (void) {
   puts ("criando simples instância de entry ...");

   // chaves das entradas de todos tipos:
   wchar_t* string_chave = L"minha_chave";
   uint32_t inteiro_chave = 12;
   unsigned char char_chave = 'F';
   float decimal_chave = 2.73f;
   // valores da entrada:
   float decimal_valor = 3.14159;
   bool logico_valor = false;
   wchar_t* string_valor = L"feminino";
   int32_t inteiro_valor = -1000000;

   nodulo_t e = { string_chave, &decimal_valor };
   nodulo_t a = { &inteiro_chave, &logico_valor };
   nodulo_t b = { &char_chave, string_valor };
   nodulo_t c = { &decimal_chave, &inteiro_valor };

   puts ("imprimindo para confirmar:");
   printf (
      "#===>%12ls: %f\n\n", 
      (wchar_t*)e.chave, 
      *((float*)e.valor)
   );
   printf (
      "#===>%3u: %s\n\n", 
      *((uint32_t*)a.chave), 
      bool_to_str(*((bool*)a.valor))
   );
   printf (
      "#===>%2c: %ls\n\n", 
      *((unsigned char*)b.chave), 
      (wchar_t*)b.valor
   );

   printf (
      "#===>%9f: %i\n\n",
      *((float*)c.chave),
      *((int32_t*)c.valor)
   );
}

bool iguais_string (generico_t a, generico_t b) { 
   return wcscmp ((wchar_t*)a, (wchar_t*)b) == 0; 
}

void alocao_e_desacalocao_simples_instancia (void) {
   HashTable mapa = cria_ht (hash_string, iguais_string);
   destroi_ht (mapa);
}

void visualiza_interna (HashTable m) {
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
            float* vl = atual->valor;
            wchar_t* key = atual->chave;
            printf ("'%ls': %0.3f, ", key, *vl);
            atual = atual->seta;
         } while (atual != NULL);
         puts ("\b\b>");
      }
   }
}

struct chave_valor { wchar_t* key; float value; };

struct chave_valor entradas[] = {
   {L"laranja", 1.59}, {L"uva", 3.85},
   {L"pêssego", 5.99}, {L"melância", 10.15},
   {L"banana", 4.31}, {L"abacate", 0.80},
   {L"maçã", 2.50}
};

void aplicacao_de_simples_insercoes (void) {
   HashTable mapa = cria_ht (hash_string, iguais_string);

   assert (vazia_ht (mapa));
   for (size_t i = 1; i <= 7; i++) {
      float* ptr_value = &entradas[i - 1].value;
      wchar_t* ptr_key = entradas[i - 1].key;
      assert (insere_ht (mapa, ptr_key, ptr_value));
   }
   assert (tamanho_ht (mapa) == 7);
   visualiza_interna (mapa);

   destroi_ht (mapa);
}

void verifica_operacao_de_pertencimento (void) {
   HashTable mapa = cria_ht (hash_string, iguais_string);

   for (size_t i = 1; i <= 7; i++) {
      float* ptr_value = &entradas[i - 1].value;
      wchar_t* ptr_key = entradas[i - 1].key;
      assert (insere_ht (mapa, ptr_key, ptr_value));
   }
   visualiza_interna (mapa);

   uint8_t indices[] = { 5, 1, 4, 6, 3, 2 };

   for (size_t i = 1; i <= 7; i++) {
      size_t p = indices[i - 1];
      wchar_t* ptr_key = entradas[i - 1].key;
      printf ("atual chave '%ls' ...", ptr_key);
      assert (contem_ht (mapa, ptr_key));
      puts ("tem.");
   }

   destroi_ht (mapa);
}

void ascii_code_de_wide_strings (void) {
   wchar_t string[] = L"ármario";
   for (size_t i = 1; i <= 7; i++) 
      printf ("'%lc' -- %u\n", string[i - 1], (uint32_t)*(string + i - 1));
}

void simples_atualizacoes_de_alguns_valores (void) {
   HashTable mapa = cria_ht (hash_string, iguais_string);

   for (size_t i = 1; i <= 7; i++) {
      float* ptr_value = &entradas[i - 1].value;
      wchar_t* ptr_key = entradas[i - 1].key;
      assert (insere_ht (mapa, ptr_key, ptr_value));
   }
   size_t total = tamanho_ht (mapa);
   visualiza_interna (mapa);

   puts ("dobrando os preços...");
   for (size_t i = 1; i <= 7; i++) {
      size_t p = i - 1;
      struct chave_valor E = entradas[p];
      wchar_t* key = E.key;
      float* value = malloc (sizeof (float));
      *value = 2 * E.value;

      assert (atualiza_ht (mapa, key, value));
      puts ("atualização ocorreu corretamente!");
   }
   visualiza_interna (mapa);

   // a quantia interna não mudou.
   assert (tamanho_ht (mapa) == total);
   destroi_ht (mapa);
}

void visualiza_mapa_wchar_e_float (HashTable m) {
   size_t cP = m->capacidade;

   printf ("{..");
   for (size_t p = 1; p <= cP; p++) {
      nodulo_t* lista = m->locais[p - 1];

      while (lista != NULL) {
         wchar_t* k = lista->chave;
         float* v = lista->valor;

         printf ("%ls: %2.1f, ", k, *v);

         lista = lista->seta;
      }
   }
   puts ("\b\b}");
}

void algumas_remocoes_feitas (void) {
   HashTable mapa = cria_ht (hash_string, iguais_string);

   for (size_t i = 1; i <= 7; i++) {
      float* ptr_value = &entradas[i - 1].value;
      wchar_t* ptr_key = entradas[i - 1].key;
      assert (insere_ht (mapa, ptr_key, ptr_value));
   }
   puts ("\n .. .. .....antes .. .. .. ......");
   visualiza_interna (mapa);

   size_t antes = tamanho_ht (mapa);
   wchar_t* key = entradas[3].key;
   printf ("removendo a chave: '%ls'\n", key);
   assert (deleta_ht (mapa, key));
   assert (antes > tamanho_ht(mapa));
   visualiza_interna (mapa);

   /* Observação: Está com dificuldade de remover itens na primeira
    * casa da lista encadeada. */
   puts ("\n .. .. ... depois .. .. .. ......");
   antes = tamanho_ht (mapa);
   key = entradas[6].key;
   printf ("removendo a chave: '%ls'\n", key);
   assert (deleta_ht (mapa, key));
   assert (antes > tamanho_ht(mapa));
   visualiza_interna (mapa);

   antes = tamanho_ht (mapa);
   key = entradas[4].key;
   assert (deleta_ht (mapa, key));
   visualiza_mapa_wchar_e_float (mapa);
   key = entradas[1].key;
   assert (deleta_ht (mapa, key));
   visualiza_mapa_wchar_e_float (mapa);
   key = entradas[2].key;
   assert (deleta_ht (mapa, key));
   visualiza_mapa_wchar_e_float (mapa);
   assert (antes == 3 + tamanho_ht(mapa));

   destroi_ht (mapa);
}

size_t hash_int (generico_t dt, size_t cp) {
   uint16_t* ptr = dt;
   uint16_t chave = *ptr;
   // este não leva em conta o endereço virtual de memória do argumento.
   return  chave * (chave - chave / 2) % cp;
}

bool int_eq (generico_t a, generico_t b) 
   { return *((uint16_t*)a) == *((uint16_t*)b); }

void visualizacao_mapa_u16_e_str (HashTable m) {
   size_t cP = m->capacidade;

   printf ("{..");
   for (size_t p = 1; p <= cP; p++) {
      nodulo_t* lista = m->locais[p - 1];

      while (lista != NULL) {
         uint16_t* k = lista->chave;
         char* v = lista->valor;

         printf ("%u: '%s', ", *k, v);
         lista = lista->seta;
      }
   }
   puts ("\b\b}");
}

void operacoes_negadas (void) {
   uint16_t* amostras = (uint16_t*)valores_padronizados_i;
   // inserer, resgatar, e remover até não poder mais...
   HashTable M = cria_ht(hash_int, int_eq);
   assert (vazia_ht(M));

   for (size_t p = 1; p <= 8; p++) {
      uint16_t* key = (uint16_t*)&amostras[p - 1]; 
      char* value = (char*)legumes[p - 1];
      // insere_ht (M, &amostras[p - 1], legumes[p - 1]);
      insere_ht (M, key, value);
   }

   visualizacao_mapa_u16_e_str (M);
   assert (tamanho_ht(M) == 8);

   puts ("novo lotes de inserções, com mesmas chaves negados:");
   for (size_t p = 1; p <= 8; p++) {
      char* value = (char*)frutas[p - 1];
      uint16_t* key =  &amostras[p - 1];
      insere_ht (M, key, value);
   }
   assert (tamanho_ht(M) == 8);

   puts ("agora, apesar de negações, inserindo algumas...");
   size_t negacoes = 0;
   for (size_t p = 5; p <= 13; p++) {
      uint16_t* key =  amostras + p;
      char* vl = (char*)frutas[p - 5];
      // bool foi_inserido = insere_ht (M, &amostras[p], frutas[p - 5]);
      bool foi_inserido = insere_ht (M, key, vl);
      if (!foi_inserido)
         negacoes++;
   }
   printf ("houves %lu negações de inserção.\n",negacoes);
   printf ("há agora %lu items.\n", tamanho_ht (M));
   visualizacao_mapa_u16_e_str (M);

    
   // tentando obter chaves inexistentes ...
   uint16_t chaves_inexistentes[] = {
      918, 9,  380,  88, 832, 
      111, 22, 44, 100, 56, 3
   };

   puts ("\ntestando método 'obter(get)' valores de algumas 'chaves'.");
   for (size_t p = 1; p <= 10; p++) {
      size_t q = p - 1;
      uint16_t* key = &chaves_inexistentes[q];
      char* vl = obtem_ht (M, key);

      if (vl != NULL)
         printf ("chave %u existe, e tem valor '%s'.\n", *key, vl);
      else 
         printf ("chave %u não existe!\n", *key);
   }

   // tentando deletar todas chaves, e muito mais.
   negacoes = 0;
   puts ("\nlimpando a 'tabela'...");
   for (size_t p = 36; p >= 1; p--) {
      uint16_t* key = &amostras[p - 1];
      bool removido = deleta_ht (M, key);

      if (removido)
         visualizacao_mapa_u16_e_str (M);
      else {
         printf ("não foi possível remover %u.\n", *key);
         negacoes++;
      }
   }
   printf ("%lu foram negadas.\n", negacoes);

   destroi_ht (M);
}

void metodo_get_verificacao_basica (void) {
   uint16_t* amostras = (uint16_t*)valores_padronizados_i;
   // inserer, resgatar, e remover até não poder mais...
   HashTable M = cria_ht(hash_int, int_eq);
   assert (vazia_ht(M));

   for (size_t p = 1; p <= 8; p++) {
      uint16_t* key = (uint16_t*)&amostras[p - 1]; 
      char* value = (char*)legumes[p - 1];
      insere_ht (M, key, value);
      // insere_ht (M, &amostras[p - 1], legumes[p - 1]);
   }

   visualizacao_mapa_u16_e_str (M);
   assert (tamanho_ht(M) == 8);

   uint16_t chave = 1;
   char* valor_i = obtem_ht (M, &chave);
   chave = 9;
   char* valor_ii = obtem_ht (M, &chave);

   printf ("valores pegos: '%s' e '%s'\n", valor_i, valor_ii);

   // tentando acessar valores inválidos ...
   chave = 37;
   assert (obtem_ht (M, &chave) == NULL);
   chave = 43;
   assert (obtem_ht (M, &chave) == NULL);
   char* valor_iii; 
   chave = 99;
   valor_iii = obtem_ht (M, &chave);
   assert ( valor_iii != NULL);
   printf ("último pego(key=%u): '%s'\n", chave, valor_iii);
   chave = 98;
   assert (obtem_ht (M, &chave) == NULL);
   puts ("chaves inválidas produziram um valor 'null'.");

   destroi_ht (M);
}
