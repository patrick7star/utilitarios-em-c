

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

#include "terminal.h"

/* Mesma representação do que a 'Dimensão', com coordenadas 'y' e 'x', 
 * nesta ordem. */
typedef Dimensao Ponto;
typedef Ponto PONTO;

#include <inttypes.h>
/* contador de instâncias do tipo. Um inteiro de 32-bits, já que é um
 * tipo de dado que costuma-se criar na faixa das unidades de milhão.
 */
static uint32_t total_de_instancias_ponto = 0;

#include <stdlib.h>
/* Meio que fazendo o mesmo acima, renomeando funções só passando os 
 * argumentos para função interna, que é chamada, e produz o mesmo 
 * resultado. 
 */
Ponto cria_ponto(uint8_t y, uint8_t x) { 
   /* cria uma tupla representando a dimensão, onde
    * o primeiro valor é da "altura", e o segundo
    * da "largura". A "altura" é o primeiro parametro,
    * já largura, o segundo. */
   Dimensao cria_dimensao(uint8_t a, uint8_t l) {
      Dimensao tupla;
      tupla = (uint8_t*)calloc(2, sizeof(uint8_t));
      tupla[0] = a;
      tupla[1] = l;
      return tupla;
   }

   total_de_instancias_ponto++;
   return cria_dimensao(y, x); 
}

void destroi_ponto(Ponto p) { 
   total_de_instancias_ponto--;
   if (p != NULL) 
      { destroi_dimensao(p);  }
}

#include <math.h>

// a menor distância entre os dois pontos.
uint8_t distancia_ponto(Ponto p, Ponto q) { 
   // coordenadas mais legíveis.
   double yp = (double)p[0], yq = (double)q[0];
   double xp = (double)p[1], xq = (double)q[1];
   double comprimento = sqrt(pow(xp - xq, 2) + pow(yp - yq, 2));
   return (uint8_t)floor(comprimento); 
}

#include <stdio.h>
/* tipos de visualizações para o 'Ponto', tanto a versão padrão, como o 
 * 'debug'. */
void visualiza_ponto(Ponto p) 
   { printf("(%d, %d)", p[0], p[1]); }

void visualiza_ponto_debug(Ponto p) 
   { printf("Ponto(y=%d, x=%d)", p[0], p[1]); }

#include <stdbool.h>

/* comparações entre os Pontos. */ 
bool lt_ponto(Ponto A, Ponto B) {
   uint8_t y_a = A[0], x_a = A[0], y_b = B[0], x_b = B[0];
   return (y_a < y_b) && (x_a < x_b);
}

bool gt_ponto(Ponto A, Ponto B)  {
   uint8_t y_a = A[0], x_a = A[0]; 
   uint8_t y_b = B[0], x_b = B[0];
   return (y_a > y_b) && (x_a > x_b);
}

bool eq_ponto(Ponto A, Ponto B) {
   uint8_t y_a = A[0], x_a = A[1], y_b = B[0], x_b = B[1];
   return (y_a == y_b) && (x_a == x_b);
}

/* Conversão de um Ponto para o formato string, visualização dos
 * tipo de dado. É muito similar à impressão dele.
 */
char* ponto_to_str(Ponto p) { 
   /* tentando estimar o total de caractéres necessários: contando os
    * parênteses, a vírgula e o espaço necessário entres os valores,
    * chegamos em quatro, entretanto, cada valor, baseado no seu tamanho
    * pode variar de um à três dígitos, vamos só precisamos do máximo,
    * quatro dígitos; assim nota conta fica em doze caractéres, então
    * vamos dobrar por segurança.
    */
   // char formatacao[4 + 2 * 4 + 12];
   char* referencia = (char*)calloc(4 + 2*4 + 2, sizeof(char));
   sprintf(referencia, "(%d, %d)", p[0], p[1]); 
   return referencia;
}

typedef Ponto* ARRAY_PONTO;
typedef ARRAY_PONTO ArrayPonto;

ArrayPonto cria_array_de_pontos(uint16_t n) {
   ARRAY_PONTO array = (ARRAY_PONTO)calloc(n, sizeof(Ponto));
   for (uint16_t i = 0; i < n; i++)
      array[i] = NULL;
   return array;
}

void destroi_array_de_pontos(ArrayPonto a, uint16_t n) {
   if (a != NULL) {
      for (uint16_t i = 0; i < n; i++) {
         destroi_ponto(a[i]);
         // printf("%dª ponto destruído.\n", i + 1);
      }
      free(a);
      // puts("array destruída");
   } else
      puts("array de Pontos é inválida.");
}

Ponto clona_ponto(Ponto p)
   { return cria_ponto(p[0], p[1]); }

// todas vértices dos retângulo formado por dois pontos não-colineares.
ArrayPonto retangulo_vertices(Ponto p, Ponto q) {
   uint8_t xP = p[1], yP = p[0];
   uint8_t xQ = q[1], yQ = q[0];
   Ponto a, b, c, d;

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
      a = clona_ponto(p);
      b = cria_ponto(yP, xQ);
      c = clona_ponto(q);
      d = cria_ponto(yQ, xP);
   } else if (xP > xQ && yP < yQ) {
      a = cria_ponto(yP, xQ);
      b = clona_ponto(p);
      c = cria_ponto(yQ, xP);
      d = clona_ponto(q);
   } else 
      perror("pontos com x's e y's coicidentes não produzem retângulos");

   ArrayPonto vertices = cria_array_de_pontos(4);
   vertices[0] = a;
   vertices[1] = b;
   vertices[2] = c;
   vertices[3] = d;
   /* Pontos que representam as vértices dos retângulos à partir do 
    * canto-superior-esquerdo até o canto-inferior-esquerdo, portanto,
    * seguindo o sentido horário.
    */
   return vertices;
}

bool pontos_colineares(Ponto a, Ponto b) 
   { return a[0] == b[0] || a[1] == b[1]; }

#include <stdarg.h>
/* criando uma array de forma manual, apenas passando pares de 
 * argumentos. 
 */
ArrayPonto cria_manualmente_ap(uint8_t total, ...) {
   va_list argumentos;
   ARRAY_PONTO array = cria_array_de_pontos(total);

   va_start(argumentos, total);

   for(uint8_t k = 0; k < total; k++) {
      uint8_t y = va_arg(argumentos, int);
      uint8_t x = va_arg(argumentos, int);
      array[k] = cria_ponto(y, x);
   }
   va_end(argumentos);

   return array;
}

#include <string.h>

char* ap_to_str(ArrayPonto a, uint64_t t) {
   /* o total de caractéres é computado na seguinte forma: valores das
    * coordenadas são no máximo três dígitos cada, e há dois deles; mais
    * dois caractéres, sendo o primeiro o separador(vírgula), e o segundo
    * o espaçamento, isso tudo multiplicado pela quantidade de itens
    * da array; somado no final com dois caractéres para as delimitações,
    * e dois espaços de reserva.
    */
   uint64_t total = t * (3 * 2 + 2) + 2 + 2;
   char* resultado_fmt = calloc(total, sizeof(char));

   resultado_fmt[0] = '[';
   for (uint64_t k = 1; k <= t; k++) {
      strcat(resultado_fmt, ponto_to_str(a[k - 1]));
      strcat(resultado_fmt, ", ");
   }
   strcat(resultado_fmt, "\b\b]");

   return resultado_fmt;
}

void visualiza_ap(ArrayPonto a, uint64_t t) {
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
   
#if defined(_UT_PONTO)
// biblioteca padrão em C(libs muito utilizadas.):
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "teste.h"

void criacao_da_array_de_pontos() {
   ArrayPonto array = cria_array_de_pontos(7);

   srand(time(NULL));
   for (int k = 1; k <= 7; k++) {
      uint8_t y = rand() % 10;
      uint8_t x = rand() % 100;
      array[k - 1] = cria_ponto(y, x);
   }

   for (int p = 1; p <= 7; p++)
      printf("%s\t", ponto_to_str(array[p - 1]));
   destroi_array_de_pontos(array, 7);
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

   destroi_array_de_pontos(vertices, 4);
   destroi_ponto(p); destroi_ponto(q);
   destroi_ponto(p1); destroi_ponto(p2);
   destroi_ponto(p3); destroi_ponto(p4);

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

   destroi_array_de_pontos(vertices, 4);
   destroi_ponto(p); destroi_ponto(q);
   destroi_ponto(p1); destroi_ponto(p2);
   destroi_ponto(p3); destroi_ponto(p4);
}

void criacao_de_forma_manual_da_array_de_pontos(void) {
   ArrayPonto array = cria_manualmente_ap(
      7, 8, 10, 5, 19, 0, 7, 5, 25, 9, 10,
      15, 3, 20, 20
   );
   printf("%s\n", ap_to_str(array, 7));
   destroi_array_de_pontos(array, 7);
}

void main(int argc, char** argv) {
   executa_testes(
      3, criacao_da_array_de_pontos, true,
      todos_vertices_do_retangulo, true,
      criacao_de_forma_manual_da_array_de_pontos, true
   );
}
#endif // testes-unitários.
