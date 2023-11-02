

/* computar a dimensão do terminal e seus 
 * derivados. Também inputs personalizados.
 */
#include <stdint.h>

#ifndef TERMINAL_H
# define TERMINAL_H

   typedef uint8_t* Dimensao;
   /* tupla com dimensão do terminal, onde o primeiro
    * elemento é o nª de linhas, e o segundo só
    * pode ser as colunas. */
   extern Dimensao dimensao_terminal(); 

   // destrói o tipo de dado específico.
   extern void destroi_dimensao(Dimensao d);

   /* tipos de plano para, tanto a 'Matriz de Char',
    * como para a 'String de Array'. */
   // para manter compatibilidade com que usa este apelido.
   typedef char** StringArray;
   typedef StringArray StrArray;
   typedef enum plano_de_fundo { Invisivel, Visivel  } PlanoFundo;

   extern StringArray cria_string_array(
      uint8_t linhas, 
      uint8_t colunas, 
      PlanoFundo tipo
   ); 
   extern StrArray cria_string_array_manual(uint8_t, ...);
   extern void destroi_string_array(StrArray, uint8_t); 

   /* visualiza a 'array de string', criando uma borda
    * para delimitar sua extensão. */
   extern void visualiza_string_array(StrArray, int8_t, uint8_t);

#endif 
