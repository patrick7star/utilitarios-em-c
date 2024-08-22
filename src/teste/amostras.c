
#include "progresso.h"
// Funções que são amostras para os protótipos:

// [okay]
int capitaliza_palavras(void) {
   puts("Capitalizando tais palavras ...");

   for (size_t i = 1; i <= FRUTAS; i++) {
      char* fruta = (char*)frutas[i - 1];
      size_t size = strlen(fruta) + 1;
      char copia[size];

      strcpy(copia, fruta);
      copia[0] = (char)toupper(copia[0]);
      printf("\t%s ===> %s\n", fruta, copia);
   } 
   return EXIT_SUCCESS;
}

// [okay]
int string_alternada(void) {
   puts("Colocando caractéres de forma alternada ...");
   for (size_t i = 1; i <= GIRLS_NAMES; i++) {
      char* nome = (char*)girls_names[i - 1];
      size_t size = strlen(nome) + 1;
      char copia[size];

      strcpy(copia, nome);

      for (size_t i = 0; i < strlen(copia); i++) {
         if (i % 2 == 0) 
            copia[i] = toupper(copia[i]);
         else
            copia[i] = tolower(copia[i]);
      }
      printf("\t%s ===> %s\n", nome, copia);
   } 
   return EXIT_SUCCESS;
}

// [falha]
int carregando_ate_cem(void) {
   PG bar = cria_bp(Simples, 100, 50);
   size_t contagem = 1;
   // 70 milisegundos ...
   struct timespec pausa = { 0, 70000000 };

   do {
      visualiza_bp(&bar);
      atualiza_bp(&bar, contagem++);
      nanosleep(&pausa, NULL);

      if (contagem == 70)
         return EXIT_FAILURE;
   } while (contagem <= 100);

   return EXIT_SUCCESS;
}

// [falha]
int simples_mensagem_do_fortune(void) {
   FILE* stream = popen("fortune", "r");
   char buffer[5000];

   fread(buffer, sizeof(char), 5000, stream);
   pclose(stream);
   puts(buffer);

   return EXIT_FAILURE;
}

// [falha]
int executa_algo_mas_crash(void) {
   for (int p = 1; p <= 11; p++) {
      puts(veiculos[p - 1]);
      if (p == 7)
         assert (p == 8);
   }
   return EXIT_SUCCESS;
}
