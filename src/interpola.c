/*   Vamos fazer algumas funções e módulos inteiros aqui, portável para Python.
 * Por enquanto vejo apenas a aleatório como único. Ele tem uma função que 
 * retorna palavras baseado no dicionário instalado no Linux, ou dado pela
 * uma variável de ambiente. Ele foi o único simples, porém distinto método 
 * que ambos utilitários não compartilham, portanto, não ficariam redundantes.
 *
 *   O negócio inicial é apenas um protótipo, por isso, mesmo que há em Python 
 * uma versão similar, aqui serve apenas como o que deve ser feitos para 
 * possíveis coisas únicas. o sorteador de palavras aleatórios também é algo 
 * que vejo como único deste utilitários em C.
 */

// Biblioteca do módulo:
#include "aleatorio.h"
#include "impressao.h"
// Biblioteca de terceiros instalados:
#include <Python.h>

const char* const PALAVRA_ALEATORIA_DOC = {
 "Sorteia uma palavra randômica do dicionário, este geralmente nativo do "
 "sistema operacional. Se não houver um dicionário(usa como padrão o do " 
 "Linux), e você tem algum dicionário com palavras(uma palavra por linha), "
 "você pode adiciona-lo definindo a variável DICIONARIO_UTILITARIOS como "
 "o tal caminho para o arquivo. "
};
const char* const MODULO_DOC = {
   "Compatibilidade do código em C para o Python. Algumas funções e tipos de "
   "dados exclusivos deste código, nesta linguagem, serão portados para "
   "Python. Então, neste módulo haverá algumas delas. Nâo todas, pois como "
   "disse, algumas são bem redudantes, o a linguagem em específico permite "
   "capilaridades que, uma versão ficaria bem limitada do que ela pode fazer "
   "na outra. Também ao fato que linguagens tem objetivos diferentes, assim "
   "ficaria também algo sem sentido em outra tal funcionalidade."
};
const char* const COLORI_STRING_DOC = {
   "Adiciona caractéres especiais, e protocolos ANSI numa string, assim "
   "ela fica colorida."
};


static PyObject* palavra_aleatoria_embrulho(PyObject* self, PyObject* args);
static PyObject* colori_string_embrulho(PyObject* self, PyObject* args);


static PyMethodDef tabela_de_metodos[] = {
   {
      "palavra_aleatoria", palavra_aleatoria_embrulho, 
      METH_VARARGS, PALAVRA_ALEATORIA_DOC
   },
   {
      "colori_string", colori_string_embrulho, 
      METH_VARARGS, COLORI_STRING_DOC
   },
   // Marcador do fim da tabela.
   {NULL,NULL, 0, NULL}
};

// Tabela de registro:
static struct PyModuleDef modulo = {
   PyModuleDef_HEAD_INIT,
   "interpola", MODULO_DOC,
   -1, tabela_de_metodos
};

// Inicialização do módulo:
PyMODINIT_FUNC
PyInit_interpola()
   { return PyModule_Create(&modulo); }

static PyObject* colori_string_embrulho
  (PyObject* self, PyObject* argumentos)
{
   enum Formatacao escolha;
   char* input = NULL;
   char* output = NULL;

   if (!PyArg_Parse(argumentos, "(s, i)", input, &escolha))
      return NULL; 
   else
      output = colori_string(input, escolha);

   return Py_BuildValue("s", output);
}

/* Nome com sufixo embrulho para não conflitar com o nome real da função. Este
 * que pode ser definido posteriormente como argumento na definição oficial
 * do Python. */
static PyObject* palavra_aleatoria_embrulho
  (PyObject* self, PyObject* args)
{
   const int MAX = 100;
   char buffer[MAX];
   char* result = NULL;
   PyObject* output;

   /* Copia num espaço local e libera a alocação aqui, pois não sei se o 
    * Python faz isso com um bloco
    * alocado dinamicamente no C. */
   result = palavra_aleatoria();
   strcpy(buffer, result);
   free(result);
   output = Py_BuildValue("s", buffer);

   return output;
}
