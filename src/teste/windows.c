
#ifndef _WIN64
// Só compatível na plataforma Windows.
#include <time.h>

enum submultiplo_de_tempo { NANOSEG, MICROSEG, MILISEG, SEG };
typedef enum submultiplo_de_tempo ORDEM;

/* modificação da função 'sleep' para aceitar de forma facil todos 
 * arrangos possíveis de tempo do sistema. 
 */
static void pausa(ORDEM escala, uint16_t t) {
   size_t tamanho = sizeof(struct timespec);
   struct timespec* duracao;
   duracao = (struct timespec*)malloc(tamanho);
   /* converte cada tempo em 'nanosegundos', dado
    * sua ordem, claro que se for 'nanoseg', então
    * é passado direto como argumento. */
   switch (escala) {
      case NANOSEG:
         duracao->tv_nsec = t;
         duracao->tv_sec = 0;
         if (t > 1000)
            perror("só valor menores que mil nesta ORDEM.");
         break;
      case MICROSEG:
         duracao->tv_nsec = t * 1000;
         duracao->tv_sec = 0;
         if (t > 1000)
            perror("só valor menores que mil nesta ORDEM.");
         break;
      case MILISEG:
         duracao->tv_nsec = t * 1000000;
         duracao->tv_sec = 0;
         if (t > 1000)
            perror("só valor menores que mil nesta ORDEM.");
         break;
      case SEG:
         duracao->tv_nsec = t * 100;
         duracao->tv_sec = t;
         break;
   }
   /* espera em 'nanosegundos', mesmo que 
    * não tenha sido isso inicialmente, porém
    * lembre-se: alguns milhares de 'nanosegundos' são
    * alguns meros 'microsegundos', e alguns
    * milhares de 'microsegundos' são só alguns 
    * 'milisegundos', e assim em diante até 'segundos'. */
   nanosleep(duracao, NULL);
}

/* série de testes */
void espera_pouco_segundos() {
   printf("tempo em espera...");
   fflush(stdout);
   pausa(SEG, 5); 
   puts("passado 8seg.");
}

void intervalo_de_milisegundos() {
   for(uint8_t k = 1; k <= 23; k++) {
      puts("passado 300 milisegundos.");
      fflush(stdout);
      pausa(MILISEG, 300);
   }
}
#endif
