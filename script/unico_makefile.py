"""
   Como a compatibilidade deste código com Windows, precisa meio que outras
 diretizes de compilação -- a maioria está incompleta no momento que escrevo
 este script; dois makefiles são escritos. O que fica um pouco chato quando
 quero usar o do Windows, pois ele tem um nome maior, então preciso escrever
 um comando com a opção de trocar o makefile, ou seja, não apenas 'make'
 para executar. Explicado isso, este script tem como função alternar os
 arquivos de maneira que, o "padrão", adeque-se com o sistema em execução.

   É preciso tomar muito cuidado ao codificar tal script. Ele mexe em todas
 diretrizes de compilações já feita. Assim, se algum erro ocorrer, todo
 trabalho já feito, terá que ser refeito. É bom, no futuro, cria uma
 implementação de backup, assim, se algum erro for cometido, pode ser
 restaurado.
"""

import sys
from pathlib import (Path)
from platform import (system)

PROJETO = Path.cwd()
ENTRADAS_DIR = list(map(lambda path: path.name, PROJETO.iterdir()))
# Constantes de algumas proposições usadas:
tem_makefile_do_windows = "Makefile.win" in ENTRADAS_DIR
tem_makefile_do_linux = "Makefile.unix" in ENTRADAS_DIR
tem_makefile_padrao = "Makefile" in ENTRADAS_DIR


def info_dos_estados_dos_makefiles_no_momento():
   print("\nAtual sistema em execução: {}".format(system()))
   print("Conteúdo do diretório do projeto:")

   for entry in ENTRADAS_DIR:
      print('\t\b\b\b-', entry)

   print("\nTem o Makefile.win? %8s" % tem_makefile_do_windows)
   print("Tem o Makefile.unix? %6s" % tem_makefile_do_linux)
   print("Há um Makefile padrão? %4s" % tem_makefile_padrao, end="\n\n")

def faz_o_makefile_do_windows_como_principal():
   print("Alterando os Makefile, colocando o do Windows como padrão ...")
   # Renoameando o padrão como Unix.
   antigo = PROJETO.joinpath("Makefile")
   novo = PROJETO.joinpath("Makefile.unix")
   antigo.rename(novo)

   # Renomeando o Windows como padrão.
   antigo = PROJETO.joinpath("Makefile.win")
   novo = PROJETO.joinpath("Makefile")
   antigo.rename(novo)

def faz_o_makefile_do_unix_como_principal():
   print("Alterando os Makefile, colocando o do Linux/Unix como padrão ...")
   # Renoameando o padrão como Unix.
   antigo = PROJETO.joinpath("Makefile")
   novo = PROJETO.joinpath("Makefile.win")
   antigo.rename(novo)

   # Renomeando o Unix/Linux como padrão.
   antigo = PROJETO.joinpath("Makefile.unix")
   novo = PROJETO.joinpath("Makefile")
   antigo.rename(novo)

def converte_o_makefile_baseado_no_sistema():
   """
   Se o makefile padrão(aquele sem extensão). Está definido prá Unix, 
   entretanto, a plataforma usada é o Linux/Unix. Ele renomea os arquivos
   para que o padrão, reflita a atual plataforma.
   """
   if (not tem_makefile_padrao):
      sys.exit("Interrompido! Sem um Makefile definido.")

   if (system() == "Windows" and tem_makefile_do_windows
     and tem_makefile_padrao):
         faz_o_makefile_do_windows_como_principal()
   elif (system() == "Linux" and tem_makefile_do_linux
     and tem_makefile_padrao):
         faz_o_makefile_do_unix_como_principal()
   else:
      sys.exit("Nenhuma conversão ocorreu.")

def especificando_o_tipo_de_conversao_que_ira_ocorrer():
   NA_PLATAFORMA_WINDOWS = (system() == "Windows")
   NA_PLATAFORMA_LINUX = (system() == "Linux")
   MAKEFILE_DA_ATUAL_PLATAFORMA_JA_DEFINIDO = (
      # Verifica se o atual 'makefile' já é padrão na plataforma Windows.
      (
         NA_PLATAFORMA_WINDOWS and
         (not tem_makefile_do_windows)
         and tem_makefile_padrao
      ) or
      # A mesma coisa que acima, porém para o OSs(Linux/Unix).
      (
         NA_PLATAFORMA_LINUX and
         (not tem_makefile_do_linux)
         and tem_makefile_padrao
      )
   )

   if (system() == "Windows" and tem_makefile_do_windows
     and tem_makefile_padrao):
         print("Os'makefiles' serão convertidos de,... Linux ==> Windows.")
   if MAKEFILE_DA_ATUAL_PLATAFORMA_JA_DEFINIDO:
         print("O 'makefile' já reflete a atual plataforma.")
   elif (system() == "Linux" and tem_makefile_do_linux
     and tem_makefile_padrao):
         print("Os 'makefiles' serão convertidos de,... Windows ==> Linux.")
   elif (not tem_makefile_padrao):
         print("Nem mesmo há um 'makefile padrão' há converter.")
   else:
      pass

# === === === === === === === === === === === === === === === === === === == 
#                       Execução do script em sí.
# === === === === === === === === === === === === === === === === === === == 
info_dos_estados_dos_makefiles_no_momento()
especificando_o_tipo_de_conversao_que_ira_ocorrer()
converte_o_makefile_baseado_no_sistema()
