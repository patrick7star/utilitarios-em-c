#ifndef _CAPTURA_STDOUT_H_
#define _CAPTURA_STDOUT_H_
#include "arvoreligada.h"

 // Apelidos mais amigaveis e legíveis:
 typedef void (*ArvorePrint)(Tree);
 typedef ArvorePrint Viewer;

 void captura_visualizacao_da_arvore (Tree, Viewer);

#endif
