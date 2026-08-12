/* Tradução de vários métodos, constantes e estruturas para o inglês. Os métodos gerais
 * são conhecidos sempre em inglês, por isso esta interface apenas padroniza para o que já
 * é amplamente conhecido. 
 *   Acho que este pode ser posto em qualquer parte do outro, porém, será posicionado 
 * exatamente onde foi recortado.
 */

PilhaLigada new_pl (void)
   { return cria_pl(); }

bool delete_pl (PilhaLigada s)
   { return destroi_pl(s); }

bool push_pl(PilhaLigada s, generico_t e)
   { return coloca_pl(s, e); }

generico_t pop_pl(PilhaLigada s)
   { return retira_pl(s); }

size_t len_pl (PilhaLigada s)
   { return comprimento_pl(s); }

bool empty_pl (PilhaLigada s)
   { return vazia_pl(s); }

void* top_pl (PilhaLigada s)
   { return topo_pl(s); }

void print_pl (PilhaLigada s, ToString f)
   { imprime_pl(s, f);  }

generico_t* stack_to_array_pl(PilhaLigada S)
   { return pilha_to_array_pl(S); }

