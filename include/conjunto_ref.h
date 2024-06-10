
#include <stddef.h>
#include <stdbool.h>

#ifndef _CONJUNTO_REF
 // Valor genérico, cĺássico apelido:
 typedef void* generico_t;
 // Todos apelidos que a estrutura 'conjunto' assume:
 typedef struct tabela_de_dispersao set_t, conjunto_t, 
  *Conjunto, *Set, SET, CONJUNTO; 

 // Renomeando todos traços referênte aos 'fat pointers' abaixo:
 typedef size_t (*Hash)(generico_t, size_t);
 typedef bool (*Eq)(generico_t, generico_t);
 typedef char* (*ToString)(generico_t);

 // Alocação e desalocação da estrutura:
 Conjunto cria_com_capacidade_set(size_t, Hash, Eq);
 Conjunto cria_branco_set (void); 
 bool destroi_set (Conjunto); 
 /* Destroi dados triviais da 'heap', que foram inseridos no 'conjunto'. É
  * preciso ser muito precavido com o uso de tal método, pois se nem todo 
  * 'dado' estiver na 'heap', então tal chamada fará com que o programa 
  * seja imediatamente interrompido; já se a liberação de tal 'alocação 
  * dinâmica' não for simples, então obviamente, toda memória não será 
  * desalocada. */
 void destroi_interno_set(Conjunto); 
 /* Essencial na construção da instância, pois é possível postergar métodos
  * para realizar tais operações sobre a isntância. Está função geralmente
  * é usada quando a função 'cria_branco_set' é chamada, ao invés das demais
  * que cuidam de alocação. */
 bool adiciona_metodos_set (Conjunto, Hash, Eq); 

 // Operações que alteram a estrutura interna do 'conjunto':
 bool adiciona_set (Conjunto, generico_t); 
 bool deleta_set (Conjunto, generico_t); 

 // Operações que validam, ou retorna informação da estrutura(conjunto):
 bool pertence_set (Conjunto, generico_t); 
 bool vazia_set (Conjunto); 
 size_t tamanho_set (Conjunto); 
 void impressao_generica(Conjunto, ToString); 

#endif
