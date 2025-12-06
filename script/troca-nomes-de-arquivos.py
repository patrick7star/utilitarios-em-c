"""
   Troca qualquer nome que foi colocada num arquivo, que alguma parte do seu
 código importou. Este script, troca o nome dele, e subsequente cada arquivo
 que o usa.
   Por enquanto, é exclusivo para este código, e para C. Entretanto, pode
 ser modificado futuramente.
"""
import glob
from pathlib import (Path)

BATE = "dados_testes.h"
NADA = ''


def laudo_do_encontrado(linha: int, caminho: Path, trecho: str) -> None:
   nome = caminho.name
   trecho = trecho.rstrip('\n')

   print("linha[{:>04d}] | {}: '{}'".format(linha, nome, trecho))

def detector(caminho: Path) -> bool:
   linha = 1
   nome = caminho.name
   LEITURA = "rt"

   try:
      with open(caminho, LEITURA) as stream:
         conteudo = stream.readline()

         while conteudo != NADA:
            if BATE in conteudo:
               laudo_do_encontrado(linha, caminho, conteudo)
               return True
            linha += 1
            conteudo = stream.readline()

   except UnicodeDecodeError:
      print("Não foi possível com o arquivo %s" % nome)
      return False

def vasculho_do_que_sera_substituido() -> None:
   todos_caminhos = (
      # Vasculha em todos os diretórios abaixo:
        glob.glob("src/*.c") +
      glob.glob("src/*/*.c") + 
      glob.glob("tests/*.c")
   )
   output = []

   for caminho in todos_caminhos:
      caminho = Path(caminho)

      if detector(caminho):
         output.append(caminho)
   return output
   

if __name__ == "__main__":
   lista = vasculho_do_que_sera_substituido()
   quantia = len(lista)

   print("Você deseja trocar todos os {} nomes?".format(quantia))
