

/* Componentes de array da 'deque ligada' colocados para cá também.*/

#define SIZE_ARRAY_DL sizeof(DequeLigada)
typedef DequeLigada* ARRAY_DL;

// cria uma array de 'n' tamanho com deques-ligadas.
ARRAY_DL cria_array_dl(uint32_t n) {
   ARRAY_DL array = (ARRAY_DL)calloc(n, SIZE_ARRAY_DL);

   for(uint32_t i = 0; i < n; i++)
      array[i] = cria_dl();

   return array;
}

// destrói uma array-de-deque-ligadas de 'n' tamanho.
void destroi_array_dl(ARRAY_DL array, uint32_t n) {
   for(uint32_t i = 0; i < n; i++)
      destroi_dl(array[i]);
   free(array);
}
