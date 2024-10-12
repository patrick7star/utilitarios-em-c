
static void nome_do_teste(char *nome) {
/* Imprime o separador entre os testes executados. */
   uint8_t str_length = strlen(nome);
   Dimensao d = dimensao_terminal();
   uint8_t largura = d[1] - 3;
   char* barra = calloc(largura, 1);

   // fazendo separador.
   for (uint8_t k = 0; k <= largura; k++) 
      { barra[k] = '-'; }

   // margens do texto.
   barra[5] = ' ';
   barra[5 + str_length + 1] = ' ';

   // colocando título no separado.
   for (uint8_t k = 0; k < str_length; k++) {
      if (k < str_length && nome[k] != '\0')
         { barra[k + 6] = nome[k]; }
   }

   printf("\n%s\n\n", barra);
   // não é preciso mais desta string, já foi impressa, então libera.
   // A dimensão criada também não é mais necessária.
   destroi_dimensao(d);
   free(barra);
}
/* contabilizando testes sem nomes/ ou mensagens passadas. */
static uint8_t testes_contagem = 0;

/* executa o teste, dado a função que o faz; também é preciso do nome que 
 * ele tem, pode ser o nome da função, ou não. */
static void executa_teste(char* nome, void (*funcao)()) {
   // imprime o nome do programa.
   if (strlen(nome) == 0) { 
      testes_contagem += 1;
      char mensagem_padrao[100];
      char* referencia = mensagem_padrao;
      sprintf(
         referencia, 
         "%dº teste anônimo", 
         testes_contagem
      );
      nome_do_teste(mensagem_padrao); 
   } else 
      { nome_do_teste(nome); }
   // executa função passada.
   funcao();
   // espaço para outro possível teste, ou só o 'shell'.
   puts("");
}

void executa_teste_interruptor(char* descricacao, Fn call, bool acionado) 
{
/* O mesmo que o 'executa_teste', porém este pode ser facilmente desativado
 * se for preciso, apenas mudando o parâmetro. */
   // não executa nada se for falso.
   if (strlen(descricacao) == 0)
   { 
      testes_contagem += 1;
      char mensagem_padrao[100];
      char* referencia = mensagem_padrao;
      sprintf(
         referencia, 
         "%dº teste anônimo", 
         testes_contagem
      );
      nome_do_teste(mensagem_padrao); 
         
   } else 
      nome_do_teste(descricacao);

   if (acionado)
      // quebra-de-linha no final.
      { call(); puts(""); }
   else
      puts("DESATIVADO TEMPORIAMENTE!");
}

void executa_testes(int total, ...) {
   Cronometro medicao = cria_cronometro();
   va_list args;
   size_t habilitados = 0;

   // como também conta o valor lógico se é para executa-lá no momento.
   va_start(args, total);

   for (uint8_t t = 1; t <= 2 * total; t+=2) {
      // primeiro pega a função.
      Fn funcao = va_arg(args, Fn);
      // a confirmação se é para executa-lá, ou não.
      bool e_para_executar = va_arg(args, int);

      if (e_para_executar) {
         executa_teste("", funcao);
         marca(medicao);
         habilitados++;
      }
   }
   va_end(args);

   // Para não compilar no Windows, assim mantém compatibilidade
   #ifndef _WIN64
   // Informando tempo total da execução.
   double tempo_total = marca(medicao);

   if (tempo_total <= 0)
      puts("\nexecução \"instântanea\".");
   else 
      printf(
         "\ntodos testem levaram %s\n", 
         tempo_legivel(tempo_total)
      );
   #elif defined(__linux__)
   printf ("há %lu testes desativados.\n", total - habilitados);
   #endif
}
