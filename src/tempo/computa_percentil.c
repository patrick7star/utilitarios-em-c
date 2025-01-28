
struct tupla_duracao { TEMPO_TIPO t; uint64_t n; };
typedef struct tupla_duracao Duracao;

static Duracao transforma_para_nanoseg(TEMPO_TIPO t, uint64_t n) {
   uint64_t unidades = UINT64_MAX;

   switch (t) {
      case Segundo:
         // unidades = n * potencia(10, 9);
         unidades = n * 1.0e9;
         break;
      case Miliseg:
         // unidades = n * potencia(10, 6);
         unidades = n * 1.0e6;
         break;
      case Microseg:
         // unidades = n * potencia(10, 3);
         unidades = n * 1.0e3;
         break;
      case Nanoseg:
         unidades = n;
         break;
      case Minuto:
         // unidades = n * 60 * potencia(10, 9);
         unidades = n * 60 * 1.0e9;
         break;
      case Hora:
         // unidades = n * 3600 * potencia(10, 9);
         unidades = n * 3600 * 1.0e9;
         break;
      default:
         perror("não implementado para tal!");
   }

   Duracao resultado = { Nanoseg, unidades };
   return resultado;
}

static Duracao reducao_de_grandeza(Duracao nanosD) {
   uint64_t unidades = nanosD.n;
   TEMPO_TIPO tipo[4] = { Nanoseg, Microseg, Miliseg, Segundo };
   uint8_t posicao = 0;

   while (unidades > 1000) {
      unidades = (uint64_t)(unidades / 1000);
      posicao++;
   }

   Duracao resultado = { tipo[posicao], unidades };
   return resultado;
}

const char* tempotipo_str(TEMPO_TIPO tipo) {
   switch (tipo) {
      case Segundo:
         return "segundos";
      case Minuto:
         return "minutos";
      case Hora:
         return "horas";
      case Miliseg:
         return "milisegundos";
      case Microseg:
         return "microsegundos";
      case Nanoseg:
         return "nanosegundos";
      default:
         return "desconhecido";
   }
}

Duracao centesima_unidade(TEMPO_TIPO t, uint16_t  n) {
   // numa unidade de nanosegundos qualquer conversão fica muito fácil.
   Duracao novo = transforma_para_nanoseg(t, (uint64_t)n);

   // dividindo as unidades(em nanosegundos) em cem partes iguais.
   novo.n = (uint64_t)(novo.n / 100);

   /* transforma o período para algo mais legível, e até funcional,
    * pois a 'função de pausa' não aceita unidades com mais de um
    * milhar para qualquer grandeza de tempo. 
    */
   return reducao_de_grandeza(novo);
}
