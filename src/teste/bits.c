
static char algarismo(uint8_t alg) {
/* Transforma um algarismo(valor de 0 à 9) na sua versão caractére. Se
 * o valor passado exceder isso, gerará um erro. */
   switch (alg) {
      case 0:
         return '0';
      case 1:
         return '1';
      case 2:
         return '2';
      case 3:
         return '3';
      case 4:
         return '4';
      case 5:
         return '5';
      case 6:
         return '6';
      case 7:
         return '7';
      case 8:
         return '8';
      case 9:
         return '9';
      default:
         perror("não é válido para tal número(apenas 0 à 9).");
         abort();
   }
}

extern char* binario_str(size_t decimal) {
/* O modo de computar os digitos binários são seguinte: Pegado o valor
 * decimal(um inteiro de base 10), nós contabilizando o total de 
 * dígitos na base dois que seu valor produziria. Criamos uma array
 * com mais ou menos este tamanho para colocar todos dígitos binários
 * correspodentes. Então criamos uma string dinâmica, com o mesmo 
 * tamanho da array com dígitos obviamente, iteramos todos valores
 * que representam dígitos na array, porém, acessando primeiro o último
 * elemento da array(o mais a direita), pois o modo de acha-lôs é 
 * invertido, colocando último "dígito" acessado na array como o 
 * primeiro na array de caractéres(string), e o primeiro dígito na 
 * array como o último "dígito" na string. 
 */
   if (decimal == 0) return "0000";
   #ifdef __linux__
   uint8_t qtd_digitos = floor(log2(decimal)) + 1;
   #elif defined(_WIN32)
   double x = log2((double)decimal);
   double y = floor(x);
   uint8_t qtd_digitos = y + 1;
   #endif
   /* criando a VLA depois de calcular a quantidade de dígitos exata.*/
   uint8_t digitos[qtd_digitos];
   // acrescenta mais uma 'vagão' na string para o caractére nulo.
   char* bitpattern = malloc((qtd_digitos + 1) * sizeof(char));
   // já marcando a string com o caractére 'nulo'.
   bitpattern[qtd_digitos] = '\0';

   /* os retos da divisão por 2 dão o equivalente em binário, porém
    * numa ordem inversa. Também é necessário dividir o valor inicial
    * por 2, já que deixando inalterado, produziria sempre o mesmo
    * dígito. */
   for (size_t i = 0; i < qtd_digitos; i++, decimal /= 2) 
      digitos[i] = decimal % 2; 

   /* o primeiro na string é o último na 'array de dígitos', e assim em
    * diante. */
   for (size_t k = 0; k < qtd_digitos; k++) 
      bitpattern[k] = algarismo(digitos[qtd_digitos - (k + 1)]);

   return bitpattern;
}

uint8_t* binario_complemento_de_dois_oito_bits(uint8_t n) {
/* Descobre mesmo o padrão binário do valor, provavelmente escrito em
 * complemento de dois, para especificamente 8-bits.
 */
   uint8_t* lista_de_bits = malloc(8 * sizeof(uint8_t));
   // mascaras necessárias para operações AND com todos bits.
   uint8_t mascaras[] = { 
      0x01, 0x02, 0x04, 0x08, 
      0x10, 0x20, 0x40, 0x80
   };

   /* A captura de bits será feita da direita para esquerda. */
   for (size_t i = 0; i < 8; i++) {
      /* extração do bit é simples, uso a máscara compatível ao seu 
       * atual algarismo para manter o valor apenas do algarismo desejado
       * e zerar os demais, então aplico um shift à direita, de acordo
       * quanto tal algarismo está da ponta direita. 
       * A indexação inversa é necessária pois está maquina, e as demais
       * usadas são todas little-endian.
       */
      // indexando na última casa, já que a CPU é 'little-endian'.
      lista_de_bits[8 - (i + 1)] = (n & mascaras[i]) >> i;
   }
   return lista_de_bits;
}

static void inverte_array(uint8_t* array, size_t t) {
/* inverte a ordem dos itens na array. O algoritmo para fazer tal coisa
 * é o seguinte: executa o swap entre todos pares de pontas da array,
 * tipo, primeira troca com a última, a segunda com a penúltima, a 
 * terceira com a antipenúltima e assim em diante. O total de swaps
 * é metade da array obviamente. 
 */
   for (size_t p = 0; p < (t / 2); p++) {
      uint8_t salvo = array[p];
      array[p] = array[t - (p + 1)];
      array[t - (p + 1)] = salvo;
   }
}

uint8_t* binario_complemento_de_dois(size_t n) {
   // computando o total de dígitos binários necessários.
   uint8_t qtd = floor(log2(n)) + 1;
   /* Todos valores serão armazenados numa array de bytes, pois os 
    * únicos valores que serão extraídos estão são 0 e 1. */
   uint8_t* lista_de_bits = malloc(qtd * sizeof(uint8_t));
   // mascaras necessárias para operações AND com todos bits.
   size_t mascaras[qtd]; 

   // complementando máscaras...
   for (size_t p = 1; p <= qtd; p++)
      mascaras[p - 1] = (size_t)pow(2, p - 1);

   /* A captura de bits será feita da direita para esquerda. */
   for (size_t i = 0; i < qtd; i++) {
      /* extração do bit é simples, uso a máscara compatível ao seu 
       * atual algarismo para manter o valor apenas do algarismo desejado
       * e zerar os demais, então aplico um shift à direita, de acordo
       * quanto tal algarismo está da ponta direita. 
       * A indexação inversa é necessária pois está maquina, e as demais
       * usadas são todas little-endian.
       */
      // indexando na última casa, já que a CPU é 'little-endian'.
      lista_de_bits[i] = (n & mascaras[i]) >> i;
   }
   // faz uma inversão...
   return lista_de_bits;
}
