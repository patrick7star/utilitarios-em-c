

/* Uma 'Matriz de Caractéres' serve tanto
 * como uma 'Array de Strings', como também
 * para ser o núcleo de um 'Texto', que é um
 * objeto projetado para desenhos no terminal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "terminal.h"
#include "ponto.h"

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

/* limitações para "grade da stack" definidas
 * em tempo de compilação, são baseadas na tela
 * do computador agora disponíveis, podem ser
 * alteradas no futuro, porém não serão maior
 * do que isso aí. */
#define ALTURA 40
#define LARGURA 151

struct matriz_texto {
   // primeiro altura, depois a largura.
   Dimensao dimensao;
   /* Se não exceder uma área muito larga,
    * por motivos de velocidade, a grade 
    * pode ficar na 'stack', ou seja, uma
    * array multidimensional de caractéres
    * criada em tempo de inicialização. Tanto
    * isso é mais rápido do que alocar
    * dinamicamente, mais seu acesso e 
    * atualização também o serão. A alocação
    * na 'heap' ainda pode ser feita ao
    * instanciar tal, mas só para uma "grade"
    * realmente grande. */
    char grade_stack[ALTURA][LARGURA];
    // versão na 'heap'.
    StringArray grade_heap;
    // diz qual opção foi acionada.
    bool usando_heap;
};

// nome formal da estrutura, e sua versão curta.
typedef struct matriz_texto* MatrizTexto;
typedef MatrizTexto MT;

// caractéres que representam planos opacos e transparente.
#define OPACO '~'
#define TRANSPARENTE ' '

MT cria_matriz_texto(Dimensao d, PlanoFundo f) {
   size_t size = sizeof(struct matriz_texto);
   MT matriz = (MT)malloc(size);
   uint32_t a = d[0], l = d[1];
   matriz->dimensao = d;

   if (a >= ALTURA || l >= LARGURA) {
      matriz->usando_heap = true;
      matriz->grade_heap = cria_string_array(a, l, Visivel);
   } else {
      matriz->usando_heap = false;
      char caractere;
      switch (f) {
         case Visivel:
            caractere = OPACO;
            break;
         case Invisivel:
            caractere = TRANSPARENTE;
            break;
      }
      for(uint8_t i = 0; i < a; i++) {
         for(uint8_t j = 0; j < l; j++)
            { matriz->grade_stack[i][j] = caractere; }
      }
   } 

   return matriz;
}

/* libera "Matriz de Texto", liberando cada uma
 * das suas estruturas complexas, como, uma 
 * possível 'StringArray', e a "Dimensão", contidas
 * nesta estrutura. */
void destroi_matriz_texto(MT m) {
   if (m->grade_heap != NULL) 
      { destroi_string_array(m->grade_heap, m->dimensao[0]); }
   puts("string de array interna destruída.");
   free(m->dimensao);
   puts("'dimensão' destruída.");
   free(m);
   puts("'matriz de texto' destruída.");
}

#include <string.h>
/* Cuida essencialmente da matriz representada 
 * por uma multidimensional array.
 * Pega todo objeto, pois já vem com a "dimensão"
 * embutida, portanto menos parametros, e menos
 * digitação para codificar, também mais legível.
 * Se a borda não "for acionada", então parte
 * do código dedicada a criação de barras
 * superior/inferior e laterais, não serão mostradas.*/
void visualiza_grade_stack(MT m, bool mostra_borda) {
   uint8_t c = m->dimensao[1];
   uint8_t l = m->dimensao[0];
   char* barra;

   if (mostra_borda) {
      barra = calloc(c, sizeof(char));
      memset(barra, '+', (c + 2));
      // imprime barra superior.
      printf(".%s.\n", barra);

   }

   for(uint8_t i = 0; i < l; i++) {
      if (mostra_borda)
         { printf("+ "); }
      for(uint8_t j = 0; j < c; j++)
         { printf("%c", m->grade_stack[i][j]); }
      if (mostra_borda)
         { puts(" +"); }
      else
         { puts(""); }
   }

   if (mostra_borda) {
      // imprime barra inferior.
      printf("*%s*\n", barra);
      // liberando as estruturas das 'barra's.
      free(barra);
   }
}
void visualiza_grade_heap(MT m, bool mostra_borda) {
   uint8_t c = m->dimensao[1];
   uint8_t l = m->dimensao[0];
   char* barra;

   if (mostra_borda) {
      barra = malloc(c + 1);
      memset(barra, '+', (c + 2));
      // imprime barra superior.
      printf(".%s.\n", barra);

   }

   for(uint8_t i = 0; i < l; i++) {
      if (mostra_borda)
         { printf("+ "); }
      for(uint8_t j = 0; j < c; j++)
         { printf("%c", m->grade_heap[i][j]); }
      if (mostra_borda)
         { puts(" +"); }
      else
         { puts(""); }
   }

   if (mostra_borda) {
      // imprime barra inferior.
      printf("*%s*\n", barra);
      // liberando as estruturas das 'barra's.
      free(barra);
   }
}

/* Por motivos de se está codificando algo, e precisa
 * se ver, além de ter demais informações sobre 
 * a 'Matriz de Texto'. */
void visualiza_matriz_texto_debug(MT m) {
   uint8_t a = m->dimensao[0];
   uint8_t l = m->dimensao[1];

   if (m->usando_heap)
      { visualiza_grade_heap(m, true); }
   else{
      visualiza_grade_stack(m, true);
   }
   printf("usando a 'heap'? ");
   if (m->usando_heap)
      { puts("verdadeiro"); }
   else
      { puts("falso"); }
   printf("dimensão: %dx%d\n", a, l);
}

/* A versão não para debug, 'display' mesmo.
 * Faz o mesmo que acima, porém, sem informações
 * extras. Também dá para pedir a borda ou não 
 * da "Matriz de Texto" que será mostrada. */
void visualiza_matriz_texto(MT m, bool mostra_borda) {
   if (m->usando_heap)
      { visualiza_grade_heap(m, mostra_borda); }
   else 
      { visualiza_grade_stack(m, mostra_borda); }
}


/* No 'Ponto' escreve um símbolo também dado.
 * Se o tal 'Ponto' extrapola as fronteiras
 * da 'Matriz de Texto', nada é feito. Uma
 * resposta de confirmação é retornada dizendo
 * se a operação foi realizada, ou negando. */
bool set(MT m, Ponto p, char simbolo) {
   /* objetos são inválidos.*/
   if (m == NULL || p == NULL)
      { puts("objetos inválidos!"); return false; }
   // dimensão do objeto extrapolada.
   uint8_t y = p[0];
   uint8_t x = p[1];
   uint8_t L = m->dimensao[1];
   uint8_t H = m->dimensao[0];
   if (x >= L || y >= H)
      { puts("fora dos limites!"); return false; }
   // operação depende qual "memória" está usando.
   if (m->usando_heap) 
      { m->grade_heap[y][x] = simbolo; }
   else
      { m->grade_stack[y][x] = simbolo; }
   // se chegar até aqui, então foi realizado com sucesso.
   return true;
}

/* Pega o atual caractére na 'Matriz de Texto'.
 * Entrega um valor, caso os limites passado
 * extrapolem as fronteiras do objeto, um 
 * erro será emitido. */
char get(MT m, Ponto p) {
   /* objetos são inválidos.*/
   if (m == NULL || p == NULL)
      { perror("algum dos objetos são inválidos."); }
   // dimensão do objeto extrapolada.
   uint8_t y = p[0];
   uint8_t x = p[1];
   uint8_t L = m->dimensao[0];
   uint8_t H = m->dimensao[1];
   if (x >= L || y >= H)
      { perror("fora do limite da 'MT'."); }
   // operação depende qual "memória" está usando.
   char dado;
   if (m->usando_heap) 
      { dado = m->grade_heap[y][x]; }
   else
      { dado = m->grade_stack[y][x]; }
   // se chegar até aqui, então foi realizado com sucesso.
   return true;
}

/* convertendo a MT para uma string. Quase o mesmo método de impressão
 * porém alocado numa string.
 */
char* mt_to_str(MatrizTexto m) {
   uint8_t C = m->dimensao[1], L = m->dimensao[0];
   /* basicamente a área do retângulo que forma, porem contabilizando
    * os caractéres de quebra-de-linha. O outro é o indexação para
    * cópia um-a-um de cada caractére.
    */
   uint16_t qtd = C * (L + 1) + 3;
   char* string = (char*)calloc(qtd, sizeof(char));
   // string apenas com espaços em brancos.

   for(uint8_t l = 0; l < L; l++) {
      if (m->usando_heap)
         strcat(string, m->grade_heap[l]);
      else
         strcat(string, m->grade_stack[l]);
      // adicionando quebra-de-linha...
      // próximo vago na string, de maneira linear.
      strcat(string, "\n");
   }

   return string;
}



