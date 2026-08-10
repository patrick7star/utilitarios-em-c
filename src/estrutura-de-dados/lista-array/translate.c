
Vec new_al(void)
   { return cria_al(); }

Vec new_with_capacity_al(size_t l)
   { return cria_com_capacidade_al(l); }

Vec from_al(int t, ...) { 
   va_list args;
   Vector output;

   va_start(args, t);
   output = cria_de_al(t, args);
   va_end(args);

   return output;
}

bool drop_al(Vetor obj)
   { return destroi_al(obj); }

bool drop_with_al(Vector obj, Drop f)
   { return destroi_interno_al(obj, f); }

GenT pop_al(Vetor obj)
   { return remove_al(obj); }

GenT remove_index_al(Vetor obj, size_t p)
   { return remove_indice_al(obj, p); }

GenT get_al(Vetor obj, size_t p)
   { return indexa_al(obj, p); }

bool push_al(Vetor obj, GenT e)
   { return insere_al(obj, e); }

bool empty_al(Vetor obj)
   { return vazia_al(obj); }

size_t length_al(Vetor obj)
   { return tamanho_al(obj); }

size_t gap_al(Vetor obj)
   { return vacuo_al(obj); }

void print_al(Vetor obj, ToString f)
   { return imprime_lista_al(obj, f); }
