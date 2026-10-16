/* Como ficaram muitos extensos, os testes unitários do módulo agora serão 
 * transferidos para este arquivo. Tal que será incluido no arquivo principal.
 */
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include "teste.h"
#include "dados_testes.h"

TESTE testes_basico_da_reparticao_em_palavras         (void);
TESTE visualizacao_da_reparticao_em_palavras          (void);
TESTE experimento_concatenacao_de_multiplas_strings   (void);
TESTE criacao_simples_de_uma_instancia_String         (void);
TESTE diversos_tipos_basicos_de_insercao_String       (void);
TESTE motor_de_busca_de_ocorrencias                   (void);
TESTE preenchimento_da_string                         (void);
TESTE fazendo_strings_maiusculas_e_minusculas         (void);
TESTE capitalizacao_das_strings                       (void);
TESTE metodos_de_extracao                             (void);
TESTE criacao_de_string_homogenea                     (void);
TESTE buscando_por_padroes_na_estringue               (void);
TESTE metodo_de_substituicao_de_string                (void);
TESTE reparticao_da_strings_dado_padrao               (void);
TESTE reparticao_de_partes_do_ls_colors               (void);
TESTE unica_reparticao_ocorrendo                      (void);
TESTE apara_pontas_das_strings                        (void);
TESTE reparticao_baseado_numa_string_pattern          (void);
TESTE reparticoes_com_quebra_de_linhas                (void);


int main(int qtd, char* args[], char* vars[])
{
   executa_testes_a(
     false, 7,
			testes_basico_da_reparticao_em_palavras, true,
         visualizacao_da_reparticao_em_palavras, true,
         experimento_concatenacao_de_multiplas_strings, true,
         // Testes específicos do tipo String:
         criacao_simples_de_uma_instancia_String, true,
         diversos_tipos_basicos_de_insercao_String, true,
         motor_de_busca_de_ocorrencias, true,
			preenchimento_da_string, true
   );

   executa_testes_a(
     false, 7,
      fazendo_strings_maiusculas_e_minusculas, false,
      capitalizacao_das_strings, false,
      metodos_de_extracao, true,
      criacao_de_string_homogenea, true,
      buscando_por_padroes_na_estringue, true,
      metodo_de_substituicao_de_string, true,
      experimento_concatenacao_de_multiplas_strings, true
   );

   executa_testes_b (
     false, 4,
         Unit(reparticao_da_strings_dado_padrao, true),
         Unit(reparticao_de_partes_do_ls_colors, false),
         Unit(unica_reparticao_ocorrendo, true),
         Unit(apara_pontas_das_strings, true)
   );

   executa_testes_b(
     true, 2,
         Unit(reparticao_baseado_numa_string_pattern, false),
         Unit(reparticoes_com_quebra_de_linhas, true)
   );

   return EXIT_SUCCESS;
}

static ListStr split_file(const char* pathname, const char* pattern)
{
// Função apenas para testes, não funciona em massa para arquivos e textos
// extensos.
   const int MAX = 900, sz = sizeof(char);
   char buffer[MAX];
   FILE* stream = fopen(pathname, "rt");
   int lido = 0xabc; 
   ListStr output = {0x00000};

   memset(buffer, 0x00, MAX);
   lido = fread(buffer, sz, MAX, stream);
   printf("Foram lidos %d bytes.\n", lido);
   fclose(stream);
   output = split_matches(buffer, pattern);

   return output;
}

TESTE reparticoes_com_quebra_de_linhas(void)
{
   const char* pathnames[] = {
      "src/estringue/tests/samples/operacoes-basicas.txt",
      "src/estringue/tests/samples/palavras-bem-esparsadas.txt",
      "src/estringue/tests/samples/frutas-por-linha.txt"
   };
   ListStr output_a, output_b, output_c;
   int n = 0xabdcef;


   output_a = split_file(pathnames[0], "\n\n\n");
   // Observe que estes reparte em quatro quebra-de-linha sucessivas.
   output_b = split_file(pathnames[1], "\n\n\n\n");
   output_c = split_file(pathnames[2], "\n\n");

   for (n = 0; n < output_a.total; n++)
      printf("\n\n%dª.\n'''\n%s\n'''", n + 1, output_a.lista[n]);

   puts("\n\nSegundo arquivo:");

   for (n = 0; n < output_b.total; n++)
      printf("\t\b\b%dª. '%s'\n", n + 1, output_b.lista[n]);

   puts("\n\nTerceiro arquivo:");

   for (n = 0; n < output_c.total; n++)
      printf("\t\b\b%dª. '%s'\n", n + 1, output_c.lista[n]);

   free_lista_strings(&output_a);
   free_lista_strings(&output_b);
   free_lista_strings(&output_c);
}

TESTE reparticao_baseado_numa_string_pattern(void)
{
   char* input = 
      "Laranja, Morango, Banana, Goiaba, Pera, FIgo, Melão, Lima, Uva, "
      "Tomate, Jaca, Manga, Pêssego, Framboesa, Cereja, Maçã, Melância";
   char* pattern = ", ";
   ListStr output = split_matches(input, pattern);

   puts("Resultado da divisão:");
   debug_lista_strings(&output);
   free_lista_strings(&output);

   input = 
      ", Laranja, Morango, Banana, Goiaba, Pera, FIgo, Melão, Lima, Uva, "
      "Tomate, Jaca, Manga, Pêssego, Framboesa, Cereja, Maçã, Melância";
   output = split_matches(input, pattern);
   debug_lista_strings(&output);
   free_lista_strings(&output);

   pattern = "";
   output = split_matches(input, pattern);
   debug_lista_strings(&output);
   free_lista_strings(&output);

   input = 
      "Laranja, Morango, Banana, Goiaba, Pera, FIgo, Melão, Lima, Uva, "
      "Tomate, Jaca, Manga, Pêssego, Framboesa, Cereja, Maçã, Melância, ";
   output = split_matches(input, pattern);
   debug_lista_strings(&output);
   free_lista_strings(&output);
}

TESTE apara_pontas_das_strings(void)
{
   char* input = "nada-agora";
   char* output = rstrip(input, "ra");

   printf("Input('%s') ===> Output('%s')\n", input, output);
   free(output);

   input = "coffee";
   output = rstrip(input, "fe");
   printf("Input('%s') ===> Output('%s')\n", input, output);
   free(output);

   input = "anaaaaa";
   output = rstrip(input, "a");
   printf("Input('%s') ===> Output('%s')\n", input, output);
   free(output);

   puts("\nMétodo de aparação na esquerda ...");

   input = "banana";
   output = strip(input, "ab");
   printf("Input('%s') ===> Output('%s')\n", input, output);
   free(output);
   output = strip(input, "ba");
   printf("Input('%s') ===> Output('%s')\n", input, output);
   free(output);

   input = "abacaxi";
   output = strip(input, "ab");
   printf("Input('%s') ===> Output('%s')\n", input, output);
   free(output);
   output = strip(input, "ba");
   printf("Input('%s') ===> Output('%s')\n", input, output);
   free(output);
   output = strip(input, "ac");
   printf("Input('%s') ===> Output('%s')\n", input, output);
   free(output);
}

void testes_basico_da_reparticao_em_palavras(void) {
   for (size_t i = 1; i <= 5; i++) {
      char* phrase = (char*)frases_i[i - 1];
      printf ("\nsobre: '%s'\n", phrase);
      palavras(phrase);
   }
}

void visualizacao_da_reparticao_em_palavras(void) {
   ListaStrings result = palavras ((char*)frase_ii);

   puts ("listando o resultado: ...");
   for (size_t q = 1; q <= result.total; q++)
      printf ("\t==>  '%s'\n", result.lista[q - 1]);
}

TESTE experimento_concatenacao_de_multiplas_strings(void) {
   char* result = concatena_strings (
      4, frutas[3], frutas[5],
         frutas[7], frutas[8]
   );
   printf ("a concatenação: '%s'\n", result);
}

void criacao_simples_de_uma_instancia_String(void) {
   String sI = cria_str();
   printf("Seu tamanho: %lu\n", sI->total);

   String sII = from_str("padlock");
   printf("Seu tamanho: %lu\n", sII->total);

   assert (destroi_str(sI));
   assert (destroi_str(sII));
}

void imprime_string(String s) {
   size_t T = s->quantia;

   putchar('\"');
   for (size_t p = 1; p <= T; p++)
      printf("%lc", s->caracteres[p - 1]);
   putchar('\"');
   putchar('\n');
}

void diversos_tipos_basicos_de_insercao_String(void) {
   String X = cria_str();
   char input[] = "mAnBpCqDeRgS";
   char* ptr_input = input;

   while (*ptr_input) {
      printf("Sobrando: %lu\n", vacuo_str(X));
      adiciona_str(X, *ptr_input);
      imprime_string(X);
      ptr_input++;
   }

   puts("Anexando toda uma string ...");
   printf("Comprimento: %lu\n", comprimento_str(X));
   anexa_str(X, "A1B2C3Z9X88Y777");
   imprime_string(X);
   printf("Comprimento: %lu\n", comprimento_str(X));
   printf("Sobrando: %lu\n", vacuo_str(X));
   destroi_str(X);
}

void visualiza_int_array(size_t* A) {
   /* Apenas a declaração abaixo já faz a iteração futura inócuo, porque
    * se não há array, logo o acesso de um NULL é convertido a zero,
    * entretanto, para quem não é muito familiar com C, fica difícil de ler
    * tal declaração, por isso da "declaração de saída" abaixo. */
   if (A == NULL) { return; }

   size_t t = A[0];

   putchar('[');
   for (size_t i = 1; i <= t; i++) {
      if (i != t)
         printf("%lu, ", A[i]);
      else
         printf("%lu", A[i]);
   }
   putchar(']');
   putchar('\n');
}

static size_t* busca_todas_ocorrencias_do_caractere(String s,
  wchar_t _char, int64_t N, bool comeca_na_esquerda)
{
/*   Varre a array interna da string em busca de todas possíveis ocorrências
 * do caractére unicode dado. Pode-se configurar a partida da busca, assim
 * como a quantidade de buscas, sim, por incoerência com o nome.
 *   O retorno é uma array, onde o primeiro item é o total de índices, e os
 * demais são eles em sí. */
   size_t* indices = NULL, total = 0, Q = s->quantia;
   wchar_t* array = s->caracteres;

   /* Contando o número de ocorrências, pelo acaso da simetria do item
    * analisado, tal contagem serve tanto para a versão regular(esquerda-
    * à-direita), como o inverso. */
   for (size_t k = 1; k <= Q; k++) {
      if (array[k - 1] == _char)
         { total++; }

      /* Se foi marcado como 'nem todas ocorrências', quebra em específico
       * ponto da contagem. */
      if (N != TODOS && total == N) { break; }
   }

   #ifdef _UT_STRING
   printf("Total de índices encontrados: %lu\n", total);
   #endif

   if (comeca_na_esquerda) {
      indices = malloc((total + 1) * sizeof(size_t));
      indices[0] = total;
      size_t p = 0, k = 1;

      // Agora sim, uma busca para alocação da posição com o caractére.
      while (total > 0) {
         if (array[p] == _char) {
            total--;
            indices[k++] = p;
         }
         p++;
      }
   } else {
      indices = malloc((total + 1) * sizeof(size_t));
      indices[0] = total;
      size_t p = Q - 1, k = 1;

      while (total > 0) {
         if (array[p] == _char) {
            total--;
            indices[k++] = p;
         }
         p--;
      }
   }
   return indices;
}

TESTE motor_de_busca_de_ocorrencias(void) {
   String amostra_i = from_str("banana");
   String amostra_ii = from_str("paralelepipedo");
   size_t* (*busca)(String, wchar_t, int64_t, bool);
   busca = busca_todas_ocorrencias_do_caractere;

   size_t* posicoes_i = busca(amostra_i, L'a', TODOS, true);
   size_t* posicoes_ii = busca(amostra_i, L'a', TODOS, false);
   puts("Primeiro esquerda-à-direita, depois o inverso, resultados:");
   imprime_string(amostra_i);
   visualiza_int_array(posicoes_i);
   visualiza_int_array(posicoes_ii);
   free(posicoes_ii); free(posicoes_i);
   puts("Agora, sem contabilizar todas ocorrências:");
   posicoes_i = busca(amostra_i, L'a', 2, true);
   posicoes_ii = busca(amostra_i, L'a', 1, false);
   visualiza_int_array(posicoes_i);
   visualiza_int_array(posicoes_ii);
   free(posicoes_ii); free(posicoes_i);

   posicoes_i = busca(amostra_ii, L'e', TODOS, true);
   posicoes_ii = busca(amostra_ii, L'e', TODOS, false);
   puts("\nPrimeiro esquerda-à-direita, depois o inverso, resultados:");
   imprime_string(amostra_ii);
   visualiza_int_array(posicoes_i);
   visualiza_int_array(posicoes_ii);
   free(posicoes_ii); free(posicoes_i);
   puts("Agora, sem contabilizar todas ocorrências:");
   posicoes_i = busca(amostra_ii, L'e', 2, true);
   posicoes_ii = busca(amostra_ii, L'e', 1, false);
   visualiza_int_array(posicoes_i);
   visualiza_int_array(posicoes_ii);
   free(posicoes_ii); free(posicoes_i);

   posicoes_i = busca(amostra_i, L'a', 0, true);
   visualiza_int_array(posicoes_i);
   posicoes_i = busca(amostra_i, L'a', 0, false);
   visualiza_int_array(posicoes_i);

   destroi_str(amostra_ii);
   destroi_str(amostra_i);
}

void preenchimento_da_string(void) {
	String obj = cria_str();

	preenche_str(obj, '-', 5);
	imprime_string(obj);
	preenche_str(obj, '+', 13);
	imprime_string(obj);

	destroi_str(obj);
}

void fazendo_strings_maiusculas_e_minusculas(void)
{
   puts("Transformação das strings ...");
   for (int i = 1; i <= GIRLS_NAMES; i++)
   {
      char* q = (char*)girls_names[i - 1];
      char* r = maiuscula_ascii(q);
      char* s = minuscula_ascii(r);

      printf("\b\b\b\b\t'%s' ===> '%s', '%s'\n", q, r, s);
      free(r); free(s);
   }
}

void capitalizacao_das_strings(void)
{
   puts("Transformação das strings ...");

   for (int i = 1; i <= FRUITS; i++)
   {
      char* q = (char*)fruits[i - 1];
      char* r = capitaliza_ascii(q);

      printf("\b\b\b\b\t'%s' ===> '%s'\n", q, r);
      free(r);
   }
}

void metodos_de_extracao(void)
{
   const char* inputs[] = {
      " 501Estou# falando desta % aqui.&|",
      "kda15amI.4ll5", "__#c87A()s-a8+$$",
      "aaaa5eee_$$5---kfnas0"
   };

   puts("Antes ===> depois:");

   char* a = (char*)inputs[0];
   char* b = extrai_letras_ascii(a);
   printf("'%s' ===> '%s'\n", a, b);
   assert(strcmp(b, "Estoufalandodestaaqui.") == 0);
   free(b);

   a = (char*)inputs[1];
   b = extrai_digitos_ascii(a);
   printf("'%s' ===> '%s'\n", a, b);
   assert(strcmp(b, "15.45") == 0);
   free(b);

   a = (char*)inputs[2];
   b = extrai_letras_ascii(a);
   printf("'%s' ===> '%s'\n", a, b);
   free(b);

   a = (char*)inputs[3];
   b = extrai_digitos_ascii(a);
   printf("'%s' ===> '%s'\n", a, b);
   assert(strcmp(b, "550") == 0);
   free(b);
}

void criacao_de_string_homogenea(void)
{
   char tipos[] = {'+', '-', '=', '*'};
   char* aux = NULL, *simbolo = tipos;

   do {
      aux = repete_caractere_ascii(*simbolo, 31);
      puts(aux);
      free(aux);
   } while(*simbolo++);
}

void buscando_por_padroes_na_estringue(void)
{
   char* exemplo = {
      "Today is the day, for a lot of thing, it is gonna be okay, because "
      "it is already okay, is she here?! For get more stuff, you need "
      "go away, right now! Look there a lot of weed, let's go!"
   };
   printf("\"%s\"(%lu)\n", exemplo, strlen(exemplo));

   Posicoes a = acha_posicoes(exemplo, "is");
   Posicoes b = acha_posicoes(exemplo, "okay");
   Posicoes c = acha_posicoes(exemplo, "e");
   Posicoes d = acha_posicoes(exemplo, "a");

   free(a.array); free(b.array); free(c.array); free(d.array);
}

void metodo_de_substituicao_de_string(void)
{
   char* exemplo = {
      "Today is the day, for a lot of thing, it is gonna be okay, because "
      "it is already okay, is she here?! For get more stuff, you need "
      "go away, right now!"
   };
   char* padroes[] = { "is", "okay", ",", "stuff" };
   const int TOTAL = sizeof(padroes) / sizeof(char*);

   printf("\"%s\"(%lu)\n", exemplo, strlen(exemplo));

   for (int i = 0; i < TOTAL; i++)
   {
      char* item = padroes[i];
      char* output_a = substitui_ascii(exemplo, item, "**");
      char* output_b = substitui_ascii(exemplo, item, "******");
      char* output_c = substitui_ascii(exemplo, item, "*");
      char* output_d = substitui_ascii(exemplo, item, "");

      printf("\n - mesmo tamanho: '%s'\n", output_a);
      printf("\n - maior: '%s'\n", output_b);
      printf("\n - apenas um: '%s'\n", output_c);
      printf("\n - vázio: '%s'\n", output_d);

      free(output_d);
      free(output_c);
      free(output_b);
      free(output_a);
   }
}

void reparticao_da_strings_dado_padrao(void)
{
   char* inputs[] = {
      "gato&&cachorro&&comédia&&porta&&alicáte&&pessoas",
      "gato.cachorro.comédia.porta.alicáte.pessoas",
      "gato.cachorro&.comédia&.porta.&alicáte&.pessoas",
      "gato.cachorro.comédia.porta.alicáte.pessoas",
      // Uma estringue vázia.
      ""
   };
   char* padroes[] = {"&&", ".", "&.", "***", "#"};

   for (int k = 0; k < 5; k++)
   {
      ListaStrings output = reparte_ascii(inputs[k], padroes[k]);
      size_t n = output.total;

      printf("\n\"%s\"\nTodos %lu resultados:\n", padroes[k], n);

      for (int i = 0; i < output.total; i++) {
         printf("\t- '%s'\n", output.lista[i]);
         free(output.lista[i]);
      }
      // free(output.lista);
   }
}

void reparticao_de_partes_do_ls_colors(void) {
   char* In = (char*)getenv("LS_COLORS");
   const char PADRAO = ':';
   ListaStrings Out = split_at(In, PADRAO);

   for (int n = 1; n <= Out.total; n++)
      printf("\t\b\b- %s\n", Out.lista[n - 1]);
   free_lista_strings(&Out);
}

void unica_reparticao_ocorrendo(void) {
   const char* input = "Única frase junta#o sol raiou cedo#   nunca mais";
   ListStr output = split_once(input, '#');

   debug_lista_strings(&output);
   assert(output.total == 2);
   assert(strcmp(output.lista[0], "Única frase junta") == 0);
   assert(strcmp(output.lista[1], "o sol raiou cedo#   nunca mais") == 0);
   free_lista_strings(&output);
}
