

/* O código foi extraído do módulo 'texto', onde tal estrutura é 
 * necessária. Aqui, podemos trabalhar de forma mais organizada
 * o que compõe tal tipo de dado, e seus respectivos métodos.
 *
 * O tipo de dado se estrutura numa array de dois tipos inteiros de
 * 8-bits, nada é mais necessário -- apesar de futuramente poder ser
 * trocado, já que o ponto representa um caractére, e as maiores telas
 * trabalho, ficam no entre cento e poucos, portanto, cabível num
 * único byte. Ele também embrulha o tipo dimensão, que é uma array
 * com dois bytes.
 */

// Declaração de tipos de dados, seus métodos, e funções abaixo:
#include "ponto.h"
// Biblioteca padrão do C:
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* Como tal estrutura será usada principalmente para telas de computadores,
 * representando caractéres, não é preciso ter valores nas casas das 
 * centenas de milhares, ou milhões, entretanto, telas estão cada vez 
 * maiores hoje em dia, logo um inteiro positivo de 8-bits não será o
 * suficiente também; ficamos em tão com inteiros positivos de 16-bits. */
 // struct coordenada_bidimensional { uint16_t y; uint16_t x; };


Ponto cria_ponto(uint16_t y, uint16_t x) 
/* Apenas faz as copias dos valores paras o respecitivos pontos na estrutura
 * alocada e retornada a cópia abaixo.*/
   { return (Ponto){y, x}; }

Ponto cria_default(void)
   { return (Ponto){0, 0}; }

uint16_t distancia_ponto(Ponto p, Ponto q) { 
/* A distância entre dois pontos. Posteriormente é convertida a inteiro, 
 * pois a meta especial é trabalhando tais coordenadas como caractéres,
 * na tela do terminal, ou seja, unidades inteiras. */
   double yp = (double)p.y, yq = (double)q.y;
   double xp = (double)p.x, xq = (double)q.x;
   // Aplicando na equação para calcular a distância.
   double comprimento = sqrt(pow(xp - xq, 2) + pow(yp - yq, 2));

   // Converte resultado para inteiro.
   return (uint16_t)floor(comprimento); 
}

void imprime_ponto(Ponto p) 
// Visualização segue padrão usado na biblioteca ncurses(y, x).
   { printf("(%d, %d)\n", p.y, p.x); }

void imprime_ponto_debug(Ponto p) 
// Não segue a ordem do ncurses, porque tais valores são nomeados.
   { printf("Ponto(x=%d, y=%d)\n", p.x, p.y); }


/* Comparações entres pontos, pelo menos as que são possíveis neste tipo
 * de estrutura. Relações de maior e menor ficam sem sentidos para ela. */
bool eq_ponto(Ponto a, Ponto b) 
   { return a.y == b.y && b.x == a.x; }

bool ne_ponto(Ponto a, Ponto b)
   { return !eq_ponto(a, b); }

char* ponto_to_str(Ponto p) { 
/* Conversão de um Ponto para o formato string, visualização dos
 * tipo de dado. É muito similar à impressão dele.
 * tentando estimar o total de caractéres necessários: contando os
 * parênteses, a vírgula e o espaço necessário entres os valores,
 * chegamos em quatro, entretanto, cada valor, baseado no seu tamanho
 * pode variar de um à três dígitos, vamos só precisamos do máximo,
 * quatro dígitos; assim nota conta fica em doze caractéres, então
 * vamos dobrar por segurança. */
   size_t size = (4 + 2*4 + 2 + 6) * sizeof(char); 
   char* ponto_fmt = malloc(size);

   sprintf(ponto_fmt, "(%d, %d)", p.x, p.y); 
   return ponto_fmt;
}

// todas vértices dos retângulo formado por dois pontos não-colineares.
ArrayPonto retangulo_vertices(Ponto p, Ponto q) {
   uint16_t xP = p.x, yP = p.y;
   uint16_t xQ = q.x, yQ = q.y;
   Ponto a = cria_default(), b = cria_default(), 
         c = cria_default(), d = cria_default();

   /* achando pontos restantes, dependendo de como os dois pontos
    * estão dispersos. Eles tem apenas dois modos de o fazer: o
    * ponto do primeiro argumento está no canto-superior-esquerdo,
    * e o segundo argumento(também um ponto) ficar no
    * canto-inferior-direito; já o outro modo seria o primeiro argumento
    * ficar no canto-superior-direito, e o segundo ficar no 
    * canto-inferior-esquerdo. Ambos jeitos formam retângulos, qualquer
    * outro com coordenadas que se alinhem, não formarião qualquer
    * retângulo.
    */
   if (xP < xQ && yP < yQ) {
      // a = clona_ponto(p);
      a = p;
      b = cria_ponto(yP, xQ);
      // c = clona_ponto(q);
       c = q;
      d = cria_ponto(yQ, xP);
   } else if (xP > xQ && yP < yQ) {
      a = cria_ponto(yP, xQ);
      // b = clona_ponto(p);
      b = p;
      c = cria_ponto(yQ, xP);
      // d = clona_ponto(q);
      d = q;
   } else 
      perror("pontos com x's e y's coicidentes não produzem retângulos");

   const size_t TOTAL = 4;
   const size_t size = sizeof(struct coordenada_bidimensional);
   ArrayPonto vertices = malloc(TOTAL * size);

   vertices[0] = a;
   vertices[1] = b;
   vertices[2] = c;
   vertices[3] = d;
   /* Pontos que representam as vértices dos retângulos à partir do 
    * canto-superior-esquerdo até o canto-inferior-esquerdo, portanto,
    * seguindo o sentido horário. */
   return vertices;
}

bool pontos_colineares(Ponto a, Ponto b) 
   { return a.y == b.y || a.x == b.x; }

char* array_ponto_to_str(ArrayPonto a, size_t t) {
/* o total de caractéres é computado na seguinte forma: valores das
 * coordenadas são no máximo três dígitos cada, e há dois deles; mais
 * dois caractéres, sendo o primeiro o separador(vírgula), e o segundo
 * o espaçamento, isso tudo multiplicado pela quantidade de itens
 * da array; somado no final com dois caractéres para as delimitações,
 * e dois espaços de reserva.
 */
   // size_t total = t * (3 * 2 + 2) + 2 + 2;
   char* resultado_fmt;
   size_t comprimento = 0;

   // Primeiro loop para computar o tamanho final da string necessária. 
   for (uint64_t k = 1; k <= t; k++) {
      resultado_fmt = ponto_to_str(a[k - 1]);
      // Contabiliza a string e seu separador..
      comprimento += strlen(resultado_fmt) + 2;
      // Libera após gerar.
      free(resultado_fmt);
   }
   // Computando delimitadores finais de '[' e '\b\b]'.
   comprimento += 1 + 3;  
   
   // Aloca e coloca o delimitador inicial.
   resultado_fmt = calloc(comprimento, sizeof(char));
   resultado_fmt[0] = '[';

   for (uint64_t k = 1; k <= t; k++) {
      strcat(resultado_fmt, ponto_to_str(a[k - 1]));
      strcat(resultado_fmt, ", ");
   }
   strcat(resultado_fmt, "\b\b]");

   return resultado_fmt;
}

void imprime_array_ponto(ArrayPonto a, uint64_t t) {
   printf("Array de Ponto: [\n\t");
   for (uint64_t k = 1; k <= t; k++) {
      char* formatacao = ponto_to_str(a[k - 1]);
      if (k % 5 != 0)
         printf("%s, ", formatacao);
      else
         printf("%s, \n\t", formatacao);
   }
   printf("\b\b\n]\n");
}

ArrayPonto cria_array_ponto(size_t qtd, ...) {
   va_list argumentos;
   size_t size = (qtd / 2) * sizeof(Ponto);
   ArrayPonto pontos = malloc (size);

   va_start(argumentos, qtd);
   for (size_t p = 1; p <= qtd; p++) {
      uint16_t y = va_arg(argumentos, int);
      uint16_t x = va_arg(argumentos, int);

      pontos[p - 1].y = y;
      pontos[p - 1].x = x;
   }
   va_end(argumentos);

   return pontos;
}
   
#if defined(_UT_PONTO)
// biblioteca padrão em C(libs muito utilizadas.):
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "teste.h"

void criacao_da_array_de_pontos() {
   Ponto array[7];

   srand(time(NULL));
   for (int k = 1; k <= 7; k++) {
      uint8_t y = rand() % 10;
      uint8_t x = rand() % 100;
      array[k - 1] = cria_ponto(y, x);
   }

   for (int p = 1; p <= 7; p++)
      printf("%s\t", ponto_to_str(array[p - 1]));
}

void todos_vertices_do_retangulo() {
   Ponto p = cria_ponto(5, 10);
   Ponto q = cria_ponto(8, 19);

   ArrayPonto vertices = retangulo_vertices(p, q);

   Ponto p1 = cria_ponto(5, 10);
   assert(eq_ponto(vertices[0], p1));
   printf("%s\t", ponto_to_str(vertices[0]));

   Ponto p2 = cria_ponto(5, 19);
   assert(eq_ponto(vertices[1], p2));
   printf("%s\t", ponto_to_str(vertices[1]));

   Ponto p3 = cria_ponto(8, 19);
   assert(eq_ponto(vertices[2], p3));
   printf("%s\t", ponto_to_str(vertices[2]));

   Ponto p4 = cria_ponto(8, 10);
   assert(eq_ponto(vertices[3], p4));
   printf("%s\t", ponto_to_str(vertices[3]));

   free(vertices);

   // outro modo de arranjos dos pontos.
   p = cria_ponto(2, 30);
   q = cria_ponto(9, 15);

   vertices = retangulo_vertices(p, q);

   p1 = cria_ponto(2, 15);
   assert(eq_ponto(vertices[0], p1));
   printf("%s\t", ponto_to_str(vertices[0]));

   p2 = cria_ponto(2, 30);
   assert(eq_ponto(vertices[1], p2));
   printf("%s\t", ponto_to_str(vertices[1]));

   p3 = cria_ponto(9, 30);
   assert(eq_ponto(vertices[2], p3));
   printf("%s\t", ponto_to_str(vertices[2]));

   p4 = cria_ponto(9, 15);
   assert(eq_ponto(vertices[3], p4));
   printf("%s\t", ponto_to_str(vertices[3]));

   free(vertices);
}

void criacao_de_forma_manual_da_array_de_pontos(void) {
   ArrayPonto array = cria_array_ponto (
      7, 8, 10, 5, 19, 0, 7, 5, 25, 9, 10,
      15, 3, 20, 20
   );
   printf("%s\n", array_ponto_to_str(array, 7));
   free(array); 
}

void comparacao_entre_pontos() {
   Ponto input[] = {
      (Ponto){5, 5}, (Ponto){15, 33}, (Ponto){7, 2},
      (Ponto){2, 10}, (Ponto){5, 7}, (Ponto){13, 0}
   };
   Ponto output[] = {
      (Ponto){5, 5}, (Ponto){16, 21}, (Ponto){7, 4},
      (Ponto){2, 10}, (Ponto){5, 7}, (Ponto){13, 2}
   };

   for (size_t k = 1; k <= 5; k++) {
      Ponto a = input[k - 1];
      Ponto b = output[k - 1];

      if (eq_ponto(a, b))
         printf("%s == %s\n", ponto_to_str(a), ponto_to_str(b));
      else
         printf("%s != %s\n", ponto_to_str(a), ponto_to_str(b));
   }

   assert (eq_ponto(input[0], output[0]));
   assert (!eq_ponto(input[1], output[1]));
   assert (!eq_ponto(input[2], output[2]));
   assert (eq_ponto(input[3], output[3]));
   assert (eq_ponto(input[4], output[4]));
   assert (!eq_ponto(input[5], output[5]));
   puts("Todos testaram corretamente.");
}

void todos_tipos_de_visualizacao_das_estruturas(void) {
   Ponto a = cria_ponto(314, 171);
   Ponto b = cria_ponto(1, 9);
   Ponto c = cria_ponto(12345, 54321);
   ArrayPonto sequencia = cria_array_ponto(
      10, 1,2, 3,4, 5,6, 7,8, 9,10, 11,12,
         99,100, 101,102, 103,104, 105,106
   );

   char* str = ponto_to_str(a);
   printf("A%s\n", str);
   free(str);
   str = ponto_to_str(b);
   printf("B%s\n", str);
   free(str);
   str = ponto_to_str(c);
   printf("C%s\n", str);
   free(str);

   str = array_ponto_to_str(sequencia, 10);
   printf("Array de Pontos: %s\n", str);
   free(str);
   free(sequencia);

   imprime_array_ponto(sequencia, 10);
   imprime_ponto(a);
   imprime_ponto(b);
   imprime_ponto(c);
}

void main(int argc, char** argv) {
   executa_testes_a(
      true, 5, criacao_da_array_de_pontos, true,
         todos_vertices_do_retangulo, true,
         criacao_de_forma_manual_da_array_de_pontos, false,
         comparacao_entre_pontos, true,
         todos_tipos_de_visualizacao_das_estruturas, false
   );
}
#endif // testes-unitários.
