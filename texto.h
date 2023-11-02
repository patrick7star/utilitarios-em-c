

/* computar a dimensão do terminal e seus 
 * derivados. Também inputs personalizados.
 */
#include <stdint.h>
#include <stdbool.h>
#include "terminal.h"
#include "ponto.h"

/* limitações para "grade da stack" definidas
 * em tempo de compilação, são baseadas na tela
 * do computador agora disponíveis, podem ser
 * alteradas no futuro, porém não serão maior
 * do que isso aí. */
#define ALTURA 40
#define LARGURA 151

#ifndef TEXTO_MT_H
# define TEXTO_MT_H
   /* cria uma tupla de dois inteiros, onde o primeiro
    * espaço indica o comprimento vertical(altura),
    * e o segundo, o comprimento horizontal(largura). */
   Dimensao cria_dimensao(uint8_t a, uint8_t l);

   /* Representação estrutural do tipo de dado.*/
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

   /* criação e destruição do tipo de dado
    * 'Matriz de Texto'. 
    */
   extern MT cria_matriz_texto(Dimensao d, enum PlanoFundo f); 
   extern void destroi_matriz_texto(MT m); 

   /* altera a posição no 'ponto', se este for 
    * válido, pelo 'símbolo' passado. Retorna a 
    * confirmação se tal operação foi realizada
    * com sucesso. 
    */
   extern bool set(MT m, Ponto p, char simbolo); 

   /* acessa e retorna o valor no 'ponto', novamente,
    * se tal posição respeitar os limites da 
    * 'Matriz de Texto'. Retorna o elemento na 
    * posição que se queria acesso. 
    */
   extern char get(MT m, Ponto p); 

   // tipos de visualizações da matriz.
   extern void visualiza_matriz_texto_debug(MT m); 
   extern void visualiza_matriz_texto(MT m, bool mostra_borda); 
   // conversão para string.
   extern char* mt_to_str(MT m);

#endif 


/* outros nomes que as funções acima também podem
 * ser chamadas:*/
#ifndef NOME_ALTERNATIVO_DE_TEXTO_H
# define NOME_ALTERNATIVO_DE_TEXTO_H
   extern bool altera(MT m, Ponto p, char simbolo) 
      { return set(m, p, simbolo); }

   extern char obtem(MT m, Ponto p)
      { return get(m, p); }

   extern void visualiza_mt_debug(MT m) 
      { visualiza_matriz_texto_debug(m); }

   extern void visualiza_mt(MT m, bool borda) 
      { visualiza_matriz_texto(m, borda); }
#endif //NOME_ALTERNATIVO_DE_TEXTO_H
