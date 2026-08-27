HashTable new_with_capacity_ht (size_t cP, Hash f, Eq g)
   { return cria_com_capacidade_ht(cP, f, g); }

HashTable new_ht (Hash f, Eq g)
   { return cria_ht(f, g); }

HashTable default_ht (void)
   { return cria_branco_ht(); }

bool delete_ht (HashTable m) 
   { return destroi_ht(m); }

bool add_ht (HashTable m, generico_t key, generico_t vl) 
   { return insere_ht(m, key, vl); }

bool update_ht (HashTable m,  generico_t key,  generico_t nvl)
   { return atualiza_ht(m, key, nvl); }

bool remove_ht (HashTable m, generico_t key)
   { return deleta_ht(m, key); }

bool contains_ht (HashTable m, generico_t key)
   { return contem_ht(m, key); }

generico_t get_ht (HashTable m, generico_t key)
   { return obtem_ht(m, key); }

bool empty_ht (HashTable m) { return vazia_ht(m); }

size_t len_ht (HashTable m) { return tamanho_ht(m); }

void print_ht(HashTable m, ToString f, ToString g)
   { imprime_ht(m, f, g); }

bool drop_ht(HashTable m)
   { return destroi_ht(m); }

bool drop_i_ht(HashTable m, Drop f, Drop g)
   { return destroi_interno_ht(m, f, g); }

 IterHT new_iter_ht (HashTable m)
   { return cria_iter_ht(m); }

 IterHT clone_iter_ht(IterHT iter)
   { return clona_iter_ht(iter); }

 void drop_iter_ht(IterHT iter)
   { return destroi_iter_ht(iter); }

 size_t count_iter_ht(IterHT iter)
   { return contagem_iter_ht(iter); }

 bool exhausted_ht(IterHT iter)
   { return consumido_iter_ht(iter); }
