

/* Tupla feita especialmente para compactar vários argumentos num só,
 * que será passado para uma thread, já que ela só aceita um único.
 * Nele contendo informações que serão alteradas dentro da thread de 
 * acordo com a lógica lá imposta. E informações que não serão alteradas,
 * mas copiadas, novamente de acordo com a lógica interna que rege 
 * a thread.
 */
struct tupla_para_thread {
   uint8_t* porcentagem;

   bool* estado;

   uint16_t unidades;

   TEMPO_TIPO grandeza;
};

typedef struct tupla_para_thread TuplaThread;
