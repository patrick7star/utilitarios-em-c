#include <assert.h>
#include "teste.h"
#include "dados_testes.h"
#include "memoria.h"
#include "primitivos.h"

static bool free_str(generico_t e);
static char* generico_to_str(Generico a);
static void visualizacao_array_list_char(vetor_t* l); 
static void visualizacao_array_list_int(vetor_t* l); 
static void visualiza_array_list_string(vetor_t* l); 

TESTE demonstracao_com_caracteres(void);
TESTE demonstracao_com_inteiros(void);
TESTE demonstracao_com_strings(void);
TESTE remocao_em_pontos_criticos (void);
TESTE redimensionamento_automatico_da_capacidade(void); 
TESTE conversao_em_string(void); 
TESTE novo_tipo_de_criacao_da_lista(void); 
TESTE uso_para_chacagem_do_funcionmanento_do_iterador(void); 
TESTE desalocador_interno(void);
TESTE impressao_de_lista(void); 

int main(int total, char* argumentos[], char* variaveis[]) 
{
   executa_testes_a (
     false, 4,
         remocao_em_pontos_criticos, false,
         redimensionamento_automatico_da_capacidade, false,
         conversao_em_string, true
   );

   executa_testes_b(
   // Testes sobre as alocações e desalocações, todas as modalidades é claro.
      true, 5,
         Unit(demonstracao_com_inteiros, true),
         Unit(demonstracao_com_caracteres, false),
         Unit(demonstracao_com_strings, false),
         Unit(novo_tipo_de_criacao_da_lista, true),
         Unit(desalocador_interno, true)
   );

   executa_testes_b(
   // Testes específico dos iterador da estrutura:
      true, 2,
         Unit(uso_para_chacagem_do_funcionmanento_do_iterador, true),
         Unit(impressao_de_lista, true)
   );

   return EXIT_SUCCESS;
}

TESTE demonstracao_com_caracteres(void) {
   vetor_t* lista = cria_com_capacidade_al(15);
   char m = 'x', n = 'A', p = 'J';
   puts("alocação ocorreu1!!!");

   insere_al(lista, &p);
   insere_al(lista, &n);
   insere_al(lista, &m);
   visualizacao_array_list_char(lista);

   char* removido = remove_al(lista);
   printf ("removido: '%c'\n", *removido);
   removido = remove_al(lista);
   printf ("removido: '%c'\n", *removido);

   assert (tamanho_al(lista) == 1);
   assert (*((char*)remove_al(lista)) == 'J');
   assert (vazia_al(lista));

   destroi_al(lista);
}

TESTE demonstracao_com_inteiros(void) {
   ArrayLista outra_lista = cria_al();
   int entradas[] = {39, 73, 15, 101};

   for (size_t k = 1; k <= 4; k++)
      insere_al (outra_lista, &entradas[k - 1]);

   while (!vazia_al(outra_lista)) {
      printf ("removido: %d\n", *((int*)remove_al(outra_lista)));
      visualizacao_array_list_int(outra_lista);
   }

   destroi_al(outra_lista);
}

TESTE demonstracao_com_strings(void) {
   ArrayLista lista = cria_al();
   char* entradas[] = { "cerca", "porta", "cadeado", "mala"};

   for (size_t k = 1; k <= 4; k++)
      insere_al (lista, entradas[k - 1]);

   while (!vazia_al(lista)) {
      char* string = remove_al(lista);
      printf ("removido: '%s'\n", string);
   }
   destroi_al(lista);
}

TESTE remocao_em_pontos_criticos (void) {
   ArrayLista lista = cria_al();
   char* entradas[] = { "cerca", "porta", "cadeado", "mala", "bolsa"};

   for (size_t k = 1; k <= 5; k++)
      insere_al (lista, entradas[k - 1]);
   visualiza_array_list_string (lista);

   puts ("removendo o terceiro(cadeado) ...");
   remove_indice_al (lista, 2);
   visualiza_array_list_string (lista);

   puts ("removendo o primeiro(cerca) ...");
   remove_indice_al (lista, 0);
   visualiza_array_list_string (lista);

   destroi_al(lista);
}

TESTE redimensionamento_automatico_da_capacidade(void) {
   vetor_t* L = cria_com_capacidade_al(4);

   puts("observação da expansão da capacidade ...");
   for (size_t k = 1; k <= 5; k++) {
      size_t t = tamanho_al(L);
      // coputando a capacidade.
      size_t C = vacuo_al(L) + t;
      printf("\tcapacidade: %lu\ttotal de itens: %lu\n", C, t);

      char* input = (char*)objetos[k - 1];
      insere_al (L, input);
   }
   putchar('\t'); visualiza_array_list_string(L);

   for (size_t p = 1; p <= 5; p++) {
      size_t t = tamanho_al(L);
      // coputando a capacidade.
      size_t C = vacuo_al(L) + t;
      printf("\tcapacidade: %lu\ttotal de itens: %lu\n", C, t);

      char* input = (char*)frutas[p - 1];
      insere_al (L, input);
   }
   putchar('\t'); visualiza_array_list_string(L);

   puts("\nverificando o encolhimento dela...");
   for (size_t i = 10; i > 0; i--) {
      size_t t = tamanho_al(L);
      // coputando a capacidade.
      size_t C = vacuo_al(L) + t;
      printf("\tcapacidade: %lu\ttotal de itens: %lu\n", C, t);
      if (t % 3 == 0)
         { putchar('\t'); visualiza_array_list_string(L); }
      assert (remove_al (L) != INVALIDA);
   }

   putchar('\t'); visualiza_array_list_string(L);
   destroi_al(L);
}

TESTE conversao_em_string(void) {
   Vetor v = cria_al();
   uint16_t* array = (uint16_t*)valores_padronizados_i;

   for (size_t i = 1; i <= VALORES_PADRONIZADOS_I; i++)
      insere_al(v, &array[i - 1]);

   char* string_vetor = to_string_al(v, debug_u16);
   puts(string_vetor);
   free(string_vetor);

   destroi_al(v);
}

TESTE novo_tipo_de_criacao_da_lista(void) {
   Vetor L = cria_de_al(
      4, &valores_padronizados_i[0],
         &valores_padronizados_i[5],
         &valores_padronizados_i[10],
         &valores_padronizados_i[15]
   );
   Vetor R = from_al(5, frutas[3], frase_ii, fruits[4], objetos[2], veiculos[3]);
   int a = length_al(L), b = length_al(R);

   printf("Total de itens: %d\n", a);
   print_al(L, debug_u16);
   printf("Quantia de itens na lista(R): %d\n", b);
   print_al(R, debug_string);
   drop_al(L);
   drop_al(R);
}

TESTE uso_para_chacagem_do_funcionmanento_do_iterador(void) {
   ArrayLista v = cria_al();
   uint16_t* array = (uint16_t*)valores_padronizados_i;

   for (size_t i = 1; i <= VALORES_PADRONIZADOS_I; i++)
      insere_al(v, &array[i - 1]);
   assert (!vazia_al(v));
   IterAL I = cria_iter_al(v);

   printf("Restantes: %lu\n", contagem_iter_al(&I));
   puts("\nIterando cada item do iterador ...");

   do {
      IterOutputAL saida = next_al(&I);
      uint16_t* ptr = saida.item;
      uint16_t dado = *ptr;
      size_t count = contagem_iter_al(&I);
      printf("\t>>> iterador(faltam %lu): %u\n", count, dado);
   } while (!consumido_iter_al(&I));

   destroi_al(v);
}

TESTE impressao_de_lista(void) {
   Vetor v = cria_al();

   for (size_t i = 1; i <= FRUITS; i++) {
      char* clone = box_str((char*)fruits[i - 1]);
      insere_al(v, clone);
   }

   imprime_lista_al(v, generico_to_str);
   destroi_interno_al(v, free_str);
}

TESTE desalocador_interno(void) {
   Vetor lista = cria_al();

   for (int i = 1; i <= BOYS_NAMES; i++)
      insere_al(lista, box_str((char*)boys_names[i - 1]));

   assert(tamanho_al(lista) == BOYS_NAMES);
   bool result = destroi_interno_al(lista, free_str);
   assert(result);
   puts("A liberação, com destruição de dados, foi sucedida.");

   assert(!destroi_interno_al(NULL, free_str));
   puts("Liberação não ocorre quando não há uma lista.");
   lista = cria_al();
   assert(!destroi_interno_al(lista, NULL));
   puts("Liberação também não acontence quando não há um 'desconstrutor'.");
   assert(destroi_al(lista));
}

static void visualiza_array_list_string(vetor_t* l) {
   size_t t = tamanho_al(l);
   char* string;

   if (vazia_al(l)) {
      puts("array-lista: []");
      return;
   }

   // só fica difícil de contar valores maiores que sete.
   printf ("array-lista(%lu): [", l->quantia);

   for (size_t i = 1; i <= t; i++) {
      string = l->array[i - 1];
      printf ("%s, ", string);
   }
   puts ("\b\b]");
}

static bool free_str(generico_t e)
   { free((char*)e); return true; }

static char* generico_to_str(Generico a)
   { return (char*)a; }

static void visualizacao_array_list_char(vetor_t* l) 
{
   size_t t = tamanho_al(l);
   char* caractere;

   if (vazia_al(l)) {
      puts("array-lista: []");
      return;
   }

   if (l->quantia > 7)
      // só fica difícil de contar valores maiores que sete.
      printf ("array-lista(%lu): [", l->quantia);
   else
      printf ("array-lista: [");

   for (size_t i = t; i > 0; i--) {
      caractere = l->array[t - i];
      printf ("%c, ", *caractere);
   }
   puts ("\b\b]");
}

static void visualizacao_array_list_int(vetor_t* l) {
   size_t t = tamanho_al(l);
   int* caractere;

   if (vazia_al(l)) {
      puts("array-lista: []");
      return;
   }

   if (l->quantia > 7)
      // só fica difícil de contar valores maiores que sete.
      printf ("array-lista(%lu): [", l->quantia);
   else
      printf ("array-lista: [");

   for (size_t i = t; i > 0; i--) {
      caractere = l->array[t - i];
      printf ("%d, ", *caractere);
   }
   puts ("\b\b]");
}

