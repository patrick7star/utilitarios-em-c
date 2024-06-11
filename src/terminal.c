

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "terminal.h"

#ifdef _POSIX_C_SOURCE           // Apenas válido para "Linux".
void destroi_dimensao(Dimensao d) 
   { if (d != NULL) free(d); }

static Dimensao cria_tupla() 
   { return (uint8_t*)calloc(2, sizeof(uint8_t)); }

// tupla com as dimensões do terminal.
Dimensao dimensao_terminal(void) {
   // buffer para pegar resultado.
   char resultado[15];
   // rodando comando e, lendo o resultado.
   FILE * comando = popen("tput lines cols", "r");
   Dimensao tupla = cria_tupla();

   #ifdef _INSTANCIA
   int lido = fread_unlocked(&resultado, 1, 8, comando);
   printf("Foram lido %d bytes.\n", lido);
   #else
   // Lendo resultado da chamada da execução do comando ...
   fread_unlocked(&resultado, 1, 8, comando);
   #endif
   pclose(comando);

   const size_t T = 10;
   char linhas[T];
   char colunas[T];

   memset(linhas, '\0', 10);
   memset(colunas, '\0', 10);

   char* ptr = strchr(resultado, '\n') + 1;
   size_t t = ptr - resultado;

   memcpy(linhas, resultado, t);
   strcpy(colunas, ptr);

   #ifdef _INSTANCIA
   printf("Linha:\"%s\" e coluna:\"%s\".\n", linhas, colunas);
   #endif

   // convertendo e retornando a tupla com os dados.
   tupla[0] = (uint8_t)atoi(linhas);
   tupla[1] = (uint8_t)atoi(colunas);
   return tupla;
}
#endif

/* Como a implementação varia de sistema para sistema, portanto, em cada tipo,
 * será compilado a sua. */
#ifdef _WIN64 // Válido apenas para platformas Windows.
const char NOME_ARQUIVO_RSL[] = "terminal.width.and.height.txt";

static void remove_arquivo_de_output(void) {
   const char shell[] = "powershell -Command";
   const char programa[] = "Remove-Item";
   const char opcoes[] = "-Force";
   char comando_de_remocao[100];
   sprintf(comando_de_remocao, "%s %s %s %s", shell, programa, opcoes, NOME_ARQUIVO_RSL);

   #ifdef _UT_TERMINAL
   printf("como ficou o comando:\n\t'%s'\n", comando_de_remocao);
   #endif

   system(comando_de_remocao);
}

static void extrai_o_resultado_do_cmd(char* resultado, size_t t) {
   // Filtrando o conteúdo do comando acima...
   FILE* arquivo_do_resultado;
   fopen_s(&arquivo_do_resultado, NOME_ARQUIVO_RSL, "rt");
   fread(resultado, sizeof(char), t, arquivo_do_resultado);
   fclose(arquivo_do_resultado);

   #ifdef _UT_TERMINAL
   printf("o que foi extraído: '%s'\n", resultado);
   #endif
}

static void processa_e_decompoe_resultado(char* materia_prima, char* altura, 
  size_t H, char* largura, size_t L)
{
   // Slice de string depois do separador dos valores.
   char* segunda_parte = strchr(materia_prima, ',') + 1;
   // Posição onde acaba, e portanto o total de caractéres.
   size_t posicao_termino_largura = strcspn(materia_prima, ",");
   size_t posicao_termino_altura = strcspn(segunda_parte, "\r\n");
   
   /* Como a largura vem sempre primeiro, então está é primeira processada. */
   memcpy_s(largura, 5, materia_prima, posicao_termino_largura);
   memcpy_s(altura, 5, segunda_parte, posicao_termino_altura);

   #ifdef _UT_TERMINAL
   printf("altura: '%s'\tlargura: '%s'\n", altura, largura);
   #endif
}

Dimensao dimensao(void) {
/* Função pega as dimensões do terminal que está sendo lançadas, retorna
 * uma 'dimensão', onde sua primeira ocupação fica a 'altura', e a segunda
 * a 'largura', obviamente! 
 *
 * NOTA: Não é uma função thread-safe. Também pode quebrar se a largura, ou
 * a altura do terminal passarem um inteiro positivo de 8-bits. */
   // Formando o comando, e executando-o...
   char novo_cmd[200];
   char conteudo[10];
   char largura[5], altura[5];
   const char COMANDO[] = {
      // Programa sendo utilizado:
      "powershell -Command "
      // Comando e variável com informação:
      "Write-Host $Host.UI.RawUI.WindowSize"
   };

   // Limpando cada uma das declaradas acima:
   memset(largura, '\0', 5);
   memset(altura, '\0', 5);
   // memset(novo_cmd, '\0', 200);
   
   /* Formando um comando que pega o resultado, então despeja num arquivo 
    * temporário.Em seguida, executa o comando formado.  */
   sprintf(novo_cmd, "%s > %s", COMANDO, NOME_ARQUIVO_RSL);
   system(novo_cmd);

   // Filtrando o conteúdo do comando acima...
   extrai_o_resultado_do_cmd(conteudo, 10);
   // Processando saída...
   // Slice de string depois do separador dos valores.
   processa_e_decompoe_resultado(conteudo, altura, 5, largura, 5);
   remove_arquivo_de_output();

   // Último passo é apenas converter as strings para inteiro.
   Dimensao proporcao = malloc(2 * sizeof(uint8_t));
   proporcao[0] = (uint8_t)atoi(altura);
   proporcao[1] = (uint8_t)atoi(largura);

   return proporcao;
}
#endif


#if defined(_UT_TERMINAL) && defined(_WIN64)
const char* comandos[] = {
   "powershell -Command Write-Host $Host.UI.RawUI.WindowSize.Width",
   "powershell -Command Write-Host $Host.UI.RawUI.WindowSize.Height",
   "powershell -Command Write-Host $Host.UI.RawUI.WindowSize"
};

void testando_comando_que_usarei_como_base(void) {
   system(comandos[0]);
   system(comandos[1]);
   system(comandos[2]);
}

void tecnica_de_extrair_os_resultados(void) {
   // Formando o comando, e executando-o...
   char novo_cmd[200];
   char conteudo[10];
   char largura[5], altura[5];

   // Limpando cada uma das declaradas acima:
   memset(largura, '\0', 5);
   memset(altura, '\0', 5);
   memset(novo_cmd, '\0', 200);
   
   sprintf(novo_cmd, "%s > %s", comandos[2], NOME_ARQUIVO_RSL);
   system(novo_cmd);

   // Filtrando o conteúdo do comando acima...
   extrai_o_resultado_do_cmd(conteudo, 10);
   // Processando saída...
   // Slice de string depois do separador dos valores.
   processa_e_decompoe_resultado(conteudo, altura, 5, largura, 5);
   remove_arquivo_de_output();
}

void funcao_que_retorna_valores_da_dimensao(void) {
   Dimensao resultado = dimensao();
   printf(
      "\nO resultados totalmente processados:\n\tAltura: "
      "%u\n\tLargura: %u\n\n", 
      resultado[0], resultado[1]
   );
   free(resultado);
}

int main(void) {
   // testando_comando_que_usarei_como_base();
   // tecnica_de_extrair_os_resultados();
   funcao_que_retorna_valores_da_dimensao();
   return EXIT_SUCCESS;
}
#elif defined(_UT_TERMINAL) && defined(_POSIX_C_SOURCE)
void o_teste_apenas_chama_e_mostra_os_resultados(void) {
   puts("\nComputando dimensão mínima:");
   Dimensao A = dimensao_terminal();
   printf("\ty=%u x=%u\n", A[0], A[1]);
   destroi_dimensao(A);
}

void main(void) {
   o_teste_apenas_chama_e_mostra_os_resultados();
}
#endif


