
#include <stdbool.h>

#ifndef LEGIVEL_H
#define LEGIVEL_H

 // converte o tempo em segundos para uma forma mais legível.
 extern char* tempo_legivel(double segundos);

 // converte o tamanho(em bytes) para uma forma mais humamente legível.
 extern char* tamanho_legivel(size_t); 

 // coloca um valor de modo mais legivel.
 extern char* valor_legivel(size_t); 

 /* Muitas chamadas de sistema(systemcall), ou fuções do ISO C, retornam
  * os seguintes valores como resultado: o valor negativo(-1) para 
  * resultados fracassados, e o valor(0) como retornos positivos. Por isso,
  * estou criando um enum, assim com tais valores conversíveis com rótulos 
  * que deixaram os códigos mais legíveis e estéticamente mais belos. 
  */
 enum resultado_generalizado_de_retorno { Fail = -1, Okay = 0 };

#endif //LEGIVEL_H
