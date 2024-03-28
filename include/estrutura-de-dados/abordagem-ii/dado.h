
#include <stdbool.h>
#include <stddef.h>

#ifndef DADO_H
#define DADO_H
/* dado interno ao "embrulho" Dado, mas de forma centralizada para 
 * facil reescrita quando necessário. Fazendo a única alteração
 * necessária neste arquivo, quando o Dado for copiado para abranger
 * outro tipo de dado. */
 typedef unsigned char TIPO_DO_DADO;
 struct dado { TIPO_DO_DADO dado; };

 // para dados genéricos.
 typedef struct dado* Dado;
 typedef Dado* ArrayDado;

 // criação e liberação do 'Dado'.
 extern Dado cria_dado(TIPO_DO_DADO dado);
 extern void destroi_dado(Dado d); 
 extern Dado clona_dado(Dado d);
 extern Dado cria_dado_branco();

 /* cria uma array, inicialmente 'null' do tipo de dado acima. */
 extern Dado* cria_array_dado(size_t n); 
 extern void destroi_array_dado(Dado* a, size_t q);

 // verifica se dois dados são iguais.
 // [será descontinuada]
 extern bool dados_iguais(Dado a, Dado b);
 /* comparações entre os dados, a depender do dado interno, talvez
  * alguns 'métodos' abaixo não seja implementado, gerando um erro
  * na execução, portanto interrupção do programa. */
 extern bool lt_dado(Dado, Dado);
 extern bool le_dado(Dado, Dado);
 extern bool gt_dado(Dado, Dado);
 extern bool ge_dado(Dado, Dado);

 // pode imprimir o dado e transforma-lô numa string.
 extern void visualiza_dado(Dado d); 
 // [será descontinuada]
 extern char* dado_para_string(Dado d);
 extern char* dado_to_str(Dado);

 // contabilização de instâncias deste tipo de dado.
 extern size_t instancias_dado();

 // obter e alterar o 'dado crú' interno ao embrulho Dado.
 extern TIPO_DO_DADO get_dado(Dado);
 extern void set_dado(Dado, TIPO_DO_DADO);

#endif // DADO_H
