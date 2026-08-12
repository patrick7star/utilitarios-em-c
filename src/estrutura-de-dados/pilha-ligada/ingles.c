/* Tradução de vários métodos, constantes e estruturas para o inglês. Os métodos gerais
 * são conhecidos sempre em inglês, por isso esta interface apenas padroniza para o que já
 * é amplamente conhecido. 
 *   Acho que este pode ser posto em qualquer parte do outro, porém, será posicionado 
 * exatamente onde foi recortado.
 */

LinkedStack new_pl (void)
   { return cria_pl(); }

bool delete_pl (LinkedStack s)
   { return destroi_pl(s); }

bool push_pl(LinkedStack s, generico_t e)
   { return coloca_pl(s, e); }

generico_t pop_pl(LinkedStack s)
   { return retira_pl(s); }

size_t length_pl (LinkedStack s)
   { return comprimento_pl(s); }
// Nome mais curto, porque um monte de funções já usam tal definição.
size_t len_pl (LinkedStack s)
   { return length_pl(s); }

bool empty_pl (LinkedStack s)
   { return vazia_pl(s); }

void* top_pl (LinkedStack s)
   { return topo_pl(s); }

void print_pl (LinkedStack s, ToString f)
   { imprime_pl(s, f);  }

void extend_pl(LStack a, LStack b)
   { extende_pl(a, b); }

void reverse_pl(LStack In)
   { inverte_pl(In); }

bool destroy_pl(LStack In)
   { return destroi_pl(In); }
