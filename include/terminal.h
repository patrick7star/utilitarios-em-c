
#ifndef  TERMINAL_H
# define TERMINAL_H
#include <stdint.h>

/* Fazendo a biblioteca multiplataforma. Isso é importante aqui, pois faz
 * tal lib também utilizável no Windows. */
 #if defined(_WIN32)
  #ifdef __LIB_DLL__
    #define CROSSLIB __declspec(dllexport)
  #else
    #define CROSSLIB __declspec(dllimport)
  #endif
 #elif defined(__linux__)
    #define CROSSLIB
 #endif

 /* Resultado com linhas e colunas representando a dimensão da tela do 
  * console. Tipo de retorno que não é preciso desalocar posteriormente.*/
 struct TerminalSize { uint8_t linhas; uint8_t colunas; };

 #ifdef __linux__
 /* A dimensão é apenas uma array com dois inteiros positivos de 8-bits,
  * onde o primeiro valor é a 'altura', e o segundo a 'largura'. */
 typedef uint8_t *Dimensao, dimensao_t, DIMENSAO;

 /* Tupla com dimensão do terminal, onde o primeiro elemento é o nª de 
  * linhas, e o segundo só pode ser as colunas. */
 Dimensao dimensao_terminal (void); 
 void		 destroi_dimensao	 (Dimensao d);

 #endif

 /* Retorna a dimensão, nos devidos campos nomeados, numa tupla. */
 CROSSLIB struct TerminalSize obtem_dimensao (void);

 /* NOTA: Sim, a outra função não parece aqui. Tome isso como algo que foi
  *       descontinado. Por quê? Eu ja estava pensando em deleta-la, mas 
  *       muita depedência dela, então na versão do Windows realmente é uma
  *       uma oportunidade. Isso pois, aqui, eu só tenho a opção de não 
  *       fazer-la multi-plataforma, que foi o que fiz.
  */
#endif 
