
#ifdef _WIN64
// Só compatível na plataforma Windows.
#include <time.h>
#include <synchapi.h>


/* série de testes */
void espera_pouco_segundos() {
   printf("tempo em espera...");
   fflush(stdout);
   Sleep(5000);
   puts("passado 8seg.");
}

void intervalo_de_milisegundos() {
   for(uint8_t k = 1; k <= 23; k++) {
      puts("passado 300 milisegundos.");
      fflush(stdout);
		Sleep(300);
   }
}
#endif
