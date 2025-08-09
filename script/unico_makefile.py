"""
   Como a compatibilidade deste código com Windows, precisa meio que outras
 diretizes de compilação -- a maioria está incompleta no momento que escrevo
 este script; dois makefiles são escritos. O que fica um pouco chato quando
 quero usar o do Windows, pois ele tem um nome maior, então preciso escrever
 um comando com a opção de trocar o makefile, ou seja, não apenas 'make'
 para executar. Explicado isso, este script tem como função alternar os
 arquivos de maneira que, o "padrão", adeque-se com o sistema em execução.
"""

from pathlib import (Path)
from platform import (system)

PROJETO = Path.cwd()
ENTRADAS_DIR = list(map(lambda path: path.name, PROJETO.iterdir()))


print("Atual sistema em execução: {}".format(system()))
print("Conteúdo do diretório do projeto:")

for entry in ENTRADAS_DIR:
   print('\t\b\b\b-', entry)

tem_makefile_do_windows = "Makefile.win" in ENTRADAS_DIR
tem_makefile_do_linux = "Makefile.unix" in ENTRADAS_DIR
tem_makefile_padrao = "Makefile" in ENTRADAS_DIR

print("Tem o Makefile.win? %s" % tem_makefile_do_windows)
print("Tem o Makefile.unix? %s" % tem_makefile_do_linux)
print("Há um Makefile padrão? %s" % tem_makefile_padrao)

if system() == "Windows" and tem_makefile_do_windows and tem_makefile_padrao:
   print("Alterando os Makefile, colocando o do Windows como padrão ...")
   # Renoameando o padrão como Unix.
   antigo = PROJETO.joinpath("Makefile")
   novo = PROJETO.joinpath("Makefile.unix")
   antigo.rename(novo)

   # Renomeando o Windows como padrão.
   antigo = PROJETO.joinpath("Makefile.win")
   novo = PROJETO.joinpath("Makefile")
   antigo.rename(novo)

