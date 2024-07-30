
#include <stdbool.h>

#ifndef LEGIVEL_H
#define LEGIVEL_H

 // converte o tempo em segundos para uma forma mais legível.
 extern char* tempo_legivel(double segundos);

 // converte o tamanho(em bytes) para uma forma mais humamente legível.
 extern char* tamanho_legivel(size_t); 

 // coloca um valor de modo mais legivel.
 extern char* valor_legivel(size_t); 

 /* Muitas funções em bibliotecas do sistema, ou padrões do ISO C, retorna
  * os seguintes valores como resultado. Apenas criando um enum, assim com
  * tais valores conversíveis com rótulos legíveis adequados. */
 enum resultado_generalizado_de_retorno { Failed = -1, Okay = 0 };

#endif //LEGIVEL_H
